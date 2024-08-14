####################################################################################
################################## FENIX Makefile ##################################
####################################################################################
#
# Optional Environment variables:
#
# MOOSE_DIR        - Root directory of MOOSE
# TMAP8_DIR        - Root directory of TMAP8
# CARDINAL_DIR     - Root directory of Cardinal
# OPENMC_DIR       - Root directory of OpenMC
# DAGMC_DIR        - Root directory of DagMC
# MOAB_DIR         - Root directory of MOAB
#
# EIGEN3_DIR       - Root directory of Eigen3 (should contain FindEigen3.cmake).
#                    This is needed for DagMC.
#
# OpenMC uses HDF5; below are influential environment variables for that
# installation. None of these need to be set if HDF5 is being pulled from
# a manual PETSc installation, and HDF5_DIR is provided directly from conda.
#
# HDF5_DIR         - Root directory of HDF5
# HDF5_INCLUDE_DIR - Root directory for HDF5 headers (default: $(HDF5_DIR)/include)
# HDF5_LIBDIR      - Root directory for HDF5 libraries (default: $(HDF5_DIR)/lib)
# PETSC_DIR        - Root directory for PETSc (default: $(MOOSE_DIR)/petsc)
# PETSC_ARCH       - PETSc architecture (default: arch-moose)
####################################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
# If it doesn't exist, and MOOSE_DIR is not set, then look for it adjacent to the application
MOOSE_SUBMODULE       := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR           ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR           ?= $(shell dirname `pwd`)/moose
endif

# Use the TMAP8 submodule if it exists and TMAP8_DIR is not set
# If it doesn't exist, and TMAP8_DIR is not set, then look for it adjacent to the application
TMAP8_SUBMODULE    := $(CURDIR)/tmap8
ifneq ($(wildcard $(TMAP8_SUBMODULE)/Makefile),)
  TMAP8_DIR        ?= $(TMAP8_SUBMODULE)
else
  TMAP8_DIR        ?= $(shell dirname `pwd`)/tmap8
endif

# Use the Cardinal submodule if it exists and CARDINAL_DIR is not set
# If it doesn't exist, and CARDINAL_DIR is not set, then look for it adjacent to the application
CARDINAL_SUBMODULE    := $(CURDIR)/cardinal
ifneq ($(wildcard $(CARDINAL_SUBMODULE)/Makefile),)
  CARDINAL_DIR        ?= $(CARDINAL_SUBMODULE)
else
  CARDINAL_DIR        ?= $(shell dirname `pwd`)/cardinal
endif

# Cardinal contrib variables
CARDINAL_CONTRIB_DIR           := $(CARDINAL_DIR)/contrib
CARDINAL_CONTRIB_INSTALL_DIR   ?= $(CARDINAL_DIR)/install
OPENMC_INSTALL_DIR             := $(CARDINAL_CONTRIB_INSTALL_DIR)
OPENMC_LIBDIR                  := $(OPENMC_INSTALL_DIR)/lib
OPENMC_BUILDDIR                := $(CARDINAL_DIR)/build/openmc
OPENMC_INSTALL_DIR             := $(CARDINAL_CONTRIB_INSTALL_DIR)
OPENMC_INCLUDES                := -I$(OPENMC_INSTALL_DIR)/include
OPENMC_LIBDIR                  := $(OPENMC_INSTALL_DIR)/lib
OPENMC_LIB                     := $(OPENMC_LIBDIR)/libopenmc.so

PETSC_DIR           ?= $(MOOSE_DIR)/petsc
PETSC_ARCH          ?= arch-moose
LIBMESH_DIR         ?= $(MOOSE_DIR)/libmesh/installed/

# This is the Eigen3 location on CIVET. If you are using MOOSE's conda environment,
# you don't need to set these variables, because conda sets them for you. The only
# scenario where you might need to manually set these is if you're not using the
# conda environment. You will get a compile error about FindEigen3.cmake if you
# do indeed need to set these.
EIGEN3_DIR          ?= $(LIBMESH_DIR)/include
Eigen3_DIR          ?= $(EIGEN3_DIR)

# If HDF5_DIR is set, use those settings to link HDF5 to OpenMC.
# Otherwise, use where PETSc will put HDF5 if downloading it.
ifeq ($(HDF5_DIR),)
  HDF5_DIR          := $(PETSC_DIR)/$(PETSC_ARCH)
  export HDF5_DIR
endif

# HDF5_ROOT is used in makefiles for the Cardinal third-party dependencies.
# Set it using HDF5_DIR
HDF5_ROOT          := $(HDF5_DIR)
export HDF5_ROOT

HDF5_INCLUDE_DIR    ?= $(HDF5_DIR)/include
HDF5_LIBDIR         ?= $(HDF5_DIR)/lib
# This is used in $(FRAMEWORK_DIR)/build.mk
HDF5_INCLUDES       := -I$(HDF5_INCLUDE_DIR) -I$(HDF5_DIR)/include

# BUILD_TYPE will be passed to CMake via CMAKE_BUILD_TYPE
ifeq ($(METHOD),dbg)
	BUILD_TYPE := Debug
else
	BUILD_TYPE := Release
endif

DAGMC_BUILDDIR := $(CARDINAL_DIR)/build/DAGMC
DAGMC_INSTALL_DIR := $(CARDINAL_CONTRIB_INSTALL_DIR)

MOAB_BUILDDIR := $(CARDINAL_DIR)/build/moab
MOAB_INSTALL_DIR := $(CARDINAL_CONTRIB_INSTALL_DIR)

# Set default values for all third party dependencies
OPENMC_DIR          ?= $(CARDINAL_CONTRIB_DIR)/openmc
DAGMC_DIR           ?= $(CARDINAL_CONTRIB_DIR)/DAGMC
MOAB_DIR            ?= $(CARDINAL_CONTRIB_DIR)/moab

# Use compiler info discovered by PETSC
ifeq ($(PETSC_ARCH),)
	include $(PETSC_DIR)/$(PETSC_ARCH)/lib/petsc/conf/petscvariables
else
	include $(PETSC_DIR)/lib/petsc/conf/petscvariables
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

################################## MODULES ####################################
# To use certain physics included with MOOSE, set variables below to
# yes as needed.  Or set ALL_MODULES to yes to turn on everything (overrides
# other set variables).

ALL_MODULES                 := no

CHEMICAL_REACTIONS          := yes
CONTACT                     := no
ELECTROMAGNETICS            := yes
EXTERNAL_PETSC_SOLVER       := no
FLUID_PROPERTIES            := yes
FSI                         := no
FUNCTIONAL_EXPANSION_TOOLS  := no
GEOCHEMISTRY                := no
HEAT_TRANSFER               := yes
LEVEL_SET                   := no
MISC                        := yes
NAVIER_STOKES               := yes
OPTIMIZATION                := no
PERIDYNAMICS                := no
PHASE_FIELD                 := yes
POROUS_FLOW                 := no
RAY_TRACING                 := yes
REACTOR                     := yes
RDG                         := yes
RICHARDS                    := no
SCALAR_TRANSPORT            := yes
SOLID_MECHANICS             := yes
SOLID_PROPERTIES            := yes
STOCHASTIC_TOOLS            := yes
THERMAL_HYDRAULICS          := yes
XFEM                        := no

include $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# Add OpenMC flags
ADDITIONAL_CPPFLAGS += $(HDF5_INCLUDES) $(OPENMC_INCLUDES)
libmesh_CXXFLAGS    += -DENABLE_OPENMC_COUPLING

# Add DagMC flags (-DDAGMC is used in OpenMC)
libmesh_CXXFLAGS    += -DENABLE_DAGMC -DDAGMC

# Configure and build MOAB, DagMC, and then OpenMC
include          $(CARDINAL_DIR)/config/moab.mk
include          $(CARDINAL_DIR)/config/dagmc.mk

# autoconf-archive puts some arguments (e.g. -std=c++17) into the compiler
# variable rather than the compiler flags variable.
#
# cmake allows this, but wants any compiler arguments to be
# semicolon-separated, not space-separated
# libmesh_CC, etc., were defined in build.mk
space := $(subst ,, )
LIBMESH_CC_LIST := $(subst $(space),;,$(libmesh_CC))
LIBMESH_CXX_LIST := $(subst $(space),;,$(libmesh_CXX))
LIBMESH_F90_LIST := $(subst $(space),;,$(libmesh_F90))

ENABLE_DAGMC     := ON
include            $(CARDINAL_DIR)/config/openmc.mk

# TMAP8
APPLICATION_DIR    := $(TMAP8_DIR)
APPLICATION_NAME   := tmap8
BUILD_EXEC         := no
GEN_REVISION       := no
include            $(FRAMEWORK_DIR)/app.mk

# Cardinal
APPLICATION_DIR    := $(CARDINAL_DIR)
APPLICATION_NAME   := cardinal
BUILD_EXEC         := no
GEN_REVISION       := yes
include            $(FRAMEWORK_DIR)/app.mk

# app_objects are defined in moose.mk and built according to the rules in build.mk
# We need to build these first so we get include dirs
$(app_objects): build_moab build_dagmc build_openmc
$(test_objects): build_moab build_dagmc build_openmc

# FENIX
APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := fenix
BUILD_EXEC         := yes
GEN_REVISION       := yes

# Cardinal dependency libraries
ADDITIONAL_LIBS := -L$(CARDINAL_DIR)/lib $(CC_LINKER_SLFLAG)$(CARDINAL_DIR)/lib \
                   -L$(OPENMC_LIBDIR) -lopenmc -lhdf5_hl -ldagmc -lMOAB \
                   $(CC_LINKER_SLFLAG)$(OPENMC_LIBDIR)

include            $(FRAMEWORK_DIR)/app.mk


# Cardinal contrib flags used in app.mk targets
CARDINAL_EXTERNAL_FLAGS := -L$(CARDINAL_DIR)/lib $(CC_LINKER_SLFLAG)$(CARDINAL_DIR)/lib $(BLASLAPACK_LIB) $(PETSC_EXTERNAL_LIB_BASIC) -L$(OPENMC_LIBDIR)  -L$(HDF5_LIBDIR) -lopenmc -ldagmc -lMOAB $(CC_LINKER_SLFLAG)$(OPENMC_LIBDIR) $(CC_LINKER_SLFLAG)$(HDF5_LIBDIR)

# EXTERNAL_FLAGS is used in rules for app.mk
$(app_LIB): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
$(app_test_LIB): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
$(app_EXEC): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
