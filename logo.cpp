#include "logo.h"


Logo::Logo(QWidget *parent) : QLabel(parent)
{
    this->setPixmap(QApplication::applicationDirPath() + "/logo.png");
}

Logo::~Logo()
{

}

void Logo::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) emit clicked();
}
