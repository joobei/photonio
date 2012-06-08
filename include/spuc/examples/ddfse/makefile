# Makefile for SPUC shared library
#
CC = g++ -Wno-non-template-friend 
SPUC_DIR = ../..
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS} -g
CPPFLAGS = -Wno-non-template-friend ${IFLAGS}

#COBJS = remez_files.o test_remez.o remez_fir.o remezf.o
COBJS = baud_eq_env.o baud_equalizer_sim.o bpsk_ber_test.o fading_channel.o \
		fir.o max_pn.o noise.o 

all:  ddfse

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp


clean: 
	rm *.o *.exe

ddfse: ${COBJS} 
	g++ ${COBJS} -o ddfse

baud_eq_env.o : baud_eq_env.cpp ${SPUC_DIR}/comm/baud_eq_env.h
	${CC} -c baud_eq_env.cpp  ${CFLAGS}

fading_channel.o : ${SPUC_DIR}/comm/fading_channel.cpp
	${CC} -c ${SPUC_DIR}/comm/fading_channel.cpp  ${CFLAGS}

bpsk_ber_test.o : ${SPUC_DIR}/comm/bpsk_ber_test.cpp
	${CC} -c ${SPUC_DIR}/comm/bpsk_ber_test.cpp  ${CFLAGS}

max_pn.o : ${SPUC_DIR}/generic/max_pn.cpp
	${CC} -c ${SPUC_DIR}/generic/max_pn.cpp  ${CFLAGS}

fir.o : ${SPUC_DIR}/generic/fir.cpp
	${CC} -c ${SPUC_DIR}/generic/fir.cpp  ${CFLAGS}


noise.o : ${SPUC_DIR}/generic/noise.cpp
	${CC} -c ${SPUC_DIR}/generic/noise.cpp  ${CFLAGS}

baud_equalizer_sim.o : baud_equalizer_sim.cpp
	${CC} -c baud_equalizer_sim.cpp ${CFLAGS}
