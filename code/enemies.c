#include "../headers/structures.h"
#include "../headers/consts.h"
#include "../headers/combos.h"

#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void enemyInitialize(Entity *enemy, SDL_Surface *enemy_tex, 
    Entity* player, EntityType enemyType) {

    int min_pos = player->position.x + 100;
    int max_pos = LEVEL_WIDTH - 100;

    enemy->position.x = (rand() % (max_pos - min_pos + 1)) + min_pos;
    enemy->position.y = FLOOR_ZERO_Y + 10; // (rand() % (FLOOR_ZERO_Y - min_pos + 1)) + min_pos;
    enemy->position.z = 0;

    if (enemy->position.x < 100) {
        enemy->position.x = 100;
    };  
    if (enemy->position.x > LEVEL_WIDTH - 100) {
        enemy->position.x = LEVEL_WIDTH - 100;
    };

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
                CHARGER_COOLDOWN
            };
            enemy->attackDamage = attackVal;
            enemy->speed = CHARGER_SPEED;
            break;
        };
        case ENTITY_ENEMY_WALKER: {
            enemy->health.maxHealth = WALKER_MAX_HP;
            enemy->health.health = WALKER_MAX_HP;

            enemy->measurements.w = WALKER_WIDTH;
            enemy->measurements.h = WALKER_HEIGHT;

            AttackValues attackVal = {
                WALKER_DAMAGE,
                WALKER_STUN,
                WALKER_COOLDOWN
            };
            enemy->attackDamage = attackVal;
            enemy->speed = WALKER_SPEED;
            break;
        };
        default: break;
    };

    enemy->currentState = ENTITY_DUMMY;
    enemy->scale = 1.0f;
    enemy->tex = enemy_tex;
    enemy->isCurrentlyAttacking = 0;
};

float calcDistance(Entity player, Entity enemy) {
    return  sqrt(pow(
        (enemy.position.x - player.position.x), 
        2
    ) + pow(
        (enemy.position.y - player.position.y),
        2
    )); // oblicza odleglosc do gracza uzywajac tw. pitagorasa
};



void enemyAttack(Entity* enemy, Entity* player, double delta) {
    if (enemy->health.health <= 0) return; // martwy nie atakuje
    
    float distanceToPlayer = sqrt(
        pow(enemy->position.x - player->position.x, 2) + 
        pow(enemy->position.y - player->position.y, 2)
    );

    if (enemy->type == ENTITY_ENEMY_CHARGER && distanceToPlayer < CHARGER_ATTACK_RANGE) {
        // Charger robi samobójczy atak tylko raz
        if (!enemy->isCurrentlyAttacking) {
            player->health.health -= enemy->attackDamage.damage;
            enemy->health.health = 0; // kamikaze
            enemy->isCurrentlyAttacking = 1;
            printf("CHARGER EXPLODED! Player HP: %d\n", player->health.health);
        }
    }
    
    if (enemy->type == ENTITY_ENEMY_WALKER && distanceToPlayer < WALKER_ATTACK_RANGE) {
        // Walker atakuje co WALKER_ATTACK_COUNTDOWN sekund
        if (enemy->attackDamage.attackCooldown <= 0) {
            player->health.health -= enemy->attackDamage.damage;
            enemy->attackDamage.attackCooldown = WALKER_ATTACK_COUNTDOWN;
            enemy->isCurrentlyAttacking = 1;
            printf("WALKER HIT! Player HP: %d\n", player->health.health);
        }
    }
    
    // Zmniejszaj cooldown
    if (enemy->attackDamage.attackCooldown > 0) {
        enemy->attackDamage.attackCooldown -= delta;
        if (enemy->attackDamage.attackCooldown <= 0) {
            enemy->isCurrentlyAttacking = 0;
        }
    }
}

void enemyUpdatePosition(Entity* enemy, Entity* player, double delta) {
    if (enemy->health.health <= 0) {
        return;
    };

    float dx = player->position.x - enemy->position.x;
    float dy = player->position.y - enemy->position.y;
    float distanceTOPlayer = calcDistance(*player, *enemy);

    switch (enemy->type) {
        case ENTITY_ENEMY_WALKER: {
            if (distanceTOPlayer > WALKER_ATTACK_RANGE ) {
                float dirX = dx / distanceTOPlayer;
                float dirY = dy / distanceTOPlayer;
                
                enemy->position.x += dirX * enemy->speed * delta;
                enemy->position.y += dirY * enemy->speed * delta;
                
                enemy->currentState = ENTITY_WALKING;
                if (dx < 0) {
                    enemy->facingLeft = 1;
                } else {
                    enemy->facingLeft = 0;
                };
            } else {
                enemy->currentState = ENITY_IDLE;
            };
            break;
        };
        case ENTITY_ENEMY_CHARGER: {
                
        };
        default: break;
    };
    enemy->hitboxes.x = enemy->position.x;
    enemy->hitboxes.y = enemy->position.y - enemy->measurements.h;
    enemy->hitboxes.w = enemy->measurements.w;
    enemy->hitboxes.h = enemy->measurements.h;
}



void enemiesUpdate(EnemiesData* enemiesData, Entity* player, double delta) {
    for (int i = 0; i < enemiesData->enemies_count; i++) {
        Entity* enemy = &enemiesData->enemies[i];
        
        if (enemy->health.health > 0) {
            enemyUpdatePosition(enemy, player, delta);
            enemyAttack(enemy, player, delta);
        }
    }
}