# Makefile for SPUC shared library
#
CC = g++  
SPUC_DIR = .
OBJDIR   = ${SPUC_DIR}
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/geneic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS}  
CPPFLAGS = ${IFLAGS} 

GOBJS =
# if GSL is present, replace GOBJS with this
#GOBJS = ${OBJDIR}/generic/elliptic.o

COBJS = ${OBJDIR}/array/array1d_functions.o \
${OBJDIR}/array/array2d_functions.o \
${OBJDIR}/comm/baud_eq_env.o \
${OBJDIR}/comm/bch.o \
${OBJDIR}/comm/bped.o \
${OBJDIR}/comm/bpsk_ber_test.o \
${OBJDIR}/comm/carrier_nco.o \
${OBJDIR}/comm/commfunc.o \
${OBJDIR}/comm/convcode.o \
${OBJDIR}/comm/crc.o \
${OBJDIR}/comm/data_conv_encoder.o \
${OBJDIR}/comm/dvb_conv_encoder.o \
${OBJDIR}/comm/egolay.o \
${OBJDIR}/comm/fading_channel.o \
${OBJDIR}/comm/gf.o \
${OBJDIR}/comm/gfx.o \
${OBJDIR}/comm/hammcode.o \
${OBJDIR}/comm/modulator.o \
${OBJDIR}/comm/ofdm_data_encoder.o \
${OBJDIR}/comm/punct_convcode.o \
${OBJDIR}/comm/qam_mod.o \
${OBJDIR}/comm/qam_soft_decision.o \
${OBJDIR}/comm/qam_tx.o \
${OBJDIR}/comm/qpsk.o \
${OBJDIR}/comm/qpsk_ber_test.o \
${OBJDIR}/comm/qpsk_discriminators.o \
${OBJDIR}/comm/quad_data.o \
${OBJDIR}/comm/raised_cosine.o \
${OBJDIR}/comm/rec_syst_conv_code.o \
${OBJDIR}/comm/reed_solomon.o \
${OBJDIR}/comm/sequence.o \
${OBJDIR}/comm/sim_qam.o \
${OBJDIR}/comm/sim_qpsk.o \
${OBJDIR}/comm/turbo.o \
${OBJDIR}/comm/viterbi.o \
${OBJDIR}/equalizers/lms_dfe.o \
${OBJDIR}/equalizers/mlsd.o \
${OBJDIR}/equalizers/rls.o \
${OBJDIR}/functions/idft.o \
${OBJDIR}/functions/ifft.o \
${OBJDIR}/functions/misc.o \
${OBJDIR}/functions/scalfunc.o \
${OBJDIR}/functions/spuc_math.o \
${OBJDIR}/functions/window.o \
${OBJDIR}/generic/binary.o \
${OBJDIR}/generic/bool_nco.o \
${OBJDIR}/generic/butterworth_fir.o \
${OBJDIR}/generic/cic.o \
${OBJDIR}/generic/cordic.o \
${OBJDIR}/generic/fir.o \
${OBJDIR}/generic/gaussian_fir.o \
${OBJDIR}/generic/max_pn.o \
${OBJDIR}/generic/nco.o \
${OBJDIR}/generic/noise.o \
${OBJDIR}/generic/remezf.o \
${OBJDIR}/generic/remez_fir.o \
${OBJDIR}/generic/create_remez_lpfir.o \
${OBJDIR}/generic/sigdel.o \
${OBJDIR}/generic/sum_and_dump.o \
${OBJDIR}/generic/vco.o \
${OBJDIR}/matrix/cholesky.o \
${OBJDIR}/matrix/converters.o \
${OBJDIR}/matrix/det.o \
${OBJDIR}/matrix/eigen.o \
${OBJDIR}/matrix/fastmath.o \
${OBJDIR}/matrix/inv.o \
${OBJDIR}/matrix/ls_solve.o \
${OBJDIR}/matrix/lu.o \
${OBJDIR}/matrix/matrix.o \
${OBJDIR}/matrix/specmat.o \
${OBJDIR}/matrix/svd.o \
${OBJDIR}/matrix/vector.o \
${OBJDIR}/examples/qpsk/qpsk_variable.o \
${OBJDIR}/examples/qpsk/resampler.o \
${OBJDIR}/examples/qpsk/sim_qpsk_variable.o \
${OBJDIR}/examples/qpsk/timing_nco.o \
${OBJDIR}/specializations/scic.o \
${OBJDIR}/generic/lib_inst.o \
${OBJDIR}/utility/spucassert.o

CSRC = ${SPUC_DIR}/array/array1d_functions.cpp \
${SPUC_DIR}/array/array2d_functions.cpp \
${SPUC_DIR}/comm/baud_eq_env.cpp \
${SPUC_DIR}/comm/bch.cpp \
${SPUC_DIR}/comm/bped.cpp \
${SPUC_DIR}/comm/bpsk_ber_test.cpp \
${SPUC_DIR}/comm/carrier_nco.cpp \
${SPUC_DIR}/comm/commfunc.cpp \
${SPUC_DIR}/comm/convcode.cpp \
${SPUC_DIR}/comm/crc.cpp \
${SPUC_DIR}/comm/data_conv_encoder.cpp \
${SPUC_DIR}/comm/dvb_conv_encoder.cpp \
${SPUC_DIR}/comm/egolay.cpp \
${SPUC_DIR}/comm/fading_channel.cpp \
${SPUC_DIR}/comm/gf.cpp \
${SPUC_DIR}/comm/gfx.cpp \
${SPUC_DIR}/comm/hammcode.cpp \
${SPUC_DIR}/comm/modulator.cpp \
${SPUC_DIR}/comm/ofdm_data_encoder.cpp \
${SPUC_DIR}/comm/punct_convcode.cpp \
${SPUC_DIR}/comm/qam_mod.cpp \
${SPUC_DIR}/comm/qam_soft_decision.cpp \
${SPUC_DIR}/comm/qam_tx.cpp \
${SPUC_DIR}/comm/qpsk.cpp \
${SPUC_DIR}/comm/qpsk_ber_test.cpp \
${SPUC_DIR}/comm/qpsk_discriminators.cpp \
${SPUC_DIR}/comm/quad_data.cpp \
${SPUC_DIR}/comm/raised_cosine.cpp \
${SPUC_DIR}/comm/rec_syst_conv_code.cpp \
${SPUC_DIR}/comm/reed_solomon.cpp \
${SPUC_DIR}/comm/sequence.cpp \
${SPUC_DIR}/comm/sim_qam.cpp \
${SPUC_DIR}/comm/sim_qpsk.cpp \
${SPUC_DIR}/comm/turbo.cpp \
${SPUC_DIR}/comm/viterbi.cpp \
${SPUC_DIR}/equalizers/lms_dfe.cpp \
${SPUC_DIR}/equalizers/mlsd.cpp \
${SPUC_DIR}/equalizers/rls.cpp \
${SPUC_DIR}/examples/remez/remez_files.cpp \
${SPUC_DIR}/functions/misc.cpp \
${SPUC_DIR}/functions/scalfunc.cpp \
${SPUC_DIR}/functions/spuc_math.cpp \
${SPUC_DIR}/functions/window.cpp \
${SPUC_DIR}/generic/binary.cpp \
${SPUC_DIR}/generic/bool_nco.cpp \
${SPUC_DIR}/generic/butterworth_fir.cpp \
${SPUC_DIR}/generic/cic.cpp \
${SPUC_DIR}/generic/cordic.cpp \
${SPUC_DIR}/generic/fir.cpp \
${SPUC_DIR}/generic/gaussian_fir.cpp \
${SPUC_DIR}/generic/max_pn.cpp \
${SPUC_DIR}/generic/nco.cpp \
${SPUC_DIR}/generic/noise.cpp \
${SPUC_DIR}/generic/remezf.cpp \
${SPUC_DIR}/generic/remez_fir.cpp \
${SPUC_DIR}/generic/sigdel.cpp \
${SPUC_DIR}/generic/sum_and_dump.cpp \
${SPUC_DIR}/generic/vco.cpp \
${SPUC_DIR}/matrix/cholesky.cpp \
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
${SPUC_DIR}/examples/qpsk/qpsk_main.cpp \
${SPUC_DIR}/examples/qpsk/qpsk_variable.cpp \
${SPUC_DIR}/examples/qpsk/qpsk_variable_main.cpp \
${SPUC_DIR}/examples/qpsk/resampler.cpp \
${SPUC_DIR}/examples/qpsk/sim_qpsk_variable.cpp \
${SPUC_DIR}/examples/qpsk/timing_nco.cpp \
${SPUC_DIR}/specializations/scic.cpp \
${SPUC_DIR}/utility/spucassert.cpp

all:  libspuc.a 

doc:  
	doxygen doxconfig

.c:
	gcc ${CFLAGS} -c $@.c

#.cpp:
#	g++ ${CPPFLAGS} -c -o $@.o $@.cpp

${OBJDIR}/%.o: %.cpp
	g++ ${CPPFLAGS} -o $@ -c $<

clean: 
	rm *.o libspuc.* ./*/*.o 

libspuc.so: ${ALLOBJS} ${COBJS} ${GOBJS}
	${CC} ${COBJS} ${GOBJS} -shared -o libspuc.so

libspuc.a: ${ALLOBJS} ${COBJS} ${GOBJS}
	ar r libspuc.a ${COBJS} ${GOBJS}
	ranlib libspuc.a
