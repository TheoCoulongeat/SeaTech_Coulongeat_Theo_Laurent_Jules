#ifndef MAIN_H
#define	MAIN_H

// --- Définition des Etats du Robot ---
#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_DROITE 6
#define STATE_TOURNE_DROITE_EN_COURS 7
#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
#define STATE_TOURNE_SUR_PLACE_DROITE 10
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
#define STATE_RECULE 12
#define STATE_RECULE_EN_COURS 13
#define STATE_DEMI_TOUR 14
#define STATE_DEMI_TOUR_EN_COURS 15

// --- Masques pour les capteurs (Logique binaire) ---
#define MASK_TGG 0b10000 // Extreme Gauche
#define MASK_TG  0b01000 // Gauche
#define MASK_TC  0b00100 // Centre
#define MASK_TD  0b00010 // Droit
#define MASK_TDD 0b00001 // Extreme Droit

// --- Constantes de détection ---
#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_EN_FACE 3

// --- Paramètres de Distance et Temps ---
#define DIST_VOIE_LIBRE 40.0
#define DIST_CRITIQUE 15.0
#define DIST_CRITIQUE1 12.0

#define TIMEOUT_BLOCAGE 2000      // 2 secondes max en rotation avant de reculer
#define MARGE_SECURITE_ROTATION 200 // Temps bonus pour bien se dégager

// Definition du type de fonction (Pointeur)
typedef void (*LogicFunction_t)(void);

// Prototypes
void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode(void);

#endif