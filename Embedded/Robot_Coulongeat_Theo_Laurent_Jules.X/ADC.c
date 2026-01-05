#include <xc.h>
#include "adc.h"
#include "Robot.h"
#include "main.h"  // Pour OperatingSystemLoop
#include "PWM.h"   // Pour PWMUpdateSpeed

unsigned char ADCResultIndex = 0;
static unsigned int ADCResult[5];
unsigned char ADCConversionFinishedFlag;

void InitADC1(void) {
    // --- Configuration d'origine (Premier ADC.c) ---
    AD1CON1bits.ADON = 0; 
    AD1CON1bits.AD12B = 1; 
    AD1CON1bits.FORM = 0b00; 
    AD1CON1bits.ASAM = 0; 
    AD1CON1bits.SSRC = 0b111; 

    AD1CON2bits.VCFG = 0b000; 
    AD1CON2bits.CSCNA = 1; 
    AD1CON2bits.CHPS = 0b00; 
    AD1CON2bits.SMPI = 4; 
    AD1CON2bits.ALTS = 0;
    AD1CON2bits.BUFM = 0;

    AD1CON3bits.ADRC = 0; 
    AD1CON3bits.ADCS = 15; // Valeur d'origine
    AD1CON3bits.SAMC = 15; // Valeur d'origine
    AD1CON4bits.ADDMAEN = 0; 

    // --- Configuration des Pins (Valeurs d'origine) ---
    // Utilisation de ANSB0 au lieu de ANSA0 qui faisait planter
    ANSELBbits.ANSB0 = 1; 
    ANSELBbits.ANSB8 = 1;
    ANSELBbits.ANSB9 = 1;
    ANSELBbits.ANSB10 = 1;
    ANSELBbits.ANSB11 = 1;

    // --- Scan des canaux ---
    AD1CSSLbits.CSS0 = 1;  // Scan AN0 (Extreme Gauche)
    AD1CSSLbits.CSS8 = 1;  // Scan AN8 (Gauche)
    AD1CSSLbits.CSS9 = 1;  // Scan AN9 (Centre)
    AD1CSSLbits.CSS10 = 1; // Scan AN10 (Droit)
    AD1CSSLbits.CSS11 = 1; // Scan AN11 (Extreme Droite)

    IFS0bits.AD1IF = 0; 
    IEC0bits.AD1IE = 1; 
    AD1CON1bits.ADON = 1; 
}

void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) {
    IFS0bits.AD1IF = 0;

    // --- Lecture des Buffers ---
    // L'ADC range toujours du plus petit canal (AN0) au plus grand (AN11)
    unsigned int val_ext_gauche = ADC1BUF0; // AN0
    unsigned int val_gauche     = ADC1BUF1; // AN8
    unsigned int val_centre     = ADC1BUF2; // AN9
    unsigned int val_droit      = ADC1BUF3; // AN10
    unsigned int val_ext_droite = ADC1BUF4; // AN11

    // --- Conversion des distances ---
    
    // Extreme Gauche (AN0)
    if(val_ext_gauche > 100) robotState.distanceTelemetreExtremeGauche = (42200.0 / val_ext_gauche) - 5;
    else robotState.distanceTelemetreExtremeGauche = 80.0;
    
    // Gauche (AN8)
    if(val_gauche > 100) robotState.distanceTelemetreGauche = (42200.0 / val_gauche) - 5;
    else robotState.distanceTelemetreGauche = 80.0;

    // Centre (AN9)
    if(val_centre > 100) robotState.distanceTelemetreCentre = (42200.0 / val_centre) - 5;
    else robotState.distanceTelemetreCentre = 80.0; 

    // Droit (AN10)
    // CORRECTION ICI : val_droit au lieu de raw_droit
    if(val_droit > 100) robotState.distanceTelemetreDroit = (42200.0 / val_droit) - 5;
    else robotState.distanceTelemetreDroit = 80.0;
    
    // Extreme Droite (AN11)
    if(val_ext_droite > 100) robotState.distanceTelemetreExtremeDroite = (42200.0 / val_ext_droite) - 5;
    else robotState.distanceTelemetreExtremeDroite = 80.0;

    ADCConversionFinishedFlag = 1;

    // --- Lancement du Cerveau et des Moteurs ---
    // Si on enlève ça, le robot ne bouge pas
    OperatingSystemLoop();
    PWMUpdateSpeed();
}

void ADC1StartConversionSequence() {
    AD1CON1bits.SAMP = 1; 
}

unsigned int * ADCGetResult(void) {
    return ADCResult;
}

unsigned char ADCIsConversionFinished(void) {
    return ADCConversionFinishedFlag;
}

void ADCClearConversionFinishedFlag(void) {
    ADCConversionFinishedFlag = 0;
}