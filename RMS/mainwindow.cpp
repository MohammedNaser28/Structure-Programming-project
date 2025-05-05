#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowClass),
    recipes_grid(new QGridLayout),
    favorite_grid(new QGridLayout),
    edition_grid(new QGridLayout),
    all_recipe_grid(new QGridLayout)
{



    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(recipes_grid);
    ui->scrollArea->setWidgetResizable(true);


    //// Set scrollWidgetFavorite for scrollArea_3
    ui->scrollAreaWidgetContents_3->setLayout(favorite_grid);
    ui->scrollArea_3->setWidgetResizable(true);
    ui->scrollAreaWidgetContents_4->setLayout(edition_grid);
    ui->scrollArea_4->setWidgetResizable(true);
    ui->scrollAreaWidgetContents_5->setLayout(all_recipe_grid);
    ui->scrollArea_5->setWidgetResizable(true);

    /**************************************************************************/

    // Set stylesheet to change text color
    ui->menuRecipe->setStyleSheet(
        "QMenuBar {"
        "    background-color: #2f2f2f;"
        "}"
        "QMenuBar::item {"
        "    color: white;"          // <<== Change menu bar item text color to white
        "    padding: 4px 10px;"
        "}"
        "QMenu {"
        "    background-color: #f0f0f0;"
        "    color: black;"          // <<== Change menu item text color
        "}"
        "QMenu::item:selected {"
        "    background-color: #a0a0a0;"
        "}"
    );

    ui->menubar->setStyleSheet(
        "QMenuBar {"
        "    background-color: #2f2f2f;"
        "}"
        "QMenuBar::item {"
        "    color: white;"          // <<== Change menu bar item text color to white
        "    padding: 4px 10px;"
        "}"
        "QMenu {"
        "    background-color: #f0f0f0;"
        "    color: black;"          // <<== Change menu item text color
        "}"
        "QMenu::item:selected {"
        "    background-color: #a0a0a0;"
        "}"
    );
    /************************/
    connect(ui->actionDisplay_all_recipes, &QAction::triggered, this, &MainWindow::display_recipe);
    connect(ui->actionDisplay_favorites_recipes, &QAction::triggered, this, &MainWindow::on_go_favorite_btn_clicked);
    connect(ui->actionDisplay_suggest_recipes, &QAction::triggered, this, &MainWindow::on_home_btn_clicked);
     connect(ui->actionDisplay_my_recipes, &QAction::triggered, this, &MainWindow::on_go_edition_page_btn_clicked);

    /****************/
    ui->image_path_line->setPlaceholderText(QStringLiteral("مسار الصورة"));

    /**************************************************************************/
    connect(ui->browse_btn, &QPushButton::clicked, this, &MainWindow::choose_image);
    connect(ui->add_method_btn, &QPushButton::clicked, this, &MainWindow::add_method_row);
    connect(ui->add_ing_btn, &QPushButton::clicked, this, &MainWindow::add_ingredient_row);

    connect(ui->add_method_edit_btn_2, &QPushButton::clicked, this, &MainWindow::add_method_row_edition);
    connect(ui->add_ing_edit_btn_2, &QPushButton::clicked, this, &MainWindow::add_ingredient_row_edition);

    connect(ui->sort_combobox_all, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sort_combobox_all_clicked()));
    connect(ui->search_btn_all, &QPushButton::clicked, this, &MainWindow::on_search_btn_clicked);
    connect(ui->sort_combobox_edition, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sort_combobox_edition()));
    //connect(ui->sort_combobox_favorite, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sort()));


    /**********************/
    ui->desc_field->setLayoutDirection(Qt::RightToLeft);
    ui->desc_edit_field_2->setLayoutDirection(Qt::RightToLeft);

    //edition_grid.fill(nullptr, 1000);
    recipe_pages.fill(nullptr, 1000);
}

void MainWindow::startup()
{
    ui->user_name_label_2->setText( " أهلا وسهلا بك، " + loged_in_user->name);
    if (loged_in_user->isAdmin == true)
    {
        ui->go_favorite_btn->setVisible(false);
        ui->go_edition_page_btn->setVisible(false);
        ui->add_recipe_admin_btn->setVisible(true);
        ui->delete_recipe_btn->setVisible(true);
        //ui->rate_spin_user->setVisible(false);
        //ui->rate_label->setVisible(false);
        ui->add_favorite_btn->setVisible(false);
        ui->actionDisplay_favorites_recipes->setEnabled(false);
        ui->actionDisplay_my_recipes->setEnabled(false);
    }
    else {
        ui->add_recipe_admin_btn->setVisible(false);
        ui->go_favorite_btn->setVisible(true);
        ui->go_edition_page_btn->setVisible(true);
        ui->delete_recipe_btn->setVisible(false);
        //ui->rate_spin_user->setVisible(true);
        //ui->rate_label->setVisible(true);
        ui->add_favorite_btn->setVisible(true);
        ui->actionDisplay_favorites_recipes->setEnabled(true);
        ui->actionDisplay_my_recipes->setEnabled(true);
    }

    //ui->rate_spin_user->setRange(0, 5);
    reload_recipe_first_time();
}

void MainWindow::reload_recipe_first_time()
{
    ui->stackedWidget->setCurrentWidget(ui->home_page);
    suggest_recipes(5);
}


MainWindow::~MainWindow()
{
    delete ui;
    qDeleteAll(recipe_pages);
    recipe_pages.clear();
}

void MainWindow::suggest_recipes(int numOfsuggests )
{

    qInfo() << "INSUGGEST";
    QLayoutItem* item;
    while ((item = recipes_grid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    qInfo() << "MIddleSUGGEST";


    if (num_of_recipes < numOfsuggests)
        numOfsuggests = num_of_recipes;
    if (numOfsuggests <= 0) {
        qInfo() << "No recipes to suggest - numOfsuggests is" << numOfsuggests;
        return; // Exit if there’s nothing to display
    }
    srand(time(0));
    int r = 0, c = 0;
    int validRecipes = 0;
    qInfo() << "MIddleSUGewGEST";

    for (int i = 0; i < numOfsuggests; i++)
    {
        qInfo() << "EDIT";
        int index = rand() % num_of_recipes; //The % operator ensures the random number is within the bounds of the recipes array.

        // show recipe


        QWidget* widget = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[index]->title);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(titleStyle);
        QLabel* image = new QLabel;
        image->setPixmap(QPixmap(recipes[index]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));

        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("اعرض الوصفة");
        button->setStyleSheet(view_button_details);
        connect(button, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipes[index]);
            });


        layout->addWidget(title);
        layout->addWidget(image); // ADDED: Image label
        layout->addWidget(button);

        // Calculate grid position: 4 columns per row
        c = validRecipes % 4;
        r = validRecipes / 4;
        validRecipes++;

        // Add to grid
        recipes_grid->addWidget(widget, r, c);
        qInfo() << "Widget0  " << widget;
    }
    
}


void MainWindow::on_logout_btn_clicked()
{
    currentDisplayedRecipe = nullptr;
    loged_in_user = nullptr;
    emit switchToDialog();
}

void MainWindow::on_order_btn_all_clicked()
{
    isDescending = !isDescending;
    ui->order_btn_all->setIcon(QIcon(isDescending ? ":/MainWindow/sort-descending.png" : ":/MainWindow/sort.png"));
    display_recipe(isDescending);
}

void MainWindow::on_add_recipe_admin_btn_clicked()
{
    qInfo() << "CLicked 3";

    // Step 7: Clear all input fields after successful submission
    ui->title_field->clear();
    ui->desc_field->clear();
    ui->category_combobox_btn->setCurrentIndex(0); // Reset to default (assuming 0 is valid)
    ui->level_combobox_btn->setCurrentIndex(0);    // Reset to default
    ui->time_spin_btn->setValue(0);                // Reset to 0
    ui->image_path_line->clear();

    const QList<QPushButton*>& ing_btns = ui->ing_container->findChildren<QPushButton*>();
    for (auto btn : ing_btns) {
        if (btn->objectName() != "add_ing_btn") btn->click();
    }
    const QList<QPushButton*>& steps_btns = ui->steps_container->findChildren<QPushButton*>();
    for (auto btn : steps_btns) {
        if (btn->objectName() != "add_method_btn") btn->click();
    }
    qInfo() << "CLicked 4";
    //currentDisplayedRecipe = nullptr;
    ui->stackedWidget->setCurrentWidget(ui->admin_page);
    qInfo() << "CLicked 4";


}



void MainWindow::on_home_btn_clicked()
{
    currentDisplayedRecipe = nullptr;
  ui->stackedWidget->setCurrentWidget(ui->home_page);
    /*display_recipe();*/
  suggest_recipes(5);
}

void MainWindow::choose_image() {
    QString file = QFileDialog::getOpenFileName(this, QString("اختر صورة"), QString(), QString("صور (*.png *.jpg *.jpeg)"));
    if (!file.isEmpty()) ui->image_path_line->setText(file);
}



void MainWindow::on_sort_combobox_all_clicked()
{
    sort(recipes, num_of_recipes);

    display_recipe();

}




void MainWindow::sort(QSharedPointer<Recipe> recipes[], int size) {
    // First, compact the array to move all non-null elements to the front
   int index_combobox = ui->sort_combobox_all->currentIndex();
    int nonNullCount = 0;
    for (int i = 0; i < size; ++i) {
        if (!recipes[i].isNull()) {
            // Swap non-null element to its position in the compacted array
            if (i != nonNullCount) {
                QSharedPointer<Recipe> temp = recipes[i];
                recipes[i] = recipes[nonNullCount];
                recipes[nonNullCount] = temp;
            }
            nonNullCount++;
        }
    }

    // Bubble sort the compacted non-null portion of the array
    for (int i = 0; i < nonNullCount - 1; ++i) {
        for (int j = 0; j < nonNullCount - i - 1; ++j) {
            bool shouldSwap = false;

            // Determine comparison based on criteria
            if (index_combobox == 0) 
                shouldSwap = (recipes[j]->title.toLower() > recipes[j + 1]->title.toLower());
            else if (index_combobox == 1)
                shouldSwap = (recipes[j]->cock_time > recipes[j + 1]->cock_time);
            else if (index_combobox == 2)
                shouldSwap = (recipes[j]->level > recipes[j + 1]->level);
            else if(index_combobox == 3)
                shouldSwap = (recipes[j]->ing_num > recipes[j + 1]->ing_num);
            // Perform swap if needed
            if (shouldSwap) {
                QSharedPointer<Recipe> temp = recipes[j];
                recipes[j] = recipes[j + 1];
                recipes[j + 1] = temp;
            }
        }
    }
    


}

void MainWindow::on_search_btn_clicked() {
    QString input = ui->search_field->text().trimmed();
    int searchType = ui->search_combobox->currentIndex();

    if (input.isEmpty())
    {
        input = ui->search_field_all->text().trimmed();
        searchType = ui->search_combobox_all->currentIndex();

        if (input.isEmpty()) {
            QMessageBox::information(this, "معلومة", "ادخل كلمة للبحث");
            return;
        }
    }

    const int MAX_RESULTS = 100;
    QSharedPointer<Recipe>* results = new QSharedPointer<Recipe>[MAX_RESULTS];
    int resultCount = 0;

    try {
        // Handle all three search types
        if (searchType == 0) { // Title search
            for (int i = 0; i < num_of_recipes && resultCount < MAX_RESULTS; i++) {
                if (recipes[i]->title.toLower().contains(input.toLower())) {
                    results[resultCount++] = recipes[i];
                }
            }
        }
        else if (searchType == 1) { // Time search
            bool ok;
            qInfo() << "INPUT TIME" << " " << input;
            double targetTime = input.toDouble(&ok);

            qInfo() << "OUT TIME" << " " << targetTime;

            if (!ok || targetTime <= 0) {
                QMessageBox::warning(this, "Error", "Invalid time format. Please enter a positive number.");
                delete[] results;
                return;
            }

            for (int i = 0; i < num_of_recipes && resultCount < MAX_RESULTS; i++) {
                if (recipes[i]->cock_time == targetTime) {
                    results[resultCount++] = recipes[i];
                }
            }
        }
        else if (searchType == 2) { // Ingredient search
            QString searchTerm = input.toLower();
            for (int i = 0; i < num_of_recipes && resultCount < MAX_RESULTS; i++) {
                for (int j = 0; j < 100; j++) {
                    if (recipes[i]->ingredients[j].toLower().contains(searchTerm)) {
                        results[resultCount++] = recipes[i];
                        break;
                    }
                }
            }
        }

        if (resultCount > 0) {
            ui->search_field->clear();
            ui->search_field_all->clear();
            sort(results, resultCount);
            display_search_all(results, resultCount);
        }
        else {
            QMessageBox::information(this, "No Results", "No matching recipes found.");
        }

        // Don't delete the results array here!
              // Store it for later use when clicking "View details"
              // Clean up the previous results if they exist
        if (searchResults != nullptr) {
            delete[] searchResults;
        }

        // Save the new results and count
        searchResults = results;
        searchResultCount = resultCount;
    }
    catch (...) {
        delete[] results; // Ensure memory cleanup on error
        searchResults = nullptr;
        searchResultCount = 0;
        QMessageBox::critical(this, "Error", "An unexpected error occurred.");
    }
}


void MainWindow::display_search_all(QSharedPointer<Recipe>* recipes, int count) {
    // Clear previous results
    ui->stackedWidget->setCurrentWidget(ui->all_recipes);
    QLayoutItem* item;
    while ((item = all_recipe_grid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Display logic
    int row = 0, col = 0;
    for (int i = 0; i < count; i++) {

            QWidget* widget = new QWidget;

            QVBoxLayout* layout = new QVBoxLayout(widget);

            QLabel* title = new QLabel(recipes[i]->title);
            title->setAlignment(Qt::AlignCenter);
            title->setStyleSheet(titleStyle);
            QLabel* image = new QLabel;
            image->setPixmap(QPixmap(recipes[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
            image->setAlignment(Qt::AlignCenter);

            QPushButton* button = new QPushButton("اعرض الوصفة");
            button->setStyleSheet(view_button_details);
            connect(button, &QPushButton::clicked, this, [=]() {
                ui->stackedWidget->setCurrentWidget(ui->recipe_page);
                assign_recipe_page(recipes[i]);
                });


            layout->addWidget(title);
            layout->addWidget(image); 
            layout->addWidget(button);

            col = i % 4;
            if (!col && i) row++;
            all_recipe_grid->addWidget(widget, row, col);
        
    }
}

void MainWindow::display_search_private(QSharedPointer<Recipe>* recipes, int count, QGridLayout* layout_grid, QWidget* stacked_page,bool flag) {
    ui->stackedWidget->setCurrentWidget(stacked_page);
    QLayoutItem* item;
    while ((item = layout_grid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (flag)
    {
        QWidget* scrollWidgetFavorite = ui->scrollArea_3->takeWidget();
        if (!scrollWidgetFavorite) {
            scrollWidgetFavorite = new QWidget();
        }
        ui->scrollArea_3->setWidget(scrollWidgetFavorite);
        ui->scrollArea_3->setWidgetResizable(true);
    }
    else {
        QWidget* scrollWidgetEdition = ui->scrollArea_4->takeWidget();
        if (!scrollWidgetEdition) {
            scrollWidgetEdition = new QWidget();
        }
        ui->scrollArea_4->setWidget(scrollWidgetEdition);
        ui->scrollArea_4->setWidgetResizable(true);
    }


    QString different;
    if (flag)
        different = "المفضلة";
    else
        different = "وصفاتي";
    // Display logic
    int row = 0, col = 0;
    for (int i = 0; i < count; i++) {

        QWidget* widget = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[i]->title);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(titleStyle);
        QLabel* image = new QLabel;
        image->setPixmap(QPixmap(recipes[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("اعرض الوصفة" + different);
        button->setStyleSheet(view_button_details);
        connect(button, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipes[i]);
            });
        QPushButton* button_delete = new QPushButton(" حذف من" + different);
        button_delete->setStyleSheet(deleteButtonStyle);
        button_delete->setCursor(Qt::PointingHandCursor);
        // Pass the current index i
        int current_index = i;
        connect(button_delete, &QPushButton::clicked, this, [this, current_index,flag,different]() {
            QMessageBox::information(this, "Info", " تم الحذف من" + different);
            if (flag)
            {
            delete_favorite_btn(current_index);
            display_favorite();
            }
            else {
                delete_edition_btn(current_index);
                display_edition_page_user();
            }
            });

        layout->addWidget(title);
        layout->addWidget(image);
        layout->addWidget(button);
        layout->addWidget(button_delete);
        col = i % 4;
        if (!col && i) row++;
        layout_grid->addWidget(widget, row, col);

    }


}

void MainWindow::on_search_btn_edition_clicked() {
    QString input = ui->search_field_edition->text().trimmed();
    int searchType = ui->search_combobox_edition->currentIndex();

    if (input.isEmpty())
    {
        ui->search_field_edition->clear();

        QMessageBox::information(this, "معلومة", "ادخل كلمة للبحث");
        return;
    }

    const int MAX_RESULTS = 100;
    QSharedPointer<Recipe>* results = new QSharedPointer<Recipe>[MAX_RESULTS];
    int resultCount = 0;

    try {
        // Handle all three search types
        if (searchType == 0) { // Title search
            for (int i = 0; i < loged_in_user->my_recipes_num && resultCount < 100; i++) {
                if (loged_in_user->my_recipes[i]->title.toLower().contains(input.toLower())) {
                    results[resultCount++] = loged_in_user->my_recipes[i];
                }
            }
        }
        else if (searchType == 1) { // Time search
            bool ok;
            qInfo() << "INPUT TIME" << " " << input;
            double targetTime = input.toDouble(&ok);

            qInfo() << "OUT TIME" << " " << targetTime;

            if (!ok || targetTime <= 0) {
                QMessageBox::warning(this, "Error", "Invalid time format. Please enter a positive number.");
                delete[] results;
                return;
            }

            for (int i = 0; i < num_of_recipes && resultCount < MAX_RESULTS; i++) {
                if (recipes[i]->cock_time == targetTime) {
                    results[resultCount++] = recipes[i];
                }
            }
        }
        else if (searchType == 2) { // Ingredient search
            QString searchTerm = input.toLower();
            for (int i = 0; i < num_of_recipes && resultCount < MAX_RESULTS; i++) {
                for (int j = 0; j < 100; j++) {
                    if (recipes[i]->ingredients[j].toLower().contains(searchTerm)) {
                        results[resultCount++] = recipes[i];
                        break;
                    }
                }
            }
        }

        if (resultCount > 0) {
            ui->search_field_edition->clear();
            sort(results, resultCount);
            display_search_private(results, resultCount, edition_grid, ui->myedition_page,false);
        }
        else {
            QMessageBox::information(this, "No Results", "No matching recipes found.");
        }

        // Don't delete the results array here!
              // Store it for later use when clicking "View details"
              // Clean up the previous results if they exist
        if (searchResults != nullptr) {
            delete[] searchResults;
        }

        // Save the new results and count
        searchResults = results;
        searchResultCount = resultCount;
    }
    catch (...) {
        delete[] results; // Ensure memory cleanup on error
        searchResults = nullptr;
        searchResultCount = 0;
        QMessageBox::critical(this, "خطأ", "خطا  غير متوقع");
    }
}


void MainWindow::add_ingredient_row() {
    QWidget* row = new QWidget;
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setPlaceholderText(QStringLiteral("مكون"));
            edit->setStyleSheet("color:rgb(0,0,0);");
    edit->setCursor(Qt::PointingHandCursor);

    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    rm->setStyleSheet(deleteButtonStyleDynamic);
    rm->setCursor(Qt::PointingHandCursor);

    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    ui->ing_container->layout()->addWidget(row);
}

void MainWindow::add_ingredient_row_edition() {
    QWidget* row = new QWidget;
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setPlaceholderText(QStringLiteral("مكون"));
            edit->setStyleSheet("color:rgb(0,0,0);");
    edit->setCursor(Qt::PointingHandCursor);

    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    rm->setStyleSheet(deleteButtonStyleDynamic);
    rm->setCursor(Qt::PointingHandCursor);

    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    ui->ing_container_2->layout()->addWidget(row);
}

void MainWindow::remove_row() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QWidget* row = btn->parentWidget();
    row->setStyleSheet("color:rgb(0,0,0);");

    delete row;
}

void MainWindow::add_method_row() {
    QWidget* row = new QWidget;
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setPlaceholderText(QStringLiteral("اكتب الخطوة"));
    edit->setCursor(Qt::PointingHandCursor);

    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
            edit->setStyleSheet("color:rgb(0,0,0);");
    rm->setStyleSheet(deleteButtonStyleDynamic);
    rm->setCursor(Qt::PointingHandCursor);

    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    QLayout* method_layout = ui->steps_container->layout();
    method_layout->addWidget(row);
}

void MainWindow::add_method_row_edition() {
    QWidget* row = new QWidget;
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setPlaceholderText(QStringLiteral("اكتب الخطوة"));
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
            edit->setStyleSheet("color:rgb(0,0,0);");
    edit->setCursor(Qt::PointingHandCursor);

    rm->setStyleSheet(deleteButtonStyle);
    rm->setCursor(Qt::PointingHandCursor);

    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    QLayout* method_layout = ui->steps_container_2->layout();
    method_layout->addWidget(row);
}

void MainWindow::on_go_edition_page_btn_clicked()
{
    // Check if there are favorite recipes
    if (loged_in_user->my_recipes_num == 0) {
        qDebug() << "No edition recipes for user:" << loged_in_user->username;
        // Optionally show a message
        QMessageBox::information(this, "Info", "You have no edition recipes.");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->myedition_page);
    display_edition_page_user();
}

void MainWindow::display_edition_page_user(bool order) {
    if (!edition_grid) {
        qDebug() << "Error: edition_grid is null";
        return;
    }

    // Clear existing grid
    while (QLayoutItem* item = edition_grid->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
        delete item;
    }

    qDebug() << "Favorite recipes count:" << loged_in_user->my_recipes_num;

    // Take ownership of the existing scroll area's widget if exists
    QWidget* scrollWidgetEdition = ui->scrollArea_4->takeWidget();
    if (!scrollWidgetEdition) {
        scrollWidgetEdition = new QWidget();
    }

    // Create new grid layout on the widget
    delete edition_grid; // Delete old layout if exists
    edition_grid = new QGridLayout(scrollWidgetEdition);

    const int columns = 4;
    int row = 0, col = 0;
    for (int i = order ? loged_in_user->my_recipes_num - 1 : 0; order ? i >= 0 : loged_in_user->my_recipes_num; i += order ? -1 : 1)
    {
        if (!loged_in_user->my_recipes[i]) {
            qDebug() << "Null recipe at index:" << i;
            continue;
        }

        int recipe_id = loged_in_user->my_recipes[i]->id;


        QWidget* widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(loged_in_user->my_recipes[i]->title);
        title->setAlignment(Qt::AlignCenter);
        title->setWordWrap(true);
        title->setStyleSheet(titleStyle);
        QLabel* image = new QLabel;
        QPixmap pixmap(loged_in_user->my_recipes[i]->imagePath);
        if (!pixmap.isNull()) {
            image->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else {
            image->setText("No Image");
            qDebug() << "Failed to load image for recipe:" << loged_in_user->my_recipes[i]->title;
        }
        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("اعرض الوصفة");
        button->setStyleSheet(view_button_details);
        button->setCursor(Qt::PointingHandCursor);
        QSharedPointer<Recipe> recipe_ptr = loged_in_user->my_recipes[i];
        connect(button, &QPushButton::clicked, this, [this, recipe_ptr]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipe_ptr);
            });

        QPushButton* button_delete = new QPushButton("حذف الوصفة");
        button_delete->setStyleSheet(deleteButtonStyle);
        button_delete->setCursor(Qt::PointingHandCursor);

        connect(button_delete, &QPushButton::clicked, this, [this, i]() {
            if (QMessageBox::question(this, "Confirm", "Remove this recipe from your list?") == QMessageBox::Yes) {
                delete_edition_btn(i);
                QMessageBox::information(this, "Info", "Recipe removed from your recipes");
                display_edition_page_user();
            }
            });

        layout->addWidget(title);
        layout->addWidget(image);
        layout->addWidget(button);
        layout->addWidget(button_delete);

        qDebug() << "Adding user recipe widget at row:" << row << "col:" << col;
        edition_grid->addWidget(widget, row, col);

        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }

    ui->scrollArea_4->setWidget(scrollWidgetEdition);
    ui->scrollArea_4->setWidgetResizable(true);
    qDebug() << "scrollArea_4 visible:" << ui->scrollArea_4->isVisible();
}

void MainWindow::delete_edition_btn(int id_edition)
{

    // Remove from user's my_recipes
    for (int j = id_edition; j < loged_in_user->my_recipes_num - 1; j++) {
        loged_in_user->my_recipes[j] = loged_in_user->my_recipes[j + 1];
    }
    loged_in_user->my_recipes[loged_in_user->my_recipes_num - 1].reset();
    loged_in_user->my_recipes_num--;

    currentDisplayedRecipe = nullptr;
    qDebug() << "Deleted user recipe at index:" << id_edition << "New my_recipes_num:" << loged_in_user->my_recipes_num;

}


void MainWindow::add_ingred_to_user_page_edit() {


    /*****ingred******/
    QLayout* method_layout = ui->ing_container_2->layout();
    if (!method_layout) {
        method_layout = new QVBoxLayout(ui->ing_container_2);
        qDebug() << "Created new layout for ing_container_2";
    }

    // Clear existing rows (preserve the button)
    // Iterate backwards to safely remove items
    while (method_layout->count() > 0) {
        QLayoutItem* item = method_layout->takeAt(0); // Take first item
        if (!item) continue;

        QWidget* widget = item->widget();
        if (widget) {
            // Only delete non-button widgets
            if (widget != ui->add_ing_edit_btn_2) {
                delete widget;
            }
            else {
                // Keep the button alive but remove from layout temporarily
                ui->ing_container_2->layout()->removeWidget(widget);
            }
        }
        delete item; // Delete the layout item
    }
    method_layout->addWidget(ui->add_ing_edit_btn_2);

        for (int i = 0; i < currentDisplayedRecipe->ing_num;i++)
    {
        qDebug() << "Ingredient" << i << ":" << currentDisplayedRecipe->ingredients[i] << '\n';
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setText(currentDisplayedRecipe->ingredients[i]);
        edit->setStyleSheet("color:rgb(0,0,0);");
         QPushButton* rm = new QPushButton(QStringLiteral("حذف"));

        rm->setStyleSheet(deleteButtonStyleDynamic);
        rm->setCursor(Qt::PointingHandCursor);

        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        QLayout* method_layout = ui->ing_container_2->layout();
        method_layout->addWidget(row);
        qDebug() << "Add Ingredient" << i << ":" << currentDisplayedRecipe->ingredients[i] << '\n';

    }

    qDebug() << "OUT FROM INGREDIANTES";
}



void MainWindow::add_method_to_user_page_edit() {

    QLayout* method_layout = ui->steps_container_2->layout();
    if (!method_layout) {
        method_layout = new QVBoxLayout(ui->steps_container_2);
        qDebug() << "Created new layout for ing_container_2";
    }

    // Clear existing rows (preserve the button)
    // Iterate backwards to safely remove items
    while (method_layout->count() > 0) {
        QLayoutItem* item = method_layout->takeAt(0); // Take first item
        if (!item) continue;

        QWidget* widget = item->widget();
        if (widget) {
            // Only delete non-button widgets
            if (widget != ui->add_method_edit_btn_2) {
                delete widget;
            }
            else {
                // Keep the button alive but remove from layout temporarily
                ui->steps_container_2->layout()->removeWidget(widget);
            }
        }
        delete item; // Delete the layout item
    }
    method_layout->addWidget(ui->add_method_edit_btn_2);

    /*****steps******/

    for (int i = 0; i < currentDisplayedRecipe->steps_num;i++)
    {
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setText(currentDisplayedRecipe->steps[i]);
        QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
         edit->setStyleSheet("color:rgb(0,0,0);");
        edit->setCursor(Qt::PointingHandCursor);

        rm->setStyleSheet(deleteButtonStyleDynamic);
        rm->setCursor(Qt::PointingHandCursor);

        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        QLayout* method_layout = ui->steps_container_2->layout();
        method_layout->addWidget(row);
        qInfo() << "STEPS" << i << ":" << currentDisplayedRecipe->steps[i] << '\n';
    }
}

void MainWindow::on_save_edit_recipe_btn_2_clicked()
{
    QSharedPointer<Recipe> recipe_ptr(new Recipe());
    recipe_ptr->generate_id();
    loged_in_user->my_recipes[loged_in_user->my_recipes_num] = recipe_ptr;
    loged_in_user->my_recipes_num++;



    // Manually copy properties from currentDisplayedRecipe
    recipe_ptr->category = currentDisplayedRecipe->category;
    recipe_ptr->title = currentDisplayedRecipe->title;
    recipe_ptr->level = currentDisplayedRecipe->level;
    recipe_ptr->cock_time = currentDisplayedRecipe->cock_time;
    recipe_ptr->rates_sum = currentDisplayedRecipe->rates_sum;
    recipe_ptr->rates_num = currentDisplayedRecipe->rates_num;
    recipe_ptr->imagePath = currentDisplayedRecipe->imagePath;

    recipe_ptr->generate_id();


    // Get ingredients
    int num_ingredients = get_ingredient_count_edit();
    QString* ingredients_array = get_ingredients_edit(num_ingredients);

    // Get steps
    int num_steps = get_step_count_edit();
    QString* steps_array = get_steps_edit(num_steps);


    /***************************** Assign data from ui ************************/

    recipe_ptr->description = ui->desc_edit_field_2->toPlainText();
    recipe_ptr->description = recipe_ptr->description.trimmed();

    if (recipe_ptr->description.isEmpty()) {
        show_warning_messageBox(this,"الوصف لا يمكن أن يكون فارغاً");
        return;
    }

    // Assign ingredients
    recipe_ptr->ing_num = num_ingredients;
        if (recipe_ptr->ing_num == 0) {
            show_warning_messageBox(this, "يجب أن يكون هناك مكونات");
            return;
        }
    for (int i = 0; i < num_ingredients; ++i) {
        qDebug() << "ingredients " << i << recipe_ptr->ingredients[i] << "   ingredients_array  " << ingredients_array[i];
        recipe_ptr->ingredients[i] = ingredients_array[i];
    }

    // Assign steps
    recipe_ptr->steps_num = num_steps;
    if (recipe_ptr->steps_num == 0) {
        show_warning_messageBox(this, "يجب أن يكون هناك خطوات");
        return;
    }
    for (int i = 0; i < num_steps; ++i) {
        qDebug() << "STEPS " << i << recipe_ptr->steps[i] << "   STEPS_array  " << steps_array[i];
        recipe_ptr->steps[i] = steps_array[i];
    }

    // Clean up dynamically allocated arrays
    delete[] ingredients_array;
    delete[] steps_array;

    show_success_messageBox(this, "تم الإضافة إلى وصفاتي بنجاح");
    qDebug() << "Number of ingredients:" << get_ingredient_count();
    qDebug() << "Number of steps:" << get_step_count();
    qInfo() << "Recipe" << recipe_ptr->id << "added successfully!";

}

int MainWindow::get_ingredient_count_edit()
{
    int count=0;
    QWidget* container = ui->ing_container_2; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    for (QLineEdit* lineEdit : lineEdits) {
        if (!(lineEdit->text().trimmed() == "")) {
            ++count;
        }
    }

    return count;
}

int MainWindow::get_step_count_edit()
{
    int count = 0;
    QWidget* container = ui->steps_container_2; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    for (QLineEdit* lineEdit : lineEdits) {
        if (!(lineEdit->text().trimmed()=="")) {
            ++count;
        }
    }

    return count;
    
}

QString* MainWindow::get_ingredients_edit(int& out_count)
 {
    QWidget* container = ui->ing_container_2; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    QString* ingredients = new QString[out_count];

    for (int i = 0; i < out_count; ++i) {
        QString text = lineEdits[i]->text();
        ingredients[i] = text;
        qDebug() << "LineEdit Text:" << text;
    }
   return ingredients;
    

}

QString* MainWindow::get_steps_edit(int& out_count) {
    QWidget* container = ui->steps_container_2; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    QString* steps = new QString[out_count];

    for (int i = 0; i < out_count; ++i) {
        QString text = lineEdits[i]->text();
        steps[i] = text;
        qDebug() << "LineEdit Text:" << text;
    }

        return steps;
    

}



int MainWindow::get_ingredient_count() {
    int count = 0;
    QWidget* container = ui->ing_container; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    for (QLineEdit* lineEdit : lineEdits) {
        if (!(lineEdit->text().trimmed() == "")) {
            ++count;
        }
        qInfo() << "LINEDIT: " << lineEdit;
    }

    return count;

}

int MainWindow::get_step_count() {

    int count = 0;
    QWidget* container = ui->steps_container; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    for (QLineEdit* lineEdit : lineEdits) {
        if (!(lineEdit->text().trimmed() == "")) {
            ++count;
        }
    }

    return count;
}



void MainWindow::display_recipe(bool arrang)
{
    ui->stackedWidget->setCurrentWidget(ui->all_recipes);
    QLayoutItem* item;
    while ((item = all_recipe_grid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    int r = 0, c = 0;
    int validRecipes = 0;
    for (int i = arrang ? num_of_recipes - 1 : 0;
        arrang ? i >= 0 : i < num_of_recipes;
        i += arrang ? -1 : 1)
    {

        if (recipes[i].isNull())
            continue;
        qInfo() << "HdfGf554fHGH";

        QWidget* widget = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[i]->title);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(titleStyle);
        QLabel* image = new QLabel;
        qInfo() << recipes[i]->imagePath;;
        image->setPixmap(QPixmap(recipes[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));

        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("اعرض الوصفة");
        button->setStyleSheet(view_button_details);
        connect(button, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipes[i]);
            });


        layout->addWidget(title);
        layout->addWidget(image); // ADDED: Image label
        layout->addWidget(button);

        // Calculate grid position: 4 columns per row
        c = validRecipes % 4;
        r = validRecipes / 4;
        validRecipes++;

        // Add to grid
        all_recipe_grid->addWidget(widget, r, c);
    }
}


void MainWindow::on_submit_recipe_btn_clicked()
{
    // Step 1: Collect all input values into local variables
    QString title = ui->title_field->text().trimmed();
    QString description = ui->desc_field->toPlainText().trimmed();
    int category = ui->category_combobox_btn->currentIndex();
    int level = ui->level_combobox_btn->currentIndex();
    int cock_time = ui->time_spin_btn->value();
    QString imagePath = ui->image_path_line->text().trimmed();
    int num_ingredients = get_ingredient_count();
    int num_steps = get_step_count();

    // Step 2: Validate all required fields
    // If any field is missing, show a warning and return without saving
    if (title.isEmpty()) {
        show_warning_messageBox(this, "يجب إضافة عنوان للوصفة");
        return;
    }
    if (description.isEmpty()) {
        show_warning_messageBox(this, "يجب إضافة وصف لللوصفة");
        return;
    }
    if (imagePath.isEmpty()) {
        show_warning_messageBox(this, "يجب إضافة صورة للوصفة");
        return;
    }
    //if (num_ingredients == 0) {
    //    show_warning_messageBox(this, "يجب إضافة مكونات للوصفة");
    //    return;
    //}
    //if (num_steps == 0) {
    //    show_warning_messageBox(this, "يجب إضافة خطوات للوصفة");
    //    return;
    //}

    // Step 3: All required fields are present, proceed to retrieve additional data
    QString* ingredients_array = get_ingredients(num_ingredients);
    QString* steps_array = get_steps(num_steps);

    // Step 4: Create or update the recipe
    QSharedPointer<Recipe> recipe_ptr;
    if (currentDisplayedRecipe == nullptr) {
        // Creating a new recipe
        recipe_ptr = QSharedPointer<Recipe>(new Recipe());
        recipe_ptr->generate_id();
        recipes[num_of_recipes] = recipe_ptr;
        recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
        num_of_recipes++;
    }
    else {
        // Editing an existing recipe
        recipe_ptr = currentDisplayedRecipe;
    }

    // Step 5: Assign data to the recipe object
    recipe_ptr->title = title;
    recipe_ptr->description = description;
    recipe_ptr->category = category;
    recipe_ptr->level = level;
    recipe_ptr->cock_time = cock_time;
    recipe_ptr->imagePath = imagePath;

    recipe_ptr->ing_num = num_ingredients;
    for (int i = 0; i < num_ingredients; ++i) {
        recipe_ptr->ingredients[i] = ingredients_array[i];
    }

    recipe_ptr->steps_num = num_steps;
    for (int i = 0; i < num_steps; ++i) {
        recipe_ptr->steps[i] = steps_array[i];
    }

     //Step 6: Clean up dynamically allocated arrays
    delete[] ingredients_array;
    delete[] steps_array;
     // add ingredients
    //const QList<QLineEdit*>& ingredients = ui->ing_container->findChildren<QLineEdit*>();
    //recipe_ptr->ing_num = ingredients.size();
    //for (int i = 0; i < recipe_ptr->ing_num; i++)
    //{
    //    recipe_ptr->ingredients[i] = ingredients[i]->text().trimmed();
    //}

    //// add steps
    //const QList<QLineEdit*>& steps = ui->steps_container->findChildren<QLineEdit*>();
    //recipe_ptr->steps_num = steps.size();
    //for (int i = 0; i < recipe_ptr->steps_num; i++)
    //{
    //    recipe_ptr->steps[i] = steps[i]->text().trimmed();
    //}

    // Step 8: Reset the current displayed recipe
    currentDisplayedRecipe = nullptr;
    show_success_messageBox(this, "تم إضافة الوصفة بنجاح");
    on_add_recipe_admin_btn_clicked();

    qInfo() << "Recipe" << recipe_ptr->id << "added/updated successfully!";
}




void MainWindow::assign_recipe_page(QSharedPointer<Recipe> r_ptr)
{
    currentDisplayedRecipe = r_ptr;
    // Clear existing items in ingredient and step lists

    ui->ingred_list->clear();
    ui->steps_list->clear();
    currentDisplayedRecipe = r_ptr;
    ui->img_perview->setPixmap(QPixmap(r_ptr->imagePath).scaled(ui->img_perview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->img_perview->setAlignment(Qt::AlignCenter);
    ui->title_recipe->setText(r_ptr->title);
    ui->desc_browser->setText(r_ptr->description);
    QString s = QString::number(r_ptr->cock_time,'f',2);
    ui->display_time_recipe->setText("وقت الطهو:  " + s);
    //QString avg_rate = QString::number((r_ptr->rates_sum / r_ptr->rates_num),'f',2);
    //ui->display_avg_rate->setText("متوسط التقييمات:  " + avg_rate);
    QString level;
        if (r_ptr->level == 0)  ui->display_level_recipe->setText("مستوى العصوبة:   سهل");
        else if (r_ptr->level == 1)  ui->display_level_recipe->setText("مستوى الصعوبة:   متوسط");
        else  ui->display_level_recipe->setText("مستوى الصعوبة:   صعب");
        qInfo() << "level " << r_ptr->level;
        QString category;
        if (r_ptr->category == 0) ui->display_category_recipe->setText("التصنيف: حلو");
        else ui->display_category_recipe->setText("التصنيف: حادق");
        qInfo() << "level " << r_ptr->category;


        for (int i = 0; i < r_ptr->ing_num;i++)
            ui->ingred_list->addItem(r_ptr->ingredients[i]);
        QVBoxLayout* ing_layout = new QVBoxLayout;
        ing_layout->addWidget(ui->ingred_list);
        ing_layout->setSpacing(5);


        /*****STEPS******/
        for (int i = 0; i < r_ptr->steps_num;i++)
            ui->steps_list->addItem(r_ptr->steps[i]);
        QVBoxLayout* _layout = new QVBoxLayout;
        ing_layout->addWidget(ui->steps_list);
        ing_layout->setSpacing(5);

}

void MainWindow::on_delete_recipe_btn_clicked() {
    // return to homepage or wherever you want
    // now delete all pointers to current recipe and it will automatically delete the objet for us 
    short& idx = recipes_id_to_index[currentDisplayedRecipe->id];

    // make all recipes adjacent:
    for (int i = idx; i + 1 < num_of_recipes; ++i) {
        recipes[i] = recipes[i + 1];
        recipes_id_to_index[recipes[i]->id] = i;
    }

    recipes[num_of_recipes - 1].clear();           // drop the QSharedPointer
    recipes_id_to_index[currentDisplayedRecipe->id] = -1;           // mark “gone”
    --num_of_recipes;   
    // delete from current 
    display_recipe();
}

void MainWindow::assign_admin_page()
{
    if (currentDisplayedRecipe == nullptr)
    {
        qDebug() << "NULLL";
        return;
    }
    ui->desc_field->setPlainText(currentDisplayedRecipe->description);
    ui->title_field->setText(currentDisplayedRecipe->title);
    ui->level_combobox_btn->setCurrentIndex(currentDisplayedRecipe->level);
    

    ui->time_spin_btn->setValue(currentDisplayedRecipe->cock_time);
    ui->category_combobox_btn->setCurrentIndex(currentDisplayedRecipe->category);
    ui->image_path_line->setText(currentDisplayedRecipe->imagePath);
    /*****add******/
    //qDebug() << "Ingredients count:" << currentDisplayedRecipe->ing_num;
    for (int i = 0; i < currentDisplayedRecipe->ing_num;i++)
    {
        qDebug() << "Ingredient" << i << ":" << currentDisplayedRecipe->ingredients[i] << '\n';
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setText(currentDisplayedRecipe->ingredients[i]);
        QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
                edit->setStyleSheet("color:rgb(0,0,0);");
        edit->setCursor(Qt::PointingHandCursor);

        rm->setStyleSheet(deleteButtonStyleDynamic);
        rm->setCursor(Qt::PointingHandCursor);

        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        QLayout* method_layout = ui->ing_container->layout();
        method_layout->addWidget(row);
        qDebug() << "Add Ingredient" << i << ":" << currentDisplayedRecipe->ingredients[i] << '\n';

    }
    for (int i = 0; i < currentDisplayedRecipe->steps_num;i++)
    {
        //qDebug() << "Ingredient" << i << ":" << currentDisplayedRecipe->steps[i] << '\n';
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setText(currentDisplayedRecipe->steps[i]);
        QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
                edit->setStyleSheet("color:rgb(0,0,0);");
        edit->setCursor(Qt::PointingHandCursor);

        rm->setStyleSheet(deleteButtonStyleDynamic);
        rm->setCursor(Qt::PointingHandCursor);

        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        QLayout* method_layout = ui->steps_container->layout();
        method_layout->addWidget(row);
        qDebug() << "Add Ingredient" << i << ":" << currentDisplayedRecipe->steps[i] << '\n';

    }

   /* delete[] currentDisplayedRecipe->ingredients;
    delete[] currentDisplayedRecipe->steps;*/

    qDebug() << "OUT FROM ADMIN ASSign";
}

// ADDFvorite
void MainWindow::on_add_favorite_btn_clicked()
{

    // Check if we've reached the maximum number of favorites
    if (loged_in_user->favorite_recipes_num >= 100) {
        //QMessageBox::warning(nullptr, "Error", "وصلت للحد الأقصى من عدد الوصفات المفضلة");
        show_warning_messageBox(this, "وصلت للحد الأقصى من عدد الوصفات المفضلة");
        return;
    }

    // Check if recipe is already in favorites
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == currentDisplayedRecipe->id) {
            QMessageBox::information(nullptr, "Info", "مضافة مسبقا للمفضلة");
            qInfo("Exist");
            return;
        }
        
    }
    qInfo("Clicked");

    // Add recipe to favorites
    loged_in_user->favorite_recipes[loged_in_user->favorite_recipes_num] = currentDisplayedRecipe->id;
    loged_in_user->favorite_recipes_num++;

    //QMessageBox::information(nullptr, "Success", "تم إضافته بنجاح إلى المفضلة");
    show_success_messageBox(this, "تم إضافته بنجاح إلى المفضلة");

}
    


void MainWindow::on_go_favorite_btn_clicked()
{
    // Check if there are favorite recipes
    if (loged_in_user->favorite_recipes_num == 0) {
        QMessageBox::information(this, "Info", "ليس لديك أي وصفات مفضلة");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->favorite_page);
    display_favorite();
}

void MainWindow::on_search_btn_favorite_clicked()
{
        QString input = ui->search_field_favorite->text().trimmed();
        qInfo() << "SEARCH INPUT " << input;
        int searchType = ui->search_combox_favorite->currentIndex();
        qInfo() << "SEARCH TYPE " << searchType;

        if (input.isEmpty())
        {
            ui->search_field_favorite->clear();
            QLayoutItem* item;
            while ((item = favorite_grid->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            display_favorite();
            QMessageBox::information(this, "معلومة", "ادخل كلمة للبحث");
            return;
        }

        const int MAX_RESULTS = 100;
        QSharedPointer<Recipe>* results = new QSharedPointer<Recipe>[MAX_RESULTS];
        int resultCount = 0;
        int recipe_index;
        try {
            // Handle all three search types
            if (searchType == 0) { // Title search
                for (int i = 0; i < loged_in_user->favorite_recipes_num && resultCount < 100; i++) {
                    recipe_index = recipes_id_to_index[loged_in_user->favorite_recipes[i]];
                    qInfo() << i << " T " << recipes[recipe_index]->title;
                    qInfo() << i << " T " << recipes[i]->title;

                    if (recipes[recipe_index]->title.toLower().contains(input.toLower()))
                    {

                        results[resultCount++] = recipes[recipe_index];
                         qInfo()<<results[resultCount-1]->title << " DEBUG";
                         qInfo() << recipes[i]->title << " DEBUG";

                    }
                        
                }
            }
            else if (searchType == 1) { // Time search
                bool ok;
                qInfo() << "INPUT TIME" << " " << input;
                double targetTime = input.toDouble(&ok);

                qInfo() << "OUT TIME" << " " << targetTime;

                if (!ok || targetTime <= 0) {
                    QMessageBox::warning(this, "Error", "Invalid time format. Please enter a positive number.");
                    delete[] results;
                    return;
                }



                for (int i = 0; i < loged_in_user->favorite_recipes_num && resultCount < MAX_RESULTS; i++) {
                    recipe_index = recipes_id_to_index[loged_in_user->favorite_recipes[i]];
                    if (recipes[recipe_index]->cock_time == targetTime) {
                        results[resultCount++] = recipes[recipe_index];
                    }
                }
            }
            else if (searchType == 2) { // Ingredient search
                QString searchTerm = input.toLower();
                for (int i = 0; i < loged_in_user->favorite_recipes_num && resultCount < MAX_RESULTS; i++) {
                    recipe_index = recipes_id_to_index[loged_in_user->favorite_recipes[i]];

                    for (int j = 0; j < 100; j++) {
                        qInfo() << recipes[recipe_index]->ingredients[j] + " INGRED ";
                        if (recipes[recipe_index]->ingredients[j].toLower().contains(searchTerm)) {
                            results[resultCount++] = recipes[recipe_index];
                            break;
                        }
                    }
                }
            }
            else if (searchType == 3) {
                ;
            }
            if (resultCount > 0) {
                ui->search_field_favorite->clear();
                sort(results, resultCount);
                display_search_private(results, resultCount, favorite_grid, ui->favorite_page,true);
            }
            else {
                QMessageBox::information(this, "No Results", "No matching recipes found.");
            }

            // Don't delete the results array here!
                  // Store it for later use when clicking "View details"
                  // Clean up the previous results if they exist
            if (searchResults != nullptr) {
                delete[] searchResults;
            }

            // Save the new results and count
            searchResults = results;
            searchResultCount = resultCount;
        }
        catch (...) {
            delete[] results; // Ensure memory cleanup on error
            searchResults = nullptr;
            searchResultCount = 0;
            QMessageBox::critical(this, "خطأ", "خطا  غير متوقع");
        }
    }



void MainWindow::display_favorite(bool order)
{
    if (!favorite_grid) {
        qDebug() << "Error: favorite_grid is null";
        return;
    }

    // Clear existing widgets from the grid
    QLayoutItem* item;
    while ((item = favorite_grid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }


    QWidget* scrollWidgetFavorite = ui->scrollArea_3->takeWidget();
    if (!scrollWidgetFavorite) {
        scrollWidgetFavorite = new QWidget();
    }

    // Create new grid layout on the widget
    delete favorite_grid; // Delete old layout if exists
    favorite_grid = new QGridLayout(scrollWidgetFavorite);

    int r = 0, c = 0;

    for (int i = order ? loged_in_user->favorite_recipes_num - 1 : 0; order ? i >= 0 : loged_in_user->favorite_recipes_num; i += order ? -1 : 1)
    {
        int recipe_id = loged_in_user->favorite_recipes[i];
        int recipe_index = recipes_id_to_index[recipe_id];
        if (recipe_index == -1 || recipe_index >= num_of_recipes || !recipes[recipe_index]) {
            qDebug() << "Invalid recipe ID:" << recipe_id << "at index:" << recipe_index;
            continue;
        }

        QWidget* widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[recipe_index]->title);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(titleStyle);
        QLabel* image = new QLabel;
        QPixmap pixmap(recipes[recipe_index]->imagePath);
        if (!pixmap.isNull()) {
            image->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
        }
        else {
            image->setText("No image");
            image->setStyleSheet(titleStyle);
            qDebug() << "Failed to load image for recipe:" << recipes[recipe_index]->title;
        }
        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("اعرض الوصفة");
        button->setStyleSheet(view_button_details);
        button->setCursor(Qt::PointingHandCursor);
        // Need to copy the pointer to avoid issues with lambdas
        QSharedPointer<Recipe> recipe_ptr = recipes[recipe_index];
        connect(button, &QPushButton::clicked, this, [this, recipe_ptr]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipe_ptr);
            });

        QPushButton* button_delete = new QPushButton("حذف من المفضلة");
        button_delete->setStyleSheet(deleteButtonStyle);
        button_delete->setCursor(Qt::PointingHandCursor);
        // Pass the current index i
        int current_index = i;
        connect(button_delete, &QPushButton::clicked, this, [this, current_index]() {
            QMessageBox::information(this, "Info", "تم الحذف من المفضلة");
            delete_favorite_btn(current_index);
            display_favorite();
            });

        layout->addWidget(title);
        layout->addWidget(image);
        layout->addWidget(button);
        layout->addWidget(button_delete);

        c = i % 4;
        if (c == 0 && i > 0) r++;

        favorite_grid->addWidget(widget, r, c);
    }

    ui->scrollArea_3->setWidget(scrollWidgetFavorite);
    ui->scrollArea_3->setWidgetResizable(true);
    qDebug() << "scrollArea_3 visible:" << ui->scrollArea_3->isVisible();
}
void MainWindow::on_sort_combobox_edition()
{
    sort(loged_in_user->my_recipes, loged_in_user->my_recipes_num);
    display_edition_page_user();
}
void MainWindow::delete_favorite_btn(int id_favorite)
{
    // Get the actual recipe ID from the array
    int recipe_id = loged_in_user->favorite_recipes[id_favorite];
    int& fav_num = loged_in_user->favorite_recipes_num;

    // Shift all elements after the deleted one
    for (int i = id_favorite; i < fav_num - 1; i++) {
        loged_in_user->favorite_recipes[i] = loged_in_user->favorite_recipes[i + 1];
    }

    // Clear the last element and decrease count
    loged_in_user->favorite_recipes[fav_num - 1] = 0;
    fav_num--;

}


void MainWindow::on_edit_user_btn_clicked()
{
    if (loged_in_user->isAdmin == true) {

        on_add_recipe_admin_btn_clicked();
        assign_admin_page();
    }
    else {

    ui->stackedWidget->setCurrentWidget(ui->user_edit_page);
    assign_edition_user_page();

    }


}

void MainWindow::assign_edition_user_page()
{
    ui->desc_edit_field_2->setPlainText(currentDisplayedRecipe->description);
    add_ingred_to_user_page_edit();
    add_method_to_user_page_edit();

}


// Retrieve ingredients from the UI as a dynamic array
QString* MainWindow::get_ingredients(int& out_count) {
    QWidget* container = ui->ing_container; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    QString* ingredients = new QString[out_count];

    for (int i = 0; i < out_count; ++i) {
        QString text = lineEdits[i]->text();
        ingredients[i] = text;
        qDebug() << "LineEdit Text:" << text;
    }

    return ingredients;
}

// Retrieve steps from the UI as a dynamic array
QString* MainWindow::get_steps(int& out_count) {
    QWidget* container = ui->steps_container; // your container widget

    const QList<QLineEdit*> lineEdits = container->findChildren<QLineEdit*>();

    QString* steps = new QString[out_count];

    for (int i = 0; i < out_count; ++i) {
        QString text = lineEdits[i]->text();
        steps[i] = text;
        qDebug() << "LineEdit Text:" << text;
    }
    return steps;
}

void MainWindow::show_warning_messageBox(QWidget* parent, const QString& message)
{
    QMessageBox::warning(parent, QObject::tr("تحذير"), message);
}

void MainWindow::show_success_messageBox(QWidget* parent, const QString& message)
{
    QMessageBox::information(parent, QObject::tr("نجاح"), message);

}
