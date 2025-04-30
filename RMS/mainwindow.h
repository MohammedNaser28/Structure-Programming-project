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

private:
    Ui::MainWindowClass *ui;
    QGridLayout* recipes_grid;
    QList<QWidget*> recipe_pages;      // IDs of recipe pages

private slots:

    void on_home_btn_clicked();

    void on_logout_btn_clicked();
    void on_add_recipe_admin_clicked();
    //void on_save_recipe_btn_clicked();

    //void on_display_recipe_btn_clicked();

    //void create_recipe_page(QSharedPointer<Recipe> r_ptr);

signals:
    void switchToDialog();
};
