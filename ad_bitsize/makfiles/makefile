# BitSize 2003 - 2004 Altaf Abdul Gaffar 
# Copyright 2003-2004 Maxeler Technologies Inc. All Rights Reserved 


include makefile.bitdata

ASC_HOME= ../../ASCv0.5
include $(ASC_HOME)/makefile.config

BITSIZE_HOME=$(HOME)/Projects/bitsize/ad_bitsize
CXX_FLAGS+=-I$(BITSIZE_HOME)
include $(BITSIZE_HOME)/makefile.ascint

# =================
# STREAM INFOMATION
# =================

STREAM_INPUTS=1
STREAM_OUTPUTS=1

# ==============
# DEFAULT CONFIG
# ==============

DEFAULT = $(BITSIZE_DATA)

# ================
# SOFTWARE TARGETS
# ================

CXX_TARGET0 = asctest
CXX_SRC0 = asctest.cxx

# ===========
# EXPERIMENTS
# ===========

RUN0 = --ALT -DBITWIDTH="8;12;16" -DHWOPTIMIZE="THROUGHPUT"
RUN1 = --SW-SIM="asctest.dat" $(BITSIZE_DATA)

# ======
# GRAPHS
# ======

GRAPHS0 = -XBITWIDTH
GRAPHS1 = -XBITWIDTH

# ================
# REGRESSION TESTS
# ================

TEST0 = --SW-SIM="asctest.dat" --tolerance=0.02 $(BITSIZE_DATA) 


# ===
# END
# ===

include $(ASC_HOME)/makefile.std




