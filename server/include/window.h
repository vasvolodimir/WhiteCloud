#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QApplication>
#include <QFile>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDesktopServices>

#include "database.h"
#include "server.h"
#include "md5.h"

class Database;
class Server;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
        Database *database;
        Server *server;

private:
        void Verification(QTcpSocket *socket);
        void GetFiles(QTcpSocket *socket);
        void addNewFile(QTcpSocket *socket);
        void startViewer(QTcpSocket *socket);
        void sendFile(QTcpSocket *socket);
        void deleteFile(QTcpSocket *socket);

        QString getName(QString &path) const;

private slots:
            void ParseData(QTcpSocket *socket);
            void SignUp(QTcpSocket *socket);
};

#endif // WINDOW_H
