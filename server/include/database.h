#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QVector>

class Database
{

public:
        Database(QString db_name);
        ~Database();

        void CreateTable(QString name, int type);
        void Inputing(QString table, const QString &name, const QString &password, QString file = "", int type = 0);
        void Updating(const QString &condition, const QString &city, const QString &phone);
        void Deleting(QString table, const QString &condition);

        QVector<QVector<QString> > &Reading(QString table, int column = 2);
        QVector<QVector<QString> > &Searching(QString table, const QString &value);

public:
        QString error;

private:
        QSqlQuery *query;
        QVector< QVector<QString> > data;

};

#endif // DATABASE_H
