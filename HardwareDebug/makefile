################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.h *.bls *.libelf *.rel *.lbk *.abs *.prn *.mi  *.obj *.jmp *.d *.i *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs 1st/*.i

# All of the sources participating in the build are defined here
-include sources.mk
-include generate/subdir.mk
-include src/cg_src/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
MODBUS_MASTER_V2_R5F1056AGSP.abs

CONVERTOR_OUTPUT_OUTPUTS += \
MODBUS_MASTER_V2_R5F1056AGSP.mot

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(CONVERTOR_OUTPUT_OUTPUTS) MODBUS_MASTER_V2_R5F1056AGSP.x
	@echo 'Build complete.'

MODBUS_MASTER_V2_R5F1056AGSP.mot: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Invoking: Converter'
	@echo 'Building target:'
	@echo ConverterMODBUS_MASTER_V2_R5F1056AGSP.tmp=
	@sed -e "s/^/    /" "ConverterMODBUS_MASTER_V2_R5F1056AGSP.tmp"
	rlink   "MODBUS_MASTER_V2_R5F1056AGSP.abs" -subcommand="ConverterMODBUS_MASTER_V2_R5F1056AGSP.tmp"
	@echo 'Finished building target:'
	@echo.


MODBUS_MASTER_V2_R5F1056AGSP.abs: $(OBJS)  $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo LinkerMODBUS_MASTER_V2_R5F1056AGSP.tmp=
	@sed -e "s/^/    /" "LinkerMODBUS_MASTER_V2_R5F1056AGSP.tmp"
	rlink -subcommand="LinkerMODBUS_MASTER_V2_R5F1056AGSP.tmp"
	@echo 'Finished building:'
	@echo.


MODBUS_MASTER_V2_R5F1056AGSP.x: MODBUS_MASTER_V2_R5F1056AGSP.abs
		-C:\Renesas\e2_studio\Utilities\\ccrl\renesas_cc_converter.exe MODBUS_MASTER_V2_R5F1056AGSP.abs MODBUS_MASTER_V2_R5F1056AGSP.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS)
	-rm -rf generate/*.obj generate/*.d generate/*.asm generate/*.p generate/*.src generate/*.jmp generate/*.lst generate/*.prn generate/*.i 
	-rm -rf src/cg_src/*.obj src/cg_src/*.d src/cg_src/*.asm src/cg_src/*.p src/cg_src/*.src src/cg_src/*.jmp src/cg_src/*.lst src/cg_src/*.prn src/cg_src/*.i 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets