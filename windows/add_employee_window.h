
#ifndef ADD_EMPLOYEE_WINDOW_H
#define ADD_EMPLOYEE_WINDOW_H

#include <QDialog>
#include "entities/models.h"

namespace Ui {
class AddEmployeeWindow;
}

class AddEmployeeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployeeWindow(QWidget *parent = nullptr);
    ~AddEmployeeWindow();

public slots:
    void accept();
    void reject();

signals:
    void addEmployeeTriggered(std::shared_ptr<jp::Employee>);

private:
    Ui::AddEmployeeWindow *ui;
};

#endif // ADD_EMPLOYEE_WINDOW_H
