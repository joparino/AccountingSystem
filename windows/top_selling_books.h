
#ifndef TOP_SELLING_BOOKS_H
#define TOP_SELLING_BOOKS_H

#include <QDialog>

namespace Ui {
class TopSellingBooks;
}

class TopSellingBooks : public QDialog
{
    Q_OBJECT

public:
    explicit TopSellingBooks(QWidget *parent = nullptr);
    ~TopSellingBooks();
    void setTopSellingBooks(std::vector<std::pair<std::string, int>> topSellingBoks);

private:
    Ui::TopSellingBooks *ui;
};

#endif // TOP_SELLING_BOOKS_H
