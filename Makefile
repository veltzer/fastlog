###############
# paramaeters #
###############

DIR:=lib
SRC:=$(shell find $(DIR) -name "*.c")
OBJ:=$(addsuffix .o,$(basename $(SRC)))
LIB:=libfastlog.so
CC:=gcc
CFLAGS:=-O2 -fpic -I$(DIR)
LDFLAGS:=-shared -fpic
ALL_DEPS:=Makefile
BIN:=test/logging_speed
BINLD:=-lpthread -L. -lfastlog

# here we go...

.PHONY: all
all: $(LIB) $(BIN) $(ALL_DEPS)

# binaries and libraries

$(LIB): $(OBJ) $(ALL_DEPS)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

# special targets

.PHONY: debug
debug: $(ALL_DEPS)
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))

.PHONY: clean
clean: $(ALL_DEPS)
	rm -f $(OBJ) $(LIB) $(BIN)

.PHONY: run
run: $(BIN) $(ALL_DEPS)
	export LD_LIBRARY_PATH=. ; ./test/logging_speed

# rules

$(OBJ): %.o: %.c $(ALL_DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<
$(BIN): %: %.c $(ALL_DEPS)
	$(CC) $(BINLD) $(CFLAGS) -o $@ $<
