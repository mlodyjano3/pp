#ifndef COMBO_H
#define COMBO_H
#include "../headers/structures.h"

void InitInputBuffer(InputBuffer* buffer);
void AddInput(InputBuffer* buffer, InputType type, float currentTime);
ComboType checkCombo(InputBuffer* buffer, float currentTime);
const char* GetComboName(ComboType combo);
void PerformCombo(Entity* player, ComboType combo, float currentTime);

ComboType last_3_check(InputType last_3[], InputBuffer* buffer);
ComboType last_2_check(InputType last2[], InputBuffer* buffer);
#endif