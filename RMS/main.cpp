#include "mainwindow.h"
#include "dialog.h"
#include "global.h"
#include "data.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setLayoutDirection(Qt::RightToLeft);
    a.setWindowIcon(QIcon(":/MainWindow/systemicon.webp"));
    a.setStyleSheet(
        "QMessageBox {"
        "    background-color: #fefefe;"
        "    border: 2px solid #ccc;"
        "    border-radius: 10px;"
        "}"
        "QLabel {"
        "    color: #333;"
        "    font-size: 16px;"
        "}"
        "QPushButton {"
        "    background-color: #03A9F4;"
        "    color: white;"
        "    padding: 6px 12px;"
        "    border-radius: 8px;"
        "    min-width: 75px;"
        "    margin: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0288D1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0277BD;"
        "}"
        "QComboBox {"
        "    border: 1px solid gray;"
        "    border-radius: 4px;"
        "    padding: 2px;"
        "    background-color: #f0f0f0;"
        "    font-size: 14px;"
        "}"
        "QComboBox::drop-down {"
        "    border: 0px;"
        "    background-color: transparent;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/MainWindow/down.png);"
        "    width: 12px;"
        "    height: 12px;"
        "}"

    );
    // intialize recipes_id_to_index
    for (int i = 0; i < 10000; i++)
        recipes_id_to_index[i] = -1;

    load_users();
    load_recipes();

    Register dialog;

    MainWindow mainWindow;
    // Connect signals to switch between windows
    QObject::connect(&dialog, &Register::switchToMainWindow, &dialog, [&dialog, &mainWindow]() {
        dialog.hide();
        mainWindow.show();
        mainWindow.startup();
        
        });

    QObject::connect(&mainWindow, &MainWindow::switchToDialog, &mainWindow, [&dialog, &mainWindow]() {
        mainWindow.hide();
        dialog.show();
        });

    // Connect signal to save data whan app is about to Quit
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        save_users();
        save_recipes();
        });

    dialog.show();

    return a.exec();
}