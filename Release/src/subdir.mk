################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Network.cpp \
../src/Smartphone.cpp \
../src/input.cpp \
../src/main.cpp \
../src/manager.cpp \
../src/model.cpp \
../src/mynetwork.cpp \
../src/mysmartphone.cpp \
../src/scene.cpp \
../src/view.cpp 

OBJS += \
./src/Network.o \
./src/Smartphone.o \
./src/input.o \
./src/main.o \
./src/manager.o \
./src/model.o \
./src/mynetwork.o \
./src/mysmartphone.o \
./src/scene.o \
./src/view.o 

CPP_DEPS += \
./src/Network.d \
./src/Smartphone.d \
./src/input.d \
./src/main.d \
./src/manager.d \
./src/model.d \
./src/mynetwork.d \
./src/mysmartphone.d \
./src/scene.d \
./src/view.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


