# Computer-Arch-Lab-1
 
This is part of the 1st lab of the **Computer Architecture course at the Aristotle University of Thessaloniki**.

## Assignment 1 questions
Below are the answers to the first exercises, categorized according to the assignment description:

1. **_What are the most important parameters found in the starter_se.py configuration file?_**

- CPU type: The given types are **atomic** and **minorCPU**, with the system defaulting to atomic, while the command given in the assignment description sets it to minor.
- Frequency: 1GHz
- Number of cores: 1
- Memory type: DDR3 1600
- Cache line size: 64
- Caches: L1 and shared L2 caches
- Number of memory channels: 2
- Memory size: 2GB
\
One could change the CPU frequency either by using the _--cpu-freq_ command line option or by modifying the _starter_se.py_ file, where the --cpu-freq default value is dictated.

2. **_stats.txt_ basic output values:**
- sim_seconds: 0.000035
- sim_insts: 5027
- host_inst_rate: 80991

3. **Calculating the CPI using _congif.ini_ or _config.json_**
- IL1.miss_num = 327
\
`system.cpu_cluster.cpus.icache.overall_misses::total          327                       # number of overall misses`

- DL1.miss_num = 177
\
`system.cpu_cluster.cpus.dcache.overall_misses::total          177                       # number of overall misses` 

- L2.miss_num = 474
\
`system.cpu_cluster.l2.overall_misses::total          474                       # number of overall misses`

- Total_ins_num = 5027
\
`sim_insts                                        5027                       # Number of instructions simulated`

Which gives us a total of **CPI = ~5,31609** according to the given formula.

4. **_Using the documentation to extract information on CPU types_**
The simulator provides the user with various CPU models, with two in-order categories: **SimpleCPU** and **MinorCPU**. A little more detailed:
##### SimpleCPU
- **BaseSimpleCPU**: It is the base model of the category and acts like an Abstract Class would do in Objected Oriented Programming. It offers a basic interface and information about most of the features and functions the other SimpleCPU models are capable of. This is why SimpleCPU models can never be standalone instances of a CPU.
- **AtomicSimpleCPU**: Atomic CPU's use **atomic memory access**[1], which they use to estimate the cache time a set of instructions will take. The use of that kind of memory access makes it possible for AtomicSimpleCPU to determine what instruction gets immplemented during every CPU cycle. They also define the port used for the memory, and connects it to the cache.
- **TimingSimpleCPU**: Timing Simple CPU's use **atomic memory access**[2], which allows them to emulate cache stalls and waiting for the memory to respond to requests. Along with the functions found on AtomicSimpleCPU, Timing variants can also dictate how a response will be handled and sent out by the system.

##### MinorCPU
**MinorCPU** is the most customizable and feature-rich class out of all the mainstream in-order CPU models. It offers the necessary tools for scheduling pipelines, branch prediction and various cache levels. It functions on a `per cycle` level, which means that it's given a "template" of the functions and handlers it has to use during every CPU cycle. It is able to handle data feedback, which is vital to the proper function of the pipeling process. Apart from all these, the user is allowed to specify the data structure and size the CPU is more likely to handle, while the MinorCPU class is even capable of visualizing how it puts the pipeline in order.

---

[1] **Atomic Memory Access**: It is a faster way of accessing memory, compared to Timing. It utilizes fast-forwading and loose estimates of the time a request can take to be finished.
\
[2] **Timing Memory Access**: This is the most detailed simulation effort the system offers. It calculates the time a request will take to be realized, taking queues and busy hardware into consideration. Timing is also capable of sending NACK signals when a request hasn't been completed, on top of the actual response.

---

5. **_Conducting experiments with various CPU types, clocks and memory types_**
For this part of the report, we are going to be using a dependency free version of the sequential algorithm of the 1st assignment of the _Parallel and Distributed Systems_ course, which you can find [here](https://github.com/anthonyisafk/Parallel-Distributed-Systems-part1)
- a. 
    1. MinorCPU host time = 0.83 sec
    2. TimingSimpleCPU host time = 0.17 sec

- b. 
  1. CPU clock experiment execution times in seconds:

    |  Frequency[GHz]  |  MinorCPU  | TimingSimpleCPU   | 
    | ---------------- | ---------- | ----------------- | 
    |  1.5             |  0.66      | 0.25              | 
    |  2.0             |  0.62      | 0.17              | 
    |  3.0             |  0.61      | 0.20              |  

  2. Memory type experiment execution times in seconds:

    |  Memory Type     |  MinorCPU  | TimingSimpleCPU   | 
    | ---------------- | ---------- | ----------------- | 
    |  SimpleMemory    |  0.63      | 0.18              | 
    |  LPDDR2 1x32     |  0.72      | 0.17              | 
    |  DDR4 8x8        |  0.63      | 0.17              |

## Thoughts on the first assignment
This first part of the lab assignments was used by our team as a guide to the **gem5** emulator, but also as a way to work with a **Virtual Machine** (VM) more efficiently. Moreover, we had the chance to integrate **_git_** and **_GitHub_** into our workflows, since most of the processing of the data and writing was done independently of the VM.
\
\
Generally speaking, the requirements of the assignment were quite simple and adequate to gain knowledge to build upon for the next stages of the course and our interaction with gem5 and emulators in general, provided that we spend enough time to explore the ecosystem on our own.
\
\
When it comes to the assistance provided, it was very helpful to us that a ready-to-use VM was given, primarily due to the short time period we had to prepare for being present in the first presentation. The instructions of the assignment were clear and helpful, with the few exceptions of some typos we came accross, but were a nice little challenge to overcome, using the documentation and the command line manuals gem5 has to offer. Hence, troubleshooting was pretty brief and everything worked out as it should.

## Antonios Antoniou - 9482
## Dimitrios Xylogiannis - 9672
## Electrical and Computer Engineering - AUTh
