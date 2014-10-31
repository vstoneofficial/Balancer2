################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Balancer2_firm_Simple.axf

dependents:
	-cd C:/Users/yokoyama/Documents/lpcxpresso_3.5/workspace/Balancer2_lib/Debug && $(MAKE) clean all

# Tool invocations
Balancer2_firm_Simple.axf: $(OBJS) $(USER_OBJS) C:/Users/yokoyama/Documents/lpcxpresso_3.5/workspace/Balancer2_lib/Debug/libBalancer2_lib.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:/Users/yokoyama/Documents/lpcxpresso_3.5/workspace/Balancer2_firm_Simple/lib" -Xlinker --gc-sections -Xlinker -Map=Balancer2_firm_Simple.map -mcpu=cortex-m3 -mthumb -T "Balancer2_Debug.ld" -o "Balancer2_firm_Simple.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Balancer2_firm_Simple.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size Balancer2_firm_Simple.axf; arm-none-eabi-size Balancer2_firm_Simple.axf; arm-none-eabi-objcopy -O binary Balancer2_firm_Simple.axf Balancer2_firm_Simple.bin; checksum Balancer2_firm_Simple.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Users/yokoyama/Documents/lpcxpresso_3.5/workspace/Balancer2_lib/Debug/libBalancer2_lib.a:

-include ../makefile.targets
