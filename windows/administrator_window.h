
#ifndef ADMINISTRATOR_WINDOW_H
#define ADMINISTRATOR_WINDOW_H

#include <QMainWindow>
#include "add_employee_window.h"
#include "entities/models.h"
#include "qtablewidget.h"
#include "windows/employee_window.h"

namespace Ui {
class AdministratorWindow;
}

class AdministratorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdministratorWindow(AddEmployeeWindow& aew, EmployeeWindow& cew, QWidget *parent = nullptr);
    ~AdministratorWindow();
    void addRowEmployee(std::shared_ptr<jp::Employee> employee) noexcept;
    void addRowPublisher(std::shared_ptr<jp::Publisher> publisher) noexcept;
    void addRowAuthor(std::shared_ptr<jp::Author> author) noexcept;
    void addRowGenre(std::shared_ptr<jp::Genre> genre) noexcept;
    void clearEmployee();
    void clearPropertyBook();

public slots:
    void logout();
    void goToAddEmployee();
    void goToChangeEmployee(QTableWidgetItem* item);
    void addRowPublisher();
    void addRowAuthor();
    void addRowGenre();
    void addPublisher(QTableWidgetItem* item);
    void addAuthor(QTableWidgetItem* item);
    void addGenre(QTableWidgetItem* item);

signals:
    void logoutTriggered();
    void addPublisherTriggered(std::shared_ptr<jp::Publisher> publisher);
    void addAuthorTriggered(std::shared_ptr<jp::Author> author);
    void addGenreTriggered(std::shared_ptr<jp::Genre> genre);
    void changePublisherTriggered(std::shared_ptr<jp::Publisher> publisher);
    void changeAuthorTriggered(std::shared_ptr<jp::Author> author);
    void changeGenreTriggered(std::shared_ptr<jp::Genre> genre);


private:
    Ui::AdministratorWindow *ui;
    AddEmployeeWindow& add_employee_window;
    EmployeeWindow& change_employee_window;
};

#endif
