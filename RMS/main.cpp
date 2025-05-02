#include "mainwindow.h"
#include "dialog.h"
#include "global.h"
#include "data.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

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