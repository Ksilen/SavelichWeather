void DataBase::ifHasNotInternet() //Если кончился интернет меняет условия программы
{
    countTime = 0; //сброс запросного времени, чтобы при появлении интернета обновить всю БД
    timer2->start(1000); //таймер для пинга
    connect(timer2, SIGNAL(timeout()), SLOT(isInternet())); //"бесконечный" пинг
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

void DataBase::isInternet() //делает запросы, чтобы определить есть ли сеть
{
    int exitCode = QProcess::execute("ping -n 1 ya.ru");
    if (exitCode) {
        timer2->start(oneSecond);
    } else {
        timer2->stop(); //стоп таймер пинга интернета
        timeRequest();
    }
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
