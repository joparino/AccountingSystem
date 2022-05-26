#ifndef ADD_BOOK_WINDOW_H
#define ADD_BOOK_WINDOW_H

#include <QDialog>

namespace Ui {
class AddBookWindow;
}

class AddBookWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookWindow(QWidget *parent = nullptr);
    ~AddBookWindow();
    void addItemAuthor(QString author);
    void addItemPublisher(QString publisher);
    void addItemGenre(QString genre);

public slots:
    void addBook();

signals:
    void addBookTriggered(QString title, QString author, QString genre,
                          QString publisher, QString year, QString price);

private:
    Ui::AddBookWindow *ui;
};

#endif // ADD_BOOK_WINDOW_H
