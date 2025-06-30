#pragma once

#include "global.h"

void save_recipes(QSharedPointer<Recipe> recipes[], int& recipes_count, QTextStream* stream = nullptr);
void load_recipes(QSharedPointer<Recipe> recipes[], int& recipes_count, QTextStream* stream = nullptr);
void load_users();
void save_users();

