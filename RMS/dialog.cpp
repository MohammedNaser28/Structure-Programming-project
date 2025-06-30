#include "dialog.h"

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DialogClass())
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::on_log_in_btn_clicked()
{
    ui->warning_label->clear();

    // take data
    QString username = ui->username_line->text();
    QString password = ui->pass_line->text();
    username = username.trimmed();
    password = password.trimmed();

    // make sure user entered data
    if (username.size() == 0 || password.size() == 0)
    {
        qWarning() << "Enter a Username and a Password";
        ui->warning_label->setText("Enter a Username and a Password");
        return;
    }

    if (username.contains(" ") || password.contains(" "))
    {
        qWarning() << "Username and Password can't contain whitespace";
        ui->warning_label->setText("Username and Password can't contain whitespace");
        return;
    }

    // search for user
    for (int i = 0; i < users_count; i++)
    {
        if (users[i]->username == username && users[i]->password == password)
        {
            user = users[i];
            qInfo() << username << " loged in succesfully!";

            // Check if there ware deleted recipes in favorite
            // in case recipes deleted in previous sessions(log out without closing the app)
            // were in the favorite array of the current user
            
            int &fav_count = user->favorites_count;
            for (int l = 0, r = fav_count - 1; l < r; l++)
            {
                while (l < r && recipes_id_to_index[user->favorites[r]->id] == -1)
                    r--;

                if (recipes_id_to_index[user->favorites[l]->id] == -1)
                {
					std::swap(user->favorites[l], user->favorites[r]);
                    r--;
                }
            }
            int deleted = 0;
            for (int i = fav_count-1; i > 0; i--)
            {
                if (recipes_id_to_index[user->favorites[i]->id] != -1) break;
                else deleted++;
            }
            fav_count -= deleted;

            emit switchToMainWindow();
            break;
        }
    }

    // if user was not found
    if (user.isNull())
        QMessageBox::information(this, "Message", "User was not found please register first.");

    ui->username_line->clear();
    ui->pass_line->clear();
}

void Dialog::on_register_btn_clicked()
{
    ui->warning_label->clear();

    // take input
    QString username = ui->username_line_2->text();
    QString password = ui->pass_line_2->text();
    QString confirm_password = ui->confirm_pass_line->text();
    username = username.trimmed();
    password = password.trimmed();
    confirm_password = confirm_password.trimmed();

    // make sure user entered data
    if (!username.size() || !password.size() || !confirm_password.size())
    {
        qWarning() << "Enter a Username and a Password";
        ui->warning_label->setText("Enter a Username and a Password");
        return;
    }

    if (username.contains(" ") || password.contains(" "))
    {
        qWarning() << "Username and Password can't contain whitespace";
        ui->warning_label->setText("Username and Password can't contain whitespace");
        return;
    }

    // make sure user entered a unique username
    for (int i = 0; i < users_count; i++)
    {
        if (username == users[i]->username)
        {
            qCritical() << "This username already exists";
            ui->warning_label->setText("This username already exists");
            return;
        }
    }

    if (confirm_password != password)
    {
        qCritical() << "Passwords are not identical";
        ui->warning_label->setText("Passwords are not identical");
        return;
    }

    bool isAdmin;
    if (ui->admin_btn->isChecked()) isAdmin = true;
    else if (ui->user_btn->isChecked()) isAdmin = false;
    else
    {
        qCritical() << "Select User or Admin";
        ui->warning_label->setText("Select User or Admin");
        return;
    }

    // create new user and append it to the users list
    QSharedPointer<User> user_ptr(new User());
    user = user_ptr;

    user_ptr->isAdmin = isAdmin;
    user_ptr->username = username;
    user_ptr->password = password;
    users[users_count] = user_ptr;
    users_count++;

    // switch to main window and clear register page and switch to log in
    emit switchToMainWindow();
    ui->title_label->setText("Log In");
    ui->username_line_2->clear();
    ui->pass_line_2->clear();
    ui->confirm_pass_line->clear();
    ui->admin_btn->setChecked(false);
    ui->user_btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->log_in_page);
}

void Dialog::on_register_page_btn_clicked()
{
    ui->title_label->setText("Register");
    ui->warning_label->setText("");
    ui->stackedWidget->setCurrentWidget(ui->register_page);
}