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
MOOSE_SUBMODULE    := $(CURDIR)/../moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/../moose
endif
FRAMEWORK_DIR      ?= $(MOOSE_DIR)/framework
###############################################################################

# Use the TMAP8 submodule if it exists and TMAP8_DIR is not set
# If it doesn't exist, and TMAP8_DIR is not set, then look for it adjacent to the application
TMAP8_SUBMODULE    := $(CURDIR)/../tmap8
ifneq ($(wildcard $(TMAP8_SUBMODULE)/Makefile),)
  TMAP8_DIR        ?= $(TMAP8_SUBMODULE)
else
  TMAP8_DIR        ?= $(shell dirname `pwd`)/../tmap8
endif

# Check for optional dependencies and, if found, configure for building.
# Set default for ENABLE_CARDINAL, and override default CARDINAL_DIR for unit build.
ENABLE_CARDINAL := no
CARDINAL_DIR ?= $(CURDIR)/../cardinal
include $(CURDIR)/../config/check_deps.mk

ifeq ($(ENABLE_CARDINAL),yes)
  include $(CURDIR)/../config/configure_cardinal.mk
endif

# framework
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

include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# Extra stuff for GTEST
ADDITIONAL_INCLUDES := -I$(FRAMEWORK_DIR)/contrib/gtest
ADDITIONAL_LIBS     := $(FRAMEWORK_DIR)/contrib/gtest/libgtest.la

# TMAP8
APPLICATION_DIR    := $(TMAP8_DIR)
APPLICATION_NAME   := tmap8
BUILD_EXEC         := no
GEN_REVISION       := no
include            $(FRAMEWORK_DIR)/app.mk

# Cardinal
ifeq ($(ENABLE_CARDINAL),yes)
  include $(CURDIR)/../config/build_cardinal.mk
endif

# FENIX
CURRENT_DIR        := $(shell pwd)
APPLICATION_DIR    := $(CURRENT_DIR)/..
APPLICATION_NAME   := fenix
GEN_REVISION       := yes
include            $(FRAMEWORK_DIR)/app.mk

APPLICATION_DIR    := $(CURRENT_DIR)
APPLICATION_NAME   := fenix-unit
BUILD_EXEC         := yes
DEP_APPS    ?= $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))

# Cardinal dependency libraries needed for FENIX linking
ifeq ($(ENABLE_CARDINAL),yes)
  ADDITIONAL_LIBS := -L$(CARDINAL_DIR)/lib $(CC_LINKER_SLFLAG)$(CARDINAL_DIR)/lib \
                     -L$(OPENMC_LIBDIR) -lopenmc -lhdf5_hl -ldagmc -lMOAB \
                     $(CC_LINKER_SLFLAG)$(OPENMC_LIBDIR)
endif

include $(FRAMEWORK_DIR)/app.mk

# External flags
ifeq ($(ENABLE_CARDINAL),yes)
  include $(CURRENT_DIR)/../config/external_cardinal_flags.mk
endif

# Find all the Fenix unit test source files and include their dependencies.
fenix_unit_srcfiles := $(shell find $(CURRENT_DIR)/src -name "*.C")
fenix_unit_deps := $(patsubst %.C, %.$(obj-suffix).d, $(fenix_unit_srcfiles))
-include $(fenix_unit_deps)
