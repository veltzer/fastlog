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
BASE_FLAGS:=-O2 -fpic -Wall -Werror -std=gnu99 -Wno-dangling-pointer
# do you want debugging enabled?
DO_DEBUG:=0
# do you want dependency on the Makefile itself ?
DO_ALLDEP:=1

########
# code #
########
# where is the output folder ?
OUT:=out
LIB:=$(OUT)/lib/lib$(LIBNAME).so
SRC:=$(shell find $(SRCDIR) -type f -and -name "*.c")
OBJ:=$(addprefix $(OUT)/obj/, $(addsuffix .o,$(basename $(SRC))))
CFLAGS:=$(BASE_FLAGS) -I$(SRCDIR) -Itest
LDFLAGS:=-shared -fpic
BIN:=$(OUT)/bin/fastlog_test_speed $(OUT)/bin/fastlog_test_basic $(OUT)/bin/fastlog_test_crash
BINLD:=-L$(OUT)/lib -l$(LIBNAME) -lpthread
# what is the stamp file for the tools?
ALL:=

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

ifeq ($(DO_ALLDEP),1)
.EXTRA_PREREQS+=$(foreach mk, ${MAKEFILE_LIST},$(abspath ${mk}))
endif # DO_ALLDEP

ALL+=$(LIB) $(BIN)

#########
# rules #
#########
.PHONY: all
all: $(ALL)
	@true

# binaries and libraries

$(LIB): $(OBJ)
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJ)

.PHONY: debug
debug:
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)rm -f $(OBJ) $(LIB) $(BIN)

.PHONY: clean_hard
clean_hard:
	$(info doing [$@])
	$(Q)git clean -qffxd

.PHONY: run
run: $(BIN)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=$(OUT)/lib ; ./$(OUT)/bin/fastlog_test_speed

.PHONY: run_debug
run_debug: $(BIN)
	$(info doing [$@])
	$(Q)export LD_LIBRARY_PATH=$(OUT)/lib ; gdb ./$(OUT)/bin/fastlog_test_speed

############
# patterns #
############
$(OBJ): $(OUT)/obj/%.o: %.c
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<
$(BIN): $(OUT)/bin/%: test/%.c $(LIB)
	$(info doing [$@])
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ $< $(BINLD)
