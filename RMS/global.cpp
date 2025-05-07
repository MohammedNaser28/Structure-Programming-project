#include "global.h"

int num_of_users = 0;
QSharedPointer<User> users[1000];

QSharedPointer<User> loged_in_user = nullptr;

int num_of_recipes = 0;
QSharedPointer<Recipe> recipes[1000];
// hashtable to recipes IDs to help in deleteing recipes and saving data (-1 means doesn't exist)
short recipes_id_to_index[10000]; // intialized by (-1) in main



/***/
QString scrollBarStyle = R"(
    QScrollBar {
        background: #f0f0f0;
        margin: 0px 0px 0px 0px;
    }

    /* Vertical Scrollbar */
    QScrollBar::vertical {
        border: none;
        background: transparent;
        width: 10px;
        margin: 0px 0px 0px 0px;
    }

    QScrollBar::handle:vertical {
        background: #c0c0c0;
        min-height: 20px;
        border-radius: 5px;
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        background: none;
    }

    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
        background: none;
    }

    /* Horizontal Scrollbar */
    QScrollBar::horizontal {
        border: none;
        background: transparent;
        height: 10px;
        margin: 0px 0px 0px 0px;
    }

    QScrollBar::handle:horizontal {
        background: #c0c0c0;
        min-width: 20px;
        border-radius: 5px;
    }

    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
        background: none;
    }

    QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
        background: none;
    }
)";

QString messageBoxStyle = "QMessageBox {"
"    background-color: #fefefe;"
"    border: 2px solid #ccc;"
"    border-radius: 10px;"
"}"
"QLabel {"
"    color: #333;"
"    font-size: 16px;"
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
"}";

QString view_button_details = "QPushButton {"
"border: 2px solid #5A0D25;"  // Dark burgundy border
"border-radius: 15px;"
"padding: 4px;"
"min-width: 100px;"
"font: 500 18px \"Cairo\";"
"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
"    stop: 0 #7A112F, stop: 1 #5A0D25);"  // Burgundy gradient
"color: #FFFFFF;"  // White text for contrast
"}"

"QPushButton:default {"
"border: 3px solid #3D0818;"  // Darker border for default
"padding: 4px;"
"font: 600 18px \"Cairo\";"
"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
"    stop: 0 #6A0F2A, stop: 1 #4A0B20);"  // Deeper burgundy
"color: #FFFFFF;"
"}"

"QPushButton:hover {"
"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
"    stop: 0 #8A1535, stop: 1 #6A0F2A);"  // Brighter burgundy on hover
"border: 2px solid #6A0F2A;"
"}"

"QPushButton:pressed {"
"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
"    stop: 0 #4A0B20, stop: 1 #2A0616);"  // Darkest pressed state
"border: 2px solid #3D0818;"
"}"

"QPushButton:flat {"
"border: none;"
"background: transparent;"
"font: 500 18px \"Cairo\";"
"color: #5A0D25;"  // Burgundy text for flat buttons
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

