/********************************************************************************
** Form generated from reading UI file 'Com_Accel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COM_ACCEL_H
#define UI_COM_ACCEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Com_Accel
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QLabel *Compas_V;
    QLabel *Accel_V;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *Dif;
    QDoubleSpinBox *DIF_Spin;

    void setupUi(QWidget *Com_Accel)
    {
        if (Com_Accel->objectName().isEmpty())
            Com_Accel->setObjectName(QString::fromUtf8("Com_Accel"));
        Com_Accel->resize(372, 446);
        frame = new QFrame(Com_Accel);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 350, 350));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(Com_Accel);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 370, 191, 61));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        Compas_V = new QLabel(frame_2);
        Compas_V->setObjectName(QString::fromUtf8("Compas_V"));
        Compas_V->setGeometry(QRect(90, 30, 71, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        Compas_V->setFont(font);
        Accel_V = new QLabel(frame_2);
        Accel_V->setObjectName(QString::fromUtf8("Accel_V"));
        Accel_V->setGeometry(QRect(90, 10, 71, 16));
        Accel_V->setFont(font);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 71, 16));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 71, 16));
        label_3 = new QLabel(Com_Accel);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(210, 370, 31, 16));
        Dif = new QLabel(Com_Accel);
        Dif->setObjectName(QString::fromUtf8("Dif"));
        Dif->setGeometry(QRect(240, 370, 111, 16));
        Dif->setFont(font);
        DIF_Spin = new QDoubleSpinBox(Com_Accel);
        DIF_Spin->setObjectName(QString::fromUtf8("DIF_Spin"));
        DIF_Spin->setGeometry(QRect(210, 400, 131, 23));
        DIF_Spin->setDecimals(6);
        DIF_Spin->setMinimum(-99.000000000000000);

        retranslateUi(Com_Accel);

        QMetaObject::connectSlotsByName(Com_Accel);
    } // setupUi

    void retranslateUi(QWidget *Com_Accel)
    {
        Com_Accel->setWindowTitle(QCoreApplication::translate("Com_Accel", "Form", nullptr));
        Compas_V->setText(QString());
        Accel_V->setText(QString());
        label->setText(QCoreApplication::translate("Com_Accel", "Accel:", nullptr));
        label_2->setText(QCoreApplication::translate("Com_Accel", "Compass:", nullptr));
        label_3->setText(QCoreApplication::translate("Com_Accel", "Dif:", nullptr));
        Dif->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Com_Accel: public Ui_Com_Accel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COM_ACCEL_H
