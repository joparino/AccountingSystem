#include "authorization_window.h"
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
    emit authorizationTriggered(ui->lineLogin->text().toStdString(), ui->linePassword->text().toStdString());
}
