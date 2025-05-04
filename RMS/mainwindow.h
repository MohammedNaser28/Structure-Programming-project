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
    QGridLayout* recipes_grid;

public slots:
    void on_home_page_btn_clicked();
    void on_add_recipe_page_btn_clicked();
    void display_all_recipes();
    void add_ingredient_row(QString ing = "");
    void add_step_row(QString step = "");
    void remove_row();

    void setup_mainwindow_page();
    void on_log_out_btn_clicked();
    void on_save_recipe_btn_clicked();
    void display_recipe(QSharedPointer<Recipe> r_ptr);

    void delete_recipe(QSharedPointer<Recipe> r_ptr);


signals:
    void switchToDialog();
};
