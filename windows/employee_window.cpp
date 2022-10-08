
#include "employee_window.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_employee_window.h"
#include <QCryptographicHash>

EmployeeWindow::EmployeeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeWindow)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Изменить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

EmployeeWindow::~EmployeeWindow()
{
    delete ui;
}


void EmployeeWindow::setEmployee(std::shared_ptr<jp::Employee> employee)
{
    employee_ = employee;
    ui->lineFirstName->setText(QString::fromStdString(employee_->firstName));
    ui->lineSurname->setText(QString::fromStdString(employee_->surname));
    ui->linePatronymic->setText(QString::fromStdString(employee_->patronymic));
    ui->lineLogin->setText(QString::fromStdString(employee_->login));
    ui->comboActive->setCurrentIndex(employee_->isActive);
}


void EmployeeWindow::changeEmployee()
{
    if (!ui->lineFirstName->text().isEmpty()  && !ui->lineSurname->text().isEmpty() &&
        !ui->linePatronymic->text().isEmpty() && !ui->lineLogin->text().isEmpty()   &&
        (ui->linePassword->text().size() >= 6 || ui->linePassword->text() == ""))
    {
        employee_->firstName = ui->lineFirstName->text().toStdString();
        employee_->surname = ui->lineSurname->text().toStdString();
        employee_->patronymic = ui->linePatronymic->text().toStdString();
        employee_->login = ui->lineLogin->text().toStdString();
        if (ui->linePassword->text().toStdString() != "")
        {
            employee_->password = QString(QCryptographicHash::hash((ui->linePassword->text().toUtf8()),
                                          QCryptographicHash::Md5).toHex()).toStdString();
        }
        employee_->isActive = ui->comboActive->currentIndex();
        emit employeeChanged(employee_);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не все поля заполены или новый пароль меньше 6 символов");
    }
}


void EmployeeWindow::reject()
{
    employee_.reset();
    ui->lineFirstName->setText("");
    ui->lineSurname->setText("");
    ui->linePatronymic->setText("");
    ui->lineLogin->setText("");
    ui->linePassword->setText("");
    ui->comboActive->setCurrentIndex(-1);
    EmployeeWindow::hide();
}
