#include "recipemanagment.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecipeManagment w;
    w.show();
    return a.exec();
}
