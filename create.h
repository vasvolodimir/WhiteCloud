#ifndef CREATE_H
#define CREATE_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

class Create : public QDialog
{
    Q_OBJECT

public:
        Create(QWidget *parent = 0);
        ~Create();

private:
        void Layout();

        QPushButton *button(QString titile, QSize size);
        QString getType(QString &path) const;
        QString getName(QString &path) const;

private:
        QLabel *picture, *info;

private slots:
            void buttonClick();

signals:
            void choosenFile(QString);
};

#endif // CREATE_H
