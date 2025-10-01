#ifndef PWM_H
#define PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1


#define PWMPER 24.0

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents,int moteur);
void PWMUpdateSpeed(void);
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);

#endif
