set(DEPENDENT_MP_BIN2HEXTheOdyssey_default_8SxA0lXD "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_8SxA0lXD ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey/production/default-production.elf)
set(DEPENDENT_TARGET_DIRTheOdyssey_default_8SxA0lXD ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TheOdyssey/production)
set(DEPENDENT_BYPRODUCTSTheOdyssey_default_8SxA0lXD ${DEPENDENT_TARGET_DIRTheOdyssey_default_8SxA0lXD}/${sourceFileNameTheOdyssey_default_8SxA0lXD}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRTheOdyssey_default_8SxA0lXD}/${sourceFileNameTheOdyssey_default_8SxA0lXD}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXTheOdyssey_default_8SxA0lXD} ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_8SxA0lXD} --image ${sourceFileNameTheOdyssey_default_8SxA0lXD} ${addressTheOdyssey_default_8SxA0lXD} ${modeTheOdyssey_default_8SxA0lXD} -mdfp=/Users/andy/.mchp_packs/Microchip/PIC24F-KA-KL-KM_DFP/1.6.395/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRTheOdyssey_default_8SxA0lXD}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFTheOdyssey_default_8SxA0lXD})
add_custom_target(
    dependent_produced_source_artifactTheOdyssey_default_8SxA0lXD 
    DEPENDS ${DEPENDENT_TARGET_DIRTheOdyssey_default_8SxA0lXD}/${sourceFileNameTheOdyssey_default_8SxA0lXD}.s
    )
