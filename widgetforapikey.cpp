#include "widgetforapikey.h"

WidgetForApiKey::WidgetForApiKey()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true); //удаление при закрытии
    this->setWindowFlags(Qt::Window);
    this->setGeometry(100, 100, 430, 150);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowIcon(QIcon(":/resourse/weather.ico"));
    this->setWindowTitle("Ввести ключ API к OpenWeather");
    inTheWindow();
    this->show();
}

void WidgetForApiKey::inTheWindow()
{
    QLabel* textForHint = new QLabel(this);
    textForHint->setGeometry(10, 10, 410, 40);
    textForHint->setFont(QFont("Times", 11));
    textForHint->setText("Чтобы получить ключ для API зарегестрируйтесь на сайте <br> "
                         "<a href=\"https://home.openweathermap.org/users/sign_up\">"
                         "openweathermap.org</a>");
    textForHint->setTextFormat(Qt::RichText);
    textForHint->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textForHint->setOpenExternalLinks(true);
    lineForKey = new QLineEdit(this);
    lineForKey->setGeometry(10, 60, 410, 30);
    lineForKey->setFont(QFont("Times", 11));
    lineForKey->setStyleSheet("border: 0.5px solid #555;");
    lineForKey->setFocus();
    keyOk = new QPushButton("OK", this);
    keyOk->setGeometry(320, 110, 100, 30);
    keyOk->setStyleSheet("QPushButton{color: #333;border: 1px solid #555;border-radius: 5px;border-style: outset;"
                         "background-color: rgb(208, 208, 201);padding: 5px;}"
                         "QPushButton:hover {background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4,"
                         "radius: 1.35, stop: 0 #fff, stop: 1 #bbb);}"
                         "QPushButton:pressed {border-style: inset;"
                         "background: qradialgradient(cx: 0.4, cy: -0.1, fx: 0.4, fy: -0.1,"
                         "radius: 1.35, stop: 0 #fff, stop: 1 #ddd);}");
}

void WidgetForApiKey::keyPressEvent(QKeyEvent* event) //если нажат Enter после ввода ключа для запроса
{
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        emit keyEntered();
    }
}

void WidgetForApiKey::closeEvent(QCloseEvent* event)
{
    emit iAmClosing();
    event->accept();
}
