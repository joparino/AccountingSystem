
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "top_client.h"
#include "ui_top_client.h"

TopClient::TopClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopClient)
{
    ui->setupUi(this);
}

TopClient::~TopClient()
{
    delete ui;
}


void TopClient::setTopClient(std::vector<std::pair<std::string, int>> topClients)
{
    if (!topClients.empty())
    {
        int rounded = topClients.begin()->second;
        int i = 0;
        while ((rounded /= 10) >= 10) {
            i++;
        }

        if (topClients.begin()->second % (int)(pow(10, i + 1) + 0.5)) {
            rounded++;
        }

        for (; i >= 0; i--) {
            rounded *= 10;
        }

        QBarSet *centralFedDistr = new QBarSet("Количество");

        QStringList categories;

        for (auto& client : topClients)
        {
            categories << QString::fromStdString(client.first);
            *centralFedDistr << client.second;
        }

        QBarSeries *series = new QBarSeries();
        series->append(centralFedDistr);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Самые частые покупатели");
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
