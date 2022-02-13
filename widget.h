#ifndef WIDGET_H
#define WIDGET_H
#include "database.h"
#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
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
    ~Widget() override;
public slots:
    void closeEvent(QCloseEvent* event);
    void showContextMenu(const QPoint&);
    void ifDbHasNotInternet();
    void ifApiKeyNotValid();
    void forMenuAddAction();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void forKey();
    void settingsForKey();

private:
    Ui::Widget* ui;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void setImages();
    void runWithSize();
    void generalConnects();
    void iconInTray();
    bool lineEditEnable = false;
    bool firstApiKey = true;
    bool windowApiIs = false; //для CloseEvent, чтоб при закрытии закрыл окно QDialog
    int MouseX;
    int MouseY;
    DataBase db;
    QSystemTrayIcon* trayIcon;
    QDialog keyDialog;
    QLineEdit* lineCiti;
    QSqlQueryModel queryWeatherModel;
    QSqlQueryModel cityNameForWidget;
    QAction* viewWindow;
    QAction* setting;
    QString queryCityApi = "Las Vegas";
    QString cityText; //название города
    QLineEdit* lineForKey;
    QPushButton* keyOk;
};

#endif // WIDGET_H
