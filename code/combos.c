#include "../headers/structures.h"
#include "../headers/consts.h"
#include <stdio.h>

// init buforu
void InitInputBuffer(InputBuffer* buffer) {
    buffer->count = 0;
    buffer->lastInputTime = 0;
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
        buffer->buffer[i].type = INPUT_NONE;
        buffer->buffer[i].timestamp = 0;
    }
}

// add inputu do buforu
void AddInput(InputBuffer* buffer, InputType type, float currentTime) {
    // usuwanie starych inputow
    int validCount = 0;
    for (int i = 0; i < buffer->count; i++) {
        if (currentTime - buffer->buffer[i].timestamp < COMBO_TIMEOUT) {
            if (validCount != i) {
                buffer->buffer[validCount] = buffer->buffer[i];
            }
            validCount++;
        }
    };
    buffer->count = validCount;

    if (buffer->count < INPUT_BUFFER_SIZE) {
        buffer->buffer[buffer->count].type = type;
        buffer->buffer[buffer->count].timestamp = currentTime;
        buffer->count++;
        buffer->lastInputTime = currentTime;
    } else {
        for (int i = 0; i < INPUT_BUFFER_SIZE - 1; i++) {
            buffer->buffer[i] = buffer->buffer[i + 1];
        }
        buffer->buffer[INPUT_BUFFER_SIZE - 1].type = type;
        buffer->buffer[INPUT_BUFFER_SIZE - 1].timestamp = currentTime;
        buffer->lastInputTime = currentTime;
    }
};

ComboType CheckCombo(InputBuffer* buffer, float currentTime) {
    if (buffer->count < 2) return COMBO_NONE;

    //czas inputu check
    if (currentTime - buffer->lastInputTime > COMBO_TIMEOUT) {
        return COMBO_NONE;
    }

    // sprawdzanie combo tzry-inputowego
    if (buffer->count >= 3) {
        InputType last3[3] = {
            buffer->buffer[buffer->count - 3].type,
            buffer->buffer[buffer->count - 2].type,
            buffer->buffer[buffer->count - 1].type
        };
        //3x Q combo
        if (last3[0] == INPUT_ATTACK_LIGHT && 
            last3[1] == INPUT_ATTACK_LIGHT && 
            last3[2] == INPUT_ATTACK_LIGHT) {
            return COMBO_TRIPLE_LIGHT;
        };
        // 3x E combo
        if (last3[0] == INPUT_ATTACK_HEAVY && 
            last3[1] == INPUT_ATTACK_HEAVY && 
            last3[2] == INPUT_ATTACK_HEAVY) {
            return COMBO_TRIPLE_HEAVY;
        }
        // q-e-q combo
        if (last3[0] == INPUT_ATTACK_LIGHT && 
            last3[1] == INPUT_ATTACK_HEAVY && 
            last3[2] == INPUT_ATTACK_LIGHT) {
            return COMBO_LIGHT_HEAVY_LIGHT;
        }
    }

    // combo dwu-inputowe
    if (buffer->count>= 2){
        InputType last2[2] = {
            buffer->buffer[buffer->count - 2].type,
            buffer->buffer[buffer->count - 1].type
        };

        if (last2[0] == INPUT_MOVE_RIGHT && last2[1] == INPUT_MOVE_RIGHT) {
            return COMBO_DASH_FORWARD;
        };
    };
    return COMBO_NONE;
}

// nazwy danego combo zeby displayowac w developerskim
const char* GetComboName(ComboType combo) {
    switch (combo) {
        case COMBO_TRIPLE_LIGHT: return "Triple easy attack(3xQ)";
        case COMBO_TRIPLE_HEAVY: return "Triple heavy attack (3xE)";
        case COMBO_LIGHT_HEAVY_LIGHT: return "Heaviest combo (QEQ)";
        case COMBO_DASH_FORWARD: return "Dash Forward (>>)";
        case COMBO_DASH_BACKWARD: return "Dash Backward (<<)";
        case COMBO_NONE: return "None";
        default: return "Unknown";
    }
}

// funkcja robiaca combo
void PerformCombo(Entity* player, ComboType combo, float currentTime) {
    switch (combo) {
        case COMBO_TRIPLE_LIGHT:
            player->currentState = ENTITY_COMBO_TRIPLE_LIGHT;
            player->timer = 0.9f;
            player->frame = 0;
            break;      
        case COMBO_TRIPLE_HEAVY:
            player->currentState = ENTITY_COMBO_TRIPLE_HEAVY;
            player->timer = 2.1f;
            player->frame = 0;
            break;
        case COMBO_LIGHT_HEAVY_LIGHT:
            player->currentState = ENTITY_COMBO_MIXED;
            player->timer = 1.3f;
            player->frame = 0;
            break;
        case COMBO_DASH_FORWARD:
            player->currentState = ENTITY_DASHING;
            player->timer = 0.3f;
            player->speed = 600.0f;// zwiekszona predkosc dla dash
            break;
        case COMBO_DASH_BACKWARD:
            player->currentState = ENTITY_DASHING;
            player->timer = 0.3f;
            player->speed = 600.0f;
            player->direction.x = player->facingLeft ? 1 : -1;
            break;
        default:break;
    };
    player->activeCombo = combo;
    InitInputBuffer(&player->inputBuffer);
};