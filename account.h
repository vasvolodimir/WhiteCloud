#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QDialog>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QFile>
#include <QMessageBox>

#include "client.h"
#include "create.h"

class Client;
class Create;

class Account : public QDialog
{
    Q_OBJECT

public:
        Account(QString login, Client *client, QWidget *parent = 0);
        ~Account();

private:
         void Layout();
         QPushButton *button(QString title, QSize size);
         void createToolBar();
         void remove(QLayout *layout);
         QPropertyAnimation *setAnimation(QWidget *item, int duration);

         QString getType(QString &path) const;
         QString getName(QString &path) const;

private:
         void getFile();

private:
         Client *client;
         Create *creater;
         QWidget *header;

         QString login;

         QVBoxLayout *layout;
         QPropertyAnimation *view_anim, *del_anim, *download_anim;
         QPushButton *view, *download, *del;

private:
         QVector<QAction*> actions;

private slots:
            void choosenItem();
            void buttonClick();
            void ParseData();
            void sendNewFile(QString);
};


#endif // ACCOUNT_H
