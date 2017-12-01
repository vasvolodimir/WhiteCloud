#include "create.h"

Create::Create(QWidget *parent): QDialog(parent)
{
    Layout();
}

Create::~Create()
{

}

void Create::Layout()
{
    picture = new QLabel;
    picture->setPixmap(QPixmap(QApplication::applicationDirPath() + "/anonym.png"));

    info = new QLabel;
    info->setStyleSheet("font-size: 12px;");
    info->setText("<b>Path:</b><br><b>Name:</b><br><b>Type:</b><br><b>Size:</b><br>");
    info->setFixedHeight(80);

    QPushButton *apply = button("APPLY", QSize(130, 30));
    QPushButton *open = button("OPEN", QSize(130, 30));

    QHBoxLayout *bottom_layout = new QHBoxLayout;
    bottom_layout->setAlignment(Qt::AlignBottom);
    bottom_layout->addWidget(open);
    bottom_layout->addWidget(apply);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(picture);
    layout->addWidget(info);
    layout->addSpacing(20);
    layout->addLayout(bottom_layout);

    this->setLayout(layout);
    this->setWindowTitle("Creater");
    this->setFixedSize(300, 410);
}

QPushButton *Create::button(QString title, QSize size)
{
    QPushButton *temp = new QPushButton(title);
    temp->setStyleSheet("QPushButton { background: rgb(121, 120, 120); color: white; }"
                        "QPushButton:hover { background: rgb(202, 202, 202); color: black; }");

    connect(temp, SIGNAL(clicked(bool)), this, SLOT(buttonClick()));

    temp->setFixedSize(size);
    temp->setFont(QFont("Ubuntu", 10, QFont::Cursive));
    temp->setCursor(Qt::PointingHandCursor);

    return temp;
}

void Create::buttonClick()
{
    QPushButton *btn = qobject_cast<QPushButton*> (sender());

    static QString path;

    if(btn->text() == "OPEN")
    {
        path = QFileDialog::getOpenFileName(this, "Open...", "");
        QFileInfo file(path);
        QString img = QApplication::applicationDirPath();

        if(!path.isEmpty())
        {
            info->setText("<b>Path:</b> " + path + "<br>" + "<b>Name:</b> " + getName(path) + "<br>" +
                          "<b>Type:</b> " + getType(path) + "<br>" + "<b>Size:</b> " + QString::number(file.size()));

            if(getType(path) == "png" || getType(path) == "jpg")
                img += "/picture.png";
            else if(getType(path) == "txt" || getType(path) == "docx") img += "/text.png";
            else if(getType(path) == "empty") img += "/empty.png";
            else if(getType(path) == "mp4") img += "/video.png";
            else if(getType(path) == "mp3") img += "/audio.png";
            else img += "/empty.png";

            picture->setPixmap(QPixmap(img));

        }
    }

    if(btn->text() == "APPLY")
    {
        if(!path.isEmpty())
        {
            emit choosenFile(path);
            this->hide();
        }
    }
}

QString Create::getType(QString &path) const
{
    QStringList lst = path.split(".");

    if(lst.size() < 2) return "empty";

    return lst[lst.size() - 1];
}

QString Create::getName(QString &path) const
{
    QStringList lst = path.split("/");

    return lst[lst.size() - 1];
}
