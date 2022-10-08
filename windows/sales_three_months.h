
#ifndef SALES_THREE_MONTHS_H
#define SALES_THREE_MONTHS_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class SalesThreeMonths;
}

class SalesThreeMonths : public QDialog
{
    Q_OBJECT

public:
    explicit SalesThreeMonths(QWidget *parent = nullptr);
    ~SalesThreeMonths();
    void setSales(std::vector<float> sales);

private:
    Ui::SalesThreeMonths *ui;
};

#endif // SALES_THREE_MONTHS_H
