
#include "add_employee_window.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_add_employee_window.h"

AddEmployeeWindow::AddEmployeeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployeeWindow)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

AddEmployeeWindow::~AddEmployeeWindow()
{
    delete ui;
}


void AddEmployeeWindow::accept()
{
    if (!ui->lineFirstName->text().isEmpty() && !ui->lineSurname->text().isEmpty() && !ui->linePatronymic->text().isEmpty() &&
        !ui->lineLogin->text().isEmpty() && !ui->linePassword->text().isEmpty() && ui->comboRole->currentIndex() != -1)
    {
        jp::Employee employee;
        employee.firstName = ui->lineFirstName->text().toStdString();
        employee.surname = ui->lineSurname->text().toStdString();
        employee.patronymic = ui->linePatronymic->text().toStdString();
        employee.login = ui->lineLogin->text().toStdString();
        employee.password = ui->linePassword->text().toStdString();
        employee.role = ui->comboRole->currentIndex();
        employee.isActive = true;
        emit addEmployeeTriggered(std::make_shared<jp::Employee>(employee));
    }
    else
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не все поля заполнены");
    }
}


void AddEmployeeWindow::reject()
{
    ui->comboRole->setCurrentIndex(-1);
    ui->lineFirstName->setText("");
    ui->lineSurname->setText("");
    ui->linePatronymic->setText("");
    ui->lineLogin->setText("");
    ui->linePassword->setText("");
    AddEmployeeWindow::hide();
}
