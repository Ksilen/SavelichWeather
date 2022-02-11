#ifndef DATABASE_H
#define DATABASE_H

#include "netrequest.h"
#include <QDateTime>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

class DataBase : public QObject {
    Q_OBJECT
public:
    DataBase();
    void closeAndWrite(int appW, int appH, int X, int Y); //запись местоположения и размера виджета при закрытии
    void closeAndBackFirstTemperature(); //возврвщае t последнего обновления, чтобы при следующем включении без сети был расчет
    void whatCity(); //запрос: какой город?
    void settingsForBd();
    virtual ~DataBase();
    NetRequest isCity;
    NetRequest netR;
    QString queryCity;
    QString apiKey = "749d4862b2ad4bd9fdcada0252405c3e"; //ключ для запросов к OpenWeather(можно ввести другой и он запишется в БД)

public slots:
    void ifFirstCreateBdAndHasNotInternet(); //первый заход, Бд нет и сети нет, ждать сеть
    void writeCurrentWeather();
    void ifHasNotInternet();
    void timeRequest(); //делает "One Call API" запрос 1 раз в час, и "Current Weather Data" каждые 10 мин
    void parseCityName(); //"выуживает" имя города
    void changeTime(); //меняет данные если нет интернета меньше 48 часов
    void updateUIFromBd(int); //установка данных из БД(hours48) в UI, когда нет интернета
    void firstCitiName(); //ставит имя и координаты города запрошенного по ip
    void firstIsInternet(); //первое включение программы,если Бд нет и сети нет, пинг сети
    void whenTimeBiggerThen48(); //установка температуры, когда прошло времени больше 48 часов
    void whenTimeBiggerThenDays(int days, int twoDays); //установка будущей температуры,когда нет сети и прошло времени больше 48 часов
    void daysWithoutInternet(); //вызывает себя через сутки когда нет сети
    void currentWhenNoInternet(int days); //обновляет текущее значение,когда прошло > 48ч и нет сети
    void ifChangedApiKey(); //если API Key подошел, занести его в БД
    void whenApiKeyValid(); //если API Key подошел,connect
    void getApiKeyFromBd(); //Взять APIkey из БД и присвоить QString apiKey
    void afterNoNet(); // disconnect проверки интернета вызов timeRequest()

signals:
    void iKnowWeather(); //обновляет значения на виджете
    void iHaveNotInternet(); //при первом включении программы не создает БД если нет сети, ставит надпись "Сети нет"
    void requestDone(); //сигнал об окончании парсинга в другом потоке
    void apiKeyNotValidB();

private:
    void connectWithNetrequest(); //реакция на наличие ответа в запросе к API
    void writeCurrentTime();
    void createDataBase(); //создает БД
    void createWeatherTable(); //создает таблицы для БД
    void bigRequest(); //парсинг "One Call API"
    void littleRequest(); //парсинг "Current Weather Data"
    void openDataBase();
    void letsWork(); //регулирует начало работы, взависимости от того есть БД или нет
    void isValuesInFutureDaysTable(); //проверяет есть ли значения в базе дней меньше текущего
    void absTemperature(int& intCurTemp, double& curTemp); //если t = 22.6 сделать 23
    int hoursLeft; //прошло часов с последнего обновления
    int countTime = 0; //переменная для чередования запросов
    int tenMinutes = 600000; //запросы к АРI через 10 мин, если ЕСТЬ сеть
    int days = 0; //счетчик дней, если нет сети
    QSqlDatabase db;
    QTimer timerOne; //таймер для запросов,когда есть сеть
    QTimer* timer2 = new QTimer(this); //таймер для пинга, когда сети нет
    QString city = ""; //название города
    QString cityCoordinate = ""; //координаты города
    QString stringForRequest; //строка с запросом к API
    QSqlQueryModel timeWithHoursFromBd;
    QSqlQueryModel timeWithDaysFromBd;
};

#endif // DATABASE_H
