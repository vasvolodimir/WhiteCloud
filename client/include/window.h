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

#include "../common/log.h"
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

    void notify(QString msg);

private:
        void layout();
        QPropertyAnimation *createAnimation(QWidget *item, int duration,
	  QPropertyAnimation **animation);
        QString serializeLogin(QString text) const;
        Logo *createLogo(Logo **logo, const char *slot);
        QLabel *createInformer(QLabel **informer);
        QLineEdit *createLineEdit(QString title, QLineEdit::EchoMode mode,
	  QLineEdit **edit);
        QPushButton *createPushButton(QString title, QPushButton **btn);
        QGroupBox *createGroupBox(QLayout *layout, QGroupBox **group);
        bool isCredentialsValid();

private slots:
            void logoClicked();
            void handleBtnClick();
            void parseData();

private:
        Client *m_client;
        Logo *m_logo;
        Account *m_account;
        QLabel *m_informer;
        QLineEdit *m_login, *m_password;
        QGroupBox *m_sign_box;
        QPropertyAnimation *m_logo_anim, *m_sign_anim;

};

#endif // WINDOW_H
