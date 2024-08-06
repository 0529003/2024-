################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MPU6050/%.o: ../MPU6050/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/29683/workspace_ccstheia/M0G3507" -I"C:/Users/29683/workspace_ccstheia/M0G3507/OLED" -I"C:/Users/29683/workspace_ccstheia/M0G3507/Debug" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source" -I"C:/Users/29683/workspace_ccstheia/M0G3507/PID" -I"C:/Users/29683/workspace_ccstheia/M0G3507/KEY" -I"C:/Users/29683/workspace_ccstheia/M0G3507/MOTOR" -I"C:/Users/29683/workspace_ccstheia/M0G3507/MPU6050" -gdwarf-3 -MMD -MP -MF"MPU6050/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


