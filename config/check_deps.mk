define n


endef

# Set default value for Cardinal
CARDINAL_DIR         ?= $(CURDIR)/cardinal

# Check for CARDINAL_CONTENT within CARDINAL_DIR. This will cause Cardinal (and contribs)
# to either be built or skipped within the configure_cardinal.mk file.
CARDINAL_CONTENT     := $(shell ls $(CARDINAL_DIR) 2> /dev/null)

ifeq ($(CARDINAL_CONTENT),)
  $(warning $n"Cardinal does not seem to be available. If usage of Cardinal is desired within FENIX, make sure that either the submodule is checked out$nor that CARDINAL_DIR points to a location with the Cardinal source.")
else
  $(info FENIX is using Cardinal from     $(CARDINAL_DIR))
  ENABLE_CARDINAL    := yes
endif
