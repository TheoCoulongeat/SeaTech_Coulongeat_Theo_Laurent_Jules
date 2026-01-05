#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"
#include "ChipConfig.h"

// Déclaration des compteurs de temps
volatile unsigned long timestamp = 0;
volatile unsigned long t1 = 0;

void InitTimer1(void) {
    // Timer 1 configuré pour 10ms (exemple)
    T1CONbits.TON = 0; 
    T1CONbits.TCS = 0; 
    IFS0bits.T1IF = 0; 
    IEC0bits.T1IE = 1; 
    T1CONbits.TON = 1; 
    SetFreqTimer1(100); // 100Hz = 10ms
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    // On peut utiliser T1 pour faire clignoter une LED de vie si besoin
}

void InitTimer23(void) {
    // Timer 32 bits (si utilisé pour PWM ou autre)
    T3CONbits.TON = 0; 
    T2CONbits.TON = 0; 
    T2CONbits.T32 = 1; 
    T2CONbits.TCS = 0; 
    T2CONbits.TCKPS = 0b00; 
    TMR3 = 0x00; 
    TMR2 = 0x00; 
    PR3 = 0x0393; 
    PR2 = 0x8700; 
    IPC2bits.T3IP = 0x01; 
    IFS0bits.T3IF = 0; 
    IEC0bits.T3IE = 1; 
    T2CONbits.TON = 1; 
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; 
}

// --- TIMER 4 : LE COEUR DU SYSTEME ---
void InitTimer4(void) {
    // Timer4 pour horodater les mesures (1ms)
    T4CONbits.TON = 0; 
    T4CONbits.TCS = 0; 
    IFS1bits.T4IF = 0; 
    IEC1bits.T4IE = 1; 
    T4CONbits.TON = 1; 
    SetFreqTimer4(1000); // 1kHz = 1ms
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    IFS1bits.T4IF = 0;
    
    // INCREMENTATION DU TEMPS (VITAL)
    timestamp += 1;
    t1 += 1;
    
    // Lancement de la séquence de mesure
    // C'est ceci qui déclenche tout le reste (ADC -> OperatingSystem -> PWM)
    ADC1StartConversionSequence(); 
}

void SetFreqTimer1(float freq) {
    T1CONbits.TCKPS = 0b00; 
    if(FCY /freq > 65535) {
        T1CONbits.TCKPS = 0b01; 
        if(FCY /freq / 8 > 65535) {
            T1CONbits.TCKPS = 0b10; 
            if(FCY /freq / 64 > 65535) {
                T1CONbits.TCKPS = 0b11; 
                PR1 = (int)(FCY / freq / 256);
            } else PR1 = (int)(FCY / freq / 64);
        } else PR1 = (int)(FCY / freq / 8);
    } else PR1 = (int)(FCY / freq);
}

void SetFreqTimer4(float freq) {
    T4CONbits.TCKPS = 0b00; 
    if(FCY /freq > 65535) {
        T4CONbits.TCKPS = 0b01; 
        if(FCY /freq / 8 > 65535) {
            T4CONbits.TCKPS = 0b10; 
            if(FCY /freq / 64 > 65535) {
                T4CONbits.TCKPS = 0b11; 
                PR4 = (int)(FCY / freq / 256);
            } else PR4 = (int)(FCY / freq / 64);
        } else PR4 = (int)(FCY / freq / 8);
    } else PR4 = (int)(FCY / freq);
}