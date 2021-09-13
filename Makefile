DEBUG ?=
DEFINE ?=
CROSS_COMPILE ?= 
ARCH ?= 
LIB ?= 
ifeq ($(ARCH),arm)   # 版本: gcc version 7.5.0 														
CROSS_COMPILE = arm-linux-gnueabihf-
INC_DIR :=  
LIB = 
else	
# 仿真 x86_64-linux-gun-gcc, 版本: gcc version 9.3.0   																
ARCH := amd64
INC_DIR :=
LIB = 
endif

LIB += 

INC_DIR := inc \
						./

SRC_DIR := src \
					./ 
						
VPATH := $(SRC_DIR) #set MakeFile's file searching dir

C_FLAGS += -Wall -std=c11 $(DEBUG) $(DEFINE)
CPP_FLAGS += -Wall -std=c++17 $(DEBUG)  $(DEFINE)
LD_FLAGS += 
OUTPUT_DIR := obj
TARGET := demo

################# format handler #######################
CC := $(CROSS_COMPILE)gcc
PP := $(CROSS_COMPILE)g++
LD := $(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy 

INC_PATH := $(patsubst %, -I %, $(INC_DIR))

SRC_PATH := $(SRC_DIR)

OBJ_PATH = $(OUTPUT_DIR)

C_FILES := $(notdir $(foreach dir, $(SRC_PATH),$(wildcard $(dir)/*.c)))
CPP_FILES := $(notdir $(foreach dir, $(SRC_PATH),$(wildcard $(dir)/*.cpp)))

C_OBJS := $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_FILES))
CPP_OBJS := $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_FILES))
OBJS := $(C_OBJS) $(CPP_OBJS)



default: $(TARGET).elf


$(TARGET).elf : $(OBJS)
	$(PP)  -o $(TARGET).elf $^  $(LIB)


$(C_OBJS) : $(OBJ_PATH)/%.o : %.c
	$(CC) $(C_FLAGS)  $(INC_PATH) -c -o $@ $<


$(CPP_OBJS) : $(OBJ_PATH)/%.o : %.cpp
	$(PP) $(CPP_FLAGS) $(INC_PATH) -c -o  $@ $< 


.PHNOY:
clean:
	rm -f $(OUTPUT_DIR)/*.o $(OUTPUT_DIR)/*.bin $(OUTPUT_DIR)/*.elf
	rm -f ./*.elf 