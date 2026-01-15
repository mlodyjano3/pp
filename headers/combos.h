#ifndef COMBO_H
#define COMBO_H
#include "../headers/structures.h"

void InitInputBuffer(InputBuffer* buffer);

void AddInput(InputBuffer* buffer, InputType type, float currentTime);

ComboType CheckCombo(InputBuffer* buffer, float currentTime);

const char* GetComboName(ComboType combo);

void PerformCombo(Entity* player, ComboType combo, float currentTime);
#endif