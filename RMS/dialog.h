
#pragma once
#include <QDialog>
// #include "ui_dialog.h" // Will be deleted
#include "global.h"
#include "ui_register.h"
//QT_BEGIN_NAMESPACE
namespace Ui { class Register; };
//QT_END_NAMESPACE

class Register : public QDialog
{
	Q_OBJECT

public:
	Register(QWidget *parent = nullptr);
	~Register();

private:
	Ui::Register *ui;

private slots:

	void on_login_btn_clicked();
	void on_switch_to_create_clicked();
	void on_create_btn_clicked();
	void on_switch_to_login_clicked();

signals:
	void switchToMainWindow();
};


#pragma once

#include <QDialog>
#include "ui_dialog.h"
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DialogClass; };
QT_END_NAMESPACE

class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(QWidget *parent = nullptr);
	~Dialog();

private:
	Ui::DialogClass *ui;

private slots:
	void on_log_in_btn_clicked();
	void on_register_page_btn_clicked();
	void on_register_btn_clicked();

signals:
	void switchToMainWindow();
};

