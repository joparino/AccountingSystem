
#ifndef AUTHORIZATION_WINDOW_H
#define AUTHORIZATION_WINDOW_H

#include <QDialog>

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();

private slots:
    void authorization();

public slots:
    void hide();
    void reject();

signals:
    void authorizationTriggered(std::string login, std::string password);

private:
    Ui::AuthorizationWindow *ui;
};

#endif
