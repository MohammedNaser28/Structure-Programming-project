#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "global.h"
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startup();
    void reload_recipe_first_time();

private:
    Ui::MainWindowClass *ui;
    QGridLayout* recipes_grid;
    QList<QWidget*> recipe_pages;      // IDs of recipe pages
    QSharedPointer<Recipe> currentDisplayedRecipe = nullptr;

private slots:

    /*GENERAL*/




    /**************/
    void on_home_btn_clicked();

    void on_logout_btn_clicked();
    void on_add_recipe_admin_btn_clicked();

    //void on_save_recipe_btn_clicked();

    //void on_display_recipe_btn_clicked();

    //void create_recipe_page(QSharedPointer<Recipe> r_ptr);

    void on_search_btn_clicked();
    void on_sort_combobox_clicked();
    void add_ingredient_row();
    void add_method_row();
    void remove_row();
    void choose_image();
    int get_ingredient_count();
    int get_step_count();
    void display_recipe();
    void on_submit_recipe_btn_clicked();
    void assign_recipe_page(QSharedPointer<Recipe> r_ptr);

    void assign_admin_page();
    /**************************************/
    void on_add_favorite_btn_clicked();

    void on_go_favorite_btn_clicked();

    void display_favorite();

    /*******************************/
    void on_edit_user_btn_clicked();
    void assign_edition_user_page();
    void add_ingred_to_user_page_edit();
    void add_method_to_user_page_edit();
    void on_save_edit_recipe_btn_2_clicked();
    void add_ingredient_row_edition();
    void add_method_row_edition();
    void display_edition_page_user();
    void on_go_edition_page_btn_clicked();
    int get_ingredient_count_edit();
    int get_step_count_edit();

    QString* get_ingredients_edit(int& out_count);

    QString* get_steps_edit(int& out_count);

    QString* get_ingredients(int& out_count);

    QString* get_steps(int& out_count);

signals:
    void switchToDialog();
};
