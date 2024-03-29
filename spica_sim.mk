##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.10.0-B14] date: [Sun Nov 08 10:58:56 EST 2020]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# TODO
# ------------------------------------------------

######################################
# target
######################################
TARGET = SPICA_sim


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
lib/sys/sys.c \
lib/H3LIS100DL/H3LIS100DL.c \

# CPP sources
CPP_SOURCES = \
Core/Src/init.cpp \
lib/MPL3115A2/MPL3115A2.cpp \
lib/LSM9DS1/LSM9DS1.cpp \
lib/sim/hal_sim.c \
lib/sim/sim_main.c \
#lib/MS5607/MS5607.cpp \

CC = gcc
CXX = g++

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Include \
-I.


CFLAGS = $(C_DEFS) $(C_INCLUDES) -Wall

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -DDEBUG
endif


#######################################
# LDFLAGS
#######################################
# link script


# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of cpp objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

$(BUILD_DIR)/%.o: %.c spica_sim.mk | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp spica_sim.mk | $(BUILD_DIR)
	$(CXX) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s spica_sim.mk | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) spica_sim.mk
	$(CC) $(OBJECTS)

$(BUILD_DIR):
	-mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

# NOT Autogenerated (may get overwritten if STM32CubeMX is run on this dir!)
#######################################
# flash the chip
######################################
flash: erase
	-st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

erase:
	-st-flash erase

program: all flash

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
