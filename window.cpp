#include "window.h"

Window::Window(QWidget *parent): QDialog(parent)
{
    Layout();

    client = new Client;
    connect(client, SIGNAL(readyData()), this, SLOT(ParseData()));
}

Window::~Window()
{
    delete client;
    delete logo;
    delete account;
}

void Window::Layout()
{
    logo = new Logo; // creating logo
    logo->setCursor(Qt::PointingHandCursor);
    connect(logo, SIGNAL(clicked()), this, SLOT(logoClicked()));

    informer = new QLabel("test");

    login = edit("e-mail");
    pass = edit("password");
    pass->setEchoMode(QLineEdit::Password);

    QPushButton *sign_in = button("Sign in");
    QPushButton *sign_out = button("Sign up");

    connect(sign_in, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(sign_out, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

    QHBoxLayout *sign_buttons_layout = new QHBoxLayout;
    sign_buttons_layout->addWidget(sign_in);
    sign_buttons_layout->addWidget(sign_out);

    QVBoxLayout *sign_layout = new QVBoxLayout;
    sign_layout->setAlignment(Qt::AlignCenter);
    sign_layout->addWidget(informer);
    sign_layout->addWidget(login);
    sign_layout->addWidget(pass);
    sign_layout->addLayout(sign_buttons_layout);

    sign_box = new QGroupBox;
    sign_box->setLayout(sign_layout);
    sign_box->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);
    layout->addWidget(sign_box);

    logo_anim = setAnimation(logo, 1000);
    logo_anim->start();

    sign_anim = setAnimation(sign_box, 3000);

    QPalette pal;
    pal.setColor(this->backgroundRole(), QColor(84, 102, 201));

    this->setPalette(pal);
    this->setLayout(layout);
    this->setFixedSize(600, 400);
    this->setWindowTitle("White cloud");
}

QLineEdit *Window::edit(QString title)
{
    QLineEdit *temp = new QLineEdit(title);
    temp->setFixedWidth(200);
    temp->setFont(QFont("Ubuntu", 12, QFont::Cursive));

    return temp;
}

QPushButton *Window::button(QString title)
{
    QPushButton *temp = new QPushButton(title);
    temp->setStyleSheet("QPushButton { background: rgb(163, 142, 230); color: black; }"
                        "QPushButton:hover { background: rgb(84, 226, 150); color: white; }");

    temp->setFont(QFont("Ubuntu", 10, QFont::Bold));
    temp->setCursor(Qt::PointingHandCursor);

    return temp;
}

QPropertyAnimation *Window::setAnimation(QWidget *item, int duration)
{
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
    item->setGraphicsEffect(opacity);

    QPropertyAnimation *anim = new QPropertyAnimation(opacity, "opacity");
    anim->setDuration(duration);

    anim->setStartValue(0.01);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);

    return anim;
}

QString Window::validLogin(QString text)
{

    if(text.indexOf(".") != -1 && text.indexOf("@") != -1 )
    {
        text.remove(text.indexOf("."), 1);
        text.remove(text.indexOf("@"), 1);
    }

//    qDebug() << text;

    return text;
}

void Window::logoClicked()
{
    logo->hide();
    sign_box->show();
    sign_anim->start();
}

void Window::buttonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*> (sender());
    QVector<QVector<QString> > temp;

    if(btn->text() == "Sign in")
    {
        if(!login->text().isEmpty() && !pass->text().isEmpty())
        {
            Data *data = client->createData("sign in", validLogin(login->text()), pass->text(), "", temp);
            client->slotSendToServer(*data);
        }
        else informer->setText("<font color=DarkRed><b>Input all fields!</b></font>");
    }

    if(btn->text() == "Sign up")
    {
        if(!login->text().isEmpty() && !pass->text().isEmpty())
        {
            Data *data = client->createData("sign up", validLogin(login->text()), pass->text(), "", temp);
            client->slotSendToServer(*data);
        }
        else informer->setText("<font color=DarkRed><b>Input all fields!</b></font>");
    }
}

void Window::ParseData()
{
    if(client->getData().type == "sign in")
    {
        if(client->getData().message == "User not exist!")
            informer->setText("<font color=DarkRed><b>User not exist!</b></font>");

        if(client->getData().message == "Inccorect password!")
            informer->setText("<font color=DarkRed><b>Inccorect password!</b></font>");

        if(client->getData().message == "log in")
        {
            account = new Account(client->getData().login, client);

            account->show();
            this->hide();
        }
    }

    if(client->getData().type == "sign up")
    {
        if(client->getData().message == "User already exist!")
            informer->setText("<font color=DarkRed><b>User already exist!</b></font>");

        if(client->getData().message == "Registration was successful!")
            informer->setText("<font color=DarkRed><b>Registration was successful!</b></font>");
    }
}
