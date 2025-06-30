#pragma once

#include "dependencies.h"

static int next_id = 1;

struct Recipe
{
   Recipe() {}
   ~Recipe() {}

   int id = 0;
   int category = 0;
   QString title;
   QString description;
   double cock_time = 0;
   int level = 0;
   QString imagePath = "";

   int ing_count = 0;
   QString ingredients[100] {};

   int steps_count = 0;
   QString steps[100] {};

   void generate_id()
   {
       id = next_id;
       next_id++;
   }

};

struct User
{
   User() {}
   ~User() {}

   bool isAdmin = false;
   QString name;
   QString username;
   QString password;

   int favorites_count = 0;
   QSharedPointer<Recipe> favorites[100] {};

   int my_recipes_count = 0;
   QSharedPointer<Recipe> my_recipes[100] {};
};
