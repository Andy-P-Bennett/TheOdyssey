# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "/Users/andy/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(TheOdyssey_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=24F16KA301"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_SNAP=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h")
endfunction()
function(TheOdyssey_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=24F16KA301"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_SNAP=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h")
endfunction()
function(TheOdyssey_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=24F16KA301"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h")
endfunction()
function(TheOdyssey_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=24F16KA301"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(TheOdyssey_default_link_rule target)
    set(options
        "-g"
        "-mcpu=24F16KA301"
        "-Wl,--script=p24F16KA301.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_SNAP=1,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path=c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h"
        PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/generic/h")
endfunction()
function(TheOdyssey_default_bin2hex_rule target)
    add_custom_target(
        TheOdyssey_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${TheOdyssey_default_image_name} -a -mdfp=${PACK_REPO_PATH}/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16
        DEPENDS "${TheOdyssey_default_output_dir}/${TheOdyssey_default_image_name}"
        WORKING_DIRECTORY ${TheOdyssey_default_output_dir}
        BYPRODUCTS "${TheOdyssey_default_output_dir}/${TheOdyssey_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(TheOdyssey_default_Bin2Hex ${target})
endfunction()
function(TheOdyssey_default_objcopy_lss_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJDUMP}
        ARGS --disassemble --wide --demangle --line-numbers --section-headers --source ${TheOdyssey_default_image_name} > ${TheOdyssey_default_image_base_name}.lss
        WORKING_DIRECTORY ${TheOdyssey_default_output_dir})
endfunction()
