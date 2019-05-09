#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QDebug>

class Data
{
public:
        QString login;
        QString password;
        QString type;

        QString message;
        QVector<QVector<QString> > files;
        QByteArray file;


    friend QDataStream &operator << (QDataStream &stream, Data &object)
    {
        stream << object.type;
        stream << object.login;
        stream << object.password;
        stream << object.message;
        stream << object.files;
        stream << object.file;

        return stream;
    }

    friend QDataStream &operator >> (QDataStream &stream, Data &object)
    {
        stream >> object.type;
        stream >> object.login;
        stream >> object.password;
        stream >> object.message;
        stream >> object.files;
        stream >> object.file;

        return stream;
    }
};

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    ~Server();

private:
        QTcpServer *server;
        quint16 nextBlockSize;
        Data message_from_client;

public:
        Data getData() const;
        Data *createData(QString type, QString login, QString password, QString message, QVector<QVector<QString> > files,
                         QByteArray file);
        void SendToClient(QTcpSocket *socket, Data data);

public slots:
        virtual void slotNewConnection();
        void slotReadClient();

signals:
        void readyData(QTcpSocket*);
};


#endif // SERVER_H
