#include "server.h"

Server::Server(QWidget *parent): QWidget(parent), nextBlockSize(0)
{
    server = new QTcpServer(this);

    if(!server->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "Unable to start server";
        server->close();
        return;
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

Server::~Server()
{

}

void Server::SendToClient(QTcpSocket *socket, Data data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    out << quint16(0) << data;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    socket->write(block);
}

Data Server::getData() const
{
    return message_from_client;
}

Data *Server::createData(QString type, QString login, QString password, QString message, QVector<QVector<QString> > files, QByteArray file)
{
    Data *item = new Data;

    item->type = type;
    item->login = login;
    item->password = password;
    item->message = message;
    item->files = files;
    item->file = file;

    return item;
}

void Server::slotNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    QVector<QVector<QString> > temp; // as a temp
    QByteArray btemp;

    Data *data = this->createData("", "", "", "Server response: Connected!", temp, btemp);
    SendToClient(socket, *data);
}

void Server::slotReadClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_7);

    while(true)
    {
        if(!nextBlockSize)
        {
            if(client->bytesAvailable() < sizeof(quint16)) break;
            in >> nextBlockSize;
        }

        if(client->bytesAvailable() < nextBlockSize) break;

        in >> message_from_client;

        qDebug() << message_from_client.type;
        qDebug() << message_from_client.files;

        nextBlockSize = 0;

    }

    emit readyData(client);
}
