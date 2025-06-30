#include "global.h"

int users_count = 0;
QSharedPointer<User> users[1000];

QSharedPointer<User> user = nullptr;

int recipes_count = 0;
QSharedPointer<Recipe> recipes[1000];

// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
short recipes_id_to_index[10000]; // intialized by (-1) in main
