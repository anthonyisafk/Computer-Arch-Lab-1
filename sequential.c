/*
 * @file: sequential.c 
 *
 * A more lightweight, dependency-free version of the sequential implementation
 * of the Parallel and Distributed Systems assignment - pt. 1
 * https://github.com/anthonyisafk/Parallel-Distributed-Systems-part1
 *
 * Used to test the gem5 emulator.
 * 
 * -- Serial version of the operations. --
 * 
 * @author: Antonios Antoniou - 9482
 *    
 * 2021 Aristotle University of Thessaloniki
 * Computer Architecture
 */

#include <stdio.h>
#include <stdlib.h>


// A struct used to turn sparse matrices to CSR data structures.
// The notation and algorithm used is taken directly from the given Wikipedia page.
typedef struct {
	uint size;
	int *values;
	uint *colIndex;
	uint *rowIndex;
} csr;

typedef struct {
  uint time;
  uint *triangles;
} data_arg;


int **makeRandomSparseTable(int size) {
	int **table = (int **) malloc(size * sizeof(int *));
	for (int i = 0; i < size; i++) {
		table[i] = (int *) malloc(size * sizeof(int));
	}

	// Only add values to the upper triangle of the table.
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			table[i][j] = (rand() % 1000 < 750) ? 0 : 1; 

			// ALTERNATIVE INITIALIZATION. USE 2's INSTEAD
			// OF ONLY ONES AND ZEROS.

			// int newValue = rand() % 1000;
			// if (newValue < 750) {
			// 	table[i][j] = 0;
			// } else if (newValue >= 750 && newValue <= 930) {
			// 	table[i][j] = 1;
			// } else {
			// 	table[i][j] = 2;
			// }
		}
	}

	// Make it symmetric by setting A[i][j] = A[j][i] for the rest of the values.
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			table[i][j] = table[j][i];
		}
	}

	return table;
}


// Calculates the dot product of two vectors, that belong to the same matrix.
int dot(csr table, uint row, uint column) {
  // Symmetric table. Rows are identical to columns and vice versa.
  uint rowStart = table.rowIndex[row];
  uint rowEnd = table.rowIndex[row+1];

  uint colStart = table.rowIndex[column];
  uint colEnd = table.rowIndex[column+1];

  int value = 0;
  // Leaves a trace of the last match. The next iteratio will start from
  // the nexr position to get rid of unnecessary comparisons.
  uint lastMatch = colStart - 1;

  /**
   * 1. If table.colIndex[i] == table.colIndex[j] then we have a match. 
   * Extract the value, set lastMatch, so the next loop starts searching after it.
   * 2. If table.colIndex[i] > table.colIndex[j], the column wasn't found.
   * Set lastMatch to j, so the next row searches after it.
   * 3. If table.colIndex[i] > table.colIndex[colEnd], then there is no greater value throughout the row.
   * Since table.colIndex[] is monotonic, the next searches are guaranteed to find no matches.
   * So we stop scanning that row in general.
   */ 
  for (uint i = rowStart; i < rowEnd; i++) {
    for (uint j = lastMatch+1; j < colEnd; j++) {
      if (table.colIndex[i] == table.colIndex[j]) {
        lastMatch = j;
        value += table.values[i] * table.values[j];
        break;
      } 
      else if (table.colIndex[i] > table.colIndex[j]) {
        lastMatch = j;
      }
      else if (table.colIndex[i] > table.colIndex[colEnd]) {
        break;
      }
    }
  }

  return value;
}


// Converts a sparse matrix to CSR.
csr matrixToCSR(int **table, uint size) {
	// Keep track of the nonzero objects.
	uint nonzeros = 0; 
	uint resizes = 10;

	// Initialize the 3 necessary arrays. row_index has a standard length of "rows+1"
	uint *rowIndex = (uint *) malloc((size+1) * sizeof(uint));
	for (int i = 0; i < size+1; i++) {
		rowIndex[i] = 0;
	}

	// values and col_index have a length of the total nonzero values.
	int *values = (int *) malloc(10 * size * sizeof(int));
	uint *colIndex = (uint *) malloc(10 * size * sizeof(uint));
	for (int i = 0; i < 10*size; i++) {
		values[i] = 0;
		colIndex[i] = 0;
	}
	
	for (uint i = 0; i < size; i++) {
		// Add the nonzero values that are placed above the current row. 
		rowIndex[i] = nonzeros;

		for (uint j = 0; j < size; j++) {
			if (table[i][j] != 0) {
				nonzeros++;

				// Make sure to extend the arrays in case their size exceeds the current one.
				if (nonzeros == resizes * size -1) {
					resizes += 10;
					values = (int *) realloc(values, (resizes * size) * sizeof(int));
					colIndex = (uint *) realloc(colIndex, (resizes * size) * sizeof(uint));
				}

				// Add the nonzero value and the respective column index.
				values[nonzeros-1] = table[i][j];
				colIndex[nonzeros-1] = j; 
			}
		}

		// Add the last value before exiting the for loop.
		if (i == size - 1) {
			rowIndex[size] = nonzeros;
		}
	}

	csr converted;
	converted.size = size;
	converted.values = values;
	converted.colIndex = colIndex;
	converted.rowIndex = rowIndex;
	return converted;
}


// Simulates the multipliation of the C table with the e vector,
// which contains exclusively 1's.
uint *countTriangles(csr C) {
	uint size = C.size;
	uint *triangleCount = (uint *) calloc(size, sizeof(uint));


	// Add all the values in each row, then divide by 2.
	// Simulates the operation of multiplying the table with a nx1 vector.
	for (uint i = 0; i < size; i++) {
		uint rowStart = C.rowIndex[i];
		uint rowEnd = C.rowIndex[i+1];

    // Divide by 2 when we reach the last element.
		for (uint j = rowStart; j < rowEnd; j++) {
			triangleCount[i] = (j == rowEnd - 1) 
        ? (triangleCount[i] + C.values[j]) / 2
        : triangleCount[i] + C.values[j];
		}
    // printf(" %u ", triangleCount[i]);
	}

  return triangleCount;
}


csr hadamardSingleStep(csr table, uint start, uint end) {
  uint size = end - start;
  
	uint nonzeros = table.rowIndex[end] - table.rowIndex[start];
	uint newNonzeros = 0;
	// The new values array. Intialize all to 0. Do the same for the new column indices.
	int *newValues = (int *) calloc(nonzeros, sizeof(int));
	uint *newColIndex = (uint *) calloc(nonzeros, sizeof(uint));

	// Finally, initialize the new row index array.
	uint *newRowIndex = (uint *) calloc((size+1), sizeof(uint));

  // Find the values in A^2, iff the original matrix had a nonzero in that position.
	for (uint row = start; row < end; row++) {
		uint rowStart = table.rowIndex[row];
		uint rowEnd = table.rowIndex[row+1];

		for (uint index = rowStart; index < rowEnd; index++) {
		  uint currentColumn = table.colIndex[index];

		  int value = dot(table, row, currentColumn);

		  if (value > 0) {
		    newValues[newNonzeros] = value;
		    newColIndex[newNonzeros] = currentColumn;
		    newRowIndex[row - start + 1]++;

		    newNonzeros++;
		  }
		}

		// Pass the next value if we haven't reached the last row.
		if (row < end - 1) {
		  newRowIndex[row - start + 2] = newRowIndex[row - start + 1];
		}
	}

  csr hadamard = {size, newValues, newColIndex, newRowIndex};
  return hadamard;
}


data_arg measureTimeSerial(csr mtx) {  
	struct timeval stop, start;
	gettimeofday(&start, NULL);

	csr C = hadamardSingleStep(mtx, 0, mtx.size);
	uint *triangles = countTriangles(C);

	gettimeofday(&stop, NULL);
	uint timediff = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;

	printf("\n\nThe serial algorithm took %lu us\n", timediff);

	data_arg data = {timediff, triangles};
	return data;
}


int main(int argc, char **argv) {
  int **table = makeRandomSparseTable(25);
  csr mtx = matrixToCSR(table, 25);

  data_arg data = measureTimeSerial(mtx);

  return 0;
}
