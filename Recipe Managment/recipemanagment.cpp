#include "recipemanagment.h"

RecipeManagment::RecipeManagment(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RecipeManagmentClass())
{
    ui->setupUi(this);
}

RecipeManagment::~RecipeManagment()
{
    delete ui;
}
