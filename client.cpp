#include "client.h"

Client::Client(QWidget *parent): QWidget(parent), nextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost", 2323);
    connect(socket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

Client::~Client()
{

}

Data *Client::createData(QString type, QString login, QString password, QString message, QVector<QVector<QString> > files)
{
    Data *item = new Data;

    item->type = type;
    item->login = login;
    item->password = password;
    item->message = message;
    item->files = files;

    return item;
}

Data Client::getData() const
{
    return message_from_server;
}

void Client::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_7);

    while(true)
    {
        if(!nextBlockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint16)) break;
            in >> nextBlockSize;
        }

        if(socket->bytesAvailable() < nextBlockSize) break;

        in >> message_from_server;

        qDebug() << "Client (client) here!!: ";

        for(int i=0; i<message_from_server.files.size(); i++)
            for(int j=0; j<message_from_server.files[i].size(); j++)
                qDebug() << message_from_server.files[i][j];

        nextBlockSize = 0;
    }

    emit readyData();
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    err == QAbstractSocket::HostNotFoundError ?
    qDebug() << "The host was not found." :
    err == QAbstractSocket::RemoteHostClosedError ?
    qDebug() << "The remote host is closed." :
    err == QAbstractSocket::ConnectionRefusedError ?
    qDebug() << "The connection was refused." :
    qDebug() << socket->errorString();
}

void Client::slotSendToServer(Data &data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    out << quint16(0) << data;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    socket->write(block);
}

void Client::Connected()
{
    qDebug() << "Client connected!";
}
