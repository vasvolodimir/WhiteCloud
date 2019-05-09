#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QWidget>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>

enum data_type_t
{
    SIGNIN,
    SIGNUP,
    Unknown = -1
};

struct data_t
{
public:
        QString login;
        QString password;
        QString type;

        QString message;
        QVector<QVector<QString> > files;
        QByteArray file;

    friend QDataStream &operator << (QDataStream &stream, data_t &object)
    {
        stream << object.type;
        stream << object.login;
        stream << object.password;
        stream << object.message;
        stream << object.files;
        stream << object.file;

        return stream;
    }

    friend QDataStream &operator >> (QDataStream &stream, data_t &object)
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

class Client : public QWidget
{
    Q_OBJECT

public:
        Client(QWidget *parent = 0);
        ~Client();

        data_t *createData(QString type, QString login, QString password,
          QString message, QVector<QVector<QString> > files, QByteArray file);
        data_t getData() const;

public slots:
            void slotReadyRead();
            void slotError(QAbstractSocket::SocketError);
            void slotSendToServer(data_t &data);
            void Connected();

signals:
            void readyData();

private:
        QTcpSocket *m_socket;
        quint16 nextBlockSize;
        data_t message_from_server;
};


#endif // CLIENT_H
