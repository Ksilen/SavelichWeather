#ifndef WIDGETFORAPIKEY_H
#define WIDGETFORAPIKEY_H
#include <QCloseEvent>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
class WidgetForApiKey : public QWidget {
    Q_OBJECT
public:
    WidgetForApiKey();
    virtual ~WidgetForApiKey();
    QPushButton* keyOk;
    QLineEdit* lineForKey;

public slots:
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:
    void iAmClosing(); //сигнал о закрытии окна
    void keyEntered(); //сигнал о нажатии Enter, после ввода ключа

private:
    void inTheWindow();
};

#endif // WIDGETFORAPIKEY_H
