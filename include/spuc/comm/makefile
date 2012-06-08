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
${SPUC_DIR}/comm/bch.o \
${SPUC_DIR}/comm/bpsk_ber_test.o \
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
${SPUC_DIR}/comm/qam_conv_decoder.o \
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

ACOBJS = ${SPUC_DIR}/array/array1d_functions.o \
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

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp

all:  ${COBJS}

clean: 
	rm *.o *.exe

