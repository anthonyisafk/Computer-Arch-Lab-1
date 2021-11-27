# Computer-Arch-Lab-1

This is part of the 1st lab of the **Computer Architecture course at the Aristotle University of Thessaloniki**.
\
Below are the answers to the first exercises, categorized according to the assignment description:

1. _What are the most important parameters found in the starter_se.py configuration file?_
\
-CPU type: The given types are **atomic** and **minorCPU**, with the system defaulting to atomic, while the command given in the assignment description sets it to minor.
- Frequency: 1GHz
- Number of cores: 1
- Memory type: DDR3 1600
- Cache line size: 64
- Caches: L1 and shared L2 caches
- Number of memory channels: 2
- Memory size: 2GB
One could change the CPU frequency either by using the _--cpu-freq_ command line option or by modifying the _starter_se.py_ file, where the --cpu-freq default value is dictated.

2. _stats.txt_ basic output values:
- sim_seconds: 0.000035
- sim_insts: 5027
- host_inst_rate: 80991

3. Calculating the CPI using _congif.ini_ or _config.json_
- IL1.miss_num = 0
`system.cpu_cluster.cpus.dtb.inst_misses             0                       # ITB inst misses`
- DL1.miss_num = 147
`system.cpu_cluster.cpus.branchPred.indirectMisses          147                       # Number of indirect misses.` 
- L2.miss_num = 474
`system.cpu_cluster.l2.overall_misses::total          474                       # number of overall misses`
- Total_ins_num = 5831
\
Which gives us a total of **CPI = ~5,21574344** according to the given formula.

4. _Using the documentation to extract information on CPU types_








## Antonios Antoniou - 9482
## Electrical and Computer Engineering - AUTh
