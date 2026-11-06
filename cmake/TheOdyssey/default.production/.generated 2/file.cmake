# The following variables contains the files used by the different stages of the build process.
set(TheOdyssey_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${TheOdyssey_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${TheOdyssey_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(TheOdyssey_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${TheOdyssey_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${TheOdyssey_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(TheOdyssey_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Line_following_base.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../motor.c")
set_source_files_properties(${TheOdyssey_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(TheOdyssey_default_default_XC16_FILE_TYPE_link)
set(TheOdyssey_default_default_XC16_FILE_TYPE_bin2hex)
set(TheOdyssey_default_default_XC16_FILE_TYPE_objcopy_lss)
set(TheOdyssey_default_image_name "default-production.elf")
set(TheOdyssey_default_image_base_name "default-production")

# The output directory of the final image.
set(TheOdyssey_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/TheOdyssey/production")

# The full path to the final image.
set(TheOdyssey_default_full_path_to_image ${TheOdyssey_default_output_dir}/${TheOdyssey_default_image_name})
