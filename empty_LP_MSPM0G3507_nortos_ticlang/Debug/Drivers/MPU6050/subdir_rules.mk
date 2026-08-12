################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Drivers/MPU6050/%.o: ../Drivers/MPU6050/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/WIT" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/VL53L0X" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/Ultrasonic_GPIO" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/Ultrasonic_Capture" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/OLED_Software_SPI" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/OLED_Software_I2C" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/OLED_Hardware_SPI" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/OLED_Hardware_I2C" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/OLED_Hardware_I2C" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/MSPM0" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/MPU6050" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/LSM6DSV16X" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Drivers/BNO08X_UART_RVC" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/liuyuxuan/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -DMOTION_DRIVER_TARGET_MSPM0 -DMPU6050 -gdwarf-3 -MMD -MP -MF"Drivers/MPU6050/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


