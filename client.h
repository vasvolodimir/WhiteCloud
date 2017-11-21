#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QWidget>
#include <QTcpSocket>

class Data
{
public:
        QString login;
        QString password;
        QString type;

        QString message;

    friend QDataStream &operator << (QDataStream &stream, Data &object)
    {
        stream << object.type;
        stream << object.login;
        stream << object.password;
        stream << object.message;

        return stream;
    }

    friend QDataStream &operator >> (QDataStream &stream, Data &object)
    {
        stream >> object.type;
        stream >> object.login;
        stream >> object.password;
        stream >> object.message;

        return stream;
    }
};

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    ~Client();

private:
        QTcpSocket *socket;
        quint16 nextBlockSize;
        Data message_from_server;

public:
        Data *createData(QString type, QString login, QString password, QString message);
        Data getData() const;

public slots:
            void slotReadyRead();
            void slotError(QAbstractSocket::SocketError);
            void slotSendToServer(Data &data);
            void Connected();

signals:
            void readyData();

};


#endif // CLIENT_H