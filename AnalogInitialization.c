#include <xc.h>
#include "AnalogInitialization.h"


void init_an(void) {
    // set up pins as analog and input
    _TRISA0 = 1; // pin 2
    _TRISA1 = 1; // pin 3
    _TRISB2 = 1; // pin 6
    _TRISA2 = 1; // pin 7
    _TRISA3 = 1; // pin 8
    _TRISB12 = 1; // pin 15
    _TRISB13 = 1; // pin 16
    _TRISB14 = 1; // pin 17
    _TRISB15 = 1; // pin 18
    _ANSA0 = 1; // pin 2
    _ANSA1 = 1; // pin 3
    _ANSB2 = 1; // pin6
    _ANSA2 = 1; // pin 7
    _ANSA3 = 1; // pin 8
    _ANSB12 = 1; // pin 15
    _ANSB13 = 1; // pin 16
    _ANSB14 = 1; // pin 17
    _ANSB15 = 1; // pin 18

    // analog configuration
    _ADON = 0; // disable analog during set-up
    
    // AD1CON1----------------------------------------------------------------------
    _MODE12 = 0;  // 10-bit resolution
    _FORM = 0;    // unsigned integer output
    _SSRC = 7;    // auto convert
    _ASAM = 1;    // auto sample on
    
    // AD1CON2----------------------------------------------------------------------
    _PVCFG = 0;   // use VDD as positive reference
    _NVCFG = 0;   // use VSS as negative reference
    _BUFREGEN = 1;// store results in buffer corresponding to channel number
    _CSCNA = 1;   // scanning mode
    _SMPI = 8;    // begin new sampling sequence after every sample (9 pins - 1)
    _ALTS = 0;    // sample MUXA only

    // AD1CON3---------------------------------------------------------------------
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 0x3F; // TAD = 64*TCY

    //set up sharp pins as analog inputs---------------------------------------------
    _CSS0 = 1;  // pin 2 AN0
    _CSS1 = 1; // pin 3 AN1
    _CSS4 = 1; // pin 6 AN4
    _CSS13 = 1; // pin 7 AN13
    _CSS14 = 1; // pin 8 AN14
    _CSS12 = 1; // pin 15 AN12
    _CSS11 = 1; // pin 16 AN11
    _CSS10 = 1; // pin 17 AN10
    _CSS9 = 1; // pin 18 AN9

    _ADON = 1; // turn on analog
}