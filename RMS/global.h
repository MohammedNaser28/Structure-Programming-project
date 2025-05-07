<<<<<<< HEAD
#pragma once

#include "dependencies.h"
#include "structures.h"

static int num_of_users = 0;
static QSharedPointer<User> users[1000];

static QSharedPointer<User> loged_in_user = nullptr;

static int num_of_recipes = 0;
static QSharedPointer<Recipe> recipes[1000];

// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
static short recipes_id_to_index[10000]; // intialized in main
=======
#pragma once
#include "dependencies.h"
#include "structures.h"

extern int num_of_users;
extern QSharedPointer<User> users[1000];

extern QSharedPointer<User> loged_in_user;

extern int num_of_recipes;
extern QSharedPointer<Recipe> recipes[1000];
extern QString scrollBarStyle;
extern QString messageBoxStyle;
extern QString deleteButtonStyle;
extern QString deleteButtonStyleDynamic;
extern QString view_button_details;
extern QString titleStyle;
// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
extern short recipes_id_to_index[10000]; // intialized by (-1) in main
>>>>>>> gui
