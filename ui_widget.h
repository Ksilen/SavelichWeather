/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *imageWeather;
    QLabel *citi;
    QHBoxLayout *horizontalLayout_2;
    QLabel *temperatureNow;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *futureDays_2;
    QLabel *firstDayName;
    QLabel *secondDayName;
    QLabel *thirdDayName;
    QHBoxLayout *futureDays;
    QLabel *firstDay;
    QLabel *secondDay;
    QLabel *thirdDay;
    QHBoxLayout *futureDaysT;
    QLabel *firstDayT;
    QLabel *secondDayT;
    QLabel *thirdDayT;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->setEnabled(true);
        Widget->resize(461, 320);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        Widget->setContextMenuPolicy(Qt::CustomContextMenu);
        Widget->setStyleSheet(QLatin1String("background-color: rgba(150, 156, 156,200);\n"
""));
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 461, 321));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        imageWeather = new QLabel(layoutWidget);
        imageWeather->setObjectName(QStringLiteral("imageWeather"));
        sizePolicy.setHeightForWidth(imageWeather->sizePolicy().hasHeightForWidth());
        imageWeather->setSizePolicy(sizePolicy);
        imageWeather->setMinimumSize(QSize(100, 50));
        imageWeather->setSizeIncrement(QSize(10, 10));
        imageWeather->setBaseSize(QSize(0, 0));
        imageWeather->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;\n"
""));

        horizontalLayout->addWidget(imageWeather);

        citi = new QLabel(layoutWidget);
        citi->setObjectName(QStringLiteral("citi"));
        sizePolicy.setHeightForWidth(citi->sizePolicy().hasHeightForWidth());
        citi->setSizePolicy(sizePolicy);
        citi->setMinimumSize(QSize(100, 50));
        citi->setSizeIncrement(QSize(10, 10));
        citi->setBaseSize(QSize(0, 0));
        citi->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"QToolTip{\n"
"font-size:12;\n"
"} ;\n"
""));

        horizontalLayout->addWidget(citi);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        temperatureNow = new QLabel(layoutWidget);
        temperatureNow->setObjectName(QStringLiteral("temperatureNow"));
        temperatureNow->setEnabled(true);
        sizePolicy.setHeightForWidth(temperatureNow->sizePolicy().hasHeightForWidth());
        temperatureNow->setSizePolicy(sizePolicy);
        temperatureNow->setMinimumSize(QSize(100, 50));
        QFont font;
        font.setPointSize(70);
        font.setBold(true);
        font.setWeight(75);
        font.setKerning(true);
        font.setStyleStrategy(QFont::PreferAntialias);
        temperatureNow->setFont(font);
        temperatureNow->setMouseTracking(false);
        temperatureNow->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));
        temperatureNow->setText(QStringLiteral("-20"));
        temperatureNow->setTextFormat(Qt::PlainText);
        temperatureNow->setScaledContents(false);

        horizontalLayout_2->addWidget(temperatureNow);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        futureDays_2 = new QHBoxLayout();
        futureDays_2->setSpacing(6);
        futureDays_2->setObjectName(QStringLiteral("futureDays_2"));
        firstDayName = new QLabel(layoutWidget);
        firstDayName->setObjectName(QStringLiteral("firstDayName"));
        sizePolicy.setHeightForWidth(firstDayName->sizePolicy().hasHeightForWidth());
        firstDayName->setSizePolicy(sizePolicy);
        firstDayName->setMinimumSize(QSize(10, 15));
        firstDayName->setSizeIncrement(QSize(10, 10));
        firstDayName->setFocusPolicy(Qt::NoFocus);
        firstDayName->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));
        firstDayName->setFrameShape(QFrame::NoFrame);

        futureDays_2->addWidget(firstDayName);

        secondDayName = new QLabel(layoutWidget);
        secondDayName->setObjectName(QStringLiteral("secondDayName"));
        sizePolicy.setHeightForWidth(secondDayName->sizePolicy().hasHeightForWidth());
        secondDayName->setSizePolicy(sizePolicy);
        secondDayName->setMinimumSize(QSize(10, 10));
        secondDayName->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));

        futureDays_2->addWidget(secondDayName);

        thirdDayName = new QLabel(layoutWidget);
        thirdDayName->setObjectName(QStringLiteral("thirdDayName"));
        sizePolicy.setHeightForWidth(thirdDayName->sizePolicy().hasHeightForWidth());
        thirdDayName->setSizePolicy(sizePolicy);
        thirdDayName->setMinimumSize(QSize(10, 10));
        thirdDayName->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));

        futureDays_2->addWidget(thirdDayName);


        verticalLayout->addLayout(futureDays_2);

        futureDays = new QHBoxLayout();
        futureDays->setSpacing(6);
        futureDays->setObjectName(QStringLiteral("futureDays"));
        firstDay = new QLabel(layoutWidget);
        firstDay->setObjectName(QStringLiteral("firstDay"));
        sizePolicy.setHeightForWidth(firstDay->sizePolicy().hasHeightForWidth());
        firstDay->setSizePolicy(sizePolicy);
        firstDay->setMinimumSize(QSize(10, 10));
        firstDay->setSizeIncrement(QSize(10, 10));
        firstDay->setFocusPolicy(Qt::NoFocus);
        firstDay->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));
        firstDay->setFrameShape(QFrame::NoFrame);

        futureDays->addWidget(firstDay);

        secondDay = new QLabel(layoutWidget);
        secondDay->setObjectName(QStringLiteral("secondDay"));
        sizePolicy.setHeightForWidth(secondDay->sizePolicy().hasHeightForWidth());
        secondDay->setSizePolicy(sizePolicy);
        secondDay->setMinimumSize(QSize(10, 10));
        secondDay->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));

        futureDays->addWidget(secondDay);

        thirdDay = new QLabel(layoutWidget);
        thirdDay->setObjectName(QStringLiteral("thirdDay"));
        sizePolicy.setHeightForWidth(thirdDay->sizePolicy().hasHeightForWidth());
        thirdDay->setSizePolicy(sizePolicy);
        thirdDay->setMinimumSize(QSize(10, 10));
        thirdDay->setStyleSheet(QLatin1String("background-color: rgba(255, 170, 127,0);\n"
"qproperty-alignment: AlignCenter;"));

        futureDays->addWidget(thirdDay);


        verticalLayout->addLayout(futureDays);

        futureDaysT = new QHBoxLayout();
        futureDaysT->setSpacing(6);
        futureDaysT->setObjectName(QStringLiteral("futureDaysT"));
        firstDayT = new QLabel(layoutWidget);
        firstDayT->setObjectName(QStringLiteral("firstDayT"));
        sizePolicy.setHeightForWidth(firstDayT->sizePolicy().hasHeightForWidth());
        firstDayT->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(30);
        font1.setBold(true);
        font1.setWeight(75);
        font1.setStyleStrategy(QFont::PreferAntialias);
        firstDayT->setFont(font1);
        firstDayT->setStyleSheet(QLatin1String("background-color: rgba(60, 112, 255,0);\n"
"qproperty-alignment: AlignCenter;"));

        futureDaysT->addWidget(firstDayT);

        secondDayT = new QLabel(layoutWidget);
        secondDayT->setObjectName(QStringLiteral("secondDayT"));
        sizePolicy.setHeightForWidth(secondDayT->sizePolicy().hasHeightForWidth());
        secondDayT->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setPointSize(28);
        font2.setBold(true);
        font2.setWeight(75);
        font2.setStyleStrategy(QFont::PreferAntialias);
        secondDayT->setFont(font2);
        secondDayT->setStyleSheet(QLatin1String("background-color: rgb(172, 255, 83);\n"
"qproperty-alignment: AlignCenter;"));

        futureDaysT->addWidget(secondDayT);

        thirdDayT = new QLabel(layoutWidget);
        thirdDayT->setObjectName(QStringLiteral("thirdDayT"));
        sizePolicy.setHeightForWidth(thirdDayT->sizePolicy().hasHeightForWidth());
        thirdDayT->setSizePolicy(sizePolicy);
        QFont font3;
        font3.setPointSize(24);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setUnderline(false);
        font3.setWeight(75);
        font3.setStrikeOut(false);
        font3.setKerning(true);
        font3.setStyleStrategy(QFont::PreferAntialias);
        thirdDayT->setFont(font3);
        thirdDayT->setStyleSheet(QLatin1String("background-color: rgb(39, 216, 255);\n"
"qproperty-alignment: AlignCenter;"));

        futureDaysT->addWidget(thirdDayT);


        verticalLayout->addLayout(futureDaysT);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        imageWeather->setText(QString());
#ifndef QT_NO_TOOLTIP
        citi->setToolTip(QApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\235\320\260\320\266\320\274\320\270\321\202\320\265 \321\207\321\202\320\276\320\261\321\213 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\262\321\213\320\261\321\200\320\260\321\202\321\214 \320\263\320\276\321\200\320\276\320\264 </p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        citi->setText(QString());
        firstDayName->setText(QString());
        secondDayName->setText(QString());
        thirdDayName->setText(QString());
        firstDay->setText(QString());
        secondDay->setText(QString());
        thirdDay->setText(QString());
        firstDayT->setText(QApplication::translate("Widget", "34", 0));
        secondDayT->setText(QApplication::translate("Widget", "-20", 0));
        thirdDayT->setText(QApplication::translate("Widget", "77", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
