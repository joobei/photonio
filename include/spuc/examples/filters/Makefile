# Makefile for SPUC shared library
CC = g++ -Wno-non-template-friend 
SPUC_DIR = ../..
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix
CFLAGS = 
CPPFLAGS = -Wno-non-template-friend ${IFLAGS} -g

COBJS = test_ellip.o spuc_math.o elliptic.o
ROBJS = test_remez.o remez_fir.o remezf.o
BOBJS = fir.o test_bfir.o butterworth_fir.o
AOBJS = fir.o butterworth_fir.o spuc_math.o elliptic.o remez_fir.o remezf.o create_remez_lpfir.o gaussian_fir.o

ITPP_HOME = ../../../it++3.8.0
FLAGS     = `${ITPP_HOME}/bin/it-config --flags`
LIBS      = `${ITPP_HOME}/bin/it-config --libs`
CC        = g++ -g
AR        = ar

ICPP_SOURCES := lpf_ls.cpp lpf_ls_square.cpp lpf_wls.cpp ls_solver_test.cpp

all: filters

ls: $(ICPP_SOURCES:.cpp=.run) 

#Pattern rules
%.run: %.cpp
	-$(CC) $(FLAGS) $< -o $@ $(LIBS)

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp

clean: 
	rm *.o ellip

run: ellip
	ellip

spuc_math.o : ${SPUC_DIR}/functions/spuc_math.cpp
	g++ -c ${SPUC_DIR}/functions/spuc_math.cpp ${CPPFLAGS} ${IFLAGS}

elliptic.o : ${GDIR}/elliptic.h ${GDIR}/elliptic.cpp
	g++ -Wno-non-template-friend ${CPPFLAGS} ${IFLAGS} -c ${GDIR}/elliptic.cpp

fir.o: ${GDIR}/fir.cpp
	g++ ${CPPFLAGS} -c -o $@ ${GDIR}/fir.cpp

filters: test_filters.o ${AOBJS}
	g++ ${AOBJS} test_filters.o -g -o filters -lgsl -lgslcblas

gaussian_fir.o: ${GDIR}/gaussian_fir.cpp
	g++ ${CPPFLAGS} -c -o $@ ${GDIR}/gaussian_fir.cpp

butterworth_fir.o: ${GDIR}/butterworth_fir.cpp
	g++ ${CPPFLAGS} -c -o $@ ${GDIR}/butterworth_fir.cpp

remezf.o : ${SPUC_DIR}/generic/remezf.cpp
	${CC} -c ${SPUC_DIR}/generic/remezf.cpp  ${CFLAGS} ${IFLAGS}

remez_fir.o : ${SPUC_DIR}/generic/remez_fir.cpp
	${CC} -c ${SPUC_DIR}/generic/remez_fir.cpp  ${CFLAGS} ${IFLAGS}

create_remez_lpfir.o : ${SPUC_DIR}/generic/create_remez_lpfir.cpp
	${CC} -c ${SPUC_DIR}/generic/create_remez_lpfir.cpp  ${CFLAGS} ${IFLAGS}
