SRC:=$(shell find lib -name "*.c")
OBJ:=$(addsuffix .o,$(basename $(SRC)))
LIB:=libfastlog.so
CC:=gcc
CFLAGS:=-O2 -fpic
LDFLAGS:=-shared -fpic
ALL_DEPS:=Makefile

.PHONY: all
all: $(LIB) $(ALL_DEPS)

$(LIB): $(OBJ) $(ALL_DEPS)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)


.PHONY: debug
debug: $(ALL_DEPS)
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))

.PHONY: clean
clean: $(ALL_DEPS)
	rm -f $(OBJ) $(LIB)


$(OBJ): %.o: %.c $(ALL_DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<
