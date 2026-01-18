#ifndef CONSTS_H
#define CONSTS_H

#include <SDL.h>
#include <SDL_main.h>


// proporcje widoku itd
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480 // 480
#define LEVEL_WIDTH 2000
#define BASE_PLAYER_SIZE 0.7f

#define BACKGROUND_HEIGHT (SCREEN_HEIGHT / 5) * 2// 2/5 ekranu
#define FLOOR_HEIGHT (SCREEN_HEIGHT - BACKGROUND_HEIGHT) // 3/5ekranu

#define BACKGROUND_WIDTH SCREEN_WIDTH
#define FLOOR_WIDTH SCREEN_WIDTH

#define FLOOR_ZERO_Y  (BACKGROUND_HEIGHT)
#define FLOOR_ZERO_X 0

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

// czy range sa pokazane tylko w trybie deva czy w trybie zwyklym tez
#define RANGES_DISPLAYED_DEV_ONLY 0

// combo define'y
#define MAX_COMBOS_TRACKED 100
#define COMBO_TIMEOUT_MS 200.0f

#define FIRST_COMBO_a {SDL_SCANCODE_D, SDL_SCANCODE_D}
#define FIRST_COMBO_b {SDL_SCANCODE_RIGHT, SDL_SCANCODE_RIGHT}
#define SECOND_COMBO_a {SDL_SCANCODE_Q, SDL_SCANCODE_Q, SDL_SCANCODE_Q}
#define THIRD_COMBO_a {SDL_SCANCODE_E, SDL_SCANCODE_Q, SDL_SCANCODE_E}
#define MAX_COMBO_STEPS 3
//

// stale atakowe
#define LIGHT_ATTACK_WIDTH 60
#define LIGHT_ATTACK_HEIGHT 40
#define HEAVY_ATTACK_WIDTH 80
#define HEAVY_ATTACK_HEIGHT 50
//


// stale dla wrogow
#define MAX_ENEMIES 40

#define WALKER_MAX_HP 100
#define WALKER_WIDTH 50
#define WALKER_HEIGHT 50
#define WALKER_DAMAGE 20
#define WALKER_ATTACK_RANGE 60
#define WALKER_ATTACK_COUNTDOWN 1.5
#define WALKER_SPEED 50
#define WALKER_STUN 1
#define WALKER_COOLDOWN 5

#define CHARGER_MAX_HP 200
#define CHARGER_WIDTH 50
#define CHARGER_HEIGHT 50
#define CHARGER_DAMAGE 35
#define CHARGER_ATTACK_RANGE 30
#define CHARGER_ATTACK_COUNTDOWN 1
#define CHARGER_SPEED 40
#define CHARGER_STUN 1
#define CHARGER_COOLDOWN 7
#define CHARGER_DETECT_RANGE 300
#define CHARGER_CHARGE_SPEED 200
#define CHARGER_CHARGE_DURATION 1.5f
//



// punkty
#define POINTS_LIGHT_ATTACK 10
#define POINTS_HEAVY_ATTACK 25
#define POINTS_COMBO_TRIPLE_LIGHT 60
#define POINTS_COMBO_TRIPLE_HEAVY 100
#define POINTS_COMBO_MIXED 80
#define COMBO_MULTIPLIER_TIME 3.0f // 3 sekundy na kontynuacje combo

#define PLAYER_HIT_DURATION 0.3f // czas animacji trafienia
#define PLAYER_INVINCIBILITY_TIME 1 // po trafieniu jest nietykalny na PLAYER_inv_time



#define IS_PLAYER_ATTACKING_SHORTCUT (player->currentState == ENTITY_ATTACK_LIGHT || player->currentState == ENTITY_ATTACK_HEAVY || player->currentState == ENTITY_COMBO_TRIPLE_LIGHT || player->currentState == ENTITY_COMBO_TRIPLE_HEAVY || player->currentState == ENTITY_COMBO_MIXED)

#endif