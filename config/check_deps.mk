define n


endef

# Set default values for Cardinal location and ENABLE_CARDINAL.
CARDINAL_DIR         ?= $(CURDIR)/cardinal
ENABLE_CARDINAL      := yes

# Check for CARDINAL_CONTENT within CARDINAL_DIR.
CARDINAL_CONTENT     := $(shell ls $(CARDINAL_DIR) 2> /dev/null)

ifeq ($(CARDINAL_CONTENT),)
  $(warning $n"Cardinal does not seem to be available. If usage of Cardinal is desired within FENIX, make sure that either the submodule is checked out$nor that CARDINAL_DIR points to a location with the Cardinal source. $n In the meantime, FENIX will be built without Cardinal.")
  ENABLE_CARDINAL    := no
else
  $(info FENIX is using Cardinal from     $(CARDINAL_DIR))
endif
