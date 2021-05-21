BIN = mqtt_pub
DATE = $(shell date +%Y%m%d)
BIN_NAME = $(BIN)_$(DATE)
CC ?= arm-linux-gnueabihf-gcc
BUILD_DIR = ./.build

LIB_USER_SRC_PATH = ./lib/lib_user/src
LIB_USER_INC_PATH = ./lib/lib_user/inc
SRC_PATH = ./app/src
INC_PATH = ./app/inc

LIB_SO_LIB_PATH = $(wildcard ./lib/*/lib)
LIB_SO_INC_PATH = $(wildcard ./lib/*/inc)

LIB_USER_SRC_FILE = $(wildcard $(LIB_USER_SRC_PATH)/*.c)
SRC_FILE = $(wildcard $(SRC_PATH)/*.c)

LIB_USER_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(LIB_USER_SRC_FILE)))
SRC_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC_FILE)))

CFLAGS = -Wall $(patsubst %,-I%,$(LIB_USER_INC_PATH)) $(patsubst %,-I%,$(INC_PATH)) $(patsubst %,-I%,$(LIB_SO_INC_PATH))
LDFLAGS = $(patsubst %,-L%,$(LIB_SO_LIB_PATH)) -lrt  -lpaho-mqtt3c

all: $(BIN_NAME)

$(BIN_NAME): $(LIB_USER_OBJ) $(SRC_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(LIB_USER_OBJ): $(BUILD_DIR)/%.o: $(LIB_USER_SRC_PATH)/%.c $(LIB_USER_INC_PATH)
	$(shell mkdir -p $(BUILD_DIR))
	$(CC) -o $@ $(CFLAGS) -c $<

$(SRC_OBJ): $(BUILD_DIR)/%.o: $(SRC_PATH)/%.c $(INC_PATH)
	$(shell mkdir -p $(BUILD_DIR))
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)*

.PHONY: clean all