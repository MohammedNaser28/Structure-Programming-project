#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
    , recipes_grid(new QGridLayout)
{
	ui->setupUi(this);
    this->setup_mainwindow_page();
    this->on_home_page_btn_clicked();
}

MainWindow::~MainWindow()
{
	delete ui;
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
    ui->add_ing_layout->setDirection(QBoxLayout::RightToLeft);
    ui->ing_container->setLayoutDirection(Qt::LeftToRight);
    connect(ui->add_ing_btn, &QPushButton::clicked, this, [this](bool) {
        add_ingredient_row();
        });

    add_ingredient_row();

    // Method
    ui->add_step_layout->setDirection(QBoxLayout::RightToLeft);
    ui->steps_container->setLayoutDirection(Qt::LeftToRight);
    connect(ui->add_step_btn, &QPushButton::clicked, this, [this](bool) {
        add_step_row();
        });
    
    add_step_row();

    // Info row
    ui->info_layout->setSpacing(10);
    ui->info_layout->setDirection(QBoxLayout::RightToLeft);

    // Image row
    ui->img_layout->setDirection(QBoxLayout::RightToLeft);
    connect(ui->browse_btn, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, QString("اختر صورة"), QString(), QString("صور (*.png *.jpg *.jpeg)"));
        if (!file.isEmpty()) ui->image_path_line->setText(file);
        }); 
}
void MainWindow::add_ingredient_row(QString ing)
{
    QWidget* row = new QWidget(ui->ing_container);
    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setMinimumHeight(30);
    edit->setPlaceholderText(QStringLiteral("مكون"));
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    if (!ing.isEmpty())
    {
        edit->setText(ing);
        edit->setEnabled(false);
        rm->setVisible(false);
    }
    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    ui->ing_container->layout()->addWidget(row);
}
void MainWindow::add_step_row(QString step)
{
    QWidget* row = new QWidget(ui->steps_container);

    row->setLayoutDirection(Qt::RightToLeft);
    QHBoxLayout* row_layout = new QHBoxLayout(row);
    row_layout->setSpacing(5);
    row_layout->setDirection(QBoxLayout::LeftToRight);
    QLineEdit* edit = new QLineEdit;
    edit->setMinimumHeight(30);
    edit->setPlaceholderText(QStringLiteral("اكتب الخطوة"));
    QPushButton* rm = new QPushButton(QStringLiteral("حذف"));
    if (!step.isEmpty())
    {
        edit->setText(step);
        edit->setEnabled(false);
        rm->setVisible(false);
    }
    connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
    row_layout->addWidget(edit);
    row_layout->addWidget(rm);
    QLayout* step_layout = ui->steps_container->layout();
    step_layout->addWidget(row);
}
void MainWindow::remove_row()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QWidget* row = btn->parentWidget();
    delete row;
}

void MainWindow::on_home_page_btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->home_page);
    this->display_all_recipes();
}

void MainWindow::on_add_recipe_page_btn_clicked()
{
    ui->title_line->setEnabled(true);
    ui->title_line->clear();

    ui->delete_btn->setVisible(false);
    ui->edit_btn->setVisible(false);

    const QList<QPushButton*>& ing_btns = ui->ing_container->findChildren<QPushButton*>();
    for (auto btn : ing_btns)
    {
        btn->click();
    }
    add_ingredient_row();

    const QList<QPushButton*>& steps_btns = ui->steps_container->findChildren<QPushButton*>();
    for (auto btn : steps_btns)
    {
       btn->click();
    }
    add_step_row();

    ui->des_edit->setEnabled(true);
    ui->des_edit->clear();

    ui->category_combobox->setCurrentIndex(0);
    ui->level_combobox->setCurrentIndex(0);
    ui->cock_time_spinbox->setValue(0);

    ui->img_widget->setVisible(true);
    ui->save_recipe_btn->setVisible(true);

    ui->stackedWidget->setCurrentWidget(ui->recipe_page);
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
    for (int i = 0; i < recipe_ptr->ing_num; i++)
    {
        recipe_ptr->ingredients[i] = ingredients[i]->text().trimmed();
    }

    // add steps
    const QList<QLineEdit*>& steps = ui->steps_container->findChildren<QLineEdit*>();
    recipe_ptr->steps_num = steps.size();
    for (int i = 0; i < recipe_ptr->steps_num; i++)
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

void MainWindow::display_recipe(QSharedPointer<Recipe> r_ptr)
{
    ui->title_line->setEnabled(false);
    ui->title_line->setText(r_ptr->title);

    ui->delete_btn->setVisible(true);
    ui->edit_btn->setVisible(true);

    // delete all
    const QList<QWidget*>& ingredients = ui->ing_container->findChildren<QWidget*>();
    for (int i = 0; i < ingredients.size(); i++)
    {
        ingredients[i]->deleteLater();
    }

    // set ingredients
    for (int i = 0; i < r_ptr->ing_num; i++)
    {
        add_ingredient_row(r_ptr->ingredients[i]);
    }

    // delete all
    const QList<QWidget*>& steps = ui->steps_container->findChildren<QWidget*>();
    for (int i = 0; i < steps.size(); i++)
    {
        steps[i]->deleteLater();
    }

    // set steps
    for (int i = 0; i < r_ptr->steps_num; i++)
    {
        add_step_row(r_ptr->steps[i]);
    }

    ui->des_edit->setEnabled(false);
    ui->des_edit->setText(r_ptr->description);

    ui->category_combobox->setCurrentIndex(r_ptr->category);
    ui->level_combobox->setCurrentIndex(r_ptr->level);
    ui->cock_time_spinbox->setValue(r_ptr->cock_time);

    ui->img_widget->setVisible(false);
    ui->save_recipe_btn->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->recipe_page);
}

void MainWindow::display_all_recipes()
{
    QLayoutItem* item;
    while ((item = recipes_grid->takeAt(0)) != nullptr)
    {
        if (QWidget* w = item->widget()) w->deleteLater();
        delete item;
    }

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

        QPushButton* display = new QPushButton("عرض الوصفه");
        connect(display, &QPushButton::clicked, this, [=]() {
            display_recipe(recipes[i]);
            });

        QPushButton* del = new QPushButton("حذف");
        connect(del, &QPushButton::clicked, this, [=]() {

            //recipes_grid->removeWidget(widget);
            //widget->deleteLater();
            delete_recipe(recipes[i]);
           
            });

        QWidget* btn_widget = new QWidget;
        QHBoxLayout* btn_layout = new QHBoxLayout(btn_widget);
        btn_layout->addWidget(display);
        btn_layout->addWidget(del);

        layout->addWidget(title);
        layout->addWidget(btn_widget);

        c = i % 4;
        if (!c && i) r++;
        recipes_grid->addWidget(widget, r, c);
    }
}


void MainWindow::delete_recipe(QSharedPointer<Recipe> r_ptr)
{

    // check if loged_in_user had the recipe in his favorite array   (won't need it since only admin can delete)
    // other users gets checked at log in and and when save_users gets excuted
    //int &fav_num = loged_in_user->favorite_recipes_num;
    //for (int j = 0; j < fav_num; j++)
    //{
    //    // if the id refers to deleted recipe 
    //    // switch it with the last one and decrease favorite_recipes_num
    //    if (recipes_id_to_index[loged_in_user->favorite_recipes[j]] == -1)
    //    {
    //        int last_id = loged_in_user->favorite_recipes[fav_num - 1];
    //        loged_in_user->favorite_recipes[fav_num - 1] = 0;
    //        loged_in_user->favorite_recipes[j] = last_id;

    //        fav_num--;
    //    }
    //}

    // now delete all pointers to current recipe and it will automatically delete the objet for us 
    short idx = recipes_id_to_index[r_ptr->id];

    // make all recipes adjacent:
    for (int i = idx; i + 1 < num_of_recipes; ++i) {
        recipes[i] = recipes[i + 1];
        recipes_id_to_index[recipes[i]->id] = i;
    }

    recipes[num_of_recipes - 1].clear();           // drop the QSharedPointer
    recipes_id_to_index[r_ptr->id] = -1;           // mark “gone”
    --num_of_recipes;

    on_home_page_btn_clicked();
}