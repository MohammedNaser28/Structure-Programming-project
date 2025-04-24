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
