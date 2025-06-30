#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);
	setup_mainwindow();
	on_home_page_btn_clicked();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setup_mainwindow()
{
	// UpperBar 
	if (user->isAdmin)
	{
		ui->my_favorites_page_btn->setVisible(false);
		ui->my_recipes_page_btn->setVisible(false);
	}

	// Homepage
	// Generate random recipes
	if (user->isAdmin) ui->random_recipes_container->setVisible(false);
	else
	{
		ui->random_recipes_container->setVisible(true);

		srand(static_cast<unsigned int>(time(0)));
		int* random_idxs = new int[3];
		for (int i = 0; i < 3; i++) random_idxs[i] = rand() % recipes_count;
		while (random_idxs[0] == random_idxs[1] || random_idxs[0] == random_idxs[2] || random_idxs[1] == random_idxs[2])
		{
			if (random_idxs[0] == random_idxs[1])
				random_idxs[1] = rand() % recipes_count;
			else if (random_idxs[0] == random_idxs[2] || random_idxs[1] == random_idxs[2])
				random_idxs[2] = rand() % recipes_count;
		}
		for(int i = 0; i < 3; i++) random_recipes[i] = recipes[random_idxs[i]];
		fill_grid(ui->random_scrollArea, random_recipes, 3);
	}

	// Connect tabs
	connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](bool) 
		{
            int tab = ui->tabWidget->currentIndex();

			temp_recipes_count = 0;

			if (tab == 0)
			{
				fill_grid(ui->all_recipes_scrollArea, recipes, recipes_count);
			}
			else if (tab == 1)
			{
				for (int i = 0; i < recipes_count; i++)
				{
					if (recipes[i]->category == 0)
						temp_recipes[temp_recipes_count++] = recipes[i];
				}
				fill_grid(ui->savory_scrollArea, temp_recipes, temp_recipes_count);
			}
			else if (tab == 2)
			{
				for (int i = 0; i < recipes_count; i++)
				{
					if (recipes[i]->category == 1)
						temp_recipes[temp_recipes_count++] = recipes[i];
				}
				fill_grid(ui->sweet_scrollArea, temp_recipes, temp_recipes_count);
			}
			else
			{
				for (int i = 0; i < recipes_count; i++)
				{
					if (recipes[i]->category == 2)
						temp_recipes[temp_recipes_count++] = recipes[i];
				}
				fill_grid(ui->drinks_scrollArea, temp_recipes, temp_recipes_count);
			}
		});

	// Recipe page
	// Ingredients
	connect(ui->add_ing_btn, &QPushButton::clicked, this, [this](bool) {add_ingredient_row();});

	// Steps
	connect(ui->add_step_btn, &QPushButton::clicked, this, [this](bool) {add_step_row();});

	// Image row
	connect(ui->browse_btn, &QPushButton::clicked, this, [this]() {
		QString file = QFileDialog::getOpenFileName(this, QString("اختر صورة"), QString(), QString("صور (*.png *.jpg *.jpeg)"));
		if (!file.isEmpty()) ui->image_path_line->setText(file);
		});
}

// Upper bar
void MainWindow::on_home_page_btn_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->home_page);

	if(ui->tabWidget->currentIndex() == 0)
		fill_grid(ui->all_recipes_scrollArea, recipes, recipes_count);
	else
		ui->tabWidget->setCurrentIndex(0);
}
void MainWindow::on_add_recipe_btn_clicked()
{
	clear_recipe_page();

	ui->buttons_menu->setVisible(false);
	ui->save_btn->setVisible(true);
	connect(ui->save_btn, &QPushButton::clicked, this, [&]() {
		add_recipe();
		on_add_recipe_btn_clicked();
		});

	add_ingredient_row();
	add_step_row();

	ui->stackedWidget->setCurrentWidget(ui->recipe_page);

}
void MainWindow::on_log_out_btn_clicked()
{
	user = nullptr;
	emit switchToDialog();
}

void MainWindow::on_my_favorites_page_btn_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->favorite_page);
	fill_grid(ui->favorite_scrollArea, user->favorites, user->favorites_count);
}
void MainWindow::on_my_recipes_page_btn_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->my_recipes_page);
	fill_grid(ui->my_recipes_scrollArea, user->my_recipes, user->my_recipes_count);
}


// recipe_page
void MainWindow::clear_recipe_page()
{
	// disconnect all buttons from old slots to avoid errors
	disconnect(ui->delete_btn, nullptr, nullptr, nullptr);
	disconnect(ui->edit_btn, nullptr, nullptr, nullptr);
	disconnect(ui->save_btn, nullptr, nullptr, nullptr);
	disconnect(ui->favorite_btn, nullptr, nullptr, nullptr);
	disconnect(ui->my_recipe_btn, nullptr, nullptr, nullptr);

	ui->title_line->clear();

	const QList<QPushButton*>& ing_btns = ui->ing_container->findChildren<QPushButton*>();
	for (auto btn : ing_btns) btn->click();

	const QList<QPushButton*>& steps_btns = ui->steps_container->findChildren<QPushButton*>();
	for (auto btn : steps_btns) btn->click();

	ui->des_edit->clear();

	ui->category_combobox->setCurrentIndex(0);
	ui->level_combobox->setCurrentIndex(0);
	ui->cock_time_spinbox->setValue(0);
}
void MainWindow::fill_page_from_recipe(QSharedPointer<Recipe> r_ptr)
{
	ui->title_line->setText(r_ptr->title);

	// fill ingredients
	for (int i = 0; i < r_ptr->ing_count; i++)
		add_ingredient_row(r_ptr->ingredients[i]);

	// fill steps
	for (int i = 0; i < r_ptr->steps_count; i++)
		add_step_row(r_ptr->steps[i]);

	ui->des_edit->setText(r_ptr->description);

	ui->category_combobox->setCurrentIndex(r_ptr->category);
	ui->level_combobox->setCurrentIndex(r_ptr->level);
	ui->cock_time_spinbox->setValue(r_ptr->cock_time);

	// image

	// go to page
	ui->stackedWidget->setCurrentWidget(ui->recipe_page);

}
void MainWindow::fill_recipe_from_page(QSharedPointer<Recipe> r_ptr)
{
	// add title
	r_ptr->title = ui->title_line->text().trimmed();

	// add description
	r_ptr->description = ui->des_edit->toPlainText().trimmed();

	// add ingredients
	const QList<QLineEdit*>& ingredients = ui->ing_container->findChildren<QLineEdit*>();
	r_ptr->ing_count = ingredients.size();
	for (int i = 0; i < r_ptr->ing_count; i++)
	{
		r_ptr->ingredients[i] = ingredients[i]->text().trimmed();
	}

	// add steps
	const QList<QLineEdit*>& steps = ui->steps_container->findChildren<QLineEdit*>();
	r_ptr->steps_count = steps.size();
	for (int i = 0; i < r_ptr->steps_count; i++)
	{
		r_ptr->steps[i] = steps[i]->text().trimmed();
	}

	// add info
	r_ptr->category = ui->category_combobox->currentIndex();
	r_ptr->level = ui->level_combobox->currentIndex();
	r_ptr->cock_time = ui->cock_time_spinbox->value();

	// add image
	r_ptr->imagePath = ui->image_path_line->text().trimmed();
}

void MainWindow::add_ingredient_row(QString ing)
{
	QWidget* row = new QWidget(ui->ing_container);
	QHBoxLayout* row_layout = new QHBoxLayout(row);
	row_layout->setSpacing(5);

	QLineEdit* edit = new QLineEdit;
	edit->setMinimumHeight(30);
	edit->setPlaceholderText("مكون");
	QPushButton* rm = new QPushButton("حذف");
	connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
	if (ing.isEmpty() == false)
	{
		edit->setText(ing);
	}

	row_layout->addWidget(edit);
	row_layout->addWidget(rm);
	ui->ing_container->layout()->addWidget(row);
}
void MainWindow::add_step_row(QString step)
{
	QWidget* row = new QWidget(ui->steps_container);
	QHBoxLayout* row_layout = new QHBoxLayout(row);
	row_layout->setSpacing(5);

	QLineEdit* edit = new QLineEdit;
	edit->setMinimumHeight(30);
	edit->setPlaceholderText("اكتب الخطوة");
	QPushButton* rm = new QPushButton("حذف");
	connect(rm, &QPushButton::clicked, this, &MainWindow::remove_row);
	if (step.isEmpty() == false)
	{
		edit->setText(step);
	}

	row_layout->addWidget(edit);
	row_layout->addWidget(rm);
	ui->steps_container->layout()->addWidget(row);
}
void MainWindow::remove_row()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (!btn) return;
	QWidget* row = btn->parentWidget();
	delete row;
}


// Functions
void MainWindow::fill_grid(QScrollArea* scroll_area, QSharedPointer<Recipe>* recipes_arr, int size)
{
	// Create a new empty widget and set it as the scroll_area widget
	// that automatically removes the old one with all of its content
	QWidget* new_widget = new QWidget(scroll_area);
	QGridLayout* container_layout = new QGridLayout(new_widget);
	scroll_area->setWidget(new_widget);

	// Check if user is allowed to edit
	bool admin = user->isAdmin;
	bool allowed_to_edit = (admin || (!admin && size && recipes_arr[0]->id == -1));

	// Fill the widget
	int max_in_row = 3;
	int row = 0, column = 0;
	for (int i = 0; i < size; i++)
	{
		QWidget* widget = new QWidget;
		widget->setMaximumWidth(350);
		widget->setMinimumHeight(170);
		QVBoxLayout* layout = new QVBoxLayout(widget);
		QLabel* title = new QLabel(recipes_arr[i]->title);
		title->setAlignment(Qt::AlignCenter);

		// QLabel *image = new QLabel;
		// image->setPixmap(QPixmap(imagePath).scaled(150, 150, Qt::KeepAspectRatio));
		// image->setAlignment(Qt::AlignCenter);

		QPushButton* display_btn = new QPushButton("عرض الوصفه");
		connect(display_btn, &QPushButton::clicked, this, [=]() {
			display_recipe(recipes_arr[i]);
			});


		QWidget* btn_widget = new QWidget;
		QHBoxLayout* btn_layout = new QHBoxLayout(btn_widget);
		btn_layout->addWidget(display_btn);

		if (allowed_to_edit)
		{
			QPushButton* delete_btn = new QPushButton("حذف");
			connect(delete_btn, &QPushButton::clicked, this, [=]() 
				{
					delete_recipe(recipes_arr[i]);

					if(user->isAdmin)
						on_home_page_btn_clicked();
					else
						on_my_recipes_page_btn_clicked();
				});

			QPushButton* edit_btn = new QPushButton("تعديل");
			connect(edit_btn, &QPushButton::clicked, this, [=]()
				{
					display_recipe(recipes_arr[i]);
					ui->edit_btn->click();
				});

			btn_layout->addWidget(delete_btn);
			btn_layout->addWidget(edit_btn);
		}
		// if a user opens his favorite recipes page
		else if(scroll_area->objectName() == "favorite_scrollArea")
		{
			QPushButton* delete_fav = new QPushButton("حذف");
			connect(delete_fav, &QPushButton::clicked, this, [=]()
				{
					display_recipe(recipes_arr[i]);
					ui->favorite_btn->click();
					on_my_favorites_page_btn_clicked();
				});

			btn_layout->addWidget(delete_fav);
		}


		layout->addWidget(title);
		layout->addWidget(btn_widget);

		// sets 3 widgets in a raw
		column = i % max_in_row;
		if (!column && i) row++;
		container_layout->addWidget(widget, row, column);
	}
}

void MainWindow::display_recipe(QSharedPointer<Recipe> r_ptr)
{
	clear_recipe_page();

	ui->buttons_menu->setVisible(true);
	ui->delete_btn->setVisible(true);
	ui->edit_btn->setVisible(true);
	ui->save_btn->setVisible(false);
	ui->my_recipe_btn->setVisible(false);
	ui->favorite_btn->setVisible(false);

	// allowed to edit
	bool admin = user->isAdmin;
	if (admin || (!admin && r_ptr->id == -1))
	{
		connect(ui->delete_btn, &QPushButton::clicked, this, [this, r_ptr]() 
			{
				delete_recipe(r_ptr);

				if (user->isAdmin) on_home_page_btn_clicked();
				else on_my_recipes_page_btn_clicked();
			});

		connect(ui->edit_btn, &QPushButton::clicked, this, [this, r_ptr]()
			{
				ui->buttons_menu->setVisible(false);
				ui->save_btn->setVisible(true);

				connect(ui->save_btn, &QPushButton::clicked, this, [this, r_ptr]() {
					QMessageBox::information(this, "Message", QString("تم تعديل " + r_ptr->title + " بنجاح"));
					fill_recipe_from_page(r_ptr);
					display_recipe(r_ptr);
					});
			});
	}
	else
	{
		ui->delete_btn->setVisible(false);
		ui->edit_btn->setVisible(false);

		ui->my_recipe_btn->setVisible(true);
		ui->favorite_btn->setVisible(true);

		connect_favorite_btn(r_ptr);
		connect_my_recipe_btn(r_ptr);
	}

	// fill recipe_page and open it
	fill_page_from_recipe(r_ptr);
}

void MainWindow::connect_favorite_btn(QSharedPointer<Recipe> r_ptr)
{
	disconnect(ui->favorite_btn, nullptr, nullptr, nullptr);

	int idx_in_fav = -1;
    for (int i = 0; i < user->favorites_count; i++)
    {
		if (user->favorites[i] == r_ptr)
		{
			idx_in_fav = i;
			break;
		}
    }

	// if not found in favorite, Add it.
	if (idx_in_fav == -1)
	{
		ui->favorite_btn->setText("اضف للمفضلة");
		connect(ui->favorite_btn, &QPushButton::clicked, this, [this, r_ptr]()
			{
				user->favorites[user->favorites_count] = r_ptr;
				user->favorites_count += 1;
				QMessageBox::information(this, "Message", QString("تم اضافة " + r_ptr->title + " للوصفات المفضله بنجاح"));
				display_recipe(r_ptr);
			});
	}
	// if found in favorite, Delete it.
	else
	{
		ui->favorite_btn->setText("احذف من المفضلة");
		connect(ui->favorite_btn, &QPushButton::clicked, this, [this, r_ptr, idx_in_fav]()
			{
				// Swipe recipes so that all recipes be adjacent:
				for (int i = idx_in_fav; i + 1 < user->favorites_count; i++)
					user->favorites[i] = user->favorites[i + 1];

				// delete last element and decrement count by 1
				user->favorites[--user->favorites_count] = nullptr;

				QMessageBox::information(this, "Message", QString("تم حذف " + r_ptr->title + " من الوصفات المفضله بنجاح"));
				display_recipe(r_ptr);
			});
	}
}
void MainWindow::connect_my_recipe_btn(QSharedPointer<Recipe> r_ptr)
{
	disconnect(ui->my_recipe_btn, nullptr, nullptr, nullptr);

	int idx = -1;
	for (int i = 0; i < user->my_recipes_count; i++)
	{
		if (user->my_recipes[i]->title == r_ptr->title)
		{
			idx = i;
			break;
		}
	}

	// if not found, Add it.
	if (idx == -1)
	{
		ui->my_recipe_btn->setText("اضف لوصفاتي");
		connect(ui->my_recipe_btn, &QPushButton::clicked, this, [this, r_ptr]()
			{
				add_recipe();
				display_recipe(r_ptr);
			});
	}
	// if found, Delete it.
	else
	{
		ui->my_recipe_btn->setText("احذف من وصفاتي");
		connect(ui->my_recipe_btn, &QPushButton::clicked, this, [this, r_ptr, idx]()
			{
				// Swipe recipes so that all recipes be adjacent:
				for (int i = idx; i + 1 < user->my_recipes_count; i++)
					user->my_recipes[i] = user->my_recipes[i + 1];

				// delete last element and decrement count by 1
				user->my_recipes[--user->my_recipes_count] = nullptr;

				QMessageBox::information(this, "Message", QString("تم حذف " + r_ptr->title + " من وصفاتي بنجاح"));
				display_recipe(r_ptr);
			});
	}
}

void MainWindow::add_recipe()
{
	// Create new recipe
	QSharedPointer<Recipe> r_ptr(new Recipe());

	if (user->isAdmin)
	{
		recipes[recipes_count] = r_ptr;
		r_ptr->generate_id();
		recipes_id_to_index[r_ptr->id] = recipes_count;
		recipes_count++;
	}
	else
	{
		r_ptr->id = -1;
		user->my_recipes[user->my_recipes_count] = r_ptr;
		user->my_recipes_count++;
	}

	// fill recipe
	fill_recipe_from_page(r_ptr);

	QMessageBox::information(this, "Message", QString("الوصفه " + r_ptr->title + " اضيفت بنجاح"));
	qInfo() << "Recipe" << r_ptr->id << ". " << r_ptr->title << "added successfully!";

	clear_recipe_page();
}
void MainWindow::delete_recipe(QSharedPointer<Recipe> r_ptr)
{
	// get count of all recipes, recipes_arr, and index of recipe to be deleted
	int* count = &recipes_count;
	QSharedPointer<Recipe>* recipes_arr = recipes;
	int idx = recipes_id_to_index[r_ptr->id];

	bool admin = user->isAdmin;
	if (not admin)
	{
		count = &user->my_recipes_count;
		recipes_arr = user->my_recipes;
		// find index
		for (int i = 0; i < *count; i++)
		{
			if (recipes_arr[i] == r_ptr)
			{
				idx = i;
				break;
			}
		}
	}

	// Swipe recipes so that all recipes be adjacent:
	for (int i = idx; i + 1 < *count; i++)
	{
		recipes_arr[i] = recipes_arr[i + 1];
		if (admin) recipes_id_to_index[recipes[i]->id] = i;
	}

	recipes_arr[*count - 1].reset();					 // drop the QSharedPointer
	if (admin) recipes_id_to_index[r_ptr->id] = -1;       // mark “gone”
	*count -= 1;

	QMessageBox::information(this, "Message", QString("تم حذف " + r_ptr->title + " بنجاح"));

	clear_recipe_page();
}

