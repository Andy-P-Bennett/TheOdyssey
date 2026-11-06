set(DEPENDENT_MP_BIN2HEXTheOdyssey_default_3A0J0cIh "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_3A0J0cIh ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey/default.elf)
set(DEPENDENT_TARGET_DIRTheOdyssey_default_3A0J0cIh ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey)
set(DEPENDENT_BYPRODUCTSTheOdyssey_default_3A0J0cIh ${DEPENDENT_TARGET_DIRTheOdyssey_default_3A0J0cIh}/${sourceFileNameTheOdyssey_default_3A0J0cIh}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRTheOdyssey_default_3A0J0cIh}/${sourceFileNameTheOdyssey_default_3A0J0cIh}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXTheOdyssey_default_3A0J0cIh} ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_3A0J0cIh} --image ${sourceFileNameTheOdyssey_default_3A0J0cIh} ${addressTheOdyssey_default_3A0J0cIh} ${modeTheOdyssey_default_3A0J0cIh} -mdfp=/Users/andy/.mchp_packs/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRTheOdyssey_default_3A0J0cIh}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_3A0J0cIh})
add_custom_target(
    dependent_produced_source_artifactTheOdyssey_default_3A0J0cIh 
    DEPENDS ${DEPENDENT_TARGET_DIRTheOdyssey_default_3A0J0cIh}/${sourceFileNameTheOdyssey_default_3A0J0cIh}.s
    )
