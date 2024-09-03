define n


endef

# Set default values for dependency locations, or use user-defined environment variable
TMAP8_DIR            ?= $(CURDIR)/tmap8
CARDINAL_DIR         ?= $(CURDIR)/cardinal

# Set defaults for optional dependencies
ENABLE_TMAP8         := yes
ENABLE_CARDINAL      := yes

# Check for content within dependency directories
TMAP8_CONTENT        := $(shell ls $(TMAP8_DIR) 2> /dev/null)
CARDINAL_CONTENT     := $(shell ls $(CARDINAL_DIR) 2> /dev/null)

ifeq ($(TMAP8_CONTENT),)
  $(warning $n"TMAP8 does not seem to be available. If usage of TMAP8 is desired within FENIX, make sure that either the submodule is checked out$nor that TMAP8_DIR points to a location with the TMAP8 source. $n$nIn the meantime, FENIX will be built without TMAP8.")
  ENABLE_TMAP8    := no
else
  $(info FENIX is using TMAP8 from        $(TMAP8_DIR))
endif

ifeq ($(CARDINAL_CONTENT),)
  $(warning $n"Cardinal does not seem to be available. If usage of Cardinal is desired within FENIX, make sure that either the submodule is checked out$nor that CARDINAL_DIR points to a location with the Cardinal source. $n$nIn the meantime, FENIX will be built without Cardinal.")
  ENABLE_CARDINAL    := no
else
  $(info FENIX is using Cardinal from     $(CARDINAL_DIR))
endif
