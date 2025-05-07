#pragma once
#include "global.h"
#include "structures.h"

bool addToFavorites(int recipeId);

bool removeFromFavorites(int recipeId);

void toggleFavorite(int recipeId);

bool isInFavorites(int recipeId);
