################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SMatrix.cpp \
../mytest0.cpp \
../test0.cpp \
../test1.cpp \
../test2.cpp \
../test3.cpp 

OBJS += \
./SMatrix.o \
./mytest0.o \
./test0.o \
./test1.o \
./test2.o \
./test3.o 

CPP_DEPS += \
./SMatrix.d \
./mytest0.d \
./test0.d \
./test1.d \
./test2.d \
./test3.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


