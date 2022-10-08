
#include "administrator_window.h"
#include "qlabel.h"
#include "qtabbar.h"
#include "ui_administrator_window.h"

AdministratorWindow::AdministratorWindow(AddEmployeeWindow& aew, EmployeeWindow &cew, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdministratorWindow),
    add_employee_window(aew),
    change_employee_window(cew)
{
    ui->setupUi(this);

    ui->tableEmployee->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableEmployee->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tablePublisher->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablePublisher->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableAuthor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableAuthor->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableGenre->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableGenre->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableEmployee->verticalHeader()->hide();

    ui->tablePublisher->verticalHeader()->hide();
    ui->tablePublisher->horizontalHeader()->hide();

    ui->tableAuthor->verticalHeader()->hide();
    ui->tableAuthor->horizontalHeader()->hide();

    ui->tableGenre->verticalHeader()->hide();
    ui->tableGenre->horizontalHeader()->hide();


    QTabWidget *tabw =ui->tabWidget;
    tabw->setTabText(0, "");
    tabw->setTabText(1, "");

    QTabBar *tabbar = tabw->tabBar();

    QLabel *lbl1 = new QLabel();
    lbl1->setText("Сотрудники");
    tabbar->setTabButton(0, QTabBar::LeftSide, lbl1);

    QLabel *lbl2 = new QLabel();
    lbl2->setText("Книги");
    tabbar->setTabButton(1, QTabBar::LeftSide, lbl2);
}

AdministratorWindow::~AdministratorWindow()
{
    delete ui;
}


void AdministratorWindow::addRowEmployee(std::shared_ptr<jp::Employee> employee) noexcept
{
    QString role = "Администратор";
    QString isActive = "Активный";
    if (employee->role == 1)
    {
        role = "Сотрудник";
    }
    if (employee->isActive == 0)
    {
        isActive = "Не активный";
    }

    QTableWidgetItem* full_name_ = new QTableWidgetItem;
    full_name_->setText(QString::fromStdString(employee->surname + " " + employee->firstName +
                                               " " + employee->patronymic));

    QTableWidgetItem* login_ = new QTableWidgetItem;
    login_->setText(QString::fromStdString(employee->login));

    QTableWidgetItem* role_ = new QTableWidgetItem;
    role_->setText(role);

    QTableWidgetItem* isActive_ = new QTableWidgetItem;
    isActive_->setText(isActive);


    ui->tableEmployee->insertRow(0);
    ui->tableEmployee->setItem(0, 0, full_name_);
    ui->tableEmployee->item(0, 0)->setData(Qt::UserRole, QVariant::fromValue(employee));
    ui->tableEmployee->item(0, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableEmployee->setItem(0, 1, login_);
    ui->tableEmployee->item(0, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableEmployee->setItem(0, 2, role_);
    ui->tableEmployee->item(0, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableEmployee->setItem(0, 3, isActive_);
    ui->tableEmployee->item(0, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}


void AdministratorWindow::addRowPublisher(std::shared_ptr<jp::Publisher> publisher) noexcept
{
    QTableWidgetItem* name_ = new QTableWidgetItem;
    name_->setText(QString::fromStdString(publisher->name));

    ui->tablePublisher->insertRow(ui->tablePublisher->rowCount());
    int row = ui->tablePublisher->rowCount() - 1;
    ui->tablePublisher->blockSignals(true);
    ui->tablePublisher->setItem(row, 0, name_);
    ui->tablePublisher->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(publisher));
    ui->tablePublisher->blockSignals(false);
}


void AdministratorWindow::addRowAuthor(std::shared_ptr<jp::Author> author) noexcept
{
    QTableWidgetItem* name_ = new QTableWidgetItem;
    name_->setText(QString::fromStdString(author->name));

    ui->tableAuthor->insertRow(ui->tableAuthor->rowCount());
    int row = ui->tableAuthor->rowCount() - 1;
    ui->tableAuthor->blockSignals(true);
    ui->tableAuthor->setItem(row, 0, name_);
    ui->tableAuthor->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(author));
    ui->tableAuthor->blockSignals(false);
}


void AdministratorWindow::addRowGenre(std::shared_ptr<jp::Genre> genre) noexcept
{
    QTableWidgetItem* name_ = new QTableWidgetItem;
    name_->setText(QString::fromStdString(genre->name));

    ui->tableGenre->insertRow(ui->tableGenre->rowCount());
    int row = ui->tableGenre->rowCount() - 1;
    ui->tableGenre->blockSignals(true);
    ui->tableGenre->setItem(row, 0, name_);
    ui->tableGenre->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(genre));
    ui->tableGenre->blockSignals(false);
}


void AdministratorWindow::logout()
{
    this->hide();
    emit logoutTriggered();
}


void AdministratorWindow::goToAddEmployee()
{
    add_employee_window.exec();
}


void AdministratorWindow::goToChangeEmployee(QTableWidgetItem* item)
{
    int row = ui->tableEmployee->currentRow();
    std::shared_ptr<jp::Employee> employee = item->tableWidget()->item(row, 0)->data(Qt::UserRole).
                                             value<std::shared_ptr<jp::Employee>>();
    change_employee_window.setEmployee(employee);
    change_employee_window.exec();
}


void AdministratorWindow::addRowPublisher()
{
    QTableWidgetItem* empty_ = new QTableWidgetItem;
    empty_->setText("");

    ui->tablePublisher->insertRow(ui->tablePublisher->rowCount());
    int row = ui->tablePublisher->rowCount() - 1;

    ui->tablePublisher->blockSignals(true);
    ui->tablePublisher->setItem(row, 0, empty_);
    ui->tablePublisher->blockSignals(false);

    ui->tablePublisher->editItem(ui->tablePublisher->item(row, 0));
}


void AdministratorWindow::addRowAuthor()
{
    QTableWidgetItem* empty_ = new QTableWidgetItem;
    empty_->setText("");

    ui->tableAuthor->insertRow(ui->tableAuthor->rowCount());
    int row = ui->tableAuthor->rowCount() - 1;

    ui->tableAuthor->blockSignals(true);
    ui->tableAuthor->setItem(row, 0, empty_);
    ui->tableAuthor->blockSignals(false);

    ui->tableAuthor->editItem(ui->tableAuthor->item(row, 0));
}


void AdministratorWindow::addRowGenre()
{
    QTableWidgetItem* empty_ = new QTableWidgetItem;
    empty_->setText("");

    ui->tableGenre->insertRow(ui->tableGenre->rowCount());
    int row = ui->tableGenre->rowCount() - 1;

    ui->tableGenre->blockSignals(true);
    ui->tableGenre->setItem(row, 0, empty_);
    ui->tableGenre->blockSignals(false);

    ui->tableGenre->editItem(ui->tableGenre->item(row, 0));
}


void AdministratorWindow::addPublisher(QTableWidgetItem* item)
{
    std::shared_ptr<jp::Publisher> publisher = item->data(Qt::UserRole).value<std::shared_ptr<jp::Publisher>>();
    if (!publisher)
    {
        publisher.reset(new jp::Publisher);
        publisher->id = 0;
        publisher->name = item->text().toStdString();
        emit addPublisherTriggered(publisher);
    }
    else
    {
       publisher->name = item->text().toStdString();
       emit changePublisherTriggered(publisher);
    }
}


void AdministratorWindow::addAuthor(QTableWidgetItem* item)
{
    std::shared_ptr<jp::Author> author = item->data(Qt::UserRole).value<std::shared_ptr<jp::Author>>();
    if (!author)
    {
        author.reset(new jp::Author);
        author->id = 0;
        author->name = item->text().toStdString();
        emit addAuthorTriggered(author);
    }
    else
    {
        author->name = item->text().toStdString();
        emit changeAuthorTriggered(author);
    }
}


void AdministratorWindow::addGenre(QTableWidgetItem* item)
{
    std::shared_ptr<jp::Genre> genre = item->data(Qt::UserRole).value<std::shared_ptr<jp::Genre>>();
    if (!genre)
    {
        genre.reset(new jp::Genre);
        genre->id = 0;
        genre->name = item->text().toStdString();
        emit addGenreTriggered(genre);
    }
    else
    {
        genre->name = item->text().toStdString();
        emit changeGenreTriggered(genre);
    }
}

void AdministratorWindow::clearEmployee()
{
    ui->tableEmployee->clearContents();
    ui->tableEmployee->setRowCount(0);
}


void AdministratorWindow::clearPropertyBook()
{
    ui->tableAuthor->clearContents();
    ui->tableAuthor->setRowCount(0);
    ui->tableGenre->clearContents();
    ui->tableGenre->setRowCount(0);
    ui->tablePublisher->clearContents();
    ui->tablePublisher->setRowCount(0);
}
