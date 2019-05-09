#include "database.h"

Database::Database(QString db_name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);
    db.setUserName("Simon");
    db.setPassword("0000");
    db.setHostName("localhost");

    if(!db.open())
    {
        error = "Can not open db manager. " + db.lastError().text();
        qDebug() << "Can not open db manager";
    }

    query = new QSqlQuery(db);
}

Database::~Database()
{

}

void Database::CreateTable(QString name, int type)
{
    QString table;

    if(!type)
    {
        table = "CREATE TABLE " + name + "("
                                         "Name VARCHAR(40),"
                                         "Password VARCHAR(20)"
                                         ");";
    }
    else
    {
        table = "CREATE TABLE " + name + "("
                                         "Path VARCHAR(200)"
                                         ");";
    }

    if(!query->exec(table))
    {
        error = "Unable to create table!";
        qDebug() << "Unable to create table!";
    }
}

void Database::Inputing(QString table, const QString &name, const QString &password, QString file, int type)
{
    QString tamplate, command;

    if(!type)
    {
        tamplate = "INSERT INTO " + table + "(Name, Password)"
                               "VALUES('%1', '%2');";

        command = tamplate.arg(name)
                        .arg(password);
    }
    else
    {
        tamplate = "INSERT INTO " + table + "(Path)"
                               "VALUES('%1');";

        command = tamplate.arg(file);
    }

    if(!query->exec(command))
    {
        error = "Error with inputing data!";
        qDebug() << "Error with inputing data!";
    }
}

void Database::Updating(const QString &condition, const QString &city, const QString &phone)
{
    QString command = "UPDATE List SET City = \""
            + city + "\", Phone = \"" + phone + "\" WHERE Name = \""+ condition +"\";";

    if(!query->exec(command))
    {
        error = "Error with updating data!";
        qDebug() << "Error with updating data!";
    }

}

void Database::Deleting(QString table, const QString &condition)
{
    QString command = "DELETE FROM " + table + " WHERE Path = \"" + condition + "\";";

    if(!query->exec(command))
    {
        error = "Error with deleting data!";
        qDebug() << "Error with deleting data!";
    }
}

QVector< QVector<QString> > &Database::Reading(QString table, int column)
{
    data.clear();

    if(!query->exec("SELECT * FROM " + table + ";"))
    {
        error = "Unable to read data!";
        qDebug() << "Unable to read data!";
    }

    while(query->next())
    {
        QVector<QString> row;

        for(int i=0; i<column; i++)
            row.push_back(query->value(i).toString());

        data.push_back(row);
    }

    return data;
}

QVector<QVector<QString> > &Database::Searching(QString table, const QString &value)
{
    data.clear();

    if(!query->exec("SELECT * FROM " + table + " WHERE Name = \"" + value + "\";"))
    {
        error = "Unable to search data!";
        qDebug() << "Unable to search data!";
    }


    while(query->next())
    {
        QVector<QString> row;

        for(int i=0; i<2; i++)
            row.push_back(query->value(i).toString());

        data.push_back(row);
    }

    return data;
}
