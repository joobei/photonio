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

COBJS = ${SPUC_DIR}/comm/baud_eq_env.o \
${SPUC_DIR}/comm/bped.o \
${SPUC_DIR}/comm/carrier_nco.o \
${SPUC_DIR}/comm/commfunc.o \
${SPUC_DIR}/comm/convcode.o \
${SPUC_DIR}/comm/crc.o \
${SPUC_DIR}/comm/data_conv_encoder.o \
${SPUC_DIR}/comm/dvb_conv_encoder.o \
${SPUC_DIR}/comm/egolay.o \
${SPUC_DIR}/comm/fading_channel.o \
${SPUC_DIR}/comm/gf.o \
${SPUC_DIR}/comm/gfx.o \
${SPUC_DIR}/comm/hammcode.o \
${SPUC_DIR}/comm/ofdm_data_encoder.o \
${SPUC_DIR}/comm/punct_convcode.o \
${SPUC_DIR}/comm/qam_mod.o \
${SPUC_DIR}/comm/qam_soft_decision.o \
${SPUC_DIR}/comm/qam_tx.o \
${SPUC_DIR}/comm/qpsk.o \
${SPUC_DIR}/comm/qpsk_ber_test.o \
${SPUC_DIR}/comm/qpsk_discriminators.o \
${SPUC_DIR}/comm/quad_data.o \
${SPUC_DIR}/comm/raised_cosine.o \
${SPUC_DIR}/comm/rec_syst_conv_code.o \
${SPUC_DIR}/comm/sequence.o \
${SPUC_DIR}/comm/sim_qpsk.o \
${SPUC_DIR}/comm/turbo.o \
${SPUC_DIR}/comm/viterbi.o \
${SPUC_DIR}/equalizers/lms_dfe.o \
${SPUC_DIR}/equalizers/mlsd.o \
${SPUC_DIR}/equalizers/rls.o \
${SPUC_DIR}/examples/baud_equalizer_sim.o \
${SPUC_DIR}/examples/mlse/mlse_test.o \
${SPUC_DIR}/examples/remez/remez_files.o \
${SPUC_DIR}/examples/remez/test_remez.o \
${SPUC_DIR}/examples/test_rls/rls_test.o \
${SPUC_DIR}/functions/misc.o \
${SPUC_DIR}/functions/spuc_math.o \
${SPUC_DIR}/generic/binary.o \
${SPUC_DIR}/generic/bool_nco.o \
${SPUC_DIR}/generic/butterworth_fir.o \
${SPUC_DIR}/generic/cic.o \
${SPUC_DIR}/generic/cordic.o \
${SPUC_DIR}/generic/fir.o \
${SPUC_DIR}/generic/max_pn.o \
${SPUC_DIR}/generic/nco.o \
${SPUC_DIR}/generic/noise.o \
${SPUC_DIR}/generic/random.o \
${SPUC_DIR}/generic/sigdel.o \
${SPUC_DIR}/generic/sum_and_dump.o \
${SPUC_DIR}/generic/vco.o \
${SPUC_DIR}/matrix/converters.o \
${SPUC_DIR}/matrix/det.o \
${SPUC_DIR}/matrix/eigen.o \
${SPUC_DIR}/matrix/fastmath.o \
${SPUC_DIR}/matrix/inv.o \
${SPUC_DIR}/matrix/ls_solve.o \
${SPUC_DIR}/matrix/svd.o \
${SPUC_DIR}/matrix/vector.o \
${SPUC_DIR}/qpsk/qpsk_main.o \
${SPUC_DIR}/qpsk/resampler.o \
${SPUC_DIR}/qpsk/timing_nco.o \
${SPUC_DIR}/specializations/scic.o \
${SPUC_DIR}/utility/spucassert.o

ALLOBJS = ${SPUC_DIR}/array/array1d_functions.o \
${SPUC_DIR}/array/array2d_functions.o \
${SPUC_DIR}/comm/baud_eq_env.o \
${SPUC_DIR}/comm/bch.o \
${SPUC_DIR}/comm/bped.o \
${SPUC_DIR}/comm/bpsk_ber_test.o \
${SPUC_DIR}/comm/carrier_nco.o \
${SPUC_DIR}/comm/commfunc.o \
${SPUC_DIR}/comm/convcode.o \
${SPUC_DIR}/comm/crc.o \
${SPUC_DIR}/comm/data_conv_encoder.o \
${SPUC_DIR}/comm/dvb_conv_encoder.o \
${SPUC_DIR}/comm/egolay.o \
${SPUC_DIR}/comm/fading_channel.o \
${SPUC_DIR}/comm/gf.o \
${SPUC_DIR}/comm/gfx.o \
${SPUC_DIR}/comm/hammcode.o \
${SPUC_DIR}/comm/modulator.o \
${SPUC_DIR}/comm/ofdm_data_encoder.o \
${SPUC_DIR}/comm/punct_convcode.o \
${SPUC_DIR}/comm/qam_mod.o \
${SPUC_DIR}/comm/qam_soft_decision.o \
${SPUC_DIR}/comm/qam_tx.o \
${SPUC_DIR}/comm/qpsk.o \
${SPUC_DIR}/comm/qpsk_ber_test.o \
${SPUC_DIR}/comm/qpsk_discriminators.o \
${SPUC_DIR}/comm/quad_data.o \
${SPUC_DIR}/comm/raised_cosine.o \
${SPUC_DIR}/comm/rec_syst_conv_code.o \
${SPUC_DIR}/comm/reed_solomon.o \
${SPUC_DIR}/comm/sequence.o \
${SPUC_DIR}/comm/sim_qam.o \
${SPUC_DIR}/comm/sim_qpsk.o \
${SPUC_DIR}/comm/turbo.o \
${SPUC_DIR}/comm/viterbi.o \
${SPUC_DIR}/equalizers/lms_dfe.o \
${SPUC_DIR}/equalizers/mlsd.o \
${SPUC_DIR}/equalizers/rls.o \
${SPUC_DIR}/examples/baud_equalizer_sim.o \
${SPUC_DIR}/examples/equalizers_sim.o \
${SPUC_DIR}/examples/main_equalizer.o \
${SPUC_DIR}/examples/mlse/mlse_test.o \
${SPUC_DIR}/examples/reference_main.o \
${SPUC_DIR}/examples/remez/create_remez_fir.o \
${SPUC_DIR}/examples/remez/remez_files.o \
${SPUC_DIR}/examples/remez/test_remez.o \
${SPUC_DIR}/examples/test_rls/rls_test.o \
${SPUC_DIR}/functions/misc.o \
${SPUC_DIR}/functions/scalfunc.o \
${SPUC_DIR}/functions/spuc_math.o \
${SPUC_DIR}/functions/window.o \
${SPUC_DIR}/generic/binary.o \
${SPUC_DIR}/generic/bool_nco.o \
${SPUC_DIR}/generic/butterworth_fir.o \
${SPUC_DIR}/generic/cic.o \
${SPUC_DIR}/generic/cordic.o \
${SPUC_DIR}/generic/fir.o \
${SPUC_DIR}/generic/gaussian_fir.o \
${SPUC_DIR}/generic/max_pn.o \
${SPUC_DIR}/generic/nco.o \
${SPUC_DIR}/generic/noise.o \
${SPUC_DIR}/generic/random.o \
${SPUC_DIR}/generic/remezf.o \
${SPUC_DIR}/generic/remez_fir.o \
${SPUC_DIR}/generic/sigdel.o \
${SPUC_DIR}/generic/sum_and_dump.o \
${SPUC_DIR}/generic/vco.o \
${SPUC_DIR}/matrix/cholesky.o \
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
${SPUC_DIR}/qpsk/qpsk_main.o \
${SPUC_DIR}/qpsk/qpsk_variable.o \
${SPUC_DIR}/qpsk/qpsk_variable_main.o \
${SPUC_DIR}/qpsk/resampler.o \
${SPUC_DIR}/qpsk/sim_qpsk_variable.o \
${SPUC_DIR}/qpsk/timing_nco.o \
${SPUC_DIR}/specializations/fnco.o \
${SPUC_DIR}/specializations/scic.o \
${SPUC_DIR}/utility/matlab_engine.o \
${SPUC_DIR}/utility/spucassert.o

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
${SPUC_DIR}/examples/baud_equalizer_sim.cpp \
${SPUC_DIR}/examples/equalizers_sim.cpp \
${SPUC_DIR}/examples/main_equalizer.cpp \
${SPUC_DIR}/examples/mlse/mlse_test.cpp \
${SPUC_DIR}/examples/reference_main.cpp \
${SPUC_DIR}/examples/remez/create_remez_fir.cpp \
${SPUC_DIR}/examples/remez/remez_files.cpp \
${SPUC_DIR}/examples/remez/test_remez.cpp \
${SPUC_DIR}/examples/test_rls/rls_test.cpp \
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
${SPUC_DIR}/generic/random.cpp \
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
${SPUC_DIR}/qpsk/qpsk_main.cpp \
${SPUC_DIR}/qpsk/qpsk_variable.cpp \
${SPUC_DIR}/qpsk/qpsk_variable_main.cpp \
${SPUC_DIR}/qpsk/resampler.cpp \
${SPUC_DIR}/qpsk/sim_qpsk_variable.cpp \
${SPUC_DIR}/qpsk/timing_nco.cpp \
${SPUC_DIR}/specializations/fnco.cpp \
${SPUC_DIR}/specializations/scic.cpp \
${SPUC_DIR}/utility/matlab_engine.cpp \
${SPUC_DIR}/utility/spucassert.cpp

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp

objs: ${CSRC}

all:  libspuc.so 

clean: 
	rm *.o *.exe

libspuc.so: ${COBJS}
	g++ ${COBJS} -shared -o libspuc.so