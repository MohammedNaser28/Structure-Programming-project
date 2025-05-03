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
    QGridLayout* favorite_grid;
    /****/
  /*  QWidget* scrollWidgetFavorite = new QWidget;
    QGridLayout* favorite_grid = new QGridLayout(scrollWidgetFavorite);*/
    /**********/
    //QWidget* scrollWidgetEdition = new QWidget;
    //QGridLayout* edition_grid = new QGridLayout(scrollWidgetEdition);
    QGridLayout* edition_grid;
    QList<QWidget*> recipe_pages;      // IDs of recipe pages
    QSharedPointer<Recipe> currentDisplayedRecipe = nullptr;
    bool isDescending = false; // Track sort order (false = ascending, true = descending)
    QSharedPointer<Recipe>* searchResults = nullptr;
    int searchResultCount = 0;
    QString view_button_details = "QPushButton {"
        "border: 2px solid #8f8f91;"
        "border-radius: 15px;"
        "padding: 4px;"
        "width: 100px;"
        "font: 500 18px \"Cairo\";"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "    stop: 0 #e0e0e0, stop: 1 #a0a0a0);"
        "color: black;" // <<<< Text color set to black
        "}"

        "QPushButton:default {"
        "border: 3px solid #5A5A5A;"
        "padding: 4px;"
        "font: 600 18px \"Cairo\";"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "    stop: 0 #d7d7d7, stop: 1 #b0b0b0);"
        "color: black;"
        "}"

        "QPushButton:hover {"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "    stop: 0 #f0f0f0, stop: 1 #c0c0c0);"
        "}"

        "QPushButton:pressed {"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "    stop: 0 #dadbde, stop: 1 #f6f7fa);"
        "}"

        "QPushButton:flat {"
        "border: none;"
        "background: transparent;"
        "font: 500 18px \"Cairo\";"
        "color: black;"
        "}";


    QString deleteButtonStyleDynamic = "QPushButton {"
        "border-radius: 15px;"
        "background-color: rgb(255, 50, 50);"
        "border: 2px solid #8f8f91;"
        "padding:4px;"
        "font:500 18px \"Cairo\";"
        "color: white;"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(200, 0, 0);"
        "}"
        "QPushButton:pressed {"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "stop: 0 #dadbde, stop: 1 #f6f7fa);"
        "}";

    QString deleteButtonStyle = "QPushButton {"
        "border-radius: 15px;"
        "background-color: rgb(255, 50, 50);"
        "border: 2px solid #8f8f91;"
        "padding:4px;"
        "width:100px;"
        "font:500 18px \"Cairo\";"
        "color: white;"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(200, 0, 0);"
        "curosr:pointer;"
        "}"
        "QPushButton:pressed {"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "stop: 0 #dadbde, stop: 1 #f6f7fa);"
        "}";
    QString titleStyle =
        "font: bold 18px 'Reem Kufi';"
        "color: #2c2c2c;"
        "padding: 3px;"
        "margin-bottom: 4px;"
        "background-color: transparent;"
        "border: none;";
      
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
    void sort(QSharedPointer<Recipe> recipes[], int size);
    void on_sort_combobox_clicked();  
    void on_arrangment_btn_clicked();
    //QSharedPointer<Recipe> * searchRecipe(QString& input,bool ingred=false);
    void display_search_home(QSharedPointer<Recipe>* recipes, int count);
    /********/
    void add_ingredient_row();
    void add_method_row();
    void remove_row();
    void choose_image();
    int get_ingredient_count();
    int get_step_count();
    void display_recipe(bool arrang=true);
    void on_submit_recipe_btn_clicked();
    void assign_recipe_page(QSharedPointer<Recipe> r_ptr);
    void on_delete_recipe_btn_clicked();
    void assign_admin_page();
    /**************************************/
    void on_add_favorite_btn_clicked();

    void on_go_favorite_btn_clicked();

    void display_favorite();

    void delete_favorite_btn(int id_favorite);
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
    void delete_edition_btn(int id_edition);
    QString* get_ingredients_edit(int& out_count);

    QString* get_steps_edit(int& out_count);

    QString* get_ingredients(int& out_count);

    QString* get_steps(int& out_count);

signals:
    void switchToDialog();
};
