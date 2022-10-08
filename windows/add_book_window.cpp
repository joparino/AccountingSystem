
#include "qdatetime.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "windows/add_book_window.h"
#include "ui_add_book_window.h"
#include "entities/models.h"

AddBookWindow::AddBookWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookWindow)
{
    ui->setupUi(this);

    ui->comboAuthor->lineEdit()->setPlaceholderText("Выберите автора...");
    ui->comboPublisher->lineEdit()->setPlaceholderText("Выберите издателя...");
    ui->comboGenre->lineEdit()->setPlaceholderText("Выберите жанр...");

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

AddBookWindow::~AddBookWindow()
{
    delete ui;
}


void AddBookWindow::addItemAuthor(std::shared_ptr<jp::Author> author)
{
    ui->comboAuthor->addItem(QString::fromStdString(author->name), QVariant::fromValue(author));
}


void AddBookWindow::addItemPublisher(std::shared_ptr<jp::Publisher> publisher)
{
    ui->comboPublisher->addItem(QString::fromStdString(publisher->name), QVariant::fromValue(publisher));
}


void AddBookWindow::addItemGenre(std::shared_ptr<jp::Genre> genre)
{
    ui->comboGenre->addItem(QString::fromStdString(genre->name), QVariant::fromValue(genre));
}


void AddBookWindow::addBook()
{
    if (book_)
    {
        book_->title     = ui->lineTitle->text().toStdString();
        book_->year      = ui->lineYear->text().toStdString();
        book_->price     = ui->linePrice->text().toInt();
        book_->author    = ui->comboAuthor->currentData(Qt::UserRole).value<std::shared_ptr<jp::Author>>();
        book_->genre     = ui->comboGenre->currentData(Qt::UserRole).value<std::shared_ptr<jp::Genre>>();
        book_->publisher = ui->comboPublisher->currentData(Qt::UserRole).value<std::shared_ptr<jp::Publisher>>();

        emit changeBookTriggered(book_);
    }
    else if (!ui->lineTitle->text().isEmpty()         && !ui->comboAuthor->currentText().isEmpty() &&
             !ui->comboGenre->currentText().isEmpty() && !ui->comboPublisher->currentText().isEmpty() &&
             !ui->lineYear->text().isEmpty()          && !ui->linePrice->text().isEmpty())
    {
        std::shared_ptr<jp::Book> book(new jp::Book);
        book->title     = ui->lineTitle->text().toStdString();
        book->year      = ui->lineYear->text().toStdString();
        book->price     = ui->linePrice->text().toInt();
        book->author    = ui->comboAuthor->currentData(Qt::UserRole).value<std::shared_ptr<jp::Author>>();
        book->genre     = ui->comboGenre->currentData(Qt::UserRole).value<std::shared_ptr<jp::Genre>>();
        book->publisher = ui->comboPublisher->currentData(Qt::UserRole).value<std::shared_ptr<jp::Publisher>>();
        book->count     = 0;
        emit addBookTriggered(book);
    }
    else
    {
        QMessageBox::critical(this, "Не все поля заполнены", "Пожалуйста заполните все поля");
    }
}


void AddBookWindow::setBook(std::shared_ptr<jp::Book> book)
{
    this->setWindowTitle("Книга");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Изменить");
    ui->label_2->setText("Перед изменением убедитесь, что все поля заполнены правильно.");
    book_ = book;
    ui->lineTitle->setText(QString::fromStdString(book_->title));
    ui->lineYear->setText(QString::fromStdString(book_->year));
    ui->linePrice->setText(QString::number(book_->price));

    ui->comboAuthor->setCurrentIndex(ui->comboAuthor->findText(QString::fromStdString(book->author->name)));
    ui->comboGenre->setCurrentIndex(ui->comboGenre->findText(QString::fromStdString(book->genre->name)));
    ui->comboPublisher->setCurrentIndex(ui->comboPublisher->findText(QString::fromStdString(book->publisher->name)));
}


void AddBookWindow::clear()
{
    ui->comboAuthor->clear();
    ui->comboGenre->clear();
    ui->comboPublisher->clear();
}


void AddBookWindow::reject()
{
    this->setWindowTitle("Добавление книги");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->label_2->setText("Перед добавление убедитесь, что все поля заполнены правильно.");
    book_.reset();
    ui->lineTitle->setText("");
    ui->lineYear->setText("");
    ui->linePrice->setText("");

    ui->comboAuthor->setCurrentIndex(-1);
    ui->comboGenre->setCurrentIndex(-1);
    ui->comboPublisher->setCurrentIndex(-1);
    AddBookWindow::hide();
}
