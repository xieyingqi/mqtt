BIN = mqtt_sub
DATE = $(shell date +%Y%m%d)
BIN_NAME = $(BIN)_$(DATE)
CC = gcc
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
LDFLAGS = $(patsubst %,-L%,$(LIB_SO_LIB_PATH)) -lrt -lpaho-mqtt3c -lpaho-mqtt3cs

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

# BIN = com
# DATE = $(shell date +%Y%m%d)
# BIN_NAME = $(BIN)_$(DATE)
# CC = gcc

# BUILD_DIR = ./.build
# SRC_PATH = $(shell find ./ -name src -type d) #获取所有src文件夹
# INC_PATH = $(shell find ./ -name inc -type d) #获取所有inc文件夹

# SRC_FILE = $(wildcard $(foreach s,$(SRC_PATH),$(s)/*.c)) #获取目录下所有.c文件
# INC_FILE = $(wildcard $(foreach i,$(INC_PATH),$(i)/*.h)) #获取目录下所有.h文件
# OBJ_FILE = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC_FILE)))  #获取所有由.c编译成.o的文件名，并附带build文件夹

# CFLAGS = -Wall $(patsubst %,-I%,$(INC_PATH)) #编译选项 -Wall所有警告 inc路径增加-I符号 
# LDFLAGS = -lrt

# all: $(BIN_NAME)

# $(BIN_NAME): $(OBJ_FILE)
# 	$(CC) $^ -o $@ $(LDFLAGS)

# $(OBJ_FILE): $(BUILD_DIR)/%.o: $(SRC_PATH)/%.c $(INC_FILE)
# 	$(shell mkdir -p $(BUILD_DIR))
# 	$(CC) -o $@ $(CFLAGS) -c $<

# clean:
# 	rm -rf $(BUILD_DIR)
# 	rm -f $(BIN)*

# .PHONY: clean all