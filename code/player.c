#include "../headers/structures.h"
#include "../headers/consts.h"
#include "../headers/combos.h"

void UpdatePlayerAnimation(Entity *player, double deltaTime) {
    player->animationTimer += (float)deltaTime;

    float frameTime = 0.1f; // 100 ms per klatka

    if (player->animationTimer >= frameTime) {
        player->animationTimer = 0;
        player->frame++;

        int maxFrames = 4; // klatki w rzedzie, ile sprite ma klatek

        if (player->frame >= maxFrames) {
            player->frame = 0;
        }
    }
};

void playerInitialize(Entity* player, float x, float y, SDL_Surface* tex) {
    player->position.x = x; // pozycja gracza (startowa)
    player->position.y = y;

    player->position.z = 0; // gracz jest na ziemi
    player->vz = 0.0; // brak predkosci pionowej na starcie!

    //set width i height spirita
    player->w = 64;
    player->h = 64;

    player->direction.x = 0; /// wektor ruchu
    player->direction.y = 0;

    player->speed = 200.0f; // pixele na sekunde
    player->maxHealth = 100;
    player->health = 100;
    player->type = ENTITY_PLAYER;

    player->onGround = 0;
    player->tex = tex;

    player->scale = 1.0f;
    player->timer = 0;
    player->frame = 0;
    player->animationTimer = 0;

    // combosy
    InitInputBuffer(&player->inputBuffer);
    player->activeCombo = COMBO_NONE;
    player->devMode = 0;
};

void playerUpdate(Entity* player, double delta) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    static Uint8 prevStateBuffer[SDL_NUM_SCANCODES] = {0};

    // obsluga timera akcji
    if (player->timer > 0) {
        player->timer -= (float)delta;
        
        // animacja atakow i combo
        if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT ||
            player->currentState == ENTITY_COMBO_TRIPLE_HEAVY ||
            player->currentState == ENTITY_COMBO_MIXED) {
            float duration = 0.9f;
            if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) duration = 2.1f;
            if (player->currentState == ENTITY_COMBO_MIXED) duration = 1.3f;
            
            float progress = 1.0f - (player->timer / duration);
            int totalFrames = 8;
            player->frame = (int)(progress * totalFrames);
            if (player->frame >= totalFrames) player->frame = totalFrames - 1;
        }
        else if (player->currentState == ENTITY_ATTACK_LIGHT || 
                 player->currentState == ENTITY_ATTACK_HEAVY) {
            float duration = (player->currentState == ENTITY_ATTACK_LIGHT) ? 0.3f : 0.7f;
            float progress = 1.0f - (player->timer / duration);
            int totalFrames = 6;
            player->frame = (int)(progress * totalFrames);
            if (player->frame >= totalFrames) player->frame = totalFrames - 1;
        }
        else if (player->currentState == ENTITY_DASHING) {
            player->frame = (player->frame + 1) % 4;
        }

        if (player->timer <= 0) {
            player->timer = 0;
            player->currentState = ENITY_IDLE;
            player->frame = 0;
            player->speed = 200.0f;
            player->activeCombo = COMBO_NONE;
        }
    }

    int isAttacking = (player->currentState == ENTITY_ATTACK_LIGHT || 
                       player->currentState == ENTITY_ATTACK_HEAVY ||
                       player->currentState == ENTITY_COMBO_TRIPLE_LIGHT ||
                       player->currentState == ENTITY_COMBO_TRIPLE_HEAVY ||
                       player->currentState == ENTITY_COMBO_MIXED ||
                       player->currentState == ENTITY_DASHING);

    // wykrywanie nowych wcisnieć (edge detection)
    int qPressed = state[SDL_SCANCODE_Q] && !prevStateBuffer[SDL_SCANCODE_Q];
    int ePressed = state[SDL_SCANCODE_E] && !prevStateBuffer[SDL_SCANCODE_E];
    int leftPressed = (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) && 
                      !(prevStateBuffer[SDL_SCANCODE_LEFT] || prevStateBuffer[SDL_SCANCODE_A]);
    int rightPressed = (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) && 
                       !(prevStateBuffer[SDL_SCANCODE_RIGHT] || prevStateBuffer[SDL_SCANCODE_D]);

    // dodawanie inputow do bufora
    float currentTime = (float)SDL_GetTicks() / 1000.0f;
    if (qPressed && player->position.z == 0) {
        AddInput(&player->inputBuffer, INPUT_ATTACK_LIGHT, currentTime);
    }
    if (ePressed && player->position.z == 0) {
        AddInput(&player->inputBuffer, INPUT_ATTACK_HEAVY, currentTime);
    }
    if (leftPressed) {
        AddInput(&player->inputBuffer, INPUT_MOVE_LEFT, currentTime);
    }
    if (rightPressed) {
        AddInput(&player->inputBuffer, INPUT_MOVE_RIGHT, currentTime);
    }

    // sprawdzanie combo
    ComboType detectedCombo = CheckCombo(&player->inputBuffer, currentTime);
    if (detectedCombo != COMBO_NONE && !isAttacking) {
        PerformCombo(player, detectedCombo, currentTime);
        isAttacking = 1;
    }

    // reset kierunku ruchu
    player->direction.x = 0;
    player->direction.y = 0;
 
    // obsluga ruchu (tylko gdy nie atakujemy)
    if (!isAttacking) {
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
            player->direction.x = -1;
            player->facingLeft = 1;
            player->currentState = ENTITY_WALKING;
        }
        else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
            player->direction.x = 1;
            player->facingLeft = 0;
            player->currentState = ENTITY_WALKING;
        }
        else {
            if (player->position.z == 0) player->currentState = ENITY_IDLE;
        }

        if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
            player->direction.y = -1;
        }
        if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
            player->direction.y = 1;
        }

        UpdatePlayerAnimation(player, delta);
    }

    // skoki
    if (state[SDL_SCANCODE_SPACE] && player->position.z == 0) {
        player->currentState = ENTITY_JUMPING;
        player->vz = 300.0f;
    }
    
    float gravity = 500;
    player->position.z += player->vz * delta;
    if (player->position.z > 0 || player->vz > 0) {
        player->vz -= gravity * delta;
    }
    
    if (player->position.z < 0) {
        player->position.z = 0;
        player->vz = 0;
        player->currentState = ENITY_IDLE;
    }

    // ataki podstawowe (tylko gdy nie ma combo)
    if (!isAttacking && player->position.z == 0) {
        if (state[SDL_SCANCODE_Q]) {
            player->currentState = ENTITY_ATTACK_LIGHT;
            player->timer = 0.3f;
            player->frame = 0;
        }
        if (state[SDL_SCANCODE_E]) {
            player->currentState = ENTITY_ATTACK_HEAVY;
            player->timer = 0.7f;
            player->frame = 0;
        }
    }

    // ruch gracza
    if (!isAttacking) {
        player->position.x += player->direction.x * player->speed * delta;
        player->position.y += player->direction.y * player->speed * delta;
    } else if (player->currentState == ENTITY_DASHING) {
        // dash ruch
        int dashDir = player->facingLeft ? -1 : 1;
        if (player->activeCombo == COMBO_DASH_BACKWARD) dashDir *= -1;
        player->position.x += dashDir * player->speed * delta;
    }

    // obliczanie skali i wysokosci
    player->scale = 0.5f + (player->position.y / (float)SCREEN_HEIGHT);
    float currentHeight = player->h * player->scale;

    // ograniczenia ruchu
    if (player->position.y + currentHeight < FLOOR_ZERO_Y) {
        player->position.y = FLOOR_ZERO_Y - currentHeight;
    }
    if (player->position.y + currentHeight > SCREEN_HEIGHT) {
        player->position.y = SCREEN_HEIGHT - currentHeight;
    }
    if (player->position.x < 0) {
        player->position.x = 0;
    }
    if (player->position.x > LEVEL_WIDTH - player->w) {
        player->position.x = LEVEL_WIDTH - player->w;
    }

    // zapisz stan klawiszy na nastepna klatke
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        prevStateBuffer[i] = state[i];
    }
};