#include "../headers/structures.h"
#include "../headers/consts.h"
#include <stdio.h>

void scoringInitialize(GameState* gameState) {
    gameState->score = 0;
    gameState->currentMultiplier = 1.0f;
    gameState->comboTimer = 0;
}

void scoringUpdate(GameState* gameState, double delta) {

    if (gameState->comboTimer > 0) {
        gameState->comboTimer -= delta;
        
        // reset myltiplieru jak konczy sie czas na combo
        if (gameState->comboTimer <= 0) {
            gameState->currentMultiplier = 1.0f;
            gameState->comboTimer = 0;
            printf("Combo przerwane! Multiplier zresetowany.\n");
        }
    }
}

void scoringAddPoints(GameState* gameState, Entity* player, int basePoints) {
    // oblicz punkty z multiplierem
    int points = (int)(basePoints * gameState->currentMultiplier);
    gameState->score += points;
    
    // przedluz czas combo
    gameState->comboTimer = COMBO_MULTIPLIER_TIME;
    
    // zwieksz multiplier (max 5x)
    if (gameState->currentMultiplier < 5.0f) {
        gameState->currentMultiplier += 0.5f;
    }
    
    printf("Zdobyto %d punktow! (x%.1f) Suma: %.0f\n", 
           points, gameState->currentMultiplier, gameState->score);
}

void scoringOnEnemyHit(GameState* gameState, Entity* player) {
    int basePoints = 0;
    
    // ustal punkty bazowe na podstawie typu ataku
    if (player->currentState == ENTITY_ATTACK_LIGHT) {
        basePoints = POINTS_LIGHT_ATTACK;
    } else if (player->currentState == ENTITY_ATTACK_HEAVY) {
        basePoints = POINTS_HEAVY_ATTACK;
    } else if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT) {
        basePoints = POINTS_COMBO_TRIPLE_LIGHT;
    } else if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) {
        basePoints = POINTS_COMBO_TRIPLE_HEAVY;
    } else if (player->currentState == ENTITY_COMBO_MIXED) {
        basePoints = POINTS_COMBO_MIXED;
    }
    
    // dodaj punkty jezeli jest jakis atak
    if (basePoints > 0) {
        scoringAddPoints(gameState, player, basePoints);
    }
}

void scoringOnPlayerHit(GameState* gameState) {
    // gracz zostal trafiony - resetuj combo
    gameState->currentMultiplier = 1.0f;
    gameState->comboTimer = 0;
    printf("Zostales trafiony! Combo stracone!\n");
}