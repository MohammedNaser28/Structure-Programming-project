#pragma once

#include "dependencies.h"
#include "structures.h"

extern int num_of_users = 0;
extern QSharedPointer<User> users[1000];

extern QSharedPointer<User> loged_in_user = nullptr;

extern int num_of_recipes = 0;
extern QSharedPointer<Recipe> recipes[1000];

// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
extern short recipes_id_to_index[10000]; // intialized in main
