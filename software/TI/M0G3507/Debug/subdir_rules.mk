################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/OLED" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/Debug" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/PID" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/KEY" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/MOTOR" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1456284001: ../main.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/TI/SYS/sysconfig_cli.bat" --script "C:/Users/29683/workspace_ccstheia/M0G3507 copy/main.syscfg" -o "." -s "D:/TI/SDK/mspm0_sdk_2_01_00_03/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1456284001 ../main.syscfg
device.opt: build-1456284001
device.cmd.genlibs: build-1456284001
ti_msp_dl_config.c: build-1456284001
ti_msp_dl_config.h: build-1456284001
Event.dot: build-1456284001

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/OLED" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/Debug" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/PID" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/KEY" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/MOTOR" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: D:/TI/SDK/mspm0_sdk_2_01_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/OLED" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/Debug" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"D:/TI/SDK/mspm0_sdk_2_01_00_03/source" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/PID" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/KEY" -I"C:/Users/29683/workspace_ccstheia/M0G3507 copy/MOTOR" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


