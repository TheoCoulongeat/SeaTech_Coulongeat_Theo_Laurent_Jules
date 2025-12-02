#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "Toolbox.h"
#include "ADC.h"

int main (void){
    /***********************************************************************************************/
    //Initialisation oscillateur
    /***********************************************************************************************/
    InitOscillator();

    /***********************************************************************************************/
    // Configuration des input et output (IO)
    /***********************************************************************************************/
    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    LED_BLANCHE_2 = 1;
    LED_BLEUE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;
 
    InitTimer1();
    InitTimer23();
    InitTimer4();
    InitPWM();
    InitADC1();
    
    //PWMSetSpeed(0, 0);
    //PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
    //PWMSetSpeedConsigne(20, MOTEUR_DROIT);

    /***********************************************************************************************/
    //Boucle Principale
    /***********************************************************************************************/
    while(1)
    {                      
        if (ADCIsConversionFinished() == 1)
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts- 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts- 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts- 5;

        }
        
        int seuil = 30;
        
        if (robotState.distanceTelemetreGauche > seuil){
            LED_BLEUE_1 = 1;
        }
        else{
            LED_BLEUE_1 = 0;
        }
        if (robotState.distanceTelemetreCentre > seuil){
            LED_ORANGE_1 = 1;
        }
        else{
            LED_ORANGE_1 = 0;
        }
        if (robotState.distanceTelemetreDroit > seuil){
            LED_ROUGE_1 = 1;
        }
        else{
            LED_ROUGE_1 = 0;
        }        
      //LED_BLANCHE_1 = !LED_BLANCHE_1;
      
    }
       // fin main
    
    unsigned char stateRobot;
    
}