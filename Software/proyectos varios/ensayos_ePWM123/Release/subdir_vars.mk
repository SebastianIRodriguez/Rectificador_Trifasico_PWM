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
C:/Users/lraff/Desktop/renolib/source/ANG_DIFF.c \
C:/Users/lraff/Desktop/renolib/source/ANG_LIM.c \
C:/Users/lraff/Desktop/renolib/source/CLARKE.c \
C:/Users/lraff/Desktop/renolib/source/DC.c \
../DSP2833x_GlobalVariableDefs.c \
../ECap_7_8_9_10_12.c \
C:/Users/lraff/Desktop/renolib/source/FE.c \
C:/Users/lraff/Desktop/renolib/source/GRID_VALUES.c \
../Gpio.c \
C:/Users/lraff/Desktop/renolib/source/INTEGRATOR.c \
C:/Users/lraff/Desktop/renolib/source/IPQ_MEAS.c \
../Interrupciones.c \
C:/Users/lraff/Desktop/renolib/source/LIMITS.c \
C:/Users/lraff/Desktop/renolib/source/NOTCH.c \
C:/Users/lraff/Desktop/renolib/source/OVER_PROT.c \
C:/Users/lraff/Desktop/renolib/source/PARK.c \
C:/Users/lraff/Desktop/renolib/source/PF2Q.c \
C:/Users/lraff/Desktop/renolib/source/PIAW_CONTROLLER.c \
C:/Users/lraff/Desktop/renolib/source/PI_CONTROLLER.c \
C:/Users/lraff/Desktop/renolib/source/PO.c \
C:/Users/lraff/Desktop/renolib/source/PT1.c \
C:/Users/lraff/Desktop/renolib/source/PWM_DAC.c \
../PWM_config.c \
C:/Users/lraff/Desktop/renolib/source/P_CONTROLLER.c \
../PieVect_5_6_7_8_9_10.c \
../Pie_config.c \
C:/Users/lraff/Desktop/renolib/source/RATE_LIMITER.c \
C:/Users/lraff/Desktop/renolib/source/SEQ_CORE.c \
C:/Users/lraff/Desktop/renolib/source/SIGNAL_1Ph.c \
C:/Users/lraff/Desktop/renolib/source/SIGNAL_3Ph.c \
C:/Users/lraff/Desktop/renolib/source/SOGI.c \
C:/Users/lraff/Desktop/renolib/source/SVPWM.c \
C:/Users/lraff/Desktop/renolib/source/SWITCH_2IN.c \
C:/Users/lraff/Desktop/renolib/source/SWITCH_2IN_3Ph.c \
C:/Users/lraff/Desktop/renolib/source/SYM_COS.c \
C:/Users/lraff/Desktop/renolib/source/SYM_SIN.c \
../SysCtrl.c \
C:/Users/lraff/Desktop/renolib/source/VCO.c \
../Watchdog.c \
../main.c \
../modulacion_vectorial.c \
../pll.c \
../pruebas.c 

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
./PieVect_5_6_7_8_9_10.d \
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
./main.d \
./modulacion_vectorial.d \
./pll.d \
./pruebas.d 

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
./PieVect_5_6_7_8_9_10.obj \
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
./main.obj \
./modulacion_vectorial.obj \
./pll.obj \
./pruebas.obj 

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
"PieVect_5_6_7_8_9_10.obj" \
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
"main.obj" \
"modulacion_vectorial.obj" \
"pll.obj" \
"pruebas.obj" 

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
"PieVect_5_6_7_8_9_10.d" \
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
"main.d" \
"modulacion_vectorial.d" \
"pll.d" \
"pruebas.d" 

ASM_DEPS__QUOTED += \
"CodeStartBranch.d" \
"DelayUs.d" 

C_SRCS__QUOTED += \
"../ADC_config.c" \
"C:/Users/lraff/Desktop/renolib/source/ANG_DIFF.c" \
"C:/Users/lraff/Desktop/renolib/source/ANG_LIM.c" \
"C:/Users/lraff/Desktop/renolib/source/CLARKE.c" \
"C:/Users/lraff/Desktop/renolib/source/DC.c" \
"../DSP2833x_GlobalVariableDefs.c" \
"../ECap_7_8_9_10_12.c" \
"C:/Users/lraff/Desktop/renolib/source/FE.c" \
"C:/Users/lraff/Desktop/renolib/source/GRID_VALUES.c" \
"../Gpio.c" \
"C:/Users/lraff/Desktop/renolib/source/INTEGRATOR.c" \
"C:/Users/lraff/Desktop/renolib/source/IPQ_MEAS.c" \
"../Interrupciones.c" \
"C:/Users/lraff/Desktop/renolib/source/LIMITS.c" \
"C:/Users/lraff/Desktop/renolib/source/NOTCH.c" \
"C:/Users/lraff/Desktop/renolib/source/OVER_PROT.c" \
"C:/Users/lraff/Desktop/renolib/source/PARK.c" \
"C:/Users/lraff/Desktop/renolib/source/PF2Q.c" \
"C:/Users/lraff/Desktop/renolib/source/PIAW_CONTROLLER.c" \
"C:/Users/lraff/Desktop/renolib/source/PI_CONTROLLER.c" \
"C:/Users/lraff/Desktop/renolib/source/PO.c" \
"C:/Users/lraff/Desktop/renolib/source/PT1.c" \
"C:/Users/lraff/Desktop/renolib/source/PWM_DAC.c" \
"../PWM_config.c" \
"C:/Users/lraff/Desktop/renolib/source/P_CONTROLLER.c" \
"../PieVect_5_6_7_8_9_10.c" \
"../Pie_config.c" \
"C:/Users/lraff/Desktop/renolib/source/RATE_LIMITER.c" \
"C:/Users/lraff/Desktop/renolib/source/SEQ_CORE.c" \
"C:/Users/lraff/Desktop/renolib/source/SIGNAL_1Ph.c" \
"C:/Users/lraff/Desktop/renolib/source/SIGNAL_3Ph.c" \
"C:/Users/lraff/Desktop/renolib/source/SOGI.c" \
"C:/Users/lraff/Desktop/renolib/source/SVPWM.c" \
"C:/Users/lraff/Desktop/renolib/source/SWITCH_2IN.c" \
"C:/Users/lraff/Desktop/renolib/source/SWITCH_2IN_3Ph.c" \
"C:/Users/lraff/Desktop/renolib/source/SYM_COS.c" \
"C:/Users/lraff/Desktop/renolib/source/SYM_SIN.c" \
"../SysCtrl.c" \
"C:/Users/lraff/Desktop/renolib/source/VCO.c" \
"../Watchdog.c" \
"../main.c" \
"../modulacion_vectorial.c" \
"../pll.c" \
"../pruebas.c" 

ASM_SRCS__QUOTED += \
"../CodeStartBranch.asm" \
"../DelayUs.asm" 


