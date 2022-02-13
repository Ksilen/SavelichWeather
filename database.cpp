#include "database.h"
DataBase::DataBase() //проверяет наличие БД
{
    settingsForBd();
    if (QFile::exists("weather")) {
        openDataBase(); //если есть открыть
        letsWork();
    } else {
        firstIsInternet(); //если нет проверить, а есть ли сеть?
    }
}
void DataBase::settingsForBd()
{
    connect(&netR, SIGNAL(apiKeyNotValid()), this, SIGNAL(apiKeyNotValidB()));
}
void DataBase::ifFirstCreateBdAndHasNotInternet()
{
    disconnect(&netR.netManage, SIGNAL(onlineStateChanged(bool)),
        this, SLOT(ifFirstCreateBdAndHasNotInternet()));
    createDataBase(); //если нет создать
    whatCity();
}
void DataBase::letsWork()
{
    connectWithNetrequest(); //сигналы от запросов к API
    timeRequest(); //взависимости от времени дает разные запросы к API
}
void DataBase::createDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("weather");
    db.open();
    createWeatherTable(); //делает таблицы в БД, если БД нет
}
void DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("weather");
    db.open();
}
void DataBase::createWeatherTable() //делает таблицы в БД если её не было
{
    QSqlQuery q1(db);
    q1.exec("CREATE TABLE timeNow("
            "time,"
            "appWidth INTEGER," //таблица с текущим временем
            "appHeight INTEGER," //и размерами и положением приложения
            "appX INTEGER,"
            "appY INTEGER,"
            "appid VARCHAR(40));");
    q1.exec("INSERT INTO timeNow(appWidth,appHeight) VALUES(461,320);");
    q1.exec("CREATE TABLE daysWeatherTable(" //таблица с погодой на 7 дней вперед
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "time,"
            "currentIcon VARCHAR(10),"
            "currentTemperature integer);");
    q1.exec("CREATE TABLE hours48(" //таблица с  погодой на 48 часов вперед
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "time,"
            "currentIcon varchar(10),"
            "currentTemperature INTEGER);");
    q1.exec("CREATE TABLE tableForUI(" //таблица для отображения в UI
            "ID INTEGER, "
            "currentTemperature INTEGER, "
            "currentIcon VARCHAR(10), "
            "time);");
    for (int i = 1; i < 5; ++i) {
        q1.prepare("INSERT INTO tableForUI(ID) VALUES(:inc);");
        q1.bindValue(":inc", i);
        q1.exec();
    }
    q1.exec("CREATE TABLE citiName(" //таблица для названия города и его координат
            "ID integer,"
            "name VARCHAR(40),"
            "nameEN VARCHAR(40),"
            "lat INTEGER,"
            "lon INTEGER);");
    q1.exec("INSERT INTO citiName(ID) VALUES(1);");
}
void DataBase::whatCity() //при первом запуске запрос города по ip
{
    connect(&netR, SIGNAL(replyDone()), SLOT(firstCitiName()));
    netR.answerAPI("http://api.sypexgeo.net/");
}
void DataBase::firstCitiName() //парсинг города по ip, если города нет БД при первом запуске программы
{
    QSqlQuery q1(db);
    QJsonValue answerCurrentCiti = netR.jDoc.object().value("city");
    QJsonObject answerOjectCurrentCiti = answerCurrentCiti.toObject();
    QString cityName = answerOjectCurrentCiti["name_ru"].toString();
    QString cityNameEN = answerOjectCurrentCiti["name_en"].toString();
    double lat = answerOjectCurrentCiti["lat"].toDouble();
    double lon = answerOjectCurrentCiti["lon"].toDouble();
    q1.prepare("UPDATE citiName SET name = :cityName, "
               "nameEN = :cityNameEN, lat = :lat, lon= :lon WHERE ID = 1;");
    q1.bindValue(":cityName", cityName);
    q1.bindValue(":cityNameEN", cityNameEN);
    q1.bindValue(":lat", lat);
    q1.bindValue(":lon", lon);
    q1.exec();
    city = cityNameEN;
    cityCoordinate = "lat=" + QString::number(lat) + "&lon=" + QString::number(lon);
    disconnect(&netR, SIGNAL(replyDone()), this, SLOT(firstCitiName()));
    countTime = 0;
    letsWork(); //запуск основных функций
}
void DataBase::writeCurrentTime() //записать текущую дату и время
{
    QSqlQuery q1(db);
    q1.prepare("UPDATE timeNow SET time=:currentDateTime;");
    q1.bindValue(":currentDateTime", QDateTime::currentDateTime());
    q1.exec();
}
void DataBase::writeCurrentWeather()
{
    writeCurrentTime();
    if (countTime != 1) {
        littleRequest(); //ответ с данными раз в 10 мин "Current Weather Data"
    } else {
        bigRequest(); //ответ с данными на 7 дней раз в 1 час "One Call API"
    }
    emit iKnowWeather(); // update UI
}
void DataBase::firstIsInternet() //используется один раз, если первый раз вкл программу, а сети нет
{
    connect(&netR, SIGNAL(apiKeyNotValid()), this, SIGNAL(apiKeyNotValidB()));
    if (netR.netManage.isOnline()) {
        ifFirstCreateBdAndHasNotInternet();
    } else {
        connect(&netR.netManage, SIGNAL(onlineStateChanged(bool)),
            this, SLOT(ifFirstCreateBdAndHasNotInternet()));
    }
    QTimer::singleShot(4000, this, SIGNAL(iHaveNotInternet()));
}
void DataBase::parseCityName() //"выуживает" название города
{
    if (!isCity.jDoc.array().isEmpty()) {
        QSqlQuery q3(db);
        QJsonObject isCityObject = isCity.jDoc.array().at(0).toObject();
        QJsonObject isCityAdress = isCityObject["address"].toObject();
        QString isCityName = isCityAdress["city"].toString();
        if (isCityName.isEmpty())
            isCityName = isCityAdress["town"].toString();
        if (isCityName.isEmpty())
            isCityName = isCityAdress["county"].toString();
        if (isCityName.isEmpty())
            isCityName = isCityAdress["state"].toString();
        if (isCityName.contains("городской округ "))
            isCityName.remove("городской округ ");
        if (isCityName.isEmpty())
            isCityName = queryCity;
        QString cityLat = isCityObject["lat"].toString();
        QString cityLon = isCityObject["lon"].toString();
        q3.prepare("UPDATE citiName SET name=:cityName,"
                   "nameEN=:cityNameEN,lat=:lat,lon=:lon WHERE ID=1;");
        q3.bindValue(":cityName", isCityName);
        q3.bindValue(":cityNameEN", isCityName);
        q3.bindValue(":lat", cityLat);
        q3.bindValue(":lon", cityLon);
        q3.exec();
        city = isCityName;
        cityCoordinate = "lat=" + cityLat + "&lon=" + cityLon;
        countTime = 0; //сброс запросного времени
        timeRequest(); //включение таймера запросов
    }
}
void DataBase::connectWithNetrequest() //реакция на наличие ответа в запросе к API
{
    connect(&netR, SIGNAL(replyDone()), SLOT(writeCurrentWeather()));
    connect(&netR, SIGNAL(replyNotDone()), SLOT(ifHasNotInternet()));
    connect(&isCity, SIGNAL(replyDone()), SLOT(parseCityName())); //для ответа на наличие города в мире
    connect(this, SIGNAL(requestDone()), SIGNAL(iKnowWeather()));
    connect(&timerOne, SIGNAL(timeout()), this, SLOT(timeRequest())); //включает таймер запроса к API каждые 10 мин
}
void DataBase::littleRequest() //парсинг запроса "Current Weather Data"
{
    QtConcurrent::run([=]() {
        QSqlQuery q1(db);
        q1.prepare("UPDATE tableForUI SET currentTemperature = :curTemp,"
                   "currentIcon = :curIcon time=:timeFromRequest WHERE ID = 1;");
        QJsonValue answerCurrent = netR.jDoc.object().value("main"); //температура сейчас
        QJsonObject answerOjectCurrent = answerCurrent.toObject();
        double curTemp = answerOjectCurrent["temp"].toDouble();
        QJsonArray curIcon = netR.jDoc.object()["weather"].toArray(); //номер картинки на температуру
        QJsonObject curIconObject = curIcon.at(0).toObject();
        QString iconWeather = curIconObject["icon"].toString();
        int intCurTemp = static_cast<int>(curTemp);
        absTemperature(intCurTemp, curTemp);
        q1.bindValue(":curTemp", intCurTemp);
        q1.bindValue(":curIcon", iconWeather);
        q1.bindValue(":timeFromRequest", QDateTime::currentDateTime());
        q1.exec();
        emit requestDone();
    });
}
void DataBase::absTemperature(int& intCurTemp, double& curTemp)
{
    if (curTemp > 0 && (curTemp - intCurTemp) > 0.50) //если t = 22.6 сделать 23
        intCurTemp += 1;
    if (curTemp < 0 && (-1) * (curTemp - intCurTemp) > 0.50) //если t = -22.6 сделать -23
        intCurTemp -= 1;
}
void DataBase::bigRequest() //парсинг запроса "One Call API"
{
    QtConcurrent::run([=]() {
        QSqlQuery q1(db);
        q1.prepare("UPDATE tableForUI SET currentTemperature=:curTemp,"
                   "currentIcon=:curIcon, time=:timeFromRequest WHERE ID=1;"); //данные t сейчас
        QJsonValue answerCurrent = netR.jDoc.object().value("current"); //температура сейчас
        QJsonObject answerOjectCurrent = answerCurrent.toObject();
        double curTemp = answerOjectCurrent["temp"].toDouble();
        QJsonArray curIcon = answerOjectCurrent["weather"].toArray(); //номер картинки на температуру
        QJsonObject curIconObject = curIcon.at(0).toObject();
        QString iconWeather = curIconObject["icon"].toString();
        int intCurTemp = static_cast<int>(curTemp);
        absTemperature(intCurTemp, curTemp);
        q1.bindValue(":curTemp", intCurTemp);
        q1.bindValue(":curIcon", iconWeather);
        q1.bindValue(":timeFromRequest", QDateTime::currentDateTime());
        q1.exec();
        q1.exec("DELETE FROM daysWeatherTable;"); //средняя температура на 7 дней вперед
        q1.exec("UPDATE sqlite_sequence SET seq=0  WHERE Name='daysWeatherTable';");
        QJsonValue answerDailyCurrent = netR.jDoc.object().value("daily");
        QJsonArray answerDailyArrayCurrent = answerDailyCurrent.toArray();
        int dayData; //дата дней
        QDateTime date7;
        bool thisDay = true; //чтобы исключить данные за сегодня
        int dayNumber = 0; //чтобы включить первые 3 дня в таблицу tableForUI
        foreach (const QJsonValue& valueDays, answerDailyArrayCurrent) {
            if (thisDay) { //чтобы исключить данные за сегодня
                thisDay = false;
                continue;
            }
            q1.prepare("insert into daysWeatherTable(time,currentTemperature,currentIcon) "
                       "values(:dataTime,:curTemp,:curIcon);");
            if (valueDays.isObject()) {
                QJsonObject objDay = valueDays.toObject();
                QJsonArray forIcon = objDay["weather"].toArray();
                QJsonObject forIconObject = forIcon.at(0).toObject();
                iconWeather = forIconObject["icon"].toString();
                QJsonObject tempDaily = objDay["temp"].toObject();
                double dayT = tempDaily["day"].toDouble();
                double eveT = tempDaily["eve"].toDouble();
                double mornT = tempDaily["morn"].toDouble();
                double nightT = tempDaily["night"].toDouble();
                dayData = objDay["dt"].toInt();
                date7 = QDateTime::fromTime_t(dayData);
                if ((dayT < 0 && eveT < 0 && mornT < 0 && nightT < 0) || //если знаки темпратуры одинаковы
                    (dayT > 0 && eveT > 0 && mornT > 0 && nightT > 0)) {
                    curTemp = (dayT + eveT + mornT + nightT) / 4;
                } else {
                    double plus = 0; //если разные знаки, складывают отдельно положительные и отрицательные
                    double minus = 0; //значения. Из большего вычитают меньшее.
                    double average = 0; //Ставят знак большей суммы.
                    plus += dayT; //Делят на число измерений. Получают среднюю температуру.
                    if (dayT > 0) {
                    } else {
                        minus += dayT;
                    }
                    if (eveT > 0) {
                        plus += eveT;
                    } else {
                        minus += eveT;
                    }
                    if (mornT > 0) {
                        plus += mornT;
                    } else {
                        minus += mornT;
                    }
                    if (nightT > 0) {
                        plus += nightT;
                    } else {
                        minus += nightT;
                    }
                    if (plus > ((-1) * minus)) {
                        average = plus - ((-1) * minus);
                        if (average < 0)
                            average *= (-1); //знак большей суммы.
                    } else {
                        average = ((-1) * minus) - plus;
                        if (average > 0)
                            average *= (-1);
                    }
                    curTemp = average / 4; //Делят на число измерений
                }
            }
            int intCurTemp = static_cast<int>(curTemp);
            absTemperature(intCurTemp, curTemp);
            q1.bindValue(":dataTime", date7); //дату в БД
            q1.bindValue(":curTemp", intCurTemp);
            q1.bindValue(":curIcon", iconWeather);
            q1.exec();
            if (dayNumber < 3) { //данные на 3 дня вперед в таблицу tableForUI
                q1.prepare("UPDATE tableForUI SET currentTemperature=:curTemp, "
                           "currentIcon=:curIcon, "
                           "time=:dataTime "
                           "WHERE ID=:dayNumber;");
                q1.bindValue(":dayNumber", dayNumber + 2);
                q1.bindValue(":dataTime", date7);
                q1.bindValue(":curTemp", intCurTemp);
                q1.bindValue(":curIcon", iconWeather);
                q1.exec();
                dayNumber++;
            }
        }
        q1.exec("DELETE FROM hours48;"); //средняя температура на 48 часов вперед
        q1.exec("UPDATE sqlite_sequence SET seq=0  WHERE Name='hours48';");
        QJsonValue answerHourly = netR.jDoc.object().value("hourly");
        QJsonArray answerHourlyArray = answerHourly.toArray();
        foreach (const QJsonValue& valueHour, answerHourlyArray) {
            q1.prepare("insert into hours48(time,currentTemperature,currentIcon) "
                       "values(:dataTime,:curTemp,:curIcon);");
            if (valueHour.isObject()) {
                QJsonObject objHour = valueHour.toObject();
                QJsonArray forIcon = objHour["weather"].toArray();
                QJsonObject forIconObject = forIcon.at(0).toObject();
                iconWeather = forIconObject["icon"].toString();
                double tempHour = objHour["temp"].toDouble();
                intCurTemp = static_cast<int>(tempHour);
                absTemperature(intCurTemp, tempHour);
                dayData = objHour["dt"].toInt();
                date7 = QDateTime::fromTime_t(dayData);
            }
            q1.bindValue(":dataTime", date7); //дату в БД
            q1.bindValue(":curTemp", intCurTemp);
            q1.bindValue(":curIcon", iconWeather);
            q1.exec();
        }
        emit requestDone();
    });
}
void DataBase::timeRequest() //запрос к openweather
{
    QSqlQuery q1(db);
    if (countTime == 0) {
        q1.exec("SELECT lat,lon FROM citiName WHERE ID=1;");
        q1.first();
        cityCoordinate = "lat=" + q1.value(0).toString() + "&lon=" + q1.value(1).toString();
        stringForRequest = QString("api.openweathermap.org/data/2.5/onecall?" //раз в час "One Call API" запрос
            + cityCoordinate + "&exclude=minutely,alerts&units=metric"
                               "&appid="
            + apiKey);
        countTime++;
    } else {
        q1.exec("SELECT nameEN FROM citiName WHERE ID=1;");
        q1.first();
        city = q1.value(0).toString();
        stringForRequest = "api.openweathermap.org/data/2.5/weather?"
            + cityCoordinate + "&units=metric"
                               "&appid="
            + apiKey;
        countTime++;
    }
    if (countTime == 5)
        countTime = 0; //ну вот и прошел 1 час, "One Call API" запрос опять
    if (!netR.netManage.isOnline()) {
        timerOne.stop(); //остановка счетчика времени для запросов
        ifHasNotInternet();
    } else {
        timerOne.start(tenMinutes);
        netR.answerAPI(stringForRequest); //запрос к API
    }
}
void DataBase::getApiKeyFromBd()
{
    QSqlQuery q1(db);
    q1.exec("SELECT appid FROM timeNow;");
    q1.first();
    if (!q1.value(0).toString().isEmpty()) {
        apiKey = q1.value(0).toString();
        q1.exec("SELECT name FROM citiName;");
        q1.first();
        if (!q1.value(0).toString().isEmpty())
            netR.answerAPI(q1.value(0).toString()); //установить город из БД
    }
}
void DataBase::closeAndWrite(int appW, int appH, int X, int Y) //сохраняет позицию виджета и закрывает БД
{
    QSqlQuery q1(db);
    q1.prepare("UPDATE timeNow SET appWidth = :appW, appHeight = :appH, appX = :X,appY = :Y;");
    q1.bindValue(":appW", appW);
    q1.bindValue(":appH", appH);
    q1.bindValue(":X", X);
    q1.bindValue(":Y", Y);
    q1.exec();
}
void DataBase::closeAndBackFirstTemperature()
{
    QSqlQuery q1(db);
    q1.exec("SELECT time FROM timeNow;");
    q1.first();
    QDateTime firstDateTime = q1.value(0).toDateTime();
    q1.prepare("UPDATE tableForUI SET time=:timeFromTimeNow WHERE ID=1;");
    q1.bindValue(":timeFromTimeNow", firstDateTime);
    q1.exec();
    db.close();
}
void DataBase::ifHasNotInternet() //Если кончился интернет меняет условия программы
{
    countTime = 0; //сброс запросного времени, чтобы при появлении интернета обновить всю БД
    connect(&netR.netManage, SIGNAL(onlineStateChanged(bool)),
        this, SLOT(afterNoNet())); //как только интернет появился снова запросы погоды каждые 10 мин
    QSqlQuery firstTimeQuery("SELECT time FROM timeNow;"); //время и дата последнего обновления
    firstTimeQuery.first();
    QDateTime firstTime = firstTimeQuery.value(0).toDateTime(); //время и дата из БД
    QTime baseTime = firstTime.time(); //время
    int baseMin = baseTime.minute(); //кол-во минут
    QDateTime secondTime = QDateTime::currentDateTime(); //время сейчас сравнить с временем последнего обновления
    QTime nowTime = secondTime.time(); //время сейчас
    int nowMin = nowTime.minute(); //кол-во минут сейчас
    int nowSec = nowTime.second(); //кол-во секунд сейчас
    firstTime = firstTime.addSecs(-(baseMin * 60)); //время последнего обновления без минут
    qint64 timeInterval = firstTime.secsTo(secondTime) / 60; //сколько прошло минут с последнего обновления
    hoursLeft = timeInterval / 60; //сколько прошло часов с последнего обновления
    if (hoursLeft < 48) { //устанавливает время первый раз после отключения интернета
        timeWithHoursFromBd.clear();
        timeWithHoursFromBd.setQuery("SELECT currentTemperature, currentIcon, time FROM hours48;");
        updateUIFromBd(hoursLeft); //ставятся значения согласно прошедшим часам
        int toFirstSingleShotLeft = (60 - nowMin) * 60 - nowSec; //до первой смены времени осталось секунд
        isValuesInFutureDaysTable();
        hoursLeft++;
        QTimer::singleShot(toFirstSingleShotLeft * 1000, this, SLOT(changeTime()));
    } else {
        whenTimeBiggerThen48();
    }
}
void DataBase::afterNoNet()
{
    disconnect(&netR.netManage, SIGNAL(onlineStateChanged(bool)),
        this, SLOT(afterNoNet()));
    timeRequest();
}
void DataBase::changeTime() //когда нет сети, определяет из какой таблицы обновлять UI
{
    if (hoursLeft < 48) {
        updateUIFromBd(hoursLeft); //смена в погоде на сегодня
        isValuesInFutureDaysTable(); //смена в погоде на будущие дни
        QTimer::singleShot(3600000, this, SLOT(changeTime())); //вызвать саму себя через час
        hoursLeft++;
    } else {
        whenTimeBiggerThen48();
    }
}
void DataBase::updateUIFromBd(int hoursLeft) // Update в UI текущую t в течении 48 часов, если нет сети
{
    QSqlQuery q1(db);
    q1.prepare("UPDATE tableForUI SET currentTemperature=:curTemp,"
               "currentIcon=:curIcon, time=:timeDay WHERE ID=1;"); //данные t из БД
    q1.bindValue(":timeDay", timeWithHoursFromBd.data(timeWithHoursFromBd.index(hoursLeft, 2)).toDateTime());
    q1.bindValue(":curTemp", timeWithHoursFromBd.data(timeWithHoursFromBd.index(hoursLeft, 0)).toInt());
    q1.bindValue(":curIcon", timeWithHoursFromBd.data(timeWithHoursFromBd.index(hoursLeft, 1)).toString());
    q1.exec();
    emit iKnowWeather(); //обновить UI
}
void DataBase::whenTimeBiggerThen48() //ставит данные из daysWeatherTable если прошло больше 48 часов
{
    QSqlQuery firstTimeQuery("SELECT ID FROM daysWeatherTable "
                             "WHERE  strftime('%s', time) > strftime('%s',datetime('now','localtime'));"); //время и дата последнего обновления
    firstTimeQuery.first();
    int idForNow = firstTimeQuery.value(0).toInt();
    timeWithDaysFromBd.clear(); //записать в модель дневные значения начиная с предыдущего дня
    timeWithDaysFromBd.setQuery(QString("SELECT currentTemperature,currentIcon,time FROM daysWeatherTable "
                                        "WHERE ID > '%1';")
                                    .arg(idForNow - 2));
    QDateTime futureDayTime = timeWithDaysFromBd.data(timeWithDaysFromBd.index(1, 2)).toDateTime();
    QTime futureTime = futureDayTime.time();
    QTime nowTime = QTime::currentTime();
    int t = futureTime.secsTo(nowTime);
    if (t < 0) { //если время перевалило за полночь
        QTime zero = QTime::fromString("23:59:59");
        QTime zeroO = QTime::fromString("00:00:00");
        int firstPartTime = (-1) * zero.secsTo(futureTime);
        int secondPartTime = (-1) * nowTime.secsTo(zeroO);
        t = firstPartTime + secondPartTime;
    }
    t = 24 * 60 * 60 - t; //до первого SingleShot осталось
    currentWhenNoInternet(days);
    QTimer::singleShot(t * 1000, this, SLOT(daysWithoutInternet())); //вызвать daysWithoutInternet() через t минут * с(1000)
}
void DataBase::currentWhenNoInternet(int days) //обновляет текущую t когда прошло 48 часов без сети
{
    QSqlQuery q1(db);
    q1.prepare("UPDATE tableForUI SET currentTemperature=:curTemp,"
               "currentIcon=:curIcon, time=:timeDay WHERE ID=1;"); //данные t из БД на дни
    q1.bindValue(":curTemp", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days, 0)).toInt());
    q1.bindValue(":curIcon", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days, 1)).toString());
    q1.bindValue(":timeDay", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days, 2)).toDateTime());
    q1.exec();
    whenTimeBiggerThenDays(days, 0);
}
void DataBase::whenTimeBiggerThenDays(int days, int twoDays) //обновляет будущую t
{
    QSqlQuery q1(db);
    for (int i = 1; i < 4; ++i) {
        q1.prepare("UPDATE tableForUI SET currentTemperature=:curTemp,"
                   "currentIcon=:curIcon, time=:timeDay WHERE ID=:id;"); //данные t из БД на дни
        q1.bindValue(":id", i + 1);
        q1.bindValue(":curTemp", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days + i - twoDays, 0)).toInt());
        q1.bindValue(":curIcon", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days + i - twoDays, 1)).toString());
        q1.bindValue(":timeDay", timeWithDaysFromBd.data(timeWithDaysFromBd.index(days + i - twoDays, 2)).toDateTime());
        q1.exec();
    }
    emit iKnowWeather(); //обновить UI
}
void DataBase::daysWithoutInternet()
{
    days++;
    currentWhenNoInternet(days);
    QTimer::singleShot(3600000 * 24, this, SLOT(daysWithoutInternet())); //вызвать саму себя через сутки
}
void DataBase::isValuesInFutureDaysTable()
{
    timeWithDaysFromBd.clear(); //записать в модель дневные значения начиная с предыдущего дня
    timeWithDaysFromBd.setQuery(QString("SELECT currentTemperature,currentIcon,time FROM daysWeatherTable "
                                        "WHERE strftime('%s', time) > strftime('%s',datetime('now','localtime'));"));
    whenTimeBiggerThenDays(0, 1);
}
void DataBase::whenApiKeyValid()
{
    connect(&netR, SIGNAL(replyDone()), SLOT(ifChangedApiKey()));
}
void DataBase::ifChangedApiKey()
{
    QSqlQuery q1(db);
    q1.prepare("UPDATE timeNow SET appid=:key;");
    q1.bindValue(":key", apiKey);
    q1.exec();
    disconnect(&netR, SIGNAL(replyDone()), this, SLOT(ifChangedApiKey()));
}
DataBase::~DataBase()
{
}
