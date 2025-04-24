#pragma once

#include "dependencies.h"
#include "structures.h"

static int num_of_users = 0;
static QSharedPointer<User> users[1000];

static QSharedPointer<User> loged_in_user = nullptr;

static int num_of_recipes = 0;
static QSharedPointer<Recipe> recipes[1000];

// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
static short recipes_id_to_index[10000]; // intialized by (-1) in main
