all: processes memory file_system input_output

processes: processes.cpp tinbes.o
	c++ processes.cpp tinbes.o -o processes

memory: memory.cpp tinbes.o
	c++ memory.cpp tinbes.o -o memory

file_system: file_system.cpp tinbes.o
	c++ file_system.cpp tinbes.o -o file_system

input_output: input_output.cpp
	c++ input_output.cpp -l wiringPi -o input_output

tinbes.o: tinbes.cpp tinbes.h
	c++ -o tinbes.o -c tinbes.cpp

clean: 
	rm tinbes.o processes memory file_system input_output
