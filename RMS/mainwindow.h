#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();


private:
	Ui::MainWindowClass *ui;

    int temp_recipes_count = 0;
    QSharedPointer<Recipe> temp_recipes[1000];
    QSharedPointer<Recipe> random_recipes[3];


public slots:
    void setup_mainwindow();

    // Upper bar
    void on_home_page_btn_clicked();
    void on_add_recipe_btn_clicked();
    void on_log_out_btn_clicked();
    void on_my_favorites_page_btn_clicked();
    void on_my_recipes_page_btn_clicked();



    // recipe_page
    void clear_recipe_page();  // Always clear recipe page and disconnect signals before you deal with a recipe
    void fill_page_from_recipe(QSharedPointer<Recipe> r_ptr);
    void fill_recipe_from_page(QSharedPointer<Recipe> r_ptr);

    void connect_favorite_btn(QSharedPointer<Recipe> r_ptr);
    void connect_my_recipe_btn(QSharedPointer<Recipe> r_ptr);

    void fill_grid(QScrollArea* scroll_area, QSharedPointer<Recipe>* recipes_arr, int size); // fill any recipes grid
    
    void add_ingredient_row(QString ing = "");
    void add_step_row(QString step = "");
    void remove_row();


    // Functions
    void display_recipe(QSharedPointer<Recipe> r_ptr);
    void add_recipe();
    void delete_recipe(QSharedPointer<Recipe> r_ptr);



signals:
    void switchToDialog();
};
