/********************************************************************************
** Form generated from reading UI file 'counterDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COUNTERDLG_H
#define UI_COUNTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Counter
{
public:
    QPushButton *button;
    QLCDNumber *lcdNumber;
    QLabel *label;
    QSlider *Slider;
    QLabel *label_2;
    QLCDNumber *lcd_periodo;
    QLabel *label_3;
    QLabel *label_4;
    QLCDNumber *lcd_tiempo;

    void setupUi(QWidget *Counter)
    {
        if (Counter->objectName().isEmpty())
            Counter->setObjectName(QString::fromUtf8("Counter"));
        Counter->resize(400, 300);
        button = new QPushButton(Counter);
        button->setObjectName(QString::fromUtf8("button"));
        button->setGeometry(QRect(60, 210, 251, 71));
        lcdNumber = new QLCDNumber(Counter);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(50, 10, 301, 91));
        label = new QLabel(Counter);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 140, 67, 17));
        Slider = new QSlider(Counter);
        Slider->setObjectName(QString::fromUtf8("Slider"));
        Slider->setGeometry(QRect(49, 110, 291, 20));
        Slider->setLayoutDirection(Qt::RightToLeft);
        Slider->setMinimum(1);
        Slider->setMaximum(1000);
        Slider->setValue(500);
        Slider->setSliderPosition(500);
        Slider->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(Counter);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(280, 140, 67, 17));
        lcd_periodo = new QLCDNumber(Counter);
        lcd_periodo->setObjectName(QString::fromUtf8("lcd_periodo"));
        lcd_periodo->setGeometry(QRect(50, 160, 121, 41));
        lcd_periodo->setProperty("intValue", QVariant(500));
        label_3 = new QLabel(Counter);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 170, 67, 17));
        label_4 = new QLabel(Counter);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(180, 170, 111, 17));
        lcd_tiempo = new QLCDNumber(Counter);
        lcd_tiempo->setObjectName(QString::fromUtf8("lcd_tiempo"));
        lcd_tiempo->setGeometry(QRect(280, 160, 121, 41));
        lcd_tiempo->setProperty("intValue", QVariant(0));

        retranslateUi(Counter);

        QMetaObject::connectSlotsByName(Counter);
    } // setupUi

    void retranslateUi(QWidget *Counter)
    {
        Counter->setWindowTitle(QApplication::translate("Counter", "Counter", nullptr));
        button->setText(QApplication::translate("Counter", "STOP", nullptr));
        label->setText(QApplication::translate("Counter", "Min.", nullptr));
        label_2->setText(QApplication::translate("Counter", "Max.", nullptr));
        label_3->setText(QApplication::translate("Counter", "Periodo", nullptr));
        label_4->setText(QApplication::translate("Counter", "T. transcurrido", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Counter: public Ui_Counter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COUNTERDLG_H
