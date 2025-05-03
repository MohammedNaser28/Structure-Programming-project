#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowClass),
    recipes_grid(new QGridLayout),
    favorite_grid(new QGridLayout),
    edition_grid(new QGridLayout) {



    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(recipes_grid);
    ui->scrollArea->setWidgetResizable(true);

    // Set layouts for scroll widgets
    //scrollWidgetFavorite->setLayout(favorite_grid);
    //scrollWidgetEdition->setLayout(edition_grid);

    //// Set scrollWidgetFavorite for scrollArea_3
    ui->scrollAreaWidgetContents_3->setLayout(favorite_grid);
    ui->scrollArea_3->setWidgetResizable(true);
    ui->scrollAreaWidgetContents_4->setLayout(edition_grid);
    ui->scrollArea_4->setWidgetResizable(true);

    /**************************************************************************/
    //if(currentUser!=NULL)
    //    ui->user_name_label->setText(users[currentUser]->name);

    ui->image_path_line->setPlaceholderText(QStringLiteral("مسار الصورة"));

    /**************************************************************************/
    connect(ui->browse_btn, &QPushButton::clicked, this, &MainWindow::choose_image);
    connect(ui->add_method_btn, &QPushButton::clicked, this, &MainWindow::add_method_row);
    connect(ui->add_ing_btn, &QPushButton::clicked, this, &MainWindow::add_ingredient_row);

    connect(ui->add_method_edit_btn_2, &QPushButton::clicked, this, &MainWindow::add_method_row_edition);
    connect(ui->add_ing_edit_btn_2, &QPushButton::clicked, this, &MainWindow::add_ingredient_row_edition);

    connect(ui->sort_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sort_combobox_clicked()));


    //edition_grid.fill(nullptr, 1000);
    recipe_pages.fill(nullptr, 1000);
}

void MainWindow::startup()
{
    if (loged_in_user->isAdmin == true)
    {
        ui->go_favorite_btn->setVisible(false);
        ui->go_edition_page_btn->setVisible(false);
        ui->add_recipe_admin_btn->setVisible(true);
        ui->delete_recipe_btn->setVisible(true);
        ui->rate_spin_user->setVisible(false);
        ui->rate_label->setVisible(false);
        ui->add_favorite_btn->setVisible(false);

    }
    else {
        ui->add_recipe_admin_btn->setVisible(false);
        ui->go_favorite_btn->setVisible(true);
        ui->go_edition_page_btn->setVisible(true);
        ui->delete_recipe_btn->setVisible(false);
        ui->rate_spin_user->setVisible(true);
        ui->rate_label->setVisible(true);
        ui->add_favorite_btn->setVisible(true);
    }

    ui->rate_spin_user->setRange(0, 5);
    reload_recipe_first_time();
}

void MainWindow::reload_recipe_first_time()
{
    ui->stackedWidget->setCurrentWidget(ui->home_page);
    display_recipe();
}


MainWindow::~MainWindow()
{
    delete ui;
    qDeleteAll(recipe_pages);
    recipe_pages.clear();
}

void MainWindow::on_logout_btn_clicked()
{
    currentDisplayedRecipe = nullptr;
    loged_in_user = nullptr;
    emit switchToDialog();
}

void MainWindow::on_add_recipe_admin_btn_clicked()
{
    currentDisplayedRecipe = nullptr;
    ui->stackedWidget->setCurrentWidget(ui->admin_page);

}



void MainWindow::on_home_btn_clicked()
{
    currentDisplayedRecipe = nullptr;
  ui->stackedWidget->setCurrentWidget(ui->home_page);
    display_recipe();
}

void MainWindow::choose_image() {
    QString file = QFileDialog::getOpenFileName(this, QString("اختر صورة"), QString(), QString("صور (*.png *.jpg *.jpeg)"));
    if (!file.isEmpty()) ui->image_path_line->setText(file);
}



void MainWindow::on_sort_combobox_clicked()
{
    qInfo() << "SORT RUN" << '\n';
    sort(recipes, num_of_recipes);
    qInfo() << "SORT MIDDLE" << '\n';

    display_recipe();
    qInfo() << "SORT END" << '\n';

}



void MainWindow::on_arrangment_btn_clicked() {
        isDescending = !isDescending;
        ui->arrangment_btn->setIcon(QIcon(isDescending ? ":/MainWindow/sort-descending.png" : ":/MainWindow/sort-descending.png"));
        display_recipe(isDescending);
}

void MainWindow::sort(QSharedPointer<Recipe> recipes[], int size) {
    // First, compact the array to move all non-null elements to the front
   int index_combobox = ui->sort_combobox->currentIndex();
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
            //else if (index_combobox == 4)
                //shouldSwap = (recipes[j]->(recipes[j]->ing_sum / recipes[j]->ing_num) > recipes[j + 1]->ing_num);
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
    if (input.isEmpty()) {
        QMessageBox::information(this, "Info", "Please enter something to search.");
        return;
    }

    const int MAX_RESULTS = 100;
    QSharedPointer<Recipe>* results = new QSharedPointer<Recipe>[MAX_RESULTS];
    int resultCount = 0;
    int searchType = ui->search_combobox->currentIndex();

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
            sort(results, resultCount);
            display_search_home(results, resultCount);
        }
        else {
            QMessageBox::information(this, "No Results", "No matching recipes found.");
        }

        delete[] results; // Clean up memory

    }
    catch (...) {
        delete[] results; // Ensure memory cleanup on error
        QMessageBox::critical(this, "Error", "An unexpected error occurred.");
    }
}



// Display function (unchanged but needs proper implementation)
void MainWindow::display_search_home(QSharedPointer<Recipe>* recipes, int count) {
    // Clear previous results
    ui->stackedWidget->setCurrentWidget(ui->home_page);
    QLayoutItem* item;
    while ((item = recipes_grid->takeAt(0)) != nullptr) {
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

            QLabel* image = new QLabel;
            image->setPixmap(QPixmap(recipes[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
            image->setAlignment(Qt::AlignCenter);

            QPushButton* button = new QPushButton("View details");
            button->setStyleSheet("color:rgb(0,0,0);");
            connect(button, &QPushButton::clicked, this, [=]() {
                ui->stackedWidget->setCurrentWidget(ui->recipe_page);
                assign_recipe_page(recipes[i]);
                });


            layout->addWidget(title);
            layout->addWidget(image); // ADDED: Image label
            layout->addWidget(button);

            col = i % 4;
            if (!col && i) row++;
            recipes_grid->addWidget(widget, row, col);
        
    }
}

//void MainWindow::display_search_home(QSharedPointer<Recipe>& recipe_search,int& recipe_num)
//{
//    currentDisplayedRecipe = nullptr;
//    ui->stackedWidget->setCurrentWidget(ui->home_page);
//    int r = 0, c = 0;
//    for (int i = 0; i < recipe_num; i++)
//    {
//        QWidget* widget = new QWidget;
//
//        QVBoxLayout* layout = new QVBoxLayout(widget);
//
//        QLabel* title = new QLabel(recipe_search[i]->title);
//        title->setAlignment(Qt::AlignCenter);
//
//        QLabel* image = new QLabel;
//        image->setPixmap(QPixmap(recipe_search[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
//        image->setAlignment(Qt::AlignCenter);
//
//        QPushButton* button = new QPushButton("View details");
//        button->setStyleSheet("color:rgb(0,0,0);");
//        connect(button, &QPushButton::clicked, this, [=]() {
//            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
//            assign_recipe_page(recipe_search[i]);
//            });
//
//
//        layout->addWidget(title);
//        layout->addWidget(image); // ADDED: Image label
//        layout->addWidget(button);
//
//        c = i % 4;
//        if (!c && i) r++;
//        recipes_grid->addWidget(widget, r, c);
//    }
//}


void MainWindow::add_ingredient_row() {
    QWidget* row = new QWidget;
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setPlaceholderText(QStringLiteral("مكون"));
    edit->setStyleSheet("color:rgb(0,0,0);");
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    rm->setStyleSheet("color:rgb(0,0,0);");

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
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    rm->setStyleSheet("color:rgb(0,0,0);");

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
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    edit->setStyleSheet("color:rgb(0,0,0);");
    rm->setStyleSheet("color:rgb(0,0,0);");

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
    rm->setStyleSheet("color:rgb(0,0,0);");

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

void MainWindow::display_edition_page_user() {
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

    for (int i = 0; i < loged_in_user->my_recipes_num; i++) {
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

        QPushButton* button = new QPushButton("View Details");
        button->setStyleSheet("color: rgb(0, 0, 0);");
        QSharedPointer<Recipe> recipe_ptr = loged_in_user->my_recipes[i];
        connect(button, &QPushButton::clicked, this, [this, recipe_ptr]() {
            assign_recipe_page(recipe_ptr);
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            });

        QPushButton* button_delete = new QPushButton("حذف الوصفة");
        button_delete->setStyleSheet("color: rgb(0, 0, 0);");
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
         QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
        edit->setStyleSheet("color:rgb(0,0,0);");
        rm->setStyleSheet("color:rgb(0,0,0);");

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
        rm->setStyleSheet("color:rgb(0,0,0);");

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

    // Generate a new unique ID
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


    // Assign ingredients
    recipe_ptr->ing_num = num_ingredients;
    for (int i = 0; i < num_ingredients; ++i) {
        qDebug() << "ingredients " << i << recipe_ptr->ingredients[i] << "   ingredients_array  " << ingredients_array[i];
        recipe_ptr->ingredients[i] = ingredients_array[i];
    }

    // Assign steps
    recipe_ptr->steps_num = num_steps;
    for (int i = 0; i < num_steps; ++i) {
        qDebug() << "STEPS " << i << recipe_ptr->steps[i] << "   STEPS_array  " << steps_array[i];
        recipe_ptr->steps[i] = steps_array[i];
    }

    // Clean up dynamically allocated arrays
    delete[] ingredients_array;
    delete[] steps_array;


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
        //QLayout* container_layout = ui->ing_container_2->layout();
        //int count = container_layout->count();
        //out_count = count;
        //for (int i = 0; i < count; ++i) {
        //    QWidget* row = container_layout->itemAt(i)->widget();
        //    if (row) {
        //        QLayout* row_layout = row->layout();
        //        if (row_layout) {
        //            QLineEdit* edit = qobject_cast<QLineEdit*>(row_layout->itemAt(0)->widget());
        //            if (edit) {
        //                ingredients[i] = edit->text();
        //            }
        //        }
        //    }
        //}


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
        //for (int i = 0; i < count; ++i) {
        //    QWidget* row = container_layout->itemAt(i)->widget();
        //    if (row) {
        //        QLayout* row_layout = row->layout();
        //        if (row_layout) {
        //            QLineEdit* edit = qobject_cast<QLineEdit*>(row_layout->itemAt(0)->widget());
        //            if (edit) {
        //                steps[i] = edit->text();
        //            }
        //        }
        //    }
        //}
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


//void MainWindow::create_recipe_page(QSharedPointer<Recipe> r_ptr)
//{
//    if (recipe_pages[r_ptr->id] == nullptr)
//    {
//        QWidget* widget = new QWidget;
//        widget->setLayoutDirection(Qt::RightToLeft);
//
//        QFont f("Tahoma", 10);
//        widget->setFont(f);
//
//
//        QLabel* title = new QLabel(r_ptr->title);
//        QFont title_font("Tahoma", 16, QFont::Bold);
//        title->setFont(title_font);
//
//        QString categ = (r_ptr->category == 0) ? "حادق" : "حلو";
//        QLabel* category = new QLabel(categ);
//
//        QString l;
//        if (r_ptr->level == 0) l = "سهل" ;
//        else if (r_ptr->level == 1) l = "متوسط";
//        else l = "صعب";
//        QLabel* level = new QLabel(l);
//
//        QLabel* cock_time = new QLabel(QString("%1 دقيقه").arg(r_ptr->cock_time));
//
//        int num = (!r_ptr->rates_num) ? 0 : r_ptr->rates_sum / r_ptr->rates_num;
//        QLabel* rate = new QLabel(QString("%1 من 5").arg(num));
//
//        QHBoxLayout* infoBar = new QHBoxLayout();
//        infoBar->addWidget(category);
//        infoBar->addWidget(level);
//        infoBar->addWidget(cock_time);
//        infoBar->addWidget(rate);
//
//
//        QLabel* des_title = new QLabel("الوصف");
//        QFont des_title_font("Tahoma", 12, QFont::Bold);
//        des_title->setFont(des_title_font);
//        QLabel* description = new QLabel(r_ptr->description);
//        description->setWordWrap(true);
//
//
//        QLabel* ing_title = new QLabel("المكونات");
//        QFont ing_title_font("Tahoma", 12, QFont::Bold);
//        ing_title->setFont(ing_title_font);
//        QListWidget* ing = new QListWidget();
//        for (int i = 0; i < r_ptr->ing_num;i++)
//            ing->addItem(r_ptr->ingredients[i]);
//
//        QVBoxLayout* title_layout = new QVBoxLayout;
//        title_layout->addWidget(title);
//        title_layout->addLayout(infoBar);
//        title_layout->setSpacing(10);
//
//        QVBoxLayout* des_layout = new QVBoxLayout;
//        des_layout->addWidget(des_title);
//        des_layout->addWidget(description);
//        des_layout->setSpacing(5);
//
//        QVBoxLayout* ing_layout = new QVBoxLayout;
//        ing_layout->addWidget(ing_title);
//        ing_layout->addWidget(ing);
//        ing_layout->setSpacing(5);
//
//        QVBoxLayout* layout = new QVBoxLayout;
//        layout->addLayout(title_layout);
//        layout->addLayout(des_layout);
//        layout->addLayout(ing_layout);
//
//        layout->setSpacing(20);
//
//        widget->setLayout(layout);
//        ui->stackedWidget->addWidget(widget);
//        recipe_pages[r_ptr->id] = widget;
//    }
//    ui->stackedWidget->setCurrentWidget(recipe_pages[r_ptr->id]);
//}
//
void MainWindow::display_recipe(bool arrang)
{
    QLayoutItem* item;
    while ((item = recipes_grid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }


    int r = 0, c = 0;
    for (int i = arrang ? num_of_recipes - 1 : 0;
        arrang ? i >= 0 : i < num_of_recipes;
        i += arrang ? -1 : 1)
    {
        if (recipes[i].isNull())
            continue;

        QWidget* widget = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[i]->title);
        title->setAlignment(Qt::AlignCenter);

         QLabel *image = new QLabel;
         image->setPixmap(QPixmap(recipes[i]->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
         image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("View details");
        button->setStyleSheet("color:rgb(0,0,0);");
        connect(button, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            assign_recipe_page(recipes[i]);
            });


        layout->addWidget(title);
        layout->addWidget(image); // ADDED: Image label
        layout->addWidget(button);

        c = i % 4;
        if (!c && i) r++;
        recipes_grid->addWidget(widget, r, c);
    }
}


void MainWindow::on_submit_recipe_btn_clicked()
{


 /*   delete[] currentDisplayedRecipe->ingredients;
    delete[] currentDisplayedRecipe->steps;*/
    QSharedPointer<Recipe> recipe_ptr;

    
    // Check if we're editing an existing recipe or creating a new one
    if (currentDisplayedRecipe != nullptr) {
        // We're editing an existing recipe - use the existing pointer
        recipe_ptr = currentDisplayedRecipe;

        // Clean up existing arrays to prevent memory leaks
        //if (recipe_ptr->ingredients != nullptr) {
        //    delete[] recipe_ptr->ingredients;
        //    recipe_ptr->ingredients = nullptr;
        //}

        //if (recipe_ptr->steps != nullptr) {
        //    delete[] recipe_ptr->steps;
        //    recipe_ptr->steps = nullptr;
        //}
    }
    else {
        // We're creating a new recipe
        recipe_ptr = QSharedPointer<Recipe>(new Recipe());
        recipe_ptr->generate_id();
        recipes[num_of_recipes] = recipe_ptr;
        recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
        num_of_recipes++;
    }



    //recipe_ptr->generate_id();
    //recipes[num_of_recipes] = recipe_ptr;
    //recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
    //num_of_recipes++;


    // Get ingredients
    int num_ingredients = get_ingredient_count();
    QString* ingredients_array = get_ingredients(num_ingredients);

    // Get steps
    int num_steps =get_step_count();
    QString* steps_array = get_steps(num_steps);


    /***************************** Assign data from ui ************************/
    recipe_ptr->title = ui->title_field->text();
    recipe_ptr->title = recipe_ptr->title.trimmed();

    recipe_ptr->description = ui->desc_field->toPlainText();
    recipe_ptr->description = recipe_ptr->description.trimmed();
    recipe_ptr->category = ui->category_combobox_btn->currentIndex();
    recipe_ptr->level = ui->level_combobox_btn->currentIndex();

    recipe_ptr->cock_time = ui->time_spin_btn->value();
    recipe_ptr->imagePath = ui->image_path_line->text();

  

    // Assign ingredients
    recipe_ptr->ing_num = num_ingredients;
    for (int i = 0; i < num_ingredients; ++i) {
        recipe_ptr->ingredients[i] = ingredients_array[i];
    }

    // Assign steps
    recipe_ptr->steps_num = num_steps;
    for (int i = 0; i < num_steps; ++i) {
        recipe_ptr->steps[i] = steps_array[i];
    }

    // Clean up dynamically allocated arrays
    delete[] ingredients_array;
    delete[] steps_array;


    qDebug() << "Number of ingredients:" << get_ingredient_count();
    qDebug() << "Number of steps:" << get_step_count();
    qInfo() << "Recipe" << recipe_ptr->id << "added successfully!";


}


void MainWindow::assign_recipe_page(QSharedPointer<Recipe> r_ptr)
{
    // Clear existing items in ingredient and step lists
    ui->ingred_list->clear();
    ui->steps_list->clear();
    currentDisplayedRecipe = r_ptr;
    ui->img_perview->setPixmap(QPixmap(r_ptr->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
    ui->img_perview->setAlignment(Qt::AlignCenter);

    ui->desc_browser->setText(r_ptr->description);
    QString s = QString::number(r_ptr->cock_time,'f',2);
    ui->display_time_recipe->setText("وقت الطهو:  " + s);
    //QString avg_rate = QString::number((r_ptr->rates_sum / r_ptr->rates_num),'f',2);
    //ui->display_avg_rate->setText("متوسط التقييمات:  " + avg_rate);
    QString level;
        if (r_ptr->level == 0)  ui->display_level_recipe->setText("مستوى العصوبة:   سهل");
        else if (r_ptr->level == 1)  ui->display_level_recipe->setText("مستوى الصعوبة:   متوسط");
        else  ui->display_level_recipe->setText("مستوى الصعوبة:   صعب");
       

        //    QLabel* ing_title = new QLabel("المكونات");
        //QFont ing_title_font("Tahoma", 12, QFont::Bold);
        //ing_title->setFont(ing_title_font);
        for (int i = 0; i < r_ptr->ing_num;i++)
            ui->ingred_list->addItem(r_ptr->ingredients[i]);
        QVBoxLayout* ing_layout = new QVBoxLayout;
        //ing_layout->addWidget(ing_title);
        ing_layout->addWidget(ui->ingred_list);
        ing_layout->setSpacing(5);


        /*****STEPS******/
        for (int i = 0; i < r_ptr->steps_num;i++)
            ui->steps_list->addItem(r_ptr->steps[i]);
        QVBoxLayout* _layout = new QVBoxLayout;
        //ing_layout->addWidget(ing_title);
        ing_layout->addWidget(ui->steps_list);
        ing_layout->setSpacing(5);

}

void MainWindow::on_delete_recipe_btn_clicked() {
    // return to homepage or wherever you want
    ui->stackedWidget->setCurrentWidget(ui->home_page);

    // check if loged_in_user had the recipe in his favorite array
    // other users gets checked at log in and and when save_users gets excuted
    int& fav_num = loged_in_user->favorite_recipes_num;
    for (int j = 0; j < fav_num; j++)
    {
        // if the id refers to deleted recipe 
        // switch it with the last one and decrease favorite_recipes_num
        if (recipes_id_to_index[loged_in_user->favorite_recipes[j]] == -1)
        {
            int last_id = loged_in_user->favorite_recipes[fav_num - 1];
            loged_in_user->favorite_recipes[fav_num - 1] = 0;
            loged_in_user->favorite_recipes[j] = last_id;

            fav_num--;
        }
    }

    // now delete all pointers and the object will be deleted automatically for us
    recipes[recipes_id_to_index[currentDisplayedRecipe->id]] = nullptr;  // delete from recipes[]
    recipes_id_to_index[currentDisplayedRecipe->id] = -1;                // delete from recipes_id_to_index
    --num_of_recipes;
    currentDisplayedRecipe = nullptr;                                    // delete from current 
    display_recipe();
}



void MainWindow::assign_admin_page()
{

    if (currentDisplayedRecipe == nullptr)
    {
        qDebug() << "NULLL";
    }
    qDebug() << "IN ASSigned ADMIN PAGE";
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
        rm->setStyleSheet("color:rgb(0,0,0);");

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
        rm->setStyleSheet("color:rgb(0,0,0);");

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
        QMessageBox::warning(nullptr, "Error", "You've reached the maximum number of favorite recipes!");
        return;
    }

    // Check if recipe is already in favorites
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++) {
        if (loged_in_user->favorite_recipes[i] == currentDisplayedRecipe->id) {
            QMessageBox::information(nullptr, "Info", "Recipe is already in your favorites!");
            qInfo("Exist");
            return;
        }
        
    }
    qInfo("Clicked");

    // Add recipe to favorites
    loged_in_user->favorite_recipes[loged_in_user->favorite_recipes_num] = currentDisplayedRecipe->id;
    loged_in_user->favorite_recipes_num++;

    QMessageBox::information(nullptr, "Success", "Recipe added to favorites!");

}
    


void MainWindow::on_go_favorite_btn_clicked()
{
    // Check if there are favorite recipes
    if (loged_in_user->favorite_recipes_num == 0) {
        qDebug() << "No favorite recipes for user:" << loged_in_user->username;
        // Optionally show a message
        QMessageBox::information(this, "Info", "You have no favorite recipes.");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->favorite_page);
    display_favorite();
}

void MainWindow::display_favorite()
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

    qDebug() << "Favorite recipes count:" << loged_in_user->favorite_recipes_num;

    // Take ownership of the existing scroll area's widget if exists
    QWidget* scrollWidgetFavorite = ui->scrollArea_3->takeWidget();
    if (!scrollWidgetFavorite) {
        scrollWidgetFavorite = new QWidget();
    }

    // Create new grid layout on the widget
    delete favorite_grid; // Delete old layout if exists
    favorite_grid = new QGridLayout(scrollWidgetFavorite);

    int r = 0, c = 0;
    for (int i = 0; i < loged_in_user->favorite_recipes_num; i++)
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

        QLabel* image = new QLabel;
        QPixmap pixmap(recipes[recipe_index]->imagePath);
        if (!pixmap.isNull()) {
            image->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
        }
        else {
            image->setText("No image");
            qDebug() << "Failed to load image for recipe:" << recipes[recipe_index]->title;
        }
        image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("View details");
        button->setStyleSheet("color:rgb(0,0,0);");

        // Need to copy the pointer to avoid issues with lambdas
        QSharedPointer<Recipe> recipe_ptr = recipes[recipe_index];
        connect(button, &QPushButton::clicked, this, [this, recipe_ptr]() {
            assign_recipe_page(recipe_ptr);
            ui->stackedWidget->setCurrentWidget(ui->recipe_page);
            });

        QPushButton* button_delete = new QPushButton("حذف من المفضلة");
        button_delete->setStyleSheet("color:rgb(0,0,0);");

        // Pass the current index i
        int current_index = i;
        connect(button_delete, &QPushButton::clicked, this, [this, current_index]() {
            QMessageBox::information(this, "Info", "Has deleted from favorite");
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


//void MainWindow::delete_favorite_btn(int id_favorite)
//{
//
//
//    int& fav_num = loged_in_user->favorite_recipes_num;
//    loged_in_user->favorite_recipes[id_favorite] = -1;
//    for (int j = 0; j < fav_num; j++)
//    {
//        // if the id refers to deleted recipe 
//        // switch it with the last one and decrease favorite_recipes_num
//        if (recipes_id_to_index[loged_in_user->favorite_recipes[j]] == -1)
//        {
//            int last_id = loged_in_user->favorite_recipes[fav_num - 1];
//            loged_in_user->favorite_recipes[fav_num - 1] = 0;
//            loged_in_user->favorite_recipes[j] = last_id;
//
//            fav_num--;
//        }
//    }
//
//}

void MainWindow::on_edit_user_btn_clicked()
{
    if (loged_in_user->isAdmin == true) {

        ui->stackedWidget->setCurrentWidget(ui->admin_page);
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
    //ui->img_edit_preview->setPixmap(QPixmap(currentDisplayedRecipe->imagePath).scaled(150, 150, Qt::KeepAspectRatio));
    //ui->img_edit_preview->setAlignment(Qt::AlignCenter);

    //ui->desc_browser->setText(currentDisplayedRecipe->description);
    //QString s = QString::number(currentDisplayedRecipe->cock_time, 'f', 2);
    //ui->display_time_recipe->setText("وقت الطهو:  " + s);
    //QString avg_rate = QString::number((currentDisplayedRecipe->rates_sum / currentDisplayedRecipe->rates_num), 'f', 2);
    //ui->display_avg_rate_2->setText("متوسط التقييمات:  " + avg_rate);
    //QString level;
    //if (currentDisplayedRecipe->level == 0)  ui->display_level_recipe_2->setText("مستوى العصوبة:   سهل");
    //else if (currentDisplayedRecipe->level == 1)  ui->display_level_recipe_2->setText("مستوى الصعوبة:   متوسط");
    //else  ui->display_level_recipe_2->setText("مستوى الصعوبة:   صعب");

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



//void MainWindow::on_save_recipe_btn_clicked()
//{
//    // take input
//    QSharedPointer<Recipe> recipe_ptr(new Recipe());
//    recipe_ptr->generate_id();
//    recipes[num_of_recipes] = recipe_ptr;
//    recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
//    num_of_recipes++;
//
//    recipe_ptr->title = ui->title_line->text();
//    recipe_ptr->title = recipe_ptr->title.trimmed();
//
//    recipe_ptr->description = ui->description_text->toPlainText();
//    recipe_ptr->description = recipe_ptr->description.trimmed();
//
//    QString ingredients = ui->ingredients_text->toPlainText();
//    ingredients = ingredients.trimmed();
//    QStringList ing_list = ingredients.split(",");
//    recipe_ptr->ing_num = ing_list.size();
//    for (int i = 0; i < recipe_ptr->ing_num; i++)
//    {
//        recipe_ptr->ingredients[i] = ing_list[i].trimmed();
//    }
//
//
//    recipe_ptr->category = ui->category_combobox->currentIndex();
//    recipe_ptr->level = ui->level_combobox->currentIndex();
//
//    recipe_ptr->cock_time = ui->cock_time_spinbox->value();
//
//    qInfo() << "Recipe" << recipe_ptr->id << "added successfully!";
//}
//