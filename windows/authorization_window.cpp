#include "authorization_window.h"
#include "qmessagebox.h"
#include "ui_authorization_window.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}

void AuthorizationWindow::authorization()
{
    if (!ui->lineLogin->text().isEmpty() && !ui->linePassword->text().isEmpty())
    {
        emit authorizationTriggered(ui->lineLogin->text().toStdString(), ui->linePassword->text().toStdString());
    }
    else
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не все поля заполнены");
    }
}


void AuthorizationWindow::hide()
{
   ui->lineLogin->setText("");
   ui->linePassword->setText("");
   AuthorizationWindow::setVisible(false);
}


void AuthorizationWindow::reject()
{
    AuthorizationWindow::close();
    qApp->quit();
}
