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
#include "main.h"

unsigned char stateRobot;

void OperatingSystemLoop() {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(22, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(22, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(19, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-19, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-19, MOTEUR_DROIT);
            PWMSetSpeedConsigne(19, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_EXTREME_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(19, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_EXTREME_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            PWMSetSpeedConsigne(19, MOTEUR_DROIT);
            stateRobot = STATE_TOURNE_EXTREME_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_EXTREME_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    if (STATE_AVANCE_EN_COURS) {
        //éDtermination de la position des obstacles en fonction des ééètlmtres
        if (robotState.distanceTelemetreExtremeDroite < 50 &&
                robotState.distanceTelemetreDroit > 50 &&
                robotState.distanceTelemetreCentre > 50)
            positionObstacle = OBSTACLE_EXTREME_DROITE;
        else if (robotState.distanceTelemetreExtremeGauche < 50 &&
                robotState.distanceTelemetreGauche > 50 &&
                robotState.distanceTelemetreCentre > 50)
            positionObstacle = OBSTACLE_EXTREME_GAUCHE;
        else if (robotState.distanceTelemetreDroit < 50 &&
                robotState.distanceTelemetreCentre > 50 &&
                robotState.distanceTelemetreGauche > 50) //Obstacle àdroite
            positionObstacle = OBSTACLE_A_DROITE;
        else if (robotState.distanceTelemetreDroit > 50 &&
                robotState.distanceTelemetreCentre > 50 &&
                robotState.distanceTelemetreGauche < 50) //Obstacle àgauche
            positionObstacle = OBSTACLE_A_GAUCHE;
        else if (robotState.distanceTelemetreCentre < 40) //Obstacle en face
            positionObstacle = OBSTACLE_EN_FACE;
        else if (robotState.distanceTelemetreDroit > 50 &&
                robotState.distanceTelemetreCentre > 50 &&
                robotState.distanceTelemetreGauche > 50 &&
                robotState.distanceTelemetreExtremeGauche > 50 &&
                robotState.distanceTelemetreExtremeDroite > 50) //pas d?obstacle
            positionObstacle = PAS_D_OBSTACLE;


        //éDtermination de lé?tat àvenir du robot
        if (positionObstacle == PAS_D_OBSTACLE)
            nextStateRobot = STATE_AVANCE;
        else if (positionObstacle == OBSTACLE_A_DROITE)
            nextStateRobot = STATE_TOURNE_GAUCHE;
        else if (positionObstacle == OBSTACLE_A_GAUCHE)
            nextStateRobot = STATE_TOURNE_DROITE;
        else if (positionObstacle == OBSTACLE_EN_FACE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        else if (positionObstacle == OBSTACLE_EXTREME_DROITE)
            nextStateRobot = STATE_TOURNE_EXTREME_GAUCHE;
        else if (positionObstacle == OBSTACLE_EXTREME_GAUCHE)
            nextStateRobot = STATE_TOURNE_EXTREME_DROITE;
        //Si l?on n?est pas dans la transition de lé?tape en cours
        if (nextStateRobot != stateRobot - 1)
            stateRobot = nextStateRobot;
        /* Ces deux lignes servent de filtre. Elles assurent que le robot ne change
         d'état proncipal que si la nouvelle action décidée par les capteurs est réellement
         différente de l'action en cours */
    } else {
        //éDtermination de la position des obstacles en fonction des ééètlmtres
        if (robotState.distanceTelemetreExtremeDroite < 30 &&
                robotState.distanceTelemetreDroit > 20 &&
                robotState.distanceTelemetreCentre > 30)
            positionObstacle = OBSTACLE_EXTREME_DROITE;
        else if (robotState.distanceTelemetreExtremeGauche < 30 &&
                robotState.distanceTelemetreGauche > 20 &&
                robotState.distanceTelemetreCentre > 30)
            positionObstacle = OBSTACLE_EXTREME_GAUCHE;
        else if (robotState.distanceTelemetreDroit < 30 &&
                robotState.distanceTelemetreCentre > 20 &&
                robotState.distanceTelemetreGauche > 30) //Obstacle àdroite
            positionObstacle = OBSTACLE_A_DROITE;
        else if (robotState.distanceTelemetreDroit > 30 &&
                robotState.distanceTelemetreCentre > 20 &&
                robotState.distanceTelemetreGauche < 30) //Obstacle àgauche
            positionObstacle = OBSTACLE_A_GAUCHE;
        else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
            positionObstacle = OBSTACLE_EN_FACE;
        else if (robotState.distanceTelemetreDroit > 30 &&
                robotState.distanceTelemetreCentre > 20 &&
                robotState.distanceTelemetreGauche > 30 &&
                robotState.distanceTelemetreExtremeGauche > 30 &&
                robotState.distanceTelemetreExtremeDroite > 30) //pas d?obstacle
            positionObstacle = PAS_D_OBSTACLE;


        //éDtermination de lé?tat àvenir du robot
        if (positionObstacle == PAS_D_OBSTACLE)
            nextStateRobot = STATE_AVANCE;
        else if (positionObstacle == OBSTACLE_A_DROITE)
            nextStateRobot = STATE_TOURNE_GAUCHE;
        else if (positionObstacle == OBSTACLE_A_GAUCHE)
            nextStateRobot = STATE_TOURNE_DROITE;
        else if (positionObstacle == OBSTACLE_EN_FACE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        else if (positionObstacle == OBSTACLE_EXTREME_DROITE)
            nextStateRobot = STATE_TOURNE_EXTREME_GAUCHE;
        else if (positionObstacle == OBSTACLE_EXTREME_GAUCHE)
            nextStateRobot = STATE_TOURNE_EXTREME_DROITE;
        //Si l?on n?est pas dans la transition de lé?tape en cours
        if (nextStateRobot != stateRobot - 1)
            stateRobot = nextStateRobot;
        /* Ces deux lignes servent de filtre. Elles assurent que le robot ne change
         d'état proncipal que si la nouvelle action décidée par les capteurs est réellement
         différente de l'action en cours */

    }
}

int main(void) {
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
    LED_BLANCHE_2 = 0;
    LED_BLEUE_2 = 0;
    LED_ORANGE_2 = 0;
    LED_ROUGE_2 = 0;
    LED_VERTE_2 = 0;

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

    LED_ORANGE_2 = 1;

    while (BOUTON_1 == 0) {
        PWMSetSpeedConsigne(0, MOTEUR_DROIT);
        PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
    }

    LED_ORANGE_2 = 0;
    timestamp = 0;
    ADC1StartConversionSequence();

    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeDroite = 34 / volts - 5;

        }

        int seuil = 30;

        if (robotState.distanceTelemetreGauche > seuil) {
            LED_BLEUE_1 = 1;
        } else {
            LED_BLEUE_1 = 0;
        }
        if (robotState.distanceTelemetreCentre > seuil) {
            LED_ORANGE_1 = 1;
        } else {
            LED_ORANGE_1 = 0;
        }
        if (robotState.distanceTelemetreDroit > seuil) {
            LED_ROUGE_1 = 1;
        } else {
            LED_ROUGE_1 = 0;
        }
        if (robotState.distanceTelemetreExtremeGauche > seuil) {
            LED_BLANCHE_1 = 1;
        } else {
            LED_BLANCHE_1 = 0;
        }
        if (robotState.distanceTelemetreExtremeDroite > seuil) {
            LED_VERTE_1 = 1;
        } else {
            LED_VERTE_1 = 0;
        }
        //LED_BLANCHE_1 = !LED_BLANCHE_1;
        SetNextRobotStateInAutomaticMode();
        if (timestamp > 60000) {
            break;
        }
    }
    // fin main


}