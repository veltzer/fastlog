include /usr/share/templar/make/Makefile
##############
# PARAMETERS #
##############
# should we show commands executed ?
DO_MKDBG:=0
# should we depend on the date of the makefile itself ?
DO_MAKEDEPS:=1
# folder where the sources are...
SRCDIR:=src
# name of the library to create
LIBNAME:=fastlog
# compiler to use...
CC:=gcc
# basic flags to use
BASE_FLAGS:=-O2 -fpic -Wall -Werror -std=gnu99
# do you want debugging enabled?
DO_DEBUG:=0
# what is the web folder ?
WEB_DIR:=../fastlog-gh-pages
# which folders to copy for web?
COPY_FOLDERS:=web static

########
# BODY #
########
ifeq ($(DO_MKDBG),1)
Q=
# we are not silent in this branch
else # DO_MKDBG
Q=@
#.SILENT:
endif # DO_MKDBG

ifeq ($(DO_DEBUG),1)
BASE_FLAGS:=$(BASE_FLAGS) -g2
endif # DO_DEBUG

ALL_DEP:=
ifeq ($(DO_MAKEDEPS),1)
	ALL_DEP:=$(ALL_DEP) Makefile
endif

LIB:=lib$(LIBNAME).so
SRC:=$(shell find $(SRCDIR) -type f -and -name "*.c")
OBJ:=$(addsuffix .o,$(basename $(SRC)))
CFLAGS:=$(BASE_FLAGS) -I$(SRCDIR) -Itest
LDFLAGS:=-shared -fpic
ALL_DEPS:=Makefile
BIN:=bin/fastlog_test_speed bin/fastlog_test_basic bin/fastlog_test_crash
BINLD:=-L. -l$(LIBNAME) -lpthread

.PHONY: all
all: $(LIB) $(BIN) $(ALL_DEPS)

# binaries and libraries

$(LIB): $(OBJ) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJ)

# special targets

.PHONY: debug_me
debug_me: $(ALL_DEPS)
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))

.PHONY: clean_me
clean_me: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)rm -f $(OBJ) $(LIB) $(BIN)

.PHONY: run
run: $(BIN) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=. ; ./bin/fastlog_test_speed

.PHONY: run_debug
run_debug: $(BIN) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=. ; gdb ./bin/fastlog_test_speed

# rules

$(OBJ): %.o: %.c $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<
$(BIN): bin/%: test/%.c $(LIB) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ $< $(BINLD)

# web page
.PHONY: install
install: $(ALL) $(ALL_DEP)
	$(info doing [$@])
	$(Q)rm -rf $(WEB_DIR)/*
	$(Q)for folder in $(COPY_FOLDERS); do cp -r $$folder $(WEB_DIR); done
	$(Q)cp support/redirector.html $(WEB_DIR)/index.html
	cd $(WEB_DIR); git commit -a -m "new version"; git push
