#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addRowOrder(QString client, QString book, QString adress, QString status, QString date, QString sum) noexcept;
    void addRowBook(QString book, QString author, QString genre, QString year, QString price, QString count) noexcept;
    void clearOrder() noexcept;

signals:
    void searchTriggered(QString str);

public slots:
    void searchOrder();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
