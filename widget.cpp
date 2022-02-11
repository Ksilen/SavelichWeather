#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    runWithSize(); //установить geometry предыдущего включения(где закрылся, там появился)
    iconInTray(); //иконка в трее
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setLayout(ui->horizontalLayout_3);
    ui->horizontalLayout_3->addWidget(new QSizeGrip(this), 0, Qt::AlignBottom | Qt::AlignRight);
    generalConnects();
}

void Widget::generalConnects()
{
    connect(&db, &DataBase::iKnowWeather, [=]() {
        queryWeatherModel.setQuery("SELECT currentTemperature,currentIcon,time FROM tableForUI;");
        cityNameForWidget.setQuery("SELECT name FROM citiName;");
        setImages();
    });
    emit db.iKnowWeather(); //сигнал, чтобы сразу установить погоду на виджет
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
        this, SLOT(showContextMenu(QPoint)));
    if (!QFile::exists("weather")) {
        connect(&db, SIGNAL(iHaveNotInternet()),
            SLOT(ifDbHasNotInternet()));
    }
    connect(&db, SIGNAL(apiKeyNotValidB()),
        this, SLOT(ifApiKeyNotValid())); //если не верно введен APIkey c OpenWeather
    db.getApiKeyFromBd(); //Взять APIkey из БД и присвоить QString apiKey
}

void Widget::setImages() //установка значений в Widget и их resize
{
    ui->temperatureNow->setStyleSheet(QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->temperatureNow->height() / 2 + 20));
    ui->temperatureNow->setText(queryWeatherModel.data(queryWeatherModel.index(0, 0)).toString());
    QPixmap imageWeatherPix(":/resourse/" + queryWeatherModel.data(queryWeatherModel.index(0, 1)).toString() + ".png");
    ui->imageWeather->setPixmap(imageWeatherPix.scaled(ui->imageWeather->size(), Qt::KeepAspectRatio));

    QString fontSizeForTemperature = QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->firstDayT->height() / 2 + 10);
    QString fontSizeForDayName = QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->firstDayName->height() / 2 + 7);

    ui->firstDayT->setStyleSheet(fontSizeForTemperature);
    ui->firstDayT->setText(queryWeatherModel.data(queryWeatherModel.index(1, 0)).toString());
    QPixmap firstDayPix(":/resourse/" + queryWeatherModel.data(queryWeatherModel.index(1, 1)).toString() + ".png");
    ui->firstDay->setPixmap(firstDayPix.scaled(ui->firstDay->size(), Qt::KeepAspectRatio));
    ui->firstDayName->setStyleSheet(fontSizeForDayName);
    ui->firstDayName->setText(QString::number(QDate::currentDate().day() + 1) + " " + QDate::shortDayName(QDate::currentDate().addDays(1).dayOfWeek()));

    ui->secondDayT->setStyleSheet(fontSizeForTemperature);
    ui->secondDayT->setText(queryWeatherModel.data(queryWeatherModel.index(2, 0)).toString());
    QPixmap secondDayPix(":/resourse/" + queryWeatherModel.data(queryWeatherModel.index(2, 1)).toString() + ".png");
    ui->secondDay->setPixmap(secondDayPix.scaled(ui->firstDay->size(), Qt::KeepAspectRatio));
    ui->secondDayName->setStyleSheet(fontSizeForDayName);
    ui->secondDayName->setText(QString::number(QDate::currentDate().day() + 2) + " " + QDate::shortDayName(QDate::currentDate().addDays(2).dayOfWeek()));

    ui->thirdDayT->setStyleSheet(fontSizeForTemperature);
    ui->thirdDayT->setText(queryWeatherModel.data(queryWeatherModel.index(3, 0)).toString());
    QPixmap thirdDayPix(":/resourse/" + queryWeatherModel.data(queryWeatherModel.index(3, 1)).toString() + ".png");
    ui->thirdDay->setPixmap(thirdDayPix.scaled(ui->firstDay->size(), Qt::KeepAspectRatio));
    ui->thirdDayName->setStyleSheet(fontSizeForDayName);
    ui->thirdDayName->setText(QString::number(QDate::currentDate().day() + 3) + " " + QDate::shortDayName(QDate::currentDate().addDays(3).dayOfWeek()));

    ui->layoutWidget->resize(this->size()); //ресайз фона
    if (lineEditEnable) { //ресайз linEdit для названия города
        this->lineCiti->setGeometry(0, ui->citi->height() / 2 - 15, ui->citi->width(), 30);
    }
    QString cityText = cityNameForWidget.data(cityNameForWidget.index(0, 0)).toString(); //название города
    if (!cityText.isEmpty() && !lineEditEnable) { //ресайз названия города
        int textSize = cityText.size();
        ui->citi->setText(cityText);
        ui->citi->setStyleSheet(QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->citi->width() / textSize + 12));
    }
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton) {
        MouseX = event->x();
        MouseY = event->y();
        int citiX = ui->citi->x();
        int citiY = ui->citi->y();
        int citiWidth = ui->citi->width();
        int citiHeight = ui->citi->height();
        if (MouseX > citiX && MouseY > citiY && MouseX < citiWidth + citiX && MouseY < citiHeight + citiY && !lineEditEnable) {
            ui->citi->setText("");
            lineEditEnable = true;
            lineCiti = new QLineEdit(ui->citi);
            lineCiti->resize(citiWidth, 30);
            lineCiti->move(0, citiHeight / 2 - 15);
            lineCiti->show();
            lineCiti->setFocus();
            lineCiti->setStyleSheet("font-size: 17pt;");
        } else {
            if (lineEditEnable) { //убирает lineEdit
                lineEditEnable = false;
                lineCiti->hide();
                setImages();
                delete lineCiti;
            }
        }
    }
}

void Widget::keyPressEvent(QKeyEvent* event) //запрос на координаты города с названием  из lineCiti
{
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        db.queryCity = this->lineCiti->text();
        queryCityApi = "https://nominatim.openstreetmap.org/search/" + db.queryCity + "?addressdetails=1&format=json&limit=1&accept-language=EN,RU";
        db.isCity.answerAPI(queryCityApi);
        lineCiti->hide();
        lineEditEnable = false;
        setImages(); //чтобы восстановить название города
        delete lineCiti;
    }
}

void Widget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton) {
        move(event->globalX() - MouseX,
            event->globalY() - MouseY);
    }
}

void Widget::resizeEvent(QResizeEvent* event)
{
    setImages();
    QWidget::resizeEvent(event);
}

void Widget::runWithSize() //берет данные о положении виджета при закрытии и ставит их
{
    QSqlQueryModel startSize;
    startSize.setQuery("SELECT appX,appY,appWidth,appHeight FROM timeNow;");
    if (!startSize.data(startSize.index(0, 0)).isNull()) {
        this->setGeometry(startSize.data(startSize.index(0, 0)).toInt(), startSize.data(startSize.index(0, 1)).toInt(),
            startSize.data(startSize.index(0, 2)).toInt(), startSize.data(startSize.index(0, 3)).toInt());
    }
}

void Widget::closeEvent(QCloseEvent* event) //закрытие приложения
{
    event->ignore();
    QThreadPool::globalInstance()->waitForDone(); //подождать завершения другого потока, а вдруг данные заносятся в БД
    db.closeAndWrite(this->width(), this->height(), this->x(), this->y());
    db.closeAndBackFirstTemperature();
    db.blockSignals(true); //чтобы не было сигнала iKnowWeather() и не вызывался setImages()
    if (isWindowForKey) { //если окно для ввода APIkey открыто, а программа закрывается
        windowForApiKey->close();
    }
    event->accept();
}

void Widget::ifDbHasNotInternet()
{
    ui->citi->setStyleSheet(QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->citi->width() / 10 + 12));
    ui->citi->setText("Нет Сети...");
}

void Widget::ifApiKeyNotValid()
{
    ui->citi->setStyleSheet(QString("font-size: %1px;background-color: rgba(255, 170, 127,0);").arg(ui->citi->width() / 10 + 12));
    ui->citi->setText("Неверный API Key");
    // db.isApiKeyValid = false;
    db.whenApiKeyValid();
}

void Widget::showContextMenu(const QPoint& pos) //меню при нажатии правой кнопки мыши по виджету
{
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Свернуть окно   ", this, SLOT(forMenuAddAction()));
    myMenu.addAction("Выход ", this, SLOT(close()));
    myMenu.setStyleSheet("QMenu::item {padding: 2px 5px 2px 2px;}"
                         "QMenu::item:selected {background-color: rgb(157, 157, 157);"
                         "color: rgb(255, 255, 255);}");
    myMenu.exec(globalPos);
}

void Widget::forMenuAddAction() //пункт меню "Свернуть окно   "
{
    this->hide();
    viewWindow->setVisible(true);
}

void Widget::iconInTray() //иконка в трее
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resourse/weather.ico"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    QMenu* menuForTray = new QMenu(this);
    setting = new QAction(trUtf8("Ввести ключ API OpenWeather"), this);
    viewWindow = new QAction(trUtf8("Развернуть окно"), this);
    QAction* quitAction = new QAction(trUtf8("Выход"), this);
    connect(setting, SIGNAL(triggered()), this, SLOT(forApiKey()));
    connect(viewWindow, &QAction::triggered, [=]() {
        viewWindow->setVisible(false);
        this->show();
    });
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    menuForTray->addAction(setting);
    menuForTray->addAction(viewWindow);
    menuForTray->addAction(quitAction);
    trayIcon->setContextMenu(menuForTray);
    trayIcon->show();
    viewWindow->setVisible(false);
}

void Widget::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) //нажатие иконки в трее
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if (!this->isVisible()) {
            viewWindow->setVisible(false);
            this->show();
            this->activateWindow();
        } else {
            viewWindow->setVisible(true);
            this->hide();
        }
        break;
    default:
        break;
    }
}

void Widget::forApiKey() //окно для ввода API OpenWeather
{
    windowForApiKey = new WidgetForApiKey;
    isWindowForKey = true; //создался объект WidgetForApiKey
    setting->setEnabled(false);
    connect(windowForApiKey, &WidgetForApiKey::iAmClosing, [=]() { //закрытие окна на 'Х'
        isWindowForKey = false; //удалился объект WidgetForApiKey, удалять при закрытии главного окна НЕ надо
        setting->setEnabled(true);
    });
    connect(windowForApiKey->keyOk, &QPushButton::clicked, [=]() { //нажат OK после ввода ApiKey
        if (!windowForApiKey->lineForKey->text().isEmpty())
            db.apiKey = windowForApiKey->lineForKey->text();
        this->hide(); //чтобы не закрывалась программа(главное окно)после нажатия OK
        this->show();
        windowForApiKey->close();
        db.whatCity();
    });
    connect(windowForApiKey, &WidgetForApiKey::keyEntered, [=]() { //нажат Enter после ввода ApiKey
        if (!windowForApiKey->lineForKey->text().isEmpty())
            db.apiKey = windowForApiKey->lineForKey->text();
        this->hide();
        this->show();
        windowForApiKey->close();
        db.whatCity();
    });
}
Widget::~Widget()
{
    delete ui;
}
