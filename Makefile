##############
# PARAMETERS #
##############
# should we show commands executed ?
DO_MKDBG:=0
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
# do you want to do tools?
DO_TOOLS:=1
# do you want dependency on the Makefile itself ?
DO_ALLDEP:=1

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

# dependency on the makefile itself
ifeq ($(DO_ALLDEP),1)
.EXTRA_PREREQS+=$(foreach mk, ${MAKEFILE_LIST},$(abspath ${mk}))
endif

ifeq ($(DO_TOOLS),1)
.EXTRA_PREREQS+=tools.stamp
endif # DO_TOOLS

LIB:=out/lib/lib$(LIBNAME).so
SRC:=$(shell find $(SRCDIR) -type f -and -name "*.c")
OBJ:=$(addprefix out/obj/, $(addsuffix .o,$(basename $(SRC))))
CFLAGS:=$(BASE_FLAGS) -I$(SRCDIR) -Itest
LDFLAGS:=-shared -fpic
BIN:=out/bin/fastlog_test_speed out/bin/fastlog_test_basic out/bin/fastlog_test_crash
BINLD:=-Lout/lib -l$(LIBNAME) -lpthread

.PHONY: all
all: $(LIB) $(BIN)

# binaries and libraries

$(LIB): $(OBJ)
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJ)

# special targets

.PHONY: debug_me
debug_me:
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))

.PHONY: clean_me
clean_me:
	$(info doing [$@])
	$(Q)rm -f $(OBJ) $(LIB) $(BIN)

.PHONY: run
run: $(BIN)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=out/lib ; ./out/bin/fastlog_test_speed

.PHONY: run_debug
run_debug: $(BIN)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=out/lib ; gdb ./out/bin/fastlog_test_speed

############
# patterns #
############
$(OBJ): out/obj/%.o: %.c
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<
$(BIN): out/bin/%: test/%.c $(LIB)
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ $< $(BINLD)
