###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Required Environment variables (one of the following)
# PACKAGES_DIR  - Location of the MOOSE redistributable package
#
# Optional Environment variables
# MOOSE_DIR     - Root directory of the MOOSE project
# FRAMEWORK_DIR - Location of the MOOSE framework
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
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
TMAP8_SUBMODULE    := $(CURDIR)/tmap8
ifneq ($(wildcard $(TMAP8_SUBMODULE)/Makefile),)
  TMAP8_DIR        ?= $(TMAP8_SUBMODULE)
else
  TMAP8_DIR        ?= $(shell dirname `pwd`)/tmap8
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

# dep apps
CURRENT_DIR        := $(shell pwd)
APPLICATION_DIR    := $(CURRENT_DIR)/..
APPLICATION_NAME   := fenix
include            $(FRAMEWORK_DIR)/app.mk

APPLICATION_DIR    := $(CURRENT_DIR)
APPLICATION_NAME   := fenix-unit
BUILD_EXEC         := yes

DEP_APPS    ?= $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include $(FRAMEWORK_DIR)/app.mk

# Find all the Fenix unit test source files and include their dependencies.
fenix_unit_srcfiles := $(shell find $(CURRENT_DIR)/src -name "*.C")
fenix_unit_deps := $(patsubst %.C, %.$(obj-suffix).d, $(fenix_unit_srcfiles))
-include $(fenix_unit_deps)

###############################################################################
# Additional special case targets should be added here
