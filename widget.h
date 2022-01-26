#ifndef WIDGET_H
#define WIDGET_H

#include "database.h"
#include "widgetforapikey.h"
#include <QCloseEvent>

#include <QMainWindow>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QSizeGrip>
#include <QSystemTrayIcon>
#include <QThread>
#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);

public:
    explicit Widget(QWidget* parent = 0);
    ~Widget();

public slots:
    void closeEvent(QCloseEvent* event);
    void showContextMenu(const QPoint&);
    void ifDbHasNotInternet();
    void ifApiKeyNotValid();
    void forMenuAddAction();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void forApiKey();

private:
    Ui::Widget* ui;
    QSystemTrayIcon* trayIcon;
    bool lineEditEnable = false;
    bool isWindowForKey = false; //создавался ли объект WidgetForApiKey
    int MouseX;
    int MouseY;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void setImages();
    void runWithSize();
    void generalConnects();
    void iconInTray();
    DataBase db;
    QLineEdit* lineCiti;
    QSqlQueryModel queryWeatherModel;
    QSqlQueryModel cityNameForWidget;
    QAction* viewWindow;
    QAction* setting;
    WidgetForApiKey* windowForApiKey;
    QString queryCityApi = "Las Vegas";
    QString cityText; //название города
};

#endif // WIDGET_H
