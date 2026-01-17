#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL.h>
#include <SDL_main.h>
#include <consts.h>

#define INPUT_BUFFER_SIZE 10
#define COMBO_TIMEOUT 0.5f  // czas w sek na perform combo

///
// hitboxy i wszystko co z nimi zwiazane
///
typedef struct {
    float x, y;
    float w, h;
} Hitbox;

// typy combosow
typedef enum {
    INPUT_NONE = 0,
    INPUT_ATTACK_LIGHT,//klawisz q
    INPUT_ATTACK_HEAVY,// klawisz e
    INPUT_MOVE_LEFT,
    INPUT_MOVE_RIGHT,
    INPUT_MOVE_UP,
    INPUT_MOVE_DOWN
} InputType;


typedef struct {
    InputType type;
    float timestamp;
} InputEntry;

typedef struct {
    InputEntry buffer[INPUT_BUFFER_SIZE];
    int count;
    float lastInputTime;
} InputBuffer;

typedef enum {
    COMBO_NONE = 0,
    COMBO_TRIPLE_LIGHT,//triple q
    COMBO_TRIPLE_HEAVY,// tripe e
    COMBO_LIGHT_HEAVY_LIGHT, //q-e-q
    COMBO_DASH_FORWARD,
    COMBO_DASH_BACKWARD
} ComboType;

typedef struct {
    float x, y, z;
} Position;

typedef struct {
    float x, y;
} Direction;

typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_WALKER,
    ENTITY_ENEMY_CHARGER
} EntityType;

typedef enum {
    ENTITY_WALKING,
    ENITY_IDLE,
    ENTITY_JUMPING,
    ENTITY_ATTACK_LIGHT,
    ENTITY_ATTACK_HEAVY,
    ENTITY_COMBO_TRIPLE_LIGHT,
    ENTITY_COMBO_TRIPLE_HEAVY,
    ENTITY_COMBO_MIXED,
    ENTITY_DASHING,

    ENTITY_DUMMY,
    ENTITY_HIT,
    ENTITY_CHARGING
} EntityState;

typedef struct {
    int health;
    int maxHealth;
} Health;

typedef struct {
    int w, h;
} Measurements;

typedef struct {
    int damage;
    float stunTimer;
    float attackCooldown;
} AttackValues;

typedef struct {
    float chargeTimer;
    int isCharging;
} ChargerData;

typedef struct {
    Position position;
    Direction direction;

    Measurements measurements;
    Health health;
    AttackValues attackDamage;
    float speed; 
    int onGround; // bool
    int frame;
    int facingLeft; // bool wartosci
    float scale;
    
    float jumpHeight;
    float vz;
    float timer;
    EntityState currentState;

    float animationTimer;

    EntityType type;
    SDL_Surface* tex;
    
    // system combo podpunkt B
    InputBuffer inputBuffer;
    ComboType activeCombo;


    // hitboxy
    Hitbox hitboxes;
    Hitbox attacking_hitboxes;
    int isCurrentlyAttacking;
    int isInvicible;
    float invicibilityTimer;

    // punkty
    int wasHitThisAttack;

    ChargerData chargerData;
} Entity;

typedef struct {
    Entity enemies[MAX_ENEMIES];
    int enemies_count;
} EnemiesData;

typedef struct { 
    Position position;
    int w, h;
} Camera;

typedef struct {
    double worldTime;
    double fpsTimer;
    double fps;
    int frames;
    double distance;
    double etiSpeed;
    int quit;

    float score;
    float currentMultiplier;
    float comboTimer;

    int devMode;
} GameState;


#endif