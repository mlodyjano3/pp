#include "../headers/structures.h"
#include "../headers/consts.h"
#include "../headers/combos.h"
#include "../headers/points.h"

#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void enemyBoundaries(Entity *enemy) {
    if (enemy->position.x < 0) {
        enemy->position.x = 0;
    }
    if (enemy->position.x > LEVEL_WIDTH - enemy->measurements.w ) {
        enemy->position.x = LEVEL_WIDTH - enemy->measurements.w;
    }
    
    float currentHeight = enemy->measurements.h * enemy->scale;
    if (enemy->position.y + currentHeight < FLOOR_ZERO_Y) {
        enemy->position.y = FLOOR_ZERO_Y -currentHeight;
    };
    if (enemy->position.y + currentHeight > SCREEN_HEIGHT) {
        enemy->position.y = SCREEN_HEIGHT - currentHeight;
    };
};

void enemyInitialize(Entity *enemy, SDL_Surface *enemy_tex, 
    Entity* player, EntityType enemyType) {

    int min_pos = player->position.x + 100;
    int max_pos = LEVEL_WIDTH - 100;
    
    enemy->position.x = (rand() % (max_pos - min_pos + 1)) + min_pos;
    enemy->position.y = FLOOR_ZERO_Y + (rand() % 300);
    enemy->position.z = 0;

    if (enemy->position.x < 100) {
        enemy->position.x = 100;
    }
    if (enemy->position.x > LEVEL_WIDTH - 100) {
        enemy->position.x = LEVEL_WIDTH - 100;
    }

    enemy->type = enemyType;
    switch (enemyType) {
        case ENTITY_ENEMY_CHARGER: {
            enemy->health.maxHealth = CHARGER_MAX_HP;
            enemy->health.health = CHARGER_MAX_HP;

            enemy->measurements.w = CHARGER_WIDTH;
            enemy->measurements.h = CHARGER_HEIGHT;

            AttackValues attackVal = {
                CHARGER_DAMAGE,
                CHARGER_STUN,
                0  // attackCooldown zaczyna się od 0!
            };
            enemy->attackDamage = attackVal;
            enemy->speed = CHARGER_SPEED;
            
            enemy->chargerData.isCharging = 0;
            enemy->chargerData.chargeTimer = 0;
            break;
        }
        case ENTITY_ENEMY_WALKER: {
            enemy->health.maxHealth = WALKER_MAX_HP;
            enemy->health.health = WALKER_MAX_HP;

            enemy->measurements.w = WALKER_WIDTH;
            enemy->measurements.h = WALKER_HEIGHT;

            AttackValues attackVal = {
                WALKER_DAMAGE,
                WALKER_STUN,
                0  // attackCooldown zaczyna się od 0!
            };
            enemy->attackDamage = attackVal;
            enemy->speed = WALKER_SPEED;
            break;
        }
        default: break;
    }

    enemy->currentState = ENTITY_DUMMY;
    enemy->scale = 1.0f;
    enemy->tex = enemy_tex;
    enemy->isCurrentlyAttacking = 0;
    enemy->wasHitThisAttack = 0;
};

float calcDistance(Entity player, Entity enemy) {
    return sqrt(pow(
        (enemy.position.x - player.position.x), 
        2
    ) + pow(
        (enemy.position.y - player.position.y),
        2
    ));
}

void enemyAttack(Entity* enemy, Entity* player, double delta, GameState* gameState) {
    if (enemy->health.health <= 0) {
        return;
    };
    
    // nie atakuj jezeli gracz jest nietykalny
    if (player->isInvicible) return;
    
    float distanceToPlayer = calcDistance(*player, *enemy);

    if (enemy->type == ENTITY_ENEMY_CHARGER && distanceToPlayer < CHARGER_ATTACK_RANGE) {
        if (!enemy->isCurrentlyAttacking) {
            player->health.health -= enemy->attackDamage.damage;
            if (player->health.health < 0 ) {
                player->health.health = 0;
            };

            enemy->health.health = 0; // kamikaze
            enemy->isCurrentlyAttacking = 1;
            
            // reset combo dla gracza
            scoringOnPlayerHit(gameState);
            
            // animacja trafienia gracza
            player->currentState = ENTITY_HIT;
            player->timer = PLAYER_HIT_DURATION;
            player->isInvicible = 1;
            player->invicibilityTimer = PLAYER_INVINCIBILITY_TIME;
            
            printf("Charger zaatakowal pomyslnie! Gracz hp: %d\n", player->health.health);
        }
    }
    
    if (enemy->type == ENTITY_ENEMY_WALKER && distanceToPlayer < WALKER_ATTACK_RANGE) {
        if (enemy->attackDamage.attackCooldown <= 0) {
            player->health.health -= enemy->attackDamage.damage;
            if (player->health.health < 0) {
                player->health.health = 0;
            };
            enemy->attackDamage.attackCooldown = WALKER_ATTACK_COUNTDOWN;
            enemy->isCurrentlyAttacking = 1;
            
            scoringOnPlayerHit(gameState);
            
            player->currentState = ENTITY_HIT;
            player->timer = PLAYER_HIT_DURATION;
            player->isInvicible = 1;
            player->invicibilityTimer = PLAYER_INVINCIBILITY_TIME;
            
            printf("Walker zaatakowal pomyslnie! Gracz HP: %d\n", player->health.health);
        }
    }
    
    // zmniejsza cooldown
    if (enemy->attackDamage.attackCooldown > 0) {
        enemy->attackDamage.attackCooldown -= delta;
        if (enemy->attackDamage.attackCooldown <= 0) {
            enemy->isCurrentlyAttacking = 0;
        }
    }
};

void chargerCharge(Entity* enemy, double delta) {
    enemy->chargerData.chargeTimer -= delta;
                
    // nie zmienia kierunku szarzy 
    int chargeDir;
    if (enemy->facingLeft) {
        chargeDir = -1;
    } else {
        chargeDir = 1;
    }
    enemy->position.x += chargeDir * CHARGER_CHARGE_SPEED * delta;
                
    // koniec ataku przez timer
    if (enemy->chargerData.chargeTimer <= 0) {
        enemy->chargerData.isCharging = 0;
        enemy->currentState = ENITY_IDLE;
        // cooldown przed nastepna
        enemy->attackDamage.attackCooldown = 3.;
        printf("Charger przestal atakowac - cooldown 3s\n");
    };
};

void chargerNoCharge(Entity* enemy, double delta, float distanceToPlayer, float dx, float dy) {
    // cooldown po szarzy
    if (enemy->attackDamage.attackCooldown > 0) {
        enemy->attackDamage.attackCooldown -= delta;
        enemy->currentState = ENITY_IDLE;
    } else {
        // 
        if (distanceToPlayer > CHARGER_DETECT_RANGE + 50) {
            float dirX = dx / distanceToPlayer;
            float dirY = dy / distanceToPlayer;

            // wolniejszy ruch niz walker
            enemy->position.x += dirX * (enemy->speed * 0.4f) * delta;
            enemy->position.y += dirY * (enemy->speed * 0.4f) * delta;

            enemy->currentState = ENTITY_WALKING;

            if (dx < 0) {
                enemy->facingLeft = 1;
            } else {
                enemy->facingLeft = 0;
            }
        } else {
            // w zasięgu
            enemy->currentState = ENITY_IDLE;
        }
    }
}

void enemyUpdatePosition(Entity* enemy, Entity* player, double delta) {
    if (enemy->health.health <= 0) {
        return;
    }

    float dx = player->position.x - enemy->position.x;
    float dy = player->position.y - enemy->position.y;
    float distanceToPlayer = calcDistance(*player, *enemy);

    switch (enemy->type) {
        case ENTITY_ENEMY_WALKER: {
            // walker zostaje 20przed graczem
            float stopDistance = WALKER_ATTACK_RANGE - 20;
            
            if (distanceToPlayer > stopDistance) {
                float dirX = dx / distanceToPlayer;
                float dirY = dy / distanceToPlayer;
                
                enemy->position.x += dirX * enemy->speed * delta;
                enemy->position.y += dirY * enemy->speed * delta;
                
                enemy->currentState = ENTITY_WALKING;
                
                // kierunek patrzenia
                if (dx < 0) {
                    enemy->facingLeft = 1;
                } else {
                    enemy->facingLeft = 0;
                }
            } else {
                // stoi w miejscu i czeka na atak
                enemy->currentState = ENITY_IDLE;
            }
            break;
        }
        
        case ENTITY_ENEMY_CHARGER: {
            // jezeli gracz jest w zasiegu wykrywania
            if (distanceToPlayer < CHARGER_DETECT_RANGE && !enemy->chargerData.isCharging) {
                if (dx < 0) {
                    enemy->facingLeft = 1;
                } else {
                    enemy->facingLeft = 0;
                }
                
                enemy->chargerData.isCharging = 1;
                enemy->chargerData.chargeTimer = CHARGER_CHARGE_DURATION;
                enemy->currentState = ENTITY_CHARGING;
                
                printf("Charger zaczyna atak na dystansie: %.0f\n", distanceToPlayer);
            }
            
            if (enemy->chargerData.isCharging) {
                chargerCharge(enemy, delta);
            } else { // jak nie atakuje
                chargerNoCharge(enemy, delta, distanceToPlayer, dx, dy);
            }
            break;
        };
        default: {
            break;
        };
    }
    
    // ograniczenia pozycji wroga
    enemyBoundaries(enemy);

    float currentHeight = enemy->measurements.h * enemy->scale;
    float currentWidth = enemy->measurements.w * enemy->scale;
    
    // update hitboxa
    enemy->hitboxes.x = enemy->position.x;
    enemy->hitboxes.y = enemy->position.y - enemy->measurements.h;
    enemy->hitboxes.w = enemy->measurements.w;
    enemy->hitboxes.h = enemy->measurements.h;
}

void enemiesUpdate(EnemiesData* enemiesData, Entity* player, double delta, GameState* gameState) {
    for (int i = 0; i < enemiesData->enemies_count; i++) {
        Entity* enemy = &enemiesData->enemies[i];
        
        if (enemy->health.health > 0) {
            enemyUpdatePosition(enemy, player, delta);
            enemyAttack(enemy, player, delta, gameState);
        }
    }
}