################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC_config.obj" \
"./CodeStartBranch.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DelayUs.obj" \
"./Gpio.obj" \
"./Interrupciones.obj" \
"./PWM_config.obj" \
"./PieVect.obj" \
"./Pie_config.obj" \
"./SysCtrl.obj" \
"./Watchdog.obj" \
"./main.obj" \
"./modulacion_vectorial.obj" \
"./pll.obj" \
"./project_interrupts.obj" \
"./rectificador/protecciones/protecciones.obj" \
"./renolib_source/ANG_DIFF.obj" \
"./renolib_source/CLARKE.obj" \
"./renolib_source/FE.obj" \
"./renolib_source/GRID_VALUES.obj" \
"./renolib_source/INTEGRATOR.obj" \
"./renolib_source/LIMITS.obj" \
"./renolib_source/PARK.obj" \
"./renolib_source/PI_CONTROLLER.obj" \
"./renolib_source/RATE_LIMITER.obj" \
"./renolib_source/SEQ_CORE.obj" \
"./renolib_source/SIGNAL_3Ph.obj" \
"./renolib_source/SYM_COS.obj" \
"./renolib_source/SYM_SIN.obj" \
"./renolib_source/VCO.obj" \
"../28335_RAM_lnk.cmd" \
"../DSP2833x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include rectificador/protecciones/subdir_vars.mk
-include renolib_source/subdir_vars.mk
-include subdir_rules.mk
-include rectificador/protecciones/subdir_rules.mk
-include renolib_source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
rectificador_vectorial_rev4.out 

EXE_OUTPUTS__QUOTED += \
"rectificador_vectorial_rev4.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rectificador_vectorial_rev4.out"

# Tool invocations
rectificador_vectorial_rev4.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"rectificador_vectorial_rev4.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="rectificador_vectorial_rev4_linkInfo.xml" --rom_model -o "rectificador_vectorial_rev4.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC_config.obj" "CodeStartBranch.obj" "DSP2833x_GlobalVariableDefs.obj" "DelayUs.obj" "Gpio.obj" "Interrupciones.obj" "PWM_config.obj" "PieVect.obj" "Pie_config.obj" "SysCtrl.obj" "Watchdog.obj" "main.obj" "modulacion_vectorial.obj" "pll.obj" "project_interrupts.obj" "rectificador\protecciones\protecciones.obj" "renolib_source\ANG_DIFF.obj" "renolib_source\CLARKE.obj" "renolib_source\FE.obj" "renolib_source\GRID_VALUES.obj" "renolib_source\INTEGRATOR.obj" "renolib_source\LIMITS.obj" "renolib_source\PARK.obj" "renolib_source\PI_CONTROLLER.obj" "renolib_source\RATE_LIMITER.obj" "renolib_source\SEQ_CORE.obj" "renolib_source\SIGNAL_3Ph.obj" "renolib_source\SYM_COS.obj" "renolib_source\SYM_SIN.obj" "renolib_source\VCO.obj" 
	-$(RM) "ADC_config.d" "DSP2833x_GlobalVariableDefs.d" "Gpio.d" "Interrupciones.d" "PWM_config.d" "PieVect.d" "Pie_config.d" "SysCtrl.d" "Watchdog.d" "main.d" "modulacion_vectorial.d" "pll.d" "project_interrupts.d" "rectificador\protecciones\protecciones.d" "renolib_source\ANG_DIFF.d" "renolib_source\CLARKE.d" "renolib_source\FE.d" "renolib_source\GRID_VALUES.d" "renolib_source\INTEGRATOR.d" "renolib_source\LIMITS.d" "renolib_source\PARK.d" "renolib_source\PI_CONTROLLER.d" "renolib_source\RATE_LIMITER.d" "renolib_source\SEQ_CORE.d" "renolib_source\SIGNAL_3Ph.d" "renolib_source\SYM_COS.d" "renolib_source\SYM_SIN.d" "renolib_source\VCO.d" 
	-$(RM) "CodeStartBranch.d" "DelayUs.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

