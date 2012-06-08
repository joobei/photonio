#---------------------------------------------------------------------
# Makefile for $(TTPP_HOME)/tests
#
# Simply write "make" to compile all .cpp files. Executables with the
# extension .run are created for each of the .cpp files. Make then
# checks if a .ref file is availiable for each executable file. If
# the .ref file is missing make runs the corresponding program and 
# directs the standard output to the .ref file
#
# To test the programs you can write "make test". Make will then create
# .tmp files and compare the .tmp files with the .ref files. If the
# .tmp file differs from the .ref file the difference is written to
# standard output. Not that several of these examples will produce
# non empty diffs between .ref and .tmp. For examples, the number of 
# errors or the execution time will differ each time a certain
# program is run.
#---------------------------------------------------------------------

# NOTE: The flags below assume that you use the Gnu g++ compiler
# and that you have compiled an optimized it++ library. You may
# need to manually change some of these flags.
ITPP_HOME = /home/tkirke/it++3.8.0
FLAGS     = `${ITPP_HOME}/bin/it-config --flags`
LIBS      = `${ITPP_HOME}/bin/it-config --libs`
CC        = g++ -g
AR        = ar

# Don't edit below this line
#----------------------------------------------------------------------

#CPP files are tested automatically by "make test"
CPP_SOURCES := $(wildcard *.cpp)

#All-----------------
all: $(CPP_SOURCES:.cpp=.run) 

#Pattern rules
%.run: %.cpp
	-$(CC) $(FLAGS) $< -o $@ $(LIBS)

clean: 
	-rm *.run 
