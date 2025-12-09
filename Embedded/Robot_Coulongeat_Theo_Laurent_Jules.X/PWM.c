#include <xc.h>
#include "IO.h"
#include "PWM.h"
#include <math.h>
#include "Robot.h"
#include "Toolbox.h"

#define PWMPER 24.0

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //ÈPriode en pourcentage
    //ÈRglage PWM moteur 1 sur hacheur 1
    IOCON1bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    FCLCON1 = 0x0003; //ÈDsactive la gestion des faults
    IOCON2bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON2bits.PENL = 1;
    IOCON2bits.PENH = 1;
    FCLCON2 = 0x0003; //ÈDsactive la gestion des faults
    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

double talon = 50; // Si on augmente beaucoup le talon on atteint plus la vitesse max et si <30 alors le hacheur ne fonctionne plus
/*
void PWMSetSpeed(float vitesseEnPourcents,int moteur)
{
    if(vitesseEnPourcents > 0){
        if(moteur == MOTEUR_GAUCHE){
            PDC1 = vitesseEnPourcents * PWMPER + talon;
            SDC1 = talon;
        }
        else{
            PDC2 = talon;
            SDC2 = vitesseEnPourcents * PWMPER + talon;
        }
        
    }
    else{
        if(moteur == MOTEUR_GAUCHE){
            PDC1 = talon;
            SDC1 = Abs(vitesseEnPourcents) * PWMPER + talon;
        }
        else{
            PDC2 = Abs(vitesseEnPourcents) * PWMPER + talon;
            SDC2 = talon;
        }
        
    }
}
*/

float acceleration = 5;
void PWMUpdateSpeed()
{
    // Cette fonction est appelee sur timer et permet de suivre des rampes d acceleration
    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(
        robotState.vitesseGaucheCommandeCourante + acceleration,
        robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(
        robotState.vitesseGaucheCommandeCourante - 2 * acceleration,
        robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante >= 0)
        {
        PDC1 = robotState.vitesseGaucheCommandeCourante * PWMPER + talon;
        SDC1 = talon;
        }
    else
        {
        PDC1 = talon;
        SDC1 = -robotState.vitesseGaucheCommandeCourante * PWMPER + talon;
        }
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Min(
        robotState.vitesseDroiteCommandeCourante + acceleration,
        robotState.vitesseDroiteConsigne);
    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Max(
        robotState.vitesseDroiteCommandeCourante - 2 * acceleration,
        robotState.vitesseDroiteConsigne);
    if (robotState.vitesseDroiteCommandeCourante >= 0)
        {
        PDC2 = robotState.vitesseDroiteCommandeCourante * PWMPER + talon;
        SDC2 = talon;
        }
    else
        {
        PDC2 = talon;
        SDC2 = -robotState.vitesseDroiteCommandeCourante * PWMPER + talon;
        }
}
/*
La fonction met ‡ jour progressivement la vitesse des moteurs du robot pour atteindre 
une consigne tout en respectant une rampe (pas fixe appelÈ acceleration). 
Si la vitesse est infÈrieure ‡ la consigne, on l?augmente avec Min(...) 
pour ne pas dÈpasser la consigne. Si elle est supÈrieure, on la diminue avec Max(...) 
pour ne pas descendre en dessous. Le signal PWM est ensuite ajustÈ selon la vitesse, 
pour gÈrer les moteurs dans le bon sens.
 Exemple pour passer de 0 ‡ 37 : 0 -> 5 -> 10 -> 15 -> 20 -> 25 -> 30 -> 35 -> 37
 */

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur){
    if (moteur == MOTEUR_GAUCHE) {
        robotState.vitesseGaucheConsigne = vitesseEnPourcents;
    } else if (moteur == MOTEUR_DROIT) {
        robotState.vitesseDroiteConsigne = -vitesseEnPourcents;
    }
}