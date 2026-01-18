#include "../headers/structures.h"
#include "../headers/consts.h"
#include "../headers/points.h"
#include "../headers/player.h"
#include "../headers/enemies.h"

#include <stdio.h>

int checkCollision(Hitbox player, Hitbox enemy) {
    // zwraca 0 jezeli nie nachodza na siebie
    if (player.x + player.w < enemy.x) {
        return 0;
    } else if (player.x > enemy.x + enemy.w) {
        return 0;
    } else if (player.y + player.h < enemy.y) {
        return 0;
    } else if (player.y > enemy.y + enemy.h) {
        return 0;
    }
    // zwraca 1 jezeli nachodza --- zwykly bool
    return 1; 
}

void updatePlayerHitboxes(Entity* player) {
    float currentHeight = player->measurements.h * player->scale;
    float currentWidth = player->measurements.w * player->scale;
    
    player->hitboxes.x = player->position.x;
    player->hitboxes.y = player->position.y - currentHeight - player->position.z;
    player->hitboxes.w = currentWidth;
    player->hitboxes.h = currentHeight;
    
    int attackOffsetX = 0;
    if (player->facingLeft) {
        attackOffsetX = -60;
    } else {
        attackOffsetX = currentWidth;
    }
    
    int attackWidth = 0;
    int attackHeight = 0;
    
    if (player->currentState == ENTITY_ATTACK_LIGHT) {
        attackWidth = LIGHT_ATTACK_WIDTH;
        attackHeight = LIGHT_ATTACK_HEIGHT;
    } else if (player->currentState == ENTITY_ATTACK_HEAVY) {
        attackWidth = HEAVY_ATTACK_WIDTH;
        attackHeight = HEAVY_ATTACK_HEIGHT;
    } else if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT) {
        attackWidth = LIGHT_ATTACK_WIDTH + 20;
        attackHeight = LIGHT_ATTACK_HEIGHT + 10;
    } else if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) {
        attackWidth = HEAVY_ATTACK_WIDTH + 30;
        attackHeight = HEAVY_ATTACK_HEIGHT + 20;
    } else if (player->currentState == ENTITY_COMBO_MIXED) {
        attackWidth = HEAVY_ATTACK_WIDTH + 10;
        attackHeight = HEAVY_ATTACK_HEIGHT + 10;
    }
    
    player->attacking_hitboxes.x = player->position.x + attackOffsetX;
    player->attacking_hitboxes.y = player->position.y - attackHeight - player->position.z;
    player->attacking_hitboxes.w = attackWidth;
    player->attacking_hitboxes.h = attackHeight;
};



void checkPlayerAttackCollisions(Entity* player, EnemiesData* enemiesData, GameState* gameState) {
    int isAttacking = (IS_PLAYER_ATTACKING_SHORTCUT);

    if (!isAttacking) {
        player->isCurrentlyAttacking = 0;
        for (int i = 0; i < enemiesData->enemies_count; i++) {
            enemiesData->enemies[i].wasHitThisAttack = 0;
        }
        return;
    }
    
    if (player->isCurrentlyAttacking == 0) {
        player->isCurrentlyAttacking = 1;
    }
    
    for (int i = 0; i < enemiesData->enemies_count; i++) {
        Entity* enemy = &enemiesData->enemies[i];
        
        // POPRAWKA: <= 0 zamiast == 0
        if (enemy->health.health <= 0) {
            continue;
        }
        
        if (checkCollision(player->attacking_hitboxes, enemy->hitboxes)) {
            if (enemy->wasHitThisAttack == 1) {
                continue;
            }
            
            int damage = 10;
            
            if (player->currentState == ENTITY_ATTACK_LIGHT) {
                damage = 15;
            } else if (player->currentState == ENTITY_ATTACK_HEAVY) {
                damage = 30;
            } else if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT) {
                damage = 50;
            } else if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) {
                damage = 80;
            } else if (player->currentState == ENTITY_COMBO_MIXED) {
                damage = 65;
            }
            
            enemy->health.health -= damage;
            
            // POPRAWKA: Ogranicz HP do 0
            if (enemy->health.health < 0) {
                enemy->health.health = 0;
            }
            
            enemy->wasHitThisAttack = 1;
            scoringOnEnemyHit(gameState, player);
            
            printf("Trafiono wroga! HP: %d, Obrazenia: %d\n", enemy->health.health, damage);
        }
    }
};



void cameraUpdate(Camera* camera, Entity player) {
    camera->position.x = player.position.x -(SCREEN_WIDTH / 2) + (player.measurements.h / 2); // logika positioningu kamery na osi x
	camera->position.y = SCREEN_HEIGHT / 2; // kamera zawsze pozostaje w tym samym miejscu osi y

	if (camera->position.x < 0) {
        camera->position.x = 0;
    };
	if (camera->position.x > LEVEL_WIDTH - SCREEN_WIDTH) {
        camera->position.x = LEVEL_WIDTH - SCREEN_WIDTH;
    };
};

void updateFPS(GameState* gameState, int* frames, double delta) {
    gameState->fpsTimer += delta;
    if (gameState->fpsTimer > 0.5) {
        gameState->fps = *frames * 2;
        *frames = 0;
        gameState->fpsTimer -= 0.5;
    }
};

void updateAll(Entity* player, EnemiesData* enemies, GameState* gameState, Camera* camera, double delta) {
    playerUpdate(player, delta, enemies, gameState);
    updatePlayerHitboxes(player);
    checkPlayerAttackCollisions(player, enemies, gameState);
    scoringUpdate(gameState, delta);
    enemiesUpdate(enemies, player, delta, gameState);
    cameraUpdate(camera, *player);
};

double calculateDelta(int* t1, GameState* gameState) {
    int t2 = SDL_GetTicks();
    double delta = (t2 - *t1) * 0.001;
    *t1 = t2;
    gameState->worldTime += delta;
    
    return delta;
};