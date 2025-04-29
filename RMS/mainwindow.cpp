#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui (new Ui::MainWindowClass()) 
    , recipes_grid(new QGridLayout)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(recipes_grid);
    ui->scrollArea->setWidgetResizable(true);

    recipe_pages.fill(nullptr, 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    qDeleteAll(recipe_pages);
    recipe_pages.clear();
}

void MainWindow::on_log_out_btn_clicked()
{
    loged_in_user = nullptr;
    emit switchToDialog();
}

void MainWindow::on_save_recipe_btn_clicked()
{
    // take input
    QSharedPointer<Recipe> recipe_ptr(new Recipe());
    recipe_ptr->generate_id();
    recipes[num_of_recipes] = recipe_ptr;
    recipes_id_to_index[recipe_ptr->id] = num_of_recipes;
    num_of_recipes++;

    recipe_ptr->title = ui->title_line->text();
    recipe_ptr->title = recipe_ptr->title.trimmed();

    recipe_ptr->description = ui->description_text->toPlainText();
    recipe_ptr->description = recipe_ptr->description.trimmed();

    QString ingredients = ui->ingredients_text->toPlainText();
    ingredients = ingredients.trimmed();
    QStringList ing_list = ingredients.split(",");
    recipe_ptr->ing_num = ing_list.size();
    for (int i = 0; i < recipe_ptr->ing_num; i++)
    {
        recipe_ptr->ingredients[i] = ing_list[i].trimmed();
    }


    recipe_ptr->category = ui->category_combobox->currentIndex();
    recipe_ptr->level = ui->level_combobox->currentIndex();

    recipe_ptr->cock_time = ui->cock_time_spinbox->value();

    qInfo() << "Recipe" << recipe_ptr->id << "added successfully!";
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
        if (r_ptr->level == 0) l = "سهل" ;
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

void MainWindow::on_home_page_btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->home_page);
}

