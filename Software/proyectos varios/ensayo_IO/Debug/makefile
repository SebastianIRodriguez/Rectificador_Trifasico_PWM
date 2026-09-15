################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC_config.obj" \
"./ANG_DIFF.obj" \
"./ANG_LIM.obj" \
"./CLARKE.obj" \
"./CodeStartBranch.obj" \
"./DC.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DelayUs.obj" \
"./ECap_7_8_9_10_12.obj" \
"./FE.obj" \
"./GRID_VALUES.obj" \
"./Gpio.obj" \
"./INTEGRATOR.obj" \
"./IPQ_MEAS.obj" \
"./Interrupciones.obj" \
"./LIMITS.obj" \
"./NOTCH.obj" \
"./OVER_PROT.obj" \
"./PARK.obj" \
"./PF2Q.obj" \
"./PIAW_CONTROLLER.obj" \
"./PI_CONTROLLER.obj" \
"./PO.obj" \
"./PT1.obj" \
"./PWM_DAC.obj" \
"./PWM_config.obj" \
"./P_CONTROLLER.obj" \
"./PieVect_5_6_7_8_9_10.obj" \
"./Pie_config.obj" \
"./RATE_LIMITER.obj" \
"./SEQ_CORE.obj" \
"./SIGNAL_1Ph.obj" \
"./SIGNAL_3Ph.obj" \
"./SOGI.obj" \
"./SVPWM.obj" \
"./SWITCH_2IN.obj" \
"./SWITCH_2IN_3Ph.obj" \
"./SYM_COS.obj" \
"./SYM_SIN.obj" \
"./SysCtrl.obj" \
"./VCO.obj" \
"./Watchdog.obj" \
"./main.obj" \
"./modulacion_vectorial.obj" \
"./pll.obj" \
"./pruebas.obj" \
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
-include subdir_rules.mk
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
ensayo_IO.out 

EXE_OUTPUTS__QUOTED += \
"ensayo_IO.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ensayo_IO.out"

# Tool invocations
ensayo_IO.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"ensayo_IO.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="ensayo_IO_linkInfo.xml" --rom_model -o "ensayo_IO.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC_config.obj" "ANG_DIFF.obj" "ANG_LIM.obj" "CLARKE.obj" "CodeStartBranch.obj" "DC.obj" "DSP2833x_GlobalVariableDefs.obj" "DelayUs.obj" "ECap_7_8_9_10_12.obj" "FE.obj" "GRID_VALUES.obj" "Gpio.obj" "INTEGRATOR.obj" "IPQ_MEAS.obj" "Interrupciones.obj" "LIMITS.obj" "NOTCH.obj" "OVER_PROT.obj" "PARK.obj" "PF2Q.obj" "PIAW_CONTROLLER.obj" "PI_CONTROLLER.obj" "PO.obj" "PT1.obj" "PWM_DAC.obj" "PWM_config.obj" "P_CONTROLLER.obj" "PieVect_5_6_7_8_9_10.obj" "Pie_config.obj" "RATE_LIMITER.obj" "SEQ_CORE.obj" "SIGNAL_1Ph.obj" "SIGNAL_3Ph.obj" "SOGI.obj" "SVPWM.obj" "SWITCH_2IN.obj" "SWITCH_2IN_3Ph.obj" "SYM_COS.obj" "SYM_SIN.obj" "SysCtrl.obj" "VCO.obj" "Watchdog.obj" "main.obj" "modulacion_vectorial.obj" "pll.obj" "pruebas.obj" 
	-$(RM) "ADC_config.d" "ANG_DIFF.d" "ANG_LIM.d" "CLARKE.d" "DC.d" "DSP2833x_GlobalVariableDefs.d" "ECap_7_8_9_10_12.d" "FE.d" "GRID_VALUES.d" "Gpio.d" "INTEGRATOR.d" "IPQ_MEAS.d" "Interrupciones.d" "LIMITS.d" "NOTCH.d" "OVER_PROT.d" "PARK.d" "PF2Q.d" "PIAW_CONTROLLER.d" "PI_CONTROLLER.d" "PO.d" "PT1.d" "PWM_DAC.d" "PWM_config.d" "P_CONTROLLER.d" "PieVect_5_6_7_8_9_10.d" "Pie_config.d" "RATE_LIMITER.d" "SEQ_CORE.d" "SIGNAL_1Ph.d" "SIGNAL_3Ph.d" "SOGI.d" "SVPWM.d" "SWITCH_2IN.d" "SWITCH_2IN_3Ph.d" "SYM_COS.d" "SYM_SIN.d" "SysCtrl.d" "VCO.d" "Watchdog.d" "main.d" "modulacion_vectorial.d" "pll.d" "pruebas.d" 
	-$(RM) "CodeStartBranch.d" "DelayUs.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

