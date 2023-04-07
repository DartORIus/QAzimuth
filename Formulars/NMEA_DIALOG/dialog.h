
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFile>
#include <QThread>
#include <QTimer>
#include <QSlider>
#include <QString>
#include <QSpinBox>
#include <QSettings>
#include <QDockWidget>

#include "Show_Text.h"

class Read_File_Thread : public QThread
{
    Q_OBJECT
private:
    int period;
public:
    Read_File_Thread() : period(10) {};

    void run()
    {
        QTimer timer;
        connect(&timer, SIGNAL(timeout()), this, SLOT(Read()));
        timer.start((period));

        exec();
    }
signals:
    void Read_Line();
public slots:
    void Set_Period(int P)
    {
        period = P;
    }

    void Read()
    {
        emit Read_Line();
    }
};

class Dialog : public QWidget
{
    Q_OBJECT

signals:
    void Write_NMEA_Data_SIGNAL(const QString &, int);
    void Parse_NMEA_Signal(const QString &);
    void Clear_Signal();
    void Push_Read_Buton_Signal(bool);
    void Read_All_Button_Signal();
   // void Set_Title(const QString &);
    void Show_Hide_Signal(bool);
    void portStateChanged_Signal(int);
public:
    explicit Dialog(QSettings &, QWidget *parent = 0);
    ~Dialog();
    void setLocalEchoEnabled(bool);
    void Disable_Enable_Send(bool);

    void addTab(const QString&);
    void deleteTab();

public slots:
    void Find_Signal_Slot(double);
//    void Read_Button_Settings(bool);
    void Get_NMEA_SLOT(const QString &);
    void Show_NMEA_Text(const QByteArray &, int);
    void isPortOpened_Slot(int);
//    void Show_NMEA_Text_1(const QByteArray &);

private slots:
    void Open_File_Slot();
    void Save_File_Slot();
    void Read_File_Slot();
    void Read_Stop_Slot(bool);
    void Read_All();
    void Peremotka_Slot(int);
    void Change_Slider(int);
    void Clear_Slot();

    void Control_Sum(const QString &);
    void putData(void);
    void changeSendStatus_Slot(int);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    void keyPressEvent(QKeyEvent *);

private:
    bool localEchoEnabled;
    QLabel *Protocol_Znak;
    QLabel *Control_Sum_Label;
    QString NMEA_Text;
    QLabel  *File_Label;

    QFile Open_File;
    QFile Save_File;
    Read_File_Thread *read_thread;
    QSlider *slider;
    QSpinBox *spin;

    QComboBox *ComboBox;

    QPushButton *Save_File_Button;
    QPushButton *Read_File_Button;
    QPushButton *Read_All_Button;
    QPushButton *Open_File_Button;
    QPushButton *Clear_Button;

    QPushButton *Sent_NMEA;

    qint64 File_Size;

    QSettings *QA_Settings;

    QTabWidget* ptabWidget;
    QList<Show_Text_QW*> tabs;
    QList<bool> portsOpenStatus;

    void Save_File_Funk(QString);
    void Read_Dialog_ComboBox();
};

#endif // CONSOLE_H
