#pragma once
#include "dependencies.h"
struct user {
	int id;
	bool isAdmin= false;
	string username;
	string password;
	//string name;
	int favorites[10];
	recipe myRecipe[10];
};

struct recipe {
	int id;
	string title;
	string decription;
	string ingrediatnes[10];
	string imagePath;
	float timeCoock;
	string steps;
	int rate;

};
