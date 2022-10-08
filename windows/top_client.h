
#ifndef TOP_CLIENT_H
#define TOP_CLIENT_H

#include <QDialog>

namespace Ui {
class TopClient;
}

class TopClient : public QDialog
{
    Q_OBJECT

public:
    explicit TopClient(QWidget *parent = nullptr);
    ~TopClient();
    void setTopClient(std::vector<std::pair<std::string, int>> topClients);

private:
    Ui::TopClient *ui;
};

#endif // TOP_CLIENT_H
