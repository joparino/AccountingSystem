
#ifndef ADD_BOOK_WINDOW_H
#define ADD_BOOK_WINDOW_H

#include <QDialog>
#include "entities/models.h"

namespace Ui {
class AddBookWindow;
}

class AddBookWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookWindow(QWidget *parent = nullptr);
    ~AddBookWindow();

    void addItemAuthor(std::shared_ptr<jp::Author> author);
    void addItemPublisher(std::shared_ptr<jp::Publisher> publisher);
    void addItemGenre(std::shared_ptr<jp::Genre> genre);
    void setBook(std::shared_ptr<jp::Book> book);
    void clear();

public slots:
    void addBook();
    void reject();

signals:
    void addBookTriggered(std::shared_ptr<jp::Book> book);
    void changeBookTriggered(std::shared_ptr<jp::Book> book);

private:
    Ui::AddBookWindow *ui;
    std::shared_ptr<jp::Book> book_;
};

#endif
