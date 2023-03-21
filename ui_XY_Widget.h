/********************************************************************************
** Form generated from reading UI file 'XY_Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XY_WIDGET_H
#define UI_XY_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XY_Widget
{
public:
    QSpinBox *Y_UP;
    QSpinBox *X_Left;
    QSpinBox *X_Right;
    QSpinBox *Y_Down;
    QFrame *frame;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpinBox *Average_spinBox;

    void setupUi(QWidget *XY_Widget)
    {
        if (XY_Widget->objectName().isEmpty())
            XY_Widget->setObjectName(QString::fromUtf8("XY_Widget"));
        XY_Widget->resize(670, 600);
        XY_Widget->setMinimumSize(QSize(670, 600));
        XY_Widget->setMaximumSize(QSize(670, 600));
        Y_UP = new QSpinBox(XY_Widget);
        Y_UP->setObjectName(QString::fromUtf8("Y_UP"));
        Y_UP->setGeometry(QRect(290, 10, 61, 23));
        Y_UP->setMinimum(-17000);
        Y_UP->setMaximum(17000);
        Y_UP->setValue(1000);
        X_Left = new QSpinBox(XY_Widget);
        X_Left->setObjectName(QString::fromUtf8("X_Left"));
        X_Left->setGeometry(QRect(10, 260, 48, 23));
        X_Left->setMinimum(-50);
        X_Left->setMaximum(50);
        X_Left->setValue(-50);
        X_Right = new QSpinBox(XY_Widget);
        X_Right->setObjectName(QString::fromUtf8("X_Right"));
        X_Right->setGeometry(QRect(600, 270, 48, 23));
        X_Right->setMinimum(-50);
        X_Right->setMaximum(50);
        X_Right->setValue(50);
        Y_Down = new QSpinBox(XY_Widget);
        Y_Down->setObjectName(QString::fromUtf8("Y_Down"));
        Y_Down->setGeometry(QRect(290, 570, 61, 23));
        Y_Down->setMinimum(-17000);
        Y_Down->setMaximum(17000);
        Y_Down->setValue(-1000);
        frame = new QFrame(XY_Widget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(70, 40, 520, 520));
        frame->setMinimumSize(QSize(520, 520));
        frame->setMaximumSize(QSize(520, 520));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayoutWidget = new QWidget(frame);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 521, 521));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Average_spinBox = new QSpinBox(XY_Widget);
        Average_spinBox->setObjectName(QString::fromUtf8("Average_spinBox"));
        Average_spinBox->setGeometry(QRect(600, 520, 61, 23));
        Average_spinBox->setMinimum(1);
        Average_spinBox->setMaximum(100);
        Average_spinBox->setValue(1);

        retranslateUi(XY_Widget);

        QMetaObject::connectSlotsByName(XY_Widget);
    } // setupUi

    void retranslateUi(QWidget *XY_Widget)
    {
        XY_Widget->setWindowTitle(QCoreApplication::translate("XY_Widget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XY_Widget: public Ui_XY_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XY_WIDGET_H
