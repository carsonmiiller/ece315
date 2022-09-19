################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
provided_code/%.obj: ../provided_code/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/ccs_base/arm/include" --include_path="C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/ccs_base/arm/include/CMSIS" --include_path="I:/ece315/ECE315-F21-Lab6" --include_path="C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="provided_code/$(basename $(<F)).d_raw" --obj_directory="provided_code" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


