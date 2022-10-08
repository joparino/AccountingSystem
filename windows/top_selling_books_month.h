
#ifndef TOP_SELLING_BOOKS_MONTH_H
#define TOP_SELLING_BOOKS_MONTH_H

#include <QDialog>

namespace Ui {
class TopSellingBooksMonth;
}

class TopSellingBooksMonth : public QDialog
{
    Q_OBJECT

public:
    explicit TopSellingBooksMonth(QWidget *parent = nullptr);
    ~TopSellingBooksMonth();
    void setTopSellingBooksMonth(std::vector<std::pair<std::string, int>> topSellingBoks);

private:
    Ui::TopSellingBooksMonth *ui;
};

#endif // TOP_SELLING_BOOKS_MONTH_H
