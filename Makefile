os = $(shell uname -s)

INCFLAGS      = -I$(ROOTSYS)/include -I$(PYTHIA8DIR)/include/Pythia8 -I$(PYTHIA8DIR)/include -I$(FASTJET)/include

ifeq ($(os),Linux)
CXXFLAGS      = -lrt -Wl,--no-as-needed -lHist -lCore -lMathCore
else
CXXFLAGS      = -O -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init
## for debugging:
# CXXFLAGS      = -g -O0 -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init
endif

ifeq ($(os),Linux)
LDFLAGS       = -g
LDFLAGSS      = -g --shared 
else
LDFLAGS       = -O -Xlinker -bind_at_load -flat_namespace
LDFLAGSS      = -flat_namespace -undefined suppress
LDFLAGSSS     = -bundle
endif

ifeq ($(os),Linux)
CXX          = g++ -std=c++11
else
CXX          = clang
endif


ROOTLIBS      = $(shell root-config --libs)

LIBPATH       = $(ROOTLIBS) -L$(PYTHIA8LIB) -L$(FASTJETLIB)
LIBS          = -lpythia8 -lfastjet -lfastjetplugins -lfastjettools -lsiscone -lsiscone_spherical  -lgfortran -ldl

# for cleanup
SDIR          = src
ODIR          = src/obj
BDIR          = bin


###############################################################################
################### Remake when these headers are touched #####################
###############################################################################
INCS = $(SDIR)/PythiaTree.h 

###############################################################################
# standard rules
$(ODIR)/%.o : $(SDIR)/%.cxx $(INCS)
	@echo 
	@echo COMPILING
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

$(BDIR)/%  : $(ODIR)/%.o
	@echo 
	@echo LINKING
	$(CXX) $(LDFLAGS) $(LIBPATH) $(LIBS) $^ -o $@
###############################################################################

###############################################################################
############################# Main Targets ####################################
###############################################################################
all    : $(BDIR)/PythiaTree  $(BDIR)  $(BDIR)/makeHistosPythiaTree   $(BDIR)/readPythiaTree
