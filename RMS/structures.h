#pragma once

#include "dependencies.h"

static int next_id = 0; // Added missing semicolon

struct Recipe
{
   Recipe() {}
   ~Recipe() {}

   int id = 0;
   int category = 0;
   QString title;
   QString description;
   int cock_time = 0;
   int level = 0;
   int rates_sum = 0, rates_num = 0;

   int ing_num = 0;
   QString ingredients[100];

   int steps_num = 0;
   QString steps[100];

   QString imagePath;

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
   QString username;
   QString password;

   int favorite_recipes_num = 0;
   int favorite_recipes[100] {};  // IDs of favorite recipes

   int my_recipes_num = 0;
   QSharedPointer<Recipe> my_recipes[100];
};
