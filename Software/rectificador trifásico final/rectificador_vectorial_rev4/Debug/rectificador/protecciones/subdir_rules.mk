################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
rectificador/protecciones/%.obj: ../rectificador/protecciones/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/lraff/Desktop/proyecto_final/workspace_cc/rectificador_vectorial_rev4" --include_path="C:/Users/lraff/Desktop/proyecto_final/lib_cc/IQmath/include" --include_path="C:/Users/lraff/Desktop/proyecto_final/lib_cc/DSP2833x_headers/include" --include_path="C:/Users/lraff/Desktop/proyecto_final/lib_cc/renolib/include" --include_path="C:/Users/lraff/Desktop/proyecto_final/lib_cc/renolib/source" --include_path="C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="rectificador/protecciones/$(basename $(<F)).d_raw" --obj_directory="rectificador/protecciones" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


