
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "top_selling_books.h"
#include "ui_top_selling_books.h"

TopSellingBooks::TopSellingBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopSellingBooks)
{
    ui->setupUi(this);
}

TopSellingBooks::~TopSellingBooks()
{
    delete ui;
}


void TopSellingBooks::setTopSellingBooks(std::vector<std::pair<std::string, int>> topSellingBoks)
{

    if (!topSellingBoks.empty())
    {
        int rounded = topSellingBoks.begin()->second;
        int i = 0;
        while ((rounded /= 10) >= 10) {
            i++;
        }

        if (topSellingBoks.begin()->second % (int)(pow(10, i + 1) + 0.5)) {
            rounded++;
        }

        for (; i >= 0; i--) {
            rounded *= 10;
        }

        QBarSet *centralFedDistr = new QBarSet("Количество");

        QStringList categories;

        for (auto& book : topSellingBoks)
        {
            categories << QString::fromStdString(book.first);
            *centralFedDistr << book.second;
        }

        QBarSeries *series = new QBarSeries();
        series->append(centralFedDistr);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Топ самых продаваемых книг за все время");
        chart->setAnimationOptions(QChart::SeriesAnimations);

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
}
