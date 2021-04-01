###########################################################################
#
#  Program:         ARPACK
#
#  Module:          ARmake.inc
#
#  Purpose:         Top-level Definitions
#
#  Creation date:   February 22, 1996
#
#  Modified:
#
#  Send bug reports, comments or suggestions to arpack@caam.rice.edu
#
############################################################################
#
# %---------------------------------%
# |  SECTION 1: PATHS AND LIBRARIES |
# %---------------------------------%
#
#
# %--------------------------------------%
# | You should change the definition of  |
# | home if ARPACK is built some place   | 
# | other than your home directory.      |
# %--------------------------------------%
#
#homedir = /home/nao/catkin_ws/src/navlibs/ARLASLU

homedir = $(PWD)
#
#  %--------------------------------------%
#  | The platform identifier to suffix to |
#  | the end of library names             |
#  %--------------------------------------%
#
PLAT = lin64
#
#  %------------------------------------------------------%
#  | The directories to find the various pieces of ARPACK |
#  %------------------------------------------------------%
#
BLASdir      = $(homedir)/BLAS
LAPACKdir    = $(homedir)/LAPACK
ARPACKdir    = $(homedir)/ARPACK
SuperLUdir   = $(homedir)/SuperLU
UTILdir      = $(homedir)/UTIL
CBLASdir     = $(homedir)/CBLAS
#
DIRS        = $(BLASdir) $(LAPACKdir) $(UTILdir) $(ARPACKdir) $(CBLASdir) $(SuperLUdir)
#

# %---------------------------------------------------%
# | The name of the libraries to be created/linked to |
# %---------------------------------------------------%
#
ARPACKLIB  = $(homedir)/../libaplpsl_$(PLAT).a
LAPACKLIB = $(homedir)/../libaplpsl_$(PLAT).a
BLASLIB =  $(homedir)/../libaplpsl_$(PLAT).a
SUPERLULIB = $(homedir)/../libaplpsl_$(PLAT).a
ARLASULIB = $(homedir)/libaplpsl_$(PLAT).a
#
ALIBS =  $(ARPACKLIB) $(LAPACKLIB) $(BLASLIB) $(SUPERLULIB)
#
# 
# %---------------------------------------------------------%
# |                  SECTION 2: COMPILERS                   |
# |                                                         |
# | The following macros specify compilers, linker/loaders, |
# | the archiver, and their options.  You need to make sure |
# | these are correct for your system.                      |
# %---------------------------------------------------------%
#
#
# %------------------------------%
# | Make our own suffixes' list. |
# %------------------------------%
#
.SUFFIXES:
.SUFFIXES:	.f	.o    .c
#
# %------------------%
# | Default command. |
# %------------------%
#
.DEFAULT:
	@$(ECHO) "Unknown target $@, try:  make help"
#

# %-----------------------------------------%
# | Various compilation programs and flags. |
# | You need to make sure these are correct |
# | for your system.                        |
# %-----------------------------------------%
#
FC      = gfortran
FFLAGS	= -O -fPIC
LDFLAGS = 
CD      = cd

ECHO    = echo

LN      = ln
LNFLAGS = -s

#MAKE    = /bin/make
MAKE    = make

RM      = rm
RMFLAGS = -f

CC           = gcc 
CFLAGS       = -DPRNTlevel=0 -O -fPIC 
NOOPTS       = 


SHELL   = /bin/sh
#
#  %----------------------------------------------------------------%
#  | The archiver and the flag(s) to use when building an archive   |
#  | (library).  Also the ranlib routine.  If your system has no    |
#  | ranlib, set RANLIB = touch.                                    |
#  %----------------------------------------------------------------%
#
AR = ar
ARFLAGS = rv
RANLIB   = ranlib
ARCH         = ar
ARCHFLAGS    = cr
#
# %----------------------------------%
# | This is the general help target. |
# %----------------------------------%
#
help:
	@$(ECHO) "usage: make ?"
