#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
    , recipes_grid(new QGridLayout)
{
	ui->setupUi(this);
    recipe_pages.fill(nullptr, 1000);
    this->setup_mainwindow_page();
    this->on_home_page_btn_clicked();
}

MainWindow::~MainWindow()
{
	delete ui;
    qDeleteAll(recipe_pages);
    recipe_pages.clear();
}

void MainWindow::setup_mainwindow_page()
{
    // Homepage 
    ui->scrollAreaWidgetContents->setLayout(recipes_grid);
    ui->scrollArea->setWidgetResizable(true);

    // Add Recipe page
    // Scrollable central area
    ui->scroll_area->setLayoutDirection(Qt::RightToLeft);
    ui->scroll_content->setLayoutDirection(Qt::RightToLeft);
    ui->scroll_content->layout()->setSpacing(12);
    ui->scroll_area->setWidgetResizable(true);

    // Ingredients
    ui->ing_container->setLayoutDirection(Qt::LeftToRight);
    connect(ui->add_ing_btn, &QPushButton::clicked, this, [&]() {
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setMinimumHeight(30);
        edit->setPlaceholderText(QStringLiteral("مكون"));
        QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        ui->ing_container->layout()->addWidget(row);
        });
    ui->add_ing_btn->click();

    // Method
    ui->methods_container->setLayoutDirection(Qt::LeftToRight);
    connect(ui->add_method_btn, &QPushButton::clicked, this, [&]() {
        QWidget* row = new QWidget;
        row->setLayoutDirection(Qt::RightToLeft);
        QHBoxLayout* row_layout = new QHBoxLayout(row);
        row_layout->setSpacing(5);
        row_layout->setDirection(QBoxLayout::LeftToRight);
        QLineEdit* edit = new QLineEdit;
        edit->setMinimumHeight(30);
        edit->setPlaceholderText(QStringLiteral("اكتب الخطوة"));
        QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
        connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
        row_layout->addWidget(edit);
        row_layout->addWidget(rm);
        QLayout* method_layout = ui->methods_container->layout();
        method_layout->addWidget(row);
        });
    ui->add_method_btn->click();

    // Info row
    ui->info_layout->setSpacing(10);
    ui->info_layout->setDirection(QBoxLayout::RightToLeft);

    // Image row
    ui->img_layout->setDirection(QBoxLayout::RightToLeft);
    connect(ui->browse_btn, &QPushButton::clicked, this, [&]() {
        QString file = QFileDialog::getOpenFileName(this, QString("اختر صورة"), QString(), QString("صور (*.png *.jpg *.jpeg)"));
        if (!file.isEmpty()) ui->image_path_line->setText(file);
        }); 
}

void MainWindow::on_home_page_btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->home_page);
    this->on_display_recipe_btn_clicked();
}

void MainWindow::on_add_recipe_page_btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->add_recipe_page);
}

void MainWindow::remove_row()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QWidget* row = btn->parentWidget();
    delete row;
}

void MainWindow::on_log_out_btn_clicked()
{
    loged_in_user = nullptr;
    emit switchToDialog();
}

void MainWindow::on_save_recipe_btn_clicked()
{
    // Create object
    QSharedPointer<Recipe> recipe_ptr(new Recipe());
    recipe_ptr->generate_id();
    recipes[num_of_recipes] = recipe_ptr;
    recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
    num_of_recipes++;

    // add title
    recipe_ptr->title = ui->title_line->text();
    recipe_ptr->title = recipe_ptr->title.trimmed();

    // add description
    recipe_ptr->description = ui->des_edit->toPlainText();
    recipe_ptr->description = recipe_ptr->description.trimmed();
    
    // add ingredients
    const QList<QLineEdit*> &ingredients = ui->ing_container->findChildren<QLineEdit*>();
    recipe_ptr->ing_num = ingredients.size();
    for (int i = 1; i < recipe_ptr->ing_num; i++)
    {
        recipe_ptr->ingredients[i] = ingredients[i]->text().trimmed();
    }

    // add steps
    const QList<QLineEdit*>& steps = ui->methods_container->findChildren<QLineEdit*>();
    recipe_ptr->steps_num = steps.size();
    for (int i = 1; i < recipe_ptr->steps_num; i++)
    {
         recipe_ptr->steps[i] = steps[i]->text().trimmed(); 
    }

    // add info
    recipe_ptr->category = ui->category_combobox->currentIndex();
    recipe_ptr->level = ui->level_combobox->currentIndex();
    recipe_ptr->cock_time = ui->cock_time_spinbox->value();

    qInfo() << "Recipe" << recipe_ptr->id << "added successfully!";

    // return to home page
    this->on_home_page_btn_clicked();
}

void MainWindow::create_recipe_page(QSharedPointer<Recipe> r_ptr)
{
    if (recipe_pages[r_ptr->id] == nullptr)
    {
        QWidget* widget = new QWidget;
        widget->setLayoutDirection(Qt::RightToLeft);

        QFont f("Tahoma", 10);
        widget->setFont(f);


        QLabel* title = new QLabel(r_ptr->title);
        QFont title_font("Tahoma", 16, QFont::Bold);
        title->setFont(title_font);

        QString categ = (r_ptr->category == 0) ? "حادق" : "حلو";
        QLabel* category = new QLabel(categ);

        QString l;
        if (r_ptr->level == 0) l = "سهل";
        else if (r_ptr->level == 1) l = "متوسط";
        else l = "صعب";
        QLabel* level = new QLabel(l);

        QLabel* cock_time = new QLabel(QString("%1 دقيقه").arg(r_ptr->cock_time));

        int num = (!r_ptr->rates_num) ? 0 : r_ptr->rates_sum / r_ptr->rates_num;
        QLabel* rate = new QLabel(QString("%1 من 5").arg(num));

        QHBoxLayout* infoBar = new QHBoxLayout();
        infoBar->addWidget(category);
        infoBar->addWidget(level);
        infoBar->addWidget(cock_time);
        infoBar->addWidget(rate);


        QLabel* des_title = new QLabel("الوصف");
        QFont des_title_font("Tahoma", 12, QFont::Bold);
        des_title->setFont(des_title_font);
        QLabel* description = new QLabel(r_ptr->description);
        description->setWordWrap(true);


        QLabel* ing_title = new QLabel("المكونات");
        QFont ing_title_font("Tahoma", 12, QFont::Bold);
        ing_title->setFont(ing_title_font);
        QListWidget* ing = new QListWidget();
        for (int i = 0; i < r_ptr->ing_num;i++)
            ing->addItem(r_ptr->ingredients[i]);

        QVBoxLayout* title_layout = new QVBoxLayout;
        title_layout->addWidget(title);
        title_layout->addLayout(infoBar);
        title_layout->setSpacing(10);

        QVBoxLayout* des_layout = new QVBoxLayout;
        des_layout->addWidget(des_title);
        des_layout->addWidget(description);
        des_layout->setSpacing(5);

        QVBoxLayout* ing_layout = new QVBoxLayout;
        ing_layout->addWidget(ing_title);
        ing_layout->addWidget(ing);
        ing_layout->setSpacing(5);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addLayout(title_layout);
        layout->addLayout(des_layout);
        layout->addLayout(ing_layout);

        layout->setSpacing(20);

        widget->setLayout(layout);
        ui->stackedWidget->addWidget(widget);
        recipe_pages[r_ptr->id] = widget;
    }
    ui->stackedWidget->setCurrentWidget(recipe_pages[r_ptr->id]);
}

void MainWindow::on_display_recipe_btn_clicked()
{
    int r = 0, c = 0;
    for (int i = 0; i < num_of_recipes; i++)
    {
        QWidget* widget = new QWidget;

        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* title = new QLabel(recipes[i]->title);
        title->setAlignment(Qt::AlignCenter);

        // QLabel *image = new QLabel;
        // image->setPixmap(QPixmap(imagePath).scaled(150, 150, Qt::KeepAspectRatio));
        // image->setAlignment(Qt::AlignCenter);

        QPushButton* button = new QPushButton("View details");
        connect(button, &QPushButton::clicked, this, [=]() {
            create_recipe_page(recipes[i]);
            });

        layout->addWidget(title);
        layout->addWidget(button);

        c = i % 4;
        if (!c && i) r++;
        recipes_grid->addWidget(widget, r, c);
    }
}


//void MainWindow::on_delete_btn_clicked()
//{
//    // return go to homepage or wherever you want
//    ui->stackedWidget->setCurrentWidget(ui->home_page);
//
//    // check if loged_in_user had the recipe in his favorite array
//    // other users gets checked at log in and and when save_users gets excuted
//    int &fav_num = loged_in_user->favorite_recipes_num;
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
//    // now delete all pointers and it will automatically delete the objet for us 
//    recipes[recipes_id_to_index[current_recipe->id]] = nullptr;  // delete from recipes[]
//    recipes_id_to_index[current_recipe->id] = -1;                // delete from recipes_id_to_index
//    current_recipe = nullptr;                                    // delete from current 
//}