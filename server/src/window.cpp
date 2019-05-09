#include "window.h"

Window::Window(QWidget *parent): QDialog(parent)
{
    bool exist = false;

    if(QFile::exists(QApplication::applicationDirPath() + "/DB")) exist = true;

    server = new Server;
    database = new Database("DB");

    if(!exist) database->CreateTable("Users", 0);

    connect(server, SIGNAL(readyData(QTcpSocket*)), this, SLOT(ParseData(QTcpSocket*)));

//    database->Inputing("Simon", "", "", "test.mp3", 1);
//    database->Inputing("Simon", "", "", "test.mp4", 1);
//    database->Inputing("Simon", "", "", "test", 1);
}

Window::~Window()
{

}

void Window::Verification(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Searching("Users", server->getData().login);
    Data *item;

    QByteArray btemp; // as a temp

    if(!data.size())
    {
        item = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                        "User not exist!", data, btemp); // data as a temp

        server->SendToClient(socket, *item);
    }
    else
    {
        if(md5(server->getData().password.toStdString()) == data[0][1].toStdString())
            item = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                            "log in", data, btemp);
        else
            item = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                            "Inccorect password!", data, btemp);

        server->SendToClient(socket, *item);
    }

    qDebug() << "User pass: " << QString::fromStdString(md5(server->getData().password.toStdString()));
    qDebug() << "DB pass: " << QString::fromStdString(md5(server->getData().password.toStdString()));
}

void Window::GetFiles(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Reading(server->getData().login, 1);

    QByteArray btemp; // as a temp

    for(int i=0; i<data.size(); i++)
        for(int j=0; j<data[i].size(); j++)
            qDebug() << data[i][j];

    Data *pack = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                    server->getData().message, data, btemp);

    server->SendToClient(socket, *pack);
}

void Window::addNewFile(QTcpSocket *socket)
{
    Data *data;
    QByteArray temp;

    QDir dir(QApplication::applicationDirPath() + "/files/");

    if(!dir.exists(server->getData().login))
        dir.mkdir(server->getData().login);

    if(!QFile::exists(QApplication::applicationDirPath() + "/files/" + server->getData().login + "/"
                      + server->getData().message))
    {
        QFile file(QApplication::applicationDirPath() + "/files/" + server->getData().login + "/"
                   + server->getData().message);
        file.open(QIODevice::WriteOnly);

        file.write(server->getData().file);
        file.close();

        database->Inputing(server->getData().login, "", "",
                           QApplication::applicationDirPath() + "/files/" + server->getData().login + "/"
                           + server->getData().message, 1);

        data = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                         server->getData().message, server->getData().files, temp);

        server->SendToClient(socket, *data);
    }
    else
    {
        data = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                         "File already exist!", server->getData().files, temp);

        server->SendToClient(socket, *data);
    }
}

void Window::startViewer(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Reading(server->getData().login, 1);
    QVector<QString> files;

    for(int i=0; i<data.size(); i++)
        for(int j=0; j<data[i].size(); j++)
            files.push_back(data[i][j]);

    for(int i=0; i<files.size(); i++)
        if(getName(files[i]) == server->getData().message)
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(files[i]));
            break;
        }
}

void Window::sendFile(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Reading(server->getData().login, 1);
    QVector<QString> files;

    Data *pack;

    for(int i=0; i<data.size(); i++)
        for(int j=0; j<data[i].size(); j++)
            files.push_back(data[i][j]);

    for(int i=0; i<files.size(); i++)
        if(getName(files[i]) == server->getData().message)
        {
            QFile file(files[i]);
            file.open(QIODevice::ReadOnly);

            QByteArray block = file.readAll();
            file.close();

            pack = server->createData(server->getData().type, server->getData().login, server->getData().password,
                               server->getData().message, server->getData().files, block);

            server->SendToClient(socket, *pack);
            break;
        }
}

void Window::deleteFile(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Reading(server->getData().login, 1);
    QVector<QString> files;

    Data *pack;

    for(int i=0; i<data.size(); i++)
        for(int j=0; j<data[i].size(); j++)
            files.push_back(data[i][j]);

    for(int i=0; i<files.size(); i++)
        if(getName(files[i]) == server->getData().message)
        {
            database->Deleting(server->getData().login, files[i]);
            QFile(files[i]).remove();

            pack = server->createData(server->getData().type, server->getData().login, server->getData().password,
                               server->getData().message, server->getData().files, server->getData().file);

            server->SendToClient(socket, *pack);


            break;
        }
}

QString Window::getName(QString &path) const
{
    QStringList lst = path.split("/");

    return lst[lst.size() - 1];
}


void Window::ParseData(QTcpSocket *socket)
{
    if(server->getData().type == "sign in") Verification(socket);
    if(server->getData().type == "sign up") SignUp(socket);
    if(server->getData().type == "getFiles") GetFiles(socket);
    if(server->getData().type == "newFile") addNewFile(socket);
    if(server->getData().type == "view") startViewer(socket);
    if(server->getData().type == "download") sendFile(socket);
    if(server->getData().type == "delete") deleteFile(socket);
}

void Window::SignUp(QTcpSocket *socket)
{
    QVector<QVector<QString> > data = database->Searching("Users", server->getData().login);
    Data *item;

    QByteArray btemp; // as a temp

    if(data.size())
    {
        item = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                  "User already exist!", data, btemp); // data as a temp

        server->SendToClient(socket, *item);
    }
    else
    {
        database->Inputing("Users", server->getData().login, QString::fromStdString(md5(server->getData().password.toStdString())));
        database->CreateTable(server->getData().login, 1);

        item = server->createData(server->getData().type, server->getData().login, server->getData().password,
                                  "Registration was successful!", data, btemp);

        server->SendToClient(socket, *item);
    }
}
