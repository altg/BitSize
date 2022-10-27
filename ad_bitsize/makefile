# $Id: makefile,v 1.45 2006/02/17 18:14:55 amag97 Exp $
# $Author: amag97 $

#include $(HOME)/makfiles/gcc_defs.mak
include makefile.config

TARGET_LIB=bitsize
#TARGET_LIB=advar

LIBNAME=lib$(TARGET_LIB).a

DEPS_ADVAR=advar.o bsclass.o 
DEPS_HWVAR=hwvar.o varnames.o
DEPS_DAG=dag.o node.o edge.o
DEPS_HWFLOAT=hwfloat.o
DEPS_HWFIX=hwfix.o bwoptfix.o
DEPS_HWINT=hwint.o
DEPS_BITSIZE= bitsize_class.o CmdLine.o
DEPS_BWOPT_PASSES= ad_dag.o optbw.o optbw_flt.o optbw_fx.o
DEPS_BWOPT_RANGE_PASSES= range_analysis.o affine_range.o sim_range.o interval_range.o
DEPS_BITSIZE_PASSES= bitsize_pass.o cost_function.o errormodel_affine.o ascbackend.o bw_results_gen.o mat_code_gen.o errormodel_affine2.o mat_code_gen_flt.o

DEPS_BITSIZE_PASSES+= errormodel_affine_cpp.o cost_func_area_cpp.o optengine.o optbw_cpp.o

DEPS_BITSIZE_PASSES+= cost_func_area_flt_cpp.o optbw_flt_cpp.o errmodel_affine_flt_cpp.o

DEPS_HWLNS=hwlns.o errmodel_affine_lns_cpp.o optbw_lns_cpp.o cost_func_area_lns_cpp.o

DEPS_ILP_PASS= ilpgen.o

DEPS_VHDL_GEN= gen_vhdl.o

DEPS_BITSIZE_POWER= pwrdata.o datalog.o cost_function_pwr.o  pwr_pass_glitch.o pwr_glitch_models.o pwr_glitch_costfunc.o

DEPS_MODELS= areamodel.o node_sched.o errormodel.o  $(DEPS_BITSIZE_PASSES)

DEPS_HFILES= varcollections.h bitsize_dbg.h bitsize_defs.h bitsize.h hwvector.h

#DEPS=$(DEPS_ADVAR) $(DEPS_HWVAR) $(DEPS_HWFLOAT) $(DEPS_HWFIX) $(DEPS_HWINT)

BITSIZE_DEPS = $(DEPS_ADVAR) $(DEPS_HWVAR) $(DEPS_HWFLOAT) 
BITSIZE_DEPS += $(DEPS_HWFIX) $(DEPS_DAG) $(DEPS_HWINT) $(DEPS_BITSIZE) $(DEPS_MODELS) 

BITSIZE_DEPS += $(DEPS_BWOPT_PASSES) $(DEPS_BWOPT_RANGE_PASSES) 

BITSIZE_DEPS += $(DEPS_BITSIZE_POWER) $(DEPS_ILP_PASS) $(DEPS_VHDL_GEN)

BITSIZE_DEPS += $(DEPS_HWLNS)


CPPFILES=$(DEPS:.o=.cpp)


HFILES=$(DEPS:.o=.h) $(DEPS_HFILES)

LIBDIR_INST= $(HOME)/lib

INSTROOT= $(HOME)

LIBDEFS= -D ASCLIB

ASA_HOME=../MyASA

CFLAGS+=  -D _BITSIZE_DEBUG_ -I $(ASA_HOME)
#-D _BITSIZE_DEBUG_LOG


TAR=tar

#ETAGS=/cygdrive/c/usr/XEmacs/bin/i686-pc-cygwin/etags.exe
ETAGS=etags	# Exuberant ctags

ifeq ($(TARGET_LIB),bitsize)
	DEPS= $(BITSIZE_DEPS)
else 
	DEPS= $(DEPS_ADVAR)
endif

#lib: $(LIBNAME)

lib: $(DEPS)
	$(AR) $(ARFLAGS) $(LIBNAME) $^

slib: $(DEPS)
#	g++ -shared -o cygbitsize.dll --out-implib=libbitsize.dll.a $^ --enable-auto-import
#	g++ -shared -o cygbitsize.dll \
#	-Wl,--out-implib=libbitsize.dll.a \
#	-Wl,--enable-auto-import \
#	-Wl,--whole-archive $^


%.o: %.cpp
	$(CC) -c $< -o $@ $(LIBDEFS) $(CFLAGS) $(INCDIRS)


tar_dist:
	$(TAR) --ignore-failed-read -zcvf  bitsize.tar.gz makefile makefile.config $(CPPFILES) $(HFILES)

testrun: testharness.exe
	./testharness
	make test_ann3.exe
	./test_ann3


etags:
	rm -f TAGS
	find . -name '*.cpp' -o -name '*.h' -o -name '*.c' \
	| xargs $(ETAGS) --append

clean:
	$(RM) $(LIBNAME) *.o

