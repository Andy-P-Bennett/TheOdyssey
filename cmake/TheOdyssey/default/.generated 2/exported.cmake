set(DEPENDENT_MP_BIN2HEXTheOdyssey_default_GBot3_ks "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_GBot3_ks ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey/default.elf)
set(DEPENDENT_TARGET_DIRTheOdyssey_default_GBot3_ks ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey)
set(DEPENDENT_BYPRODUCTSTheOdyssey_default_GBot3_ks ${DEPENDENT_TARGET_DIRTheOdyssey_default_GBot3_ks}/${sourceFileNameTheOdyssey_default_GBot3_ks}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRTheOdyssey_default_GBot3_ks}/${sourceFileNameTheOdyssey_default_GBot3_ks}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXTheOdyssey_default_GBot3_ks} ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_GBot3_ks} --image ${sourceFileNameTheOdyssey_default_GBot3_ks} ${addressTheOdyssey_default_GBot3_ks} ${modeTheOdyssey_default_GBot3_ks} -mdfp=/Users/andy/.mchp_packs/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRTheOdyssey_default_GBot3_ks}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_GBot3_ks})
add_custom_target(
    dependent_produced_source_artifactTheOdyssey_default_GBot3_ks 
    DEPENDS ${DEPENDENT_TARGET_DIRTheOdyssey_default_GBot3_ks}/${sourceFileNameTheOdyssey_default_GBot3_ks}.s
    )
