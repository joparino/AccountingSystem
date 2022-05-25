
#include "windows/add_book_window.h"
#include "ui_add_book_window.h"

AddBookWindow::AddBookWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookWindow)
{
    ui->setupUi(this);
}

AddBookWindow::~AddBookWindow()
{
    delete ui;
}


void AddBookWindow::close()
{
    AddBookWindow::reject();
}


void AddBookWindow::addBook()
{
    if (!ui->lineTitle->text().isEmpty() && !ui->lineAuthor->text().isEmpty() && !ui->lineGenre->text().isEmpty()
       && !ui->linePublisher->text().isEmpty() && !ui->lineYear->text().isEmpty() && !ui->linePrice->text().isEmpty())
    {
        emit addBookTriggered(ui->lineTitle->text(), ui->lineAuthor->text(), ui->lineGenre->text(),
                              ui->linePublisher->text(), ui->lineYear->text(), ui->linePrice->text());
    }
}
