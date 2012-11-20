#
# Makefile for Tony Kirke's QPSK simulation
#
CC = g++ -Wno-non-template-friend 
SPUC_DIR = ../..
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
cflags= -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility

COBJS = carrier_nco.o qpsk.o qpsk_ber_test.o quad_data.o \
  raised_cosine.o sim_qpsk.o 

GOBJS = fir.o max_pn.o nco.o noise.o vco.o

all:  qpsk

clean: 
	rm *.o *.exe

qpsk: ${COBJS} ${GOBJS} qpsk_main.o
	${CC} -o qpsk qpsk_main.o ${COBJS} ${GOBJS}

qpsk_main.o: qpsk_main.cpp ${CDIR}/sim_qpsk.h ${CDIR}/quad_data.h ${CDIR}/qpsk_ber_test.h ${CDIR}/qpsk.h ${CDIR}/carrier_nco.h
	${CC} -c qpsk_main.cpp ${cflags} 

carrier_nco.o: ${CDIR}/carrier_nco.cpp ${CDIR}/carrier_nco.h
	${CC} -c  ${CDIR}/carrier_nco.cpp ${cflags} 

qpsk_ber_test.o: ${CDIR}/qpsk_ber_test.cpp ${CDIR}/qpsk_ber_test.h
	${CC} -c ${CDIR}/qpsk_ber_test.cpp ${cflags} 

sim_qpsk.o: ${CDIR}/sim_qpsk.cpp ${CDIR}/sim_qpsk.h ${CDIR}/quad_data.h ${CDIR}/qpsk_ber_test.h ${CDIR}/qpsk.h ${CDIR}/carrier_nco.h
	${CC} -c ${CDIR}/sim_qpsk.cpp ${cflags} 

qpsk.o: ${CDIR}/qpsk.cpp ${CDIR}/qpsk.h ${CDIR}/raised_cosine.h ${CDIR}/carrier_nco.h
	${CC} -c ${CDIR}/qpsk.cpp ${cflags} 

quad_data.o: ${CDIR}/quad_data.cpp ${CDIR}/quad_data.h
	${CC} -c  ${CDIR}/quad_data.cpp ${cflags}

noise.o: ${GDIR}/noise.cpp ${GDIR}/noise.h ${GDIR}/complex.h
	${CC} -c ${GDIR}/noise.cpp  ${cflags}

max_pn.o: ${GDIR}/max_pn.cpp ${GDIR}/max_pn.h
	${CC} -c ${GDIR}/max_pn.cpp  ${cflags}

raised_cosine.o: ${CDIR}/raised_cosine.cpp ${CDIR}/raised_cosine.h \
                 ${GDIR}/complex.h ${GDIR}/fir.h ${GDIR}/fir_interp.h
	${CC} -c ${CDIR}/raised_cosine.cpp  ${cflags} 

spuc_math.o: ${FDIR}/spuc_math.cpp 
	${CC} -c ${FDIR}/spuc_math.cpp ${cflags} 

vco.o: ${GDIR}/vco.cpp ${GDIR}/vco.h ${GDIR}/complex.h
	${CC} -c ${GDIR}/vco.cpp  ${cflags}

nco.o: ${GDIR}/nco.cpp ${GDIR}/nco.h ${GDIR}/complex.h
	${CC} -c ${GDIR}/nco.cpp  ${cflags}

fir.o: ${GDIR}/fir.cpp ${GDIR}/fir.h ${GDIR}/complex.h
	${CC} -c ${GDIR}/fir.cpp  ${cflags}
