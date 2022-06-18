
#ifndef EMPLOYEE_WINDOW_H
#define EMPLOYEE_WINDOW_H

#include <QDialog>
#include "entities/models.h"

namespace Ui {
class EmployeeWindow;
}

class EmployeeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);
    ~EmployeeWindow();
    void setEmployee(std::shared_ptr<jp::Employee> employee);

public slots:
    void changeEmployee();
    void reject();

signals:
    void employeeChanged(std::shared_ptr<jp::Employee>);

private:
    Ui::EmployeeWindow *ui;
    std::shared_ptr<jp::Employee> employee_;
};

#endif
