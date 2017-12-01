#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "logo.h"
#include "client.h"
#include "account.h"

class Logo;
class Client;
class Account;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
        void Layout();
        QLineEdit *edit(QString title);
        QPushButton *button(QString title);
        QPropertyAnimation *setAnimation(QWidget *item, int duration);
        QString validLogin(QString text);

private:
        Client *client;
        Logo *logo;
        Account *account;

        QLabel *informer;
        QLineEdit *login, *pass;
        QGroupBox *sign_box;
        QPropertyAnimation *logo_anim, *sign_anim;

private slots:
            void logoClicked();
            void buttonClicked();
            void ParseData();
};

#endif // WINDOW_H
