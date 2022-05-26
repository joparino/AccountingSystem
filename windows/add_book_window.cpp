
#include "windows/add_book_window.h"
#include "ui_add_book_window.h"

AddBookWindow::AddBookWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookWindow)
{
    ui->setupUi(this);

    ui->comboAuthor->lineEdit()->setPlaceholderText("Выберите автора...");
    ui->comboPublisher->lineEdit()->setPlaceholderText("Выберите издателя...");
    ui->comboGenre->lineEdit()->setPlaceholderText("Выберите жанр...");
}

AddBookWindow::~AddBookWindow()
{
    delete ui;
}


void AddBookWindow::addItemAuthor(QString author)
{
    ui->comboAuthor->addItem(author);
}


void AddBookWindow::addItemPublisher(QString publisher)
{
    ui->comboPublisher->addItem(publisher);
}


void AddBookWindow::addItemGenre(QString genre)
{
    ui->comboGenre->addItem(genre);
}


void AddBookWindow::addBook()
{
    if (!ui->lineTitle->text().isEmpty() && !ui->comboAuthor->currentText().isEmpty() && !ui->comboGenre->currentText().isEmpty()
       && !ui->comboPublisher->currentText().isEmpty() && !ui->lineYear->text().isEmpty() && !ui->linePrice->text().isEmpty())
    {
        emit addBookTriggered(ui->lineTitle->text(), ui->comboAuthor->currentText(), ui->comboGenre->currentText(),
                              ui->comboPublisher->currentText(), ui->lineYear->text(), ui->linePrice->text());
    }
}
