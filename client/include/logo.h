#ifndef LOGO_H
#define LOGO_H

#include <QLabel>
#include <QMouseEvent>
#include <QApplication>

class Logo : public QLabel
{
    Q_OBJECT

public:
    Logo(QWidget *parent = 0);
    ~Logo();

protected:
        void mousePressEvent(QMouseEvent *event);

signals:
            void clicked();
};

#endif // LOGO_H
