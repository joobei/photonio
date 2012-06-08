# Makefile for SPUC shared library
#
#CC = g++ -Wno-non-template-friend
SPUC_DIR = /home/tkirke/spuc
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS}
#CPPFLAGS = -Wno-non-template-friend ${IFLAGS}
CPPFLAGS = ${IFLAGS}

COBJS = ${SPUC_DIR}/generic/binary.o \
${SPUC_DIR}/generic/bool_nco.o \
${SPUC_DIR}/generic/butterworth_fir.o \
${SPUC_DIR}/generic/cic.o \
${SPUC_DIR}/generic/cordic.o \
${SPUC_DIR}/generic/fir.o \
${SPUC_DIR}/generic/gaussian_fir.o \
${SPUC_DIR}/generic/remez_fir.o \
${SPUC_DIR}/generic/max_pn.o \
${SPUC_DIR}/generic/nco.o \
${SPUC_DIR}/generic/noise.o \
${SPUC_DIR}/generic/random.o \
${SPUC_DIR}/generic/sigdel.o \
${SPUC_DIR}/generic/sum_and_dump.o \
${SPUC_DIR}/generic/vco.o \
${SPUC_DIR}/generic/lib_inst.o \
${SPUC_DIR}/generic/ls_fir.o 

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS}  -c -o $@.o $@.cpp

all:  ${COBJS}

clean: 
	rm *.o *.exe
