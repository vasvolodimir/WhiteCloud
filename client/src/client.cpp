#include "client.h"

Client::Client(QWidget *parent)
    : QWidget(parent),
      m_socket(nullptr),
      nextBlockSize(0)
{
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost("localhost", 2323);
    connect(m_socket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
      SLOT(slotError(QAbstractSocket::SocketError)));
}

Client::~Client()
{

}

data_t *Client::createData(QString type, QString login, QString password,
 QString message, QVector<QVector<QString> > files, QByteArray file)
{
    data_t *item = new data_t;

    item->type = type;
    item->login = login;
    item->password = password;
    item->message = message;
    item->files = files;
    item->file = file;

    return item;
}

data_t Client::getData() const
{
    return message_from_server;
}

void Client::slotReadyRead()
{
    QDataStream in(m_socket);

    in.setVersion(QDataStream::Qt_5_7);

    while(true)
    {
        if (!nextBlockSize)
        {
            if (m_socket->bytesAvailable() < sizeof(quint16))
                break;

            in >> nextBlockSize;
        }

        if (m_socket->bytesAvailable() < nextBlockSize)
            break;

        in >> message_from_server;

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
    qDebug() << m_socket->errorString();
}

void Client::slotSendToServer(data_t &data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << data;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void Client::Connected()
{
    qDebug() << "Client connected!";
}
