/********************************************************************************
** Form generated from reading UI file 'XY_Area.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XY_AREA_H
#define UI_XY_AREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XY_Area
{
public:
    QMdiArea *mdiArea;

    void setupUi(QWidget *XY_Area)
    {
        if (XY_Area->objectName().isEmpty())
            XY_Area->setObjectName(QString::fromUtf8("XY_Area"));
        XY_Area->resize(768, 768);
        XY_Area->setMinimumSize(QSize(768, 768));
        XY_Area->setMaximumSize(QSize(2000, 2000));
        mdiArea = new QMdiArea(XY_Area);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setGeometry(QRect(0, 0, 768, 768));
        mdiArea->setMinimumSize(QSize(768, 768));
        mdiArea->setMaximumSize(QSize(2000, 2000));

        retranslateUi(XY_Area);

        QMetaObject::connectSlotsByName(XY_Area);
    } // setupUi

    void retranslateUi(QWidget *XY_Area)
    {
        XY_Area->setWindowTitle(QCoreApplication::translate("XY_Area", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XY_Area: public Ui_XY_Area {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XY_AREA_H
