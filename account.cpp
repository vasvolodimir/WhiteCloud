#include "account.h"

Account::Account(QString login, Client *client, QWidget *parent) : login(login), client(client) ,QDialog(parent)
{
    layout = new QVBoxLayout;
    creater = new Create;
    creater->hide();

    connect(client, SIGNAL(readyData()), this, SLOT(ParseData()));
    connect(creater, SIGNAL(choosenFile(QString)), this, SLOT(sendNewFile(QString)));

    getFile();
}

Account::~Account()
{
    delete creater;
}

void Account::Layout()
{
    this->setFixedSize(600, 400);
    this->setWindowTitle("White cloud");

    header = new QWidget;
    header->setStyleSheet("background: rgb(84, 102, 201);");
    header->setFixedSize(width(), 70);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    QHBoxLayout *header_layout = new QHBoxLayout;
    header_layout->setAlignment(Qt::AlignTop);

    QPushButton *create = button("CREATE", QSize(130, 30));
    view = button("VIEW", QSize(90, 30));
    download = button("DOWNLOAD", QSize(90, 30));
    del = button("DELETE", QSize(90, 30));

    view->setVisible(false);
    download->setVisible(false);
    del->setVisible(false);

    view_anim = setAnimation(view, 1000);
    download_anim = setAnimation(download, 1000);
    del_anim = setAnimation(del, 1000);

    header_layout->addWidget(create);
    header_layout->addSpacing(160);
    header_layout->addWidget(view);
    header_layout->addWidget(download);
    header_layout->addWidget(del);

    QVBoxLayout *header_layout_vertical = new QVBoxLayout;

    header_layout_vertical->addSpacing(15);
    header_layout_vertical->addLayout(header_layout);

    header->setLayout(header_layout_vertical);
    layout->addWidget(header);

    createToolBar();

    static bool flag = true;

    if(flag) this->setLayout(layout);
    flag = false;
}

QPushButton *Account::button(QString title, QSize size)
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

void Account::createToolBar()
{
    QVector<QHBoxLayout*> tool_layouts;
    QVector<QString> data_files;

    for(int i=0; i<client->getData().files.size(); i++)
        for(int j=0; j<client->getData().files[i].size(); j++)
            data_files.push_back(client->getData().files[i][j]);


    QToolBar *bar = new QToolBar;

    bar->setIconSize(QSize(105, 130));
    bar->setFixedWidth(width());

    actions.clear();
    tool_layouts.clear();

    tool_layouts.push_back(new QHBoxLayout);
    tool_layouts.front()->setAlignment(Qt::AlignCenter);

    for(int i=0; i<data_files.size(); i++)
    {
        if(!(i % 5) && i != 0)
        {
            tool_layouts[tool_layouts.size() - 1]->setAlignment(Qt::AlignCenter);
            tool_layouts[tool_layouts.size() - 1]->addWidget(bar);

            bar = new QToolBar;
            bar->setIconSize(QSize(105, 130));

            tool_layouts.push_back(new QHBoxLayout);
        }

        QString path = QApplication::applicationDirPath();

        if(getType(data_files[i]) == "png" || getType(data_files[i]) == "jpg")
            path += "/picture.png";
        else if(getType(data_files[i]) == "txt" || getType(data_files[i]) == "docx") path += "/text.png";
        else if(getType(data_files[i]) == "empty") path += "/empty.png";
        else if(getType(data_files[i]) == "mp4") path += "/video.png";
        else if(getType(data_files[i]) == "mp3") path += "/audio.png";
        else path += "/empty.png";

        QAction *action = bar->addAction(QIcon(QPixmap(path)), getName(data_files[i]), this, SLOT(choosenItem()));
        action->setCheckable(true);
        actions.push_back(action);

        this->setAnimation(bar, 1000);
    }

    tool_layouts[tool_layouts.size() - 1]->addWidget(bar);

    for(int i=0; i<tool_layouts.size(); i++)
        layout->addLayout(tool_layouts[i]);

}

QPropertyAnimation *Account::setAnimation(QWidget *item, int duration)
{
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
    item->setGraphicsEffect(opacity);

    QPropertyAnimation *anim = new QPropertyAnimation(opacity, "opacity");
    anim->setDuration(duration);

    anim->setStartValue(0.01);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start();

    return anim;
}

QString Account::getType(QString &path) const
{
    QStringList lst = path.split(".");

    if(lst.size() < 2) return "empty";

    return lst[lst.size() - 1];
}

QString Account::getName(QString &path) const
{
    QStringList lst = path.split("/");

    return lst[lst.size() - 1];
}

void Account::getFile()
{
    QVector<QVector<QString> > temp;
    QByteArray btemp;
    Data *data = client->createData("getFiles", client->getData().login, client->getData().password, "", temp, btemp);
    client->slotSendToServer(*data);
}


void Account::choosenItem()
{
    QAction *action = qobject_cast<QAction*> (sender());

    for(int i=0; i<actions.size(); i++)
        actions[i]->setChecked(false);

    action->setChecked(true);

    view_anim->start();
    download_anim->start();
    del_anim->start();

    view->setVisible(true);
    download->setVisible(true);
    del->setVisible(true);
}

void Account::buttonClick()
{
//    remove(layout);
//    Layout();

    QPushButton *btn = qobject_cast<QPushButton*> (sender());

    if(btn->text() == "CREATE")
    {
        creater->show();
    }
}

void Account::ParseData()
{
    if(client->getData().type == "getFiles")
    {
        remove(layout);
        Layout();
    }

    if(client->getData().type == "newFile")
    {
        if(client->getData().message == "File already exist!")
        {
            QMessageBox::critical(this, "Atention!", client->getData().message);
        }
        else
        {
            getFile();
            remove(layout);
            Layout();
        }
    }
}

void Account::sendNewFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QVector<QVector<QString> > temp;

    QByteArray block = file.readAll();
    file.close();

    Data *data = client->createData("newFile", client->getData().login, client->getData().password,
                                    getName(path), temp, block);

    client->slotSendToServer(*data);
}

void Account::remove(QLayout *layout)
{
    QLayoutItem* child;

    while(layout->count() != 0)
    {
        child = layout->takeAt(0);

        if(child->layout() != 0)
        {
            remove(child->layout());
        }

        else if(child->widget() != 0)
        {
            delete child->widget();
        }

        delete child;
    }
}
