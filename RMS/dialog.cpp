#include "dialog.h"

Register::Register(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_login_btn_clicked()
{
    ui->warning_label->clear();
    // take data
    QString username = ui->username_field->text();
    QString password = ui->pass_field->text();
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
    for (int i = 0; i < num_of_users; i++)
    {
        if (users[i]->username == username && users[i]->password == password)
        {
            loged_in_user = users[i];
            qInfo() << username << " loged in succesfully!";
            emit switchToMainWindow();
            break;
        }
    }


    // if user was not found
    if (loged_in_user.isNull())
        QMessageBox::information(this, "Message", "User was not found please register first.");

    ui->username_field->clear();
    ui->pass_field->clear();
}

void Register::on_create_btn_clicked()
{
    ui->warning_label->clear();

    // take input
    QString username = ui->username_create->text();
    QString name = ui->name_create_field->text();
    QString password = ui->pass_field_create->text();
    QString confirm_password = ui->confirm_pass_create->text();
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
    for (int i = 0; i < num_of_users; i++)
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
        ui->warning_label->setText("Passwords are not isdentical");
        return;
    }

    bool isAdmin;
    if (ui->is_admin_btn->isChecked()) isAdmin = true;
    else if (ui->is_user_btn->isChecked()) isAdmin = false;
    else
    {
        qCritical() << "Select User or Admin";
        ui->warning_label->setText("Select User or Admin");
        return;
    }

    // create new user and append it to the users list
    QSharedPointer<User> user_ptr(new User());
    user_ptr->isAdmin = isAdmin;

    user_ptr->username = username;
    user_ptr->name = name;
    user_ptr->password = password;
    users[num_of_users] = user_ptr;
    num_of_users++;
    loged_in_user = user_ptr;

    // switch to main window and clear register page and switch to log in
    emit switchToMainWindow();
    //ui->title_label->setText("Log In");
    //ui->username_line_2->clear();
    //ui->pass_line_2->clear();
    //ui->confirm_pass_line->clear();
 /*   ui->admin_btn->setChecked(false);
    ui->user_btn->setChecked(false);*/
    ui->stackedWidget->setCurrentWidget(ui->login_container);
}

void Register::on_switch_to_login_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->login_container);
}




void Register::on_switch_to_create_clicked()
{
    //ui->warning_label->setText("");
    ui->stackedWidget->setCurrentWidget(ui->create_container);
}