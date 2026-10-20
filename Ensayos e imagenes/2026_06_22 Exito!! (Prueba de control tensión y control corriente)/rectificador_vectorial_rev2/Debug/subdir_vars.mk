################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28335_RAM_lnk.cmd \
../DSP2833x_Headers_nonBIOS.cmd 

ASM_SRCS += \
../CodeStartBranch.asm \
../DelayUs.asm 

C_SRCS += \
../ADC_config.c \
../ANG_DIFF.c \
../ANG_LIM.c \
../CLARKE.c \
../DC.c \
../DSP2833x_GlobalVariableDefs.c \
../ECap_7_8_9_10_12.c \
../FE.c \
../GRID_VALUES.c \
../Gpio.c \
../INTEGRATOR.c \
../IPQ_MEAS.c \
../Interrupciones.c \
../LIMITS.c \
../NOTCH.c \
../OVER_PROT.c \
../PARK.c \
../PF2Q.c \
../PIAW_CONTROLLER.c \
../PI_CONTROLLER.c \
../PO.c \
../PT1.c \
../PWM_DAC.c \
../PWM_config.c \
../P_CONTROLLER.c \
../PieVect.c \
../Pie_config.c \
../RATE_LIMITER.c \
../SEQ_CORE.c \
../SIGNAL_1Ph.c \
../SIGNAL_3Ph.c \
../SOGI.c \
../SVPWM.c \
../SWITCH_2IN.c \
../SWITCH_2IN_3Ph.c \
../SYM_COS.c \
../SYM_SIN.c \
../SysCtrl.c \
../VCO.c \
../Watchdog.c \
../inversor.c \
../main.c \
../modulacion_vectorial.c \
../pll.c \
../project_interrupts.c \
../protections.c \
../utils.c 

C_DEPS += \
./ADC_config.d \
./ANG_DIFF.d \
./ANG_LIM.d \
./CLARKE.d \
./DC.d \
./DSP2833x_GlobalVariableDefs.d \
./ECap_7_8_9_10_12.d \
./FE.d \
./GRID_VALUES.d \
./Gpio.d \
./INTEGRATOR.d \
./IPQ_MEAS.d \
./Interrupciones.d \
./LIMITS.d \
./NOTCH.d \
./OVER_PROT.d \
./PARK.d \
./PF2Q.d \
./PIAW_CONTROLLER.d \
./PI_CONTROLLER.d \
./PO.d \
./PT1.d \
./PWM_DAC.d \
./PWM_config.d \
./P_CONTROLLER.d \
./PieVect.d \
./Pie_config.d \
./RATE_LIMITER.d \
./SEQ_CORE.d \
./SIGNAL_1Ph.d \
./SIGNAL_3Ph.d \
./SOGI.d \
./SVPWM.d \
./SWITCH_2IN.d \
./SWITCH_2IN_3Ph.d \
./SYM_COS.d \
./SYM_SIN.d \
./SysCtrl.d \
./VCO.d \
./Watchdog.d \
./inversor.d \
./main.d \
./modulacion_vectorial.d \
./pll.d \
./project_interrupts.d \
./protections.d \
./utils.d 

OBJS += \
./ADC_config.obj \
./ANG_DIFF.obj \
./ANG_LIM.obj \
./CLARKE.obj \
./CodeStartBranch.obj \
./DC.obj \
./DSP2833x_GlobalVariableDefs.obj \
./DelayUs.obj \
./ECap_7_8_9_10_12.obj \
./FE.obj \
./GRID_VALUES.obj \
./Gpio.obj \
./INTEGRATOR.obj \
./IPQ_MEAS.obj \
./Interrupciones.obj \
./LIMITS.obj \
./NOTCH.obj \
./OVER_PROT.obj \
./PARK.obj \
./PF2Q.obj \
./PIAW_CONTROLLER.obj \
./PI_CONTROLLER.obj \
./PO.obj \
./PT1.obj \
./PWM_DAC.obj \
./PWM_config.obj \
./P_CONTROLLER.obj \
./PieVect.obj \
./Pie_config.obj \
./RATE_LIMITER.obj \
./SEQ_CORE.obj \
./SIGNAL_1Ph.obj \
./SIGNAL_3Ph.obj \
./SOGI.obj \
./SVPWM.obj \
./SWITCH_2IN.obj \
./SWITCH_2IN_3Ph.obj \
./SYM_COS.obj \
./SYM_SIN.obj \
./SysCtrl.obj \
./VCO.obj \
./Watchdog.obj \
./inversor.obj \
./main.obj \
./modulacion_vectorial.obj \
./pll.obj \
./project_interrupts.obj \
./protections.obj \
./utils.obj 

ASM_DEPS += \
./CodeStartBranch.d \
./DelayUs.d 

OBJS__QUOTED += \
"ADC_config.obj" \
"ANG_DIFF.obj" \
"ANG_LIM.obj" \
"CLARKE.obj" \
"CodeStartBranch.obj" \
"DC.obj" \
"DSP2833x_GlobalVariableDefs.obj" \
"DelayUs.obj" \
"ECap_7_8_9_10_12.obj" \
"FE.obj" \
"GRID_VALUES.obj" \
"Gpio.obj" \
"INTEGRATOR.obj" \
"IPQ_MEAS.obj" \
"Interrupciones.obj" \
"LIMITS.obj" \
"NOTCH.obj" \
"OVER_PROT.obj" \
"PARK.obj" \
"PF2Q.obj" \
"PIAW_CONTROLLER.obj" \
"PI_CONTROLLER.obj" \
"PO.obj" \
"PT1.obj" \
"PWM_DAC.obj" \
"PWM_config.obj" \
"P_CONTROLLER.obj" \
"PieVect.obj" \
"Pie_config.obj" \
"RATE_LIMITER.obj" \
"SEQ_CORE.obj" \
"SIGNAL_1Ph.obj" \
"SIGNAL_3Ph.obj" \
"SOGI.obj" \
"SVPWM.obj" \
"SWITCH_2IN.obj" \
"SWITCH_2IN_3Ph.obj" \
"SYM_COS.obj" \
"SYM_SIN.obj" \
"SysCtrl.obj" \
"VCO.obj" \
"Watchdog.obj" \
"inversor.obj" \
"main.obj" \
"modulacion_vectorial.obj" \
"pll.obj" \
"project_interrupts.obj" \
"protections.obj" \
"utils.obj" 

C_DEPS__QUOTED += \
"ADC_config.d" \
"ANG_DIFF.d" \
"ANG_LIM.d" \
"CLARKE.d" \
"DC.d" \
"DSP2833x_GlobalVariableDefs.d" \
"ECap_7_8_9_10_12.d" \
"FE.d" \
"GRID_VALUES.d" \
"Gpio.d" \
"INTEGRATOR.d" \
"IPQ_MEAS.d" \
"Interrupciones.d" \
"LIMITS.d" \
"NOTCH.d" \
"OVER_PROT.d" \
"PARK.d" \
"PF2Q.d" \
"PIAW_CONTROLLER.d" \
"PI_CONTROLLER.d" \
"PO.d" \
"PT1.d" \
"PWM_DAC.d" \
"PWM_config.d" \
"P_CONTROLLER.d" \
"PieVect.d" \
"Pie_config.d" \
"RATE_LIMITER.d" \
"SEQ_CORE.d" \
"SIGNAL_1Ph.d" \
"SIGNAL_3Ph.d" \
"SOGI.d" \
"SVPWM.d" \
"SWITCH_2IN.d" \
"SWITCH_2IN_3Ph.d" \
"SYM_COS.d" \
"SYM_SIN.d" \
"SysCtrl.d" \
"VCO.d" \
"Watchdog.d" \
"inversor.d" \
"main.d" \
"modulacion_vectorial.d" \
"pll.d" \
"project_interrupts.d" \
"protections.d" \
"utils.d" 

ASM_DEPS__QUOTED += \
"CodeStartBranch.d" \
"DelayUs.d" 

C_SRCS__QUOTED += \
"../ADC_config.c" \
"../ANG_DIFF.c" \
"../ANG_LIM.c" \
"../CLARKE.c" \
"../DC.c" \
"../DSP2833x_GlobalVariableDefs.c" \
"../ECap_7_8_9_10_12.c" \
"../FE.c" \
"../GRID_VALUES.c" \
"../Gpio.c" \
"../INTEGRATOR.c" \
"../IPQ_MEAS.c" \
"../Interrupciones.c" \
"../LIMITS.c" \
"../NOTCH.c" \
"../OVER_PROT.c" \
"../PARK.c" \
"../PF2Q.c" \
"../PIAW_CONTROLLER.c" \
"../PI_CONTROLLER.c" \
"../PO.c" \
"../PT1.c" \
"../PWM_DAC.c" \
"../PWM_config.c" \
"../P_CONTROLLER.c" \
"../PieVect.c" \
"../Pie_config.c" \
"../RATE_LIMITER.c" \
"../SEQ_CORE.c" \
"../SIGNAL_1Ph.c" \
"../SIGNAL_3Ph.c" \
"../SOGI.c" \
"../SVPWM.c" \
"../SWITCH_2IN.c" \
"../SWITCH_2IN_3Ph.c" \
"../SYM_COS.c" \
"../SYM_SIN.c" \
"../SysCtrl.c" \
"../VCO.c" \
"../Watchdog.c" \
"../inversor.c" \
"../main.c" \
"../modulacion_vectorial.c" \
"../pll.c" \
"../project_interrupts.c" \
"../protections.c" \
"../utils.c" 

ASM_SRCS__QUOTED += \
"../CodeStartBranch.asm" \
"../DelayUs.asm" 


