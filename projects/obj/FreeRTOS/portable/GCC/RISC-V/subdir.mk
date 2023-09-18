################################################################################
# MRS Version: 1.9.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/RISC-V/port.c 

S_UPPER_SRCS += \
../FreeRTOS/portable/GCC/RISC-V/portASM.S 

OBJS += \
./FreeRTOS/portable/GCC/RISC-V/port.o \
./FreeRTOS/portable/GCC/RISC-V/portASM.o 

S_UPPER_DEPS += \
./FreeRTOS/portable/GCC/RISC-V/portASM.d 

C_DEPS += \
./FreeRTOS/portable/GCC/RISC-V/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/RISC-V/%.o: ../FreeRTOS/portable/GCC/RISC-V/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Debug" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Core" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\User" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Peripheral\inc" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\include" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\Common" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
FreeRTOS/portable/GCC/RISC-V/%.o: ../FreeRTOS/portable/GCC/RISC-V/%.S
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Startup" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\include" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\Common" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\MemMang" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

