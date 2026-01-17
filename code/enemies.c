#include "../headers/structures.h"
#include "../headers/consts.h"
#include "../headers/combos.h"

#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void enemyInitialize(Entity enemy, SDL_Surface *enemy_tex, 
    Entity player, EntityType enemyType) {

    srand(time(NULL));
    int min_pos = player.position.x + 100;
    int max_pos = LEVEL_WIDTH - 100;

    enemy.position.x = (rand() % (max_pos - min_pos + 1)) + min_pos;
    enemy.position.y = FLOOR_ZERO_Y + 10; // (rand() % (FLOOR_ZERO_Y - min_pos + 1)) + min_pos;
    enemy.position.z = 0;

    enemy.type = ENTITY_DUMMY;
    switch (enemyType) {
        case ENTITY_ENEMY_CHARGER: {
            enemy.health.maxHealth = CHARGER_MAX_HP;
            enemy.health.health = CHARGER_MAX_HP;

            enemy.measurements.w = CHARGER_WIDTH;
            enemy.measurements.h = CHARGER_HEIGHT;
        };
        case ENTITY_ENEMY_WALKER: {
            enemy.health.maxHealth = WALKER_MAX_HP;
            enemy.health.health = WALKER_MAX_HP;

            enemy.measurements.w = WALKER_WIDTH;
            enemy.measurements.h = WALKER_HEIGHT;
        };
    };

    enemy.currentState = ENTITY_DUMMY;
    enemy.scale = 1.0f;
    enemy.tex = enemy_tex;
    enemy.isCurrentlyAttacking = 0;
};



void enemyAttack(Entity enemy, Entity player, double delta) {
    float distanceToPlayer = sqrt(pow(
        (enemy.position.x - player.position.x), 
        2
    ) + pow(
        (enemy.position.y - player.position.y),
        2
    )); // oblicza odleglosc do gracza uzywajac tw. pitagorasa

    if (enemy.type == ENTITY_ENEMY_CHARGER && distanceToPlayer < CHARGER_ATTACK_RANGE) {
        // jakas tam animacja
        player.health.health -= enemy.attackDamage.damage;
        enemy.health.health = 0;
    };
    if (enemy.type == ENTITY_ENEMY_WALKER && distanceToPlayer < WALKER_ATTACK_RANGE) {
        // jakas tam animacja
        player.health.health -= enemy.attackDamage.damage;
        enemy.health.health = 0;
    };
};

void enemyUpdatePosition(Entity enemy) {
    switch (enemy.type) {
        case ENTITY_ENEMY_CHARGER: {
                
        };
        case ENTITY_ENEMY_WALKER: {
            
        };
    }
}



void enemiesUpdate(EnemiesData enemiesData) {
    for (int i = 0; i < enemiesData.enemies_count; i++) {
        Entity enemy = enemiesData.enemies[i];

    };
}