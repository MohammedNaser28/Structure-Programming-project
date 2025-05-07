<<<<<<< HEAD
#pragma once

#include <QtWidgets/QMainWindow>
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
    //QGridLayout* recipes_grid;
    //QList<QWidget*> recipe_pages;      // IDs of recipe pages

private slots:
    //void on_home_page_btn_clicked();

    void on_log_out_btn_clicked();

    //void on_save_recipe_btn_clicked();

    //void on_display_recipe_btn_clicked();

    //void create_recipe_page(QSharedPointer<Recipe> r_ptr);

signals:
    void switchToDialog();
};
=======
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "global.h"


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
    QGridLayout* all_recipe_grid;
    /****/
    QGridLayout* edition_grid;
    QList<QWidget*> recipe_pages;      // IDs of recipe pages
    QSharedPointer<Recipe> currentDisplayedRecipe = nullptr;
    bool isDescending = false; // Track sort order (false = ascending, true = descending)
    QSharedPointer<Recipe>* searchResults = nullptr;
    int searchResultCount = 0;
    



private slots:

    /*SERVICE GENERAL FunctionS */
    void show_warning_messageBox(QWidget* parent, const QString& message);
    void show_success_messageBox(QWidget* parent, const QString& message);

    void suggest_recipes(int numOfsuggests = 0);
    /*****CUSTOME SERVICE FUNCTIONS****/

    QString* get_ingredients_edit(int& out_count); // GET FOR USER EDITS

    QString* get_steps_edit(int& out_count);// GET FOR USER EDITS

    QString* get_ingredients(int& out_count); // FOR DISPLAY

    QString* get_steps(int& out_count);

    int get_ingredient_count_edit();

    int get_step_count_edit();
    
    int get_ingredient_count();

    int get_step_count();

    void choose_image();

    /********DISPLAY FUNCTINOS******/

    void display_recipe(bool arrang = true);
    void display_favorite(bool prder = true);
    void display_edition_page_user(bool order = true);
    
    
                /********/
    
    void display_search_all(QSharedPointer<Recipe>* recipes, int count);
    void display_search_private(QSharedPointer<Recipe>* recipes, int count, QGridLayout* layout_grid, QWidget* stacked_page,bool flag);

    /***SORT****/
    void sort(QSharedPointer<Recipe> recipes[], int size);
    void on_sort_combobox_all_clicked();
    void on_sort_combobox_edition();


    /***********BUTTONS******/
        /****UPPER BAR*****/
        /**GENREAL**/
        void on_home_btn_clicked();
        void on_logout_btn_clicked();
        void on_order_btn_all_clicked(); // ALL 
        /**ADMIN**/
        void on_add_recipe_admin_btn_clicked();
        /**USER**/
        void on_go_favorite_btn_clicked();
        void on_go_edition_page_btn_clicked();
        void on_edit_user_btn_clicked();
        void on_add_favorite_btn_clicked();
        void on_search_btn_favorite_clicked();
        void on_search_btn_edition_clicked();
        // ALL
        void on_search_btn_clicked();
        /**********DELETE BUTTONS********/
        void delete_favorite_btn(int id_favorite); 
        void on_delete_recipe_btn_clicked(); // ADMIN ONLY
        void delete_edition_btn(int id_edition);






    //void change_order_btn_state(bool fn);
   
    /****ASSSIGNES****/
    void assign_admin_page();
    void assign_recipe_page(QSharedPointer<Recipe> r_ptr);
    void assign_edition_user_page();

/********ADD Dynamic fields*******/
    void add_ingredient_row();
    void add_method_row();
    void remove_row(); // REMOVE FROM BOTH
    void add_ingred_to_user_page_edit();
    void add_method_to_user_page_edit();
    void add_ingredient_row_edition();
    void add_method_row_edition();




    

    /*******************************/
    /*******SAVE BUTTONS********/
        void on_save_edit_recipe_btn_2_clicked();// FOR USER
        void on_submit_recipe_btn_clicked(); // THIS FOR ADMINS 





signals:
    void switchToDialog();
};
>>>>>>> gui
