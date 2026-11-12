include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(TheOdyssey_default_library_list )

set(OUTPUT_SUFFIX elf)
# Handle files with suffix s, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_assemble)
add_library(TheOdyssey_default_default_XC16_assemble OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_assemble})
    TheOdyssey_default_default_XC16_assemble_rule(TheOdyssey_default_default_XC16_assemble)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(TheOdyssey_default_default_XC16_assemblePreproc OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_assemblePreproc})
    TheOdyssey_default_default_XC16_assemblePreproc_rule(TheOdyssey_default_default_XC16_assemblePreproc)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_compile)
add_library(TheOdyssey_default_default_XC16_compile OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_compile})
    TheOdyssey_default_default_XC16_compile_rule(TheOdyssey_default_default_XC16_compile)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_dependentObject)
add_library(TheOdyssey_default_default_XC16_dependentObject OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_dependentObject})
    TheOdyssey_default_default_XC16_dependentObject_rule(TheOdyssey_default_default_XC16_dependentObject)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_bin2hex)
add_library(TheOdyssey_default_default_XC16_bin2hex OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_bin2hex})
    TheOdyssey_default_default_XC16_bin2hex_rule(TheOdyssey_default_default_XC16_bin2hex)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_bin2hex>")

endif()

# Handle files with suffix elf, for group default-XC16
if(TheOdyssey_default_default_XC16_FILE_TYPE_objcopy_lss)
add_library(TheOdyssey_default_default_XC16_objcopy_lss OBJECT ${TheOdyssey_default_default_XC16_FILE_TYPE_objcopy_lss})
    TheOdyssey_default_default_XC16_objcopy_lss_rule(TheOdyssey_default_default_XC16_objcopy_lss)
    list(APPEND TheOdyssey_default_library_list "$<TARGET_OBJECTS:TheOdyssey_default_default_XC16_objcopy_lss>")

endif()


# Main target for this project
add_executable(TheOdyssey_default_image_GBot3_ks ${TheOdyssey_default_library_list})

set_target_properties(TheOdyssey_default_image_GBot3_ks PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${TheOdyssey_default_output_dir}")
target_link_libraries(TheOdyssey_default_image_GBot3_ks PRIVATE ${TheOdyssey_default_default_XC16_FILE_TYPE_link})
# Add the link options from the rule file.
TheOdyssey_default_link_rule( TheOdyssey_default_image_GBot3_ks)

# Call bin2hex function from the rule file
TheOdyssey_default_bin2hex_rule(TheOdyssey_default_image_GBot3_ks)

#Add objcopy steps
TheOdyssey_default_objcopy_lss_rule(TheOdyssey_default_image_GBot3_ks)

