# Makefile for SPUC shared library
#
CC = g++ -Wno-non-template-friend 
SPUC_DIR = /home/tkirke/spuc
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS}
CPPFLAGS = -Wno-non-template-friend ${IFLAGS}

XCOBJS = ${SPUC_DIR}/matrix/converters.o \
${SPUC_DIR}/matrix/det.o \
${SPUC_DIR}/matrix/eigen.o \
${SPUC_DIR}/matrix/fastmath.o \
${SPUC_DIR}/matrix/inv.o \
${SPUC_DIR}/matrix/ls_solve.o \
${SPUC_DIR}/matrix/svd.o \
${SPUC_DIR}/matrix/vector.o \
${SPUC_DIR}/qpsk/resampler.o \
${SPUC_DIR}/qpsk/timing_nco.o \
${SPUC_DIR}/specializations/scic.o \
${SPUC_DIR}/utility/spucassert.o

COBJS = ${SPUC_DIR}/matrix/cholesky.o \
${SPUC_DIR}/matrix/converters.o \
${SPUC_DIR}/matrix/det.o \
${SPUC_DIR}/matrix/eigen.o \
${SPUC_DIR}/matrix/fastmath.o \
${SPUC_DIR}/matrix/inv.o \
${SPUC_DIR}/matrix/ls_solve.o \
${SPUC_DIR}/matrix/lu.o \
${SPUC_DIR}/matrix/matrix.o \
${SPUC_DIR}/matrix/specmat.o \
${SPUC_DIR}/matrix/svd.o \
${SPUC_DIR}/matrix/vector.o \
${SPUC_DIR}/qpsk/qpsk_variable.o \
${SPUC_DIR}/qpsk/resampler.o \
${SPUC_DIR}/qpsk/sim_qpsk_variable.o \
${SPUC_DIR}/qpsk/timing_nco.o \
${SPUC_DIR}/specializations/scic.o \
${SPUC_DIR}/utility/spucassert.o

CSRC = ${SPUC_DIR}/matrix/cholesky.cpp \
${SPUC_DIR}/matrix/converters.cpp \
${SPUC_DIR}/matrix/det.cpp \
${SPUC_DIR}/matrix/eigen.cpp \
${SPUC_DIR}/matrix/fastmath.cpp \
${SPUC_DIR}/matrix/inv.cpp \
${SPUC_DIR}/matrix/ls_solve.cpp \
${SPUC_DIR}/matrix/lu.cpp \
${SPUC_DIR}/matrix/matrix.cpp \
${SPUC_DIR}/matrix/specmat.cpp \
${SPUC_DIR}/matrix/svd.cpp \
${SPUC_DIR}/matrix/vector.cpp \
${SPUC_DIR}/qpsk/qpsk_main.cpp \
${SPUC_DIR}/qpsk/qpsk_variable.cpp \
${SPUC_DIR}/qpsk/qpsk_variable_main.cpp \
${SPUC_DIR}/qpsk/resampler.cpp \
${SPUC_DIR}/qpsk/sim_qpsk_variable.cpp \
${SPUC_DIR}/qpsk/timing_nco.cpp \
${SPUC_DIR}/specializations/scic.cpp \
${SPUC_DIR}/utility/spucassert.cpp

all:  ${COBJS}

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp


clean: 
	rm *.o *.exe

libspuc.so: ${ALLOBJS} ${COBJS}
	g++ ${COBJS} -shared -o libspuc.so