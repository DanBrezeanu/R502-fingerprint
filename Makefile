CC=gcc
RM=rm -rf
MKDIR=mkdir -p

SRC_DIR=src
OBJ_DIR=obj
EXAMPLE_DIR=examples

TARGET_LIB=libr502.so

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

EXAMPLE_SRCS=$(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_BIN=$(patsubst $(EXAMPLE_DIR)/%.c, $(EXAMPLE_DIR)/%, $(EXAMPLE_SRCS))


CFLAGS=-Wall -O3 -fPIC -Wno-unused-function
INCLUDE=-Iinclude/
LIBS=-lserialport

.PHONY: all clean install

all: create_obj_dir ${TARGET_LIB}
example: ${EXAMPLE_DIR}

$(TARGET_LIB): $(OBJS)
	$(CC) -shared $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -c -o $@

$(EXAMPLE_DIR)/%: $(EXAMPLE_DIR)/%.c
	$(CC) $(CFLAGS) -L./ $(INCLUDE) $< -o $@ -lr502 $(LIBS)

$(EXAMPLE_DIR): $(EXAMPLE_BIN)

create_obj_dir:
	@$(MKDIR) $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR) $(TARGET_LIB) $(EXAMPLE_BIN)

ifeq ($(PREFIX),)
	@PREFIX=/usr/local
endif

install: all
	install -d $(PREFIX)/lib/
	install -m 644 $(TARGET_LIB) $(PREFIX)/lib/
	install -d $(PREFIX)/include/
	install -m 644 include/*.h $(PREFIX)/include/