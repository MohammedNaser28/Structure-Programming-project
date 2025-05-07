#include "favorite.h"


/*


void addFavorite() {
    if (count >= MAX_RECIPES) {
        cout << "Favorites are full, new recipes cannot be added.\n";
        return;
    }

    cout << "Type the name of the recipe: ";
    cin.ignore(); // ãÓÍ ÇáÈÝÑ ÞÈá getline
    getline(cin, favorites[count]);
    count++;
    cout << "The recipe has been added to favorites..\n";
    // ??? ?
}

// ÏÇáÉ áÚÑÖ ÇáãÝÖáÇÊ
void displayFavorites() {
    if (count == 0) {
        cout << "Favorites are currently emptyð.\n";
        return;
    }

    cout << "Favorite recipes:\n";
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << favorites[i] << endl;
    }
}

// ÏÇáÉ áÍÐÝ æÕÝÉ ãä ÇáãÝÖáÉ
void removeFavorite() {
    if (count == 0) {
        cout << "There are no recipes to delete..\n";
        return;
    }

    displayFavorites();

    int index;
    cout << "Enter the prescription number you want to delete.: ";
    cin >> index;
    // ??? ??????? ??? ???????
    if (index > 0 && index <= count) {
        for (int i = index - 1; i < count - 1; i++) {
            favorites[i] = favorites[i + 1];
        }
        count--;
        cout << "Êã ÍÐÝ ÇáæÕÝÉ ÈäÌÇÍ.\n";
    }
    else {
        cout << "Invalid number, try again.\n";
    }
}

*/



// Add recipe to favorites function
bool addToFavorites(int recipeId) {


    // Check if we've reached the maximum number of favorites
    if (loged_in_user->favorite_recipes_num >= 100) {
        QMessageBox::warning(nullptr, "Error", "You've reached the maximum number of favorite recipes!");
        return false;
    }

    // Check if recipe is already in favorites
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == recipeId) {
            QMessageBox::information(nullptr, "Info", "Recipe is already in your favorites!");
            return false;
        }
    }


    // Add recipe to favorites
    loged_in_user->favorite_recipes[loged_in_user->favorite_recipes_num] = recipeId;
    loged_in_user->favorite_recipes_num++;

    QMessageBox::information(nullptr, "Success", "Recipe added to favorites!");
    return true;
}

// Remove recipe from favorites function
bool removeFromFavorites(int recipeId) {
    // Make sure user is logged in

    // Find recipe in favorites
    int index = -1;
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == recipeId) {
            index = i;
            break;
        }
    }


    // Remove recipe from favorites by shifting remaining items
    for (int i = index; i < loged_in_user->favorite_recipes_num - 1; i++) {
        loged_in_user->favorite_recipes[i] = loged_in_user->favorite_recipes[i + 1];
    }
    loged_in_user->favorite_recipes_num--;

    QMessageBox::information(nullptr, "Success", "Recipe removed from favorites!");
    return true;
}

// Toggle favorite status (add if not in favorites, remove if already in favorites)
void toggleFavorite(int recipeId) {


    // Check if recipe is already in favorites
    bool isInFavorites = false;
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == recipeId) {
            isInFavorites = true;
            break;
        }
    }

    // Toggle favorite status
    if (isInFavorites) {
        removeFromFavorites(recipeId);
    }
    else {
        addToFavorites(recipeId);
    }
}

// Check if a recipe is in the user's favorites
bool isInFavorites(int recipeId) {

    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == recipeId) {
            return true;
        }
    }

    return false;
}