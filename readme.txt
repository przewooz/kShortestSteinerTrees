@ 2019.06.19 Wroclaw

The project is written in C++. It should be compiled with Visual Studio 2017.
The main project is IslandGA
The project should be compiled in "Release" and "x64" mode.


The project is made available for the "dynamic" methods versions.
To obtain "Static" method versions you need to comment out 41st line in ("#define  STEINER_TREE_SET_DYNAMIC") in "FiberNet.h" file.


In the results folder we support the seetings files and the results we have obtained.


To run experiments on your own please investigate, the "run_me_yourself" folder.
1. Except the settings file, in the same older you need to put "Euro28" and "us26" folders (both supported in "run_me_yourself" folder)
2. "Euro28" and "us26" folders contain the test case settings for all the test cases we have considered.



How to execute

1.	The examples of how to run considered methods by hand are given in “run me yourself” folder.

2.	The test took many executions and were handled by a dedicated program that was assuring that the same number of experiments is executed in a given moment. Therefore, the experiment execution may be found as complicated. We hope that examples we have supported will be clear enough, to allow anyone running his own experiments. 

3. "run_me_yourself" folder contains two different exe files:
  * “IslandGA_dyn.exe” (compiled for dynamic version) 
  * "IslandGA_st.exe" (compiled for static version)


4.	To execute an experiment you need to:
  *  Put “IslandGA_dyn.exe” (for dynamic version) or "IslandGA_st.exe" (for static version), “zobristkey”, "default.cfg" and settings file in one folder.
  * in the same folder put all ff???.txt and ss???.txt files (they contain the test case configuration, that uses the files from Euro28 and us26 folders).
  * create an "entry" file, named <number>_entry.txt (the example of the entry file "1_entry.txt" is given in the "run_me_yourself" folder)
  * entry file must contain the name of the settngs file
  * to execute experiment you shall execute the command: "IslandGA_dyn.exe <entry file number>", 
  so for the "1_entry.txt" file given in the example you need to execute:
  "IslandGA_dyn.exe 1"

  * after the experiment two files will be created:
	- “1_out.txt” that contains the output information for the summary files
	- "1_log.txt" that contains the log fro the experiment



5. To run your own experiments it is enough if you replicate “run me yourself” folder and execute the computation with different settings files pointed by the entry file.


In case of any problems please do not hesitate to contact us:
michal.przewozniczek@pwr.edu.pl
koomareex@gmail.com
