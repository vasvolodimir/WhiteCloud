#include "window.h"

Window::Window(QWidget *parent)
    : QDialog(parent),
      m_client(nullptr),
      m_logo(nullptr),
      m_account(nullptr),
      m_informer(nullptr),
      m_login(nullptr),
      m_password(nullptr),
      m_sign_box(nullptr),
      m_logo_anim(nullptr),
      m_sign_anim(nullptr)
{
    layout();

    if (m_logo_anim)
        m_logo_anim->start();

    m_client = new Client(this);
    connect(m_client, SIGNAL(readyData()), this, SLOT(parseData()));
}

Window::~Window()
{
    delete m_client;
    delete m_logo;
    delete m_account;
}

void Window::notify(QString msg)
{
    m_informer->setText("<font color=DarkRed><b>" + msg + "</b></font>");
}

void Window::layout()
{
    QPalette pal;
    QVBoxLayout *layout = new QVBoxLayout;
    QVBoxLayout *sign_layout = new QVBoxLayout;
    QHBoxLayout *sign_buttons_layout = new QHBoxLayout;

    pal.setColor(this->backgroundRole(), QColor(84, 102, 201));
    layout->setAlignment(Qt::AlignCenter);
    sign_layout->setAlignment(Qt::AlignCenter);

    if (createLogo(&m_logo, SLOT(logoClicked())))
        layout->addWidget(m_logo);

    if (createInformer(&m_informer))
        sign_layout->addWidget(m_informer);

    if (createLineEdit("e-mail", QLineEdit::Normal, &m_login))
        sign_layout->addWidget(m_login);

    if (createLineEdit("password", QLineEdit::Password, &m_password))
        sign_layout->addWidget(m_password);

    if (QPushButton *btn = createPushButton("Sign in", &btn))
        sign_buttons_layout->addWidget(btn);

    if (QPushButton *btn = createPushButton("Sign up", &btn))
        sign_buttons_layout->addWidget(btn);

    sign_layout->addLayout(sign_buttons_layout);

    if (createGroupBox(sign_layout, &m_sign_box))
    {
        m_sign_box->hide();
        layout->addWidget(m_sign_box);
    }

    createAnimation(m_logo, 1000, &m_logo_anim);
    createAnimation(m_sign_box, 3000, &m_sign_anim);

    this->setPalette(pal);
    this->setLayout(layout);
    this->setFixedSize(600, 400);
    this->setWindowTitle("White cloud");
}

QLineEdit *Window::createLineEdit(QString title, QLineEdit::EchoMode mode,
  QLineEdit **edit)
{
    *edit = new QLineEdit(this);

    (*edit)->setFixedWidth(200);
    (*edit)->setFont(QFont("Ubuntu", 12, QFont::Cursive));
    (*edit)->setEchoMode(mode);
    (*edit)->setPlaceholderText(title);

    return *edit;
}

QPushButton *Window::createPushButton(QString title, QPushButton **btn)
{
    *btn = new QPushButton(this);

    (*btn)->setStyleSheet("QPushButton { background: rgb(163, 142, 230); "
        "color: black; } QPushButton:hover { background: rgb(84, 226, 150);"
        "color: white; }");

    (*btn)->setFont(QFont("Ubuntu", 10, QFont::Bold));
    (*btn)->setCursor(Qt::PointingHandCursor);
    (*btn)->setText(title);

    connect(*btn, SIGNAL(clicked(bool)), this, SLOT(handleBtnClick()));

    return *btn;
}

QGroupBox *Window::createGroupBox(QLayout *layout, QGroupBox **group)
{
    *group = new QGroupBox(this);

    if (layout)
        (*group)->setLayout(layout);

    return *group;
}

bool Window::isCredentialsValid()
{
    if (m_login->text().isEmpty() || m_password->text().isEmpty())
    {
        notify("Incorrect credentials!");
        return false;
    }

    return true;
}

QPropertyAnimation *Window::createAnimation(QWidget *item, int duration,
   QPropertyAnimation **animation)
{
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    item->setGraphicsEffect(opacity);

    *animation = new QPropertyAnimation(opacity, "opacity");
    (*animation)->setDuration(duration);

    (*animation)->setStartValue(0.01);
    (*animation)->setEndValue(1.0);
    (*animation)->setEasingCurve(QEasingCurve::InOutQuad);

    return *animation;
}

QString Window::serializeLogin(QString text) const
{

    if(text.indexOf(".") != -1 && text.indexOf("@") != -1 )
    {
        text.remove(text.indexOf("."), 1);
        text.remove(text.indexOf("@"), 1);
    }

    return text;
}

Logo *Window::createLogo(Logo **logo, const char *slot)
{
    *logo = new Logo(this);
    (*logo)->setCursor(Qt::PointingHandCursor);
    connect(*logo, SIGNAL(clicked()), this, slot);

    return *logo;
}

QLabel *Window::createInformer(QLabel **informer)
{
    *informer = new QLabel(this);

    return *informer;
}

void Window::logoClicked()
{
    if (!m_logo || !m_sign_box || !m_sign_anim)
        LOG_EXIT("Invalid pointer!", );

    m_logo->hide();
    m_sign_box->show();
    m_sign_anim->start();
}

void Window::handleBtnClick()
{
    data_t *data;
    QByteArray btemp;
    QVector<QVector<QString> > temp; // as a temp
    QPushButton *btn = qobject_cast<QPushButton*> (sender());

    if (!btn)
        LOG_EXIT("Invalid pointer!", );

    if (!isCredentialsValid())
        LOG_EXIT("Credentials is invalid!", );

    if (btn->text() == "Sign in")
    {
        data = m_client->createData("sign in", serializeLogin(m_login->text()),
               m_password->text(), "", temp, btemp);
        m_client->slotSendToServer(*data);
    }

    if (btn->text() == "Sign up")
    {
        data = m_client->createData("sign up", serializeLogin(m_login->text()),
                                  m_password->text(), "", temp, btemp);
        m_client->slotSendToServer(*data);
    }
}

void Window::parseData()
{
    if (m_client->getData().type == "sign in")
    {
        if (m_client->getData().message == "User not exist!")
            m_informer->setText("<font color=DarkRed><b>User not exist!</b></font>");

        if (m_client->getData().message == "Inccorect password!")
            m_informer->setText("<font color=DarkRed><b>Inccorect password!</b></font>");

        if (m_client->getData().message == "log in")
        {
            m_account = new Account(m_client->getData().login, m_client);

            m_account->show();
            this->hide();
        }
    }

    if (m_client->getData().type == "sign up")
    {
        if (m_client->getData().message == "User already exist!")
            m_informer->setText("<font color=DarkRed><b>User already exist!</b></font>");

        if (m_client->getData().message == "Registration was successful!")
            m_informer->setText("<font color=DarkRed><b>Registration was successful!</b></font>");
    }
}
