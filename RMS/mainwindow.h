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
    QList<QWidget*> recipe_pages;      // IDs of recipe pages

public slots:
    void on_home_page_btn_clicked();
    void on_add_recipe_page_btn_clicked();
    void on_display_recipe_btn_clicked();

private slots:
    void setup_mainwindow_page();
    void on_log_out_btn_clicked();
    void on_save_recipe_btn_clicked();
    void create_recipe_page(QSharedPointer<Recipe> r_ptr);

    void remove_row();

signals:
    void switchToDialog();
};
