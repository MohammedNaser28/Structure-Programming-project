#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_recipemanagment.h"
#include "dependencies.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RecipeManagmentClass; };
QT_END_NAMESPACE

class RecipeManagment : public QMainWindow
{
    Q_OBJECT

public:
    RecipeManagment(QWidget *parent = nullptr);
    ~RecipeManagment();

private:
    Ui::RecipeManagmentClass *ui;
};
