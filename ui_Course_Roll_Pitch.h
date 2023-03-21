/********************************************************************************
** Form generated from reading UI file 'Course_Roll_Pitch.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COURSE_ROLL_PITCH_H
#define UI_COURSE_ROLL_PITCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Course_Roll_Pitch
{
public:
    QGraphicsView *graphicsView;

    void setupUi(QWidget *Course_Roll_Pitch)
    {
        if (Course_Roll_Pitch->objectName().isEmpty())
            Course_Roll_Pitch->setObjectName(QString::fromUtf8("Course_Roll_Pitch"));
        Course_Roll_Pitch->resize(386, 427);
        graphicsView = new QGraphicsView(Course_Roll_Pitch);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 20, 300, 300));
        graphicsView->setMinimumSize(QSize(300, 300));
        graphicsView->setMaximumSize(QSize(300, 300));
        graphicsView->setStyleSheet(QString::fromUtf8("background-image: url(:/images/64x64/cat_hiss.png);"));
        graphicsView->setFrameShape(QFrame::NoFrame);
        QBrush brush(QColor(69, 191, 199, 255));
        brush.setStyle(Qt::SolidPattern);
        graphicsView->setBackgroundBrush(brush);
        QBrush brush1(QColor(148, 98, 98, 255));
        brush1.setStyle(Qt::SolidPattern);
        graphicsView->setForegroundBrush(brush1);

        retranslateUi(Course_Roll_Pitch);

        QMetaObject::connectSlotsByName(Course_Roll_Pitch);
    } // setupUi

    void retranslateUi(QWidget *Course_Roll_Pitch)
    {
        Course_Roll_Pitch->setWindowTitle(QCoreApplication::translate("Course_Roll_Pitch", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Course_Roll_Pitch: public Ui_Course_Roll_Pitch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COURSE_ROLL_PITCH_H
