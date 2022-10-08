
#include <QCalendar>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "sales_three_months.h"
#include "ui_sales_three_months.h"

SalesThreeMonths::SalesThreeMonths(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesThreeMonths)
{
    ui->setupUi(this);
}

SalesThreeMonths::~SalesThreeMonths()
{
    delete ui;
}


void SalesThreeMonths::setSales(std::vector<float> sales)
{

    int rounded = *sales.begin();
    int i = 0;
    while ((rounded /= 10) >= 10) {
        i++;
    }

    if (rounded % (int)(pow(10, i + 1) + 0.5)) {
        rounded++;
    }

    for (; i >= 0; i--) {
        rounded *= 10;
    }

    QCalendar calendar;
    QString firstMonth =  calendar.standaloneMonthName(QLocale::Russian, QDate::currentDate().month() - 2);
    QString secondMonth =  calendar.standaloneMonthName(QLocale::Russian, QDate::currentDate().month() - 1);
    QString thirstMonth =  calendar.standaloneMonthName(QLocale::Russian, QDate::currentDate().month());

    QBarSet *centralFedDistr = new QBarSet("Продажи");

    for (auto& sum : sales)
    {
        *centralFedDistr << sum;
    }

    QBarSeries *series = new QBarSeries();
    series->append(centralFedDistr);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Продажи за 3 месяца");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << firstMonth << secondMonth << thirstMonth;

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, rounded);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignRight);

    ui->widget->setChart(chart);
    ui->widget->setRenderHint(QPainter::Antialiasing);
}
