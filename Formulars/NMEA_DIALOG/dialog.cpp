#include "dialog.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QSpinBox>
#include <QDateTime>
#include <QLineEdit>
#include <QPalette>

#define HEIGHT 30

Dialog::Dialog(QSettings &S, QWidget *parent)
    : QWidget{parent}, localEchoEnabled{true}, Settings{S}
{
    ComboBox = new QComboBox;

    QFont font("Cascadia Mono SemiBold");
    font.setStyleHint(QFont::Monospace);
    ComboBox->setFont(font);
    ComboBox->setEditable(true);
    ComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(75, 14, 58, 255));
    p.setColor(QPalette::Text, Qt::red);
    ComboBox->setPalette(p);
    ComboBox->setMaximumHeight(HEIGHT);
    ComboBox->setMaxVisibleItems(5);
    ComboBox->setInsertPolicy(QComboBox::InsertAtTop);

    Read_Dialog_ComboBox();

    Protocol_Znak = new QLabel("*");
    Control_Sum_Label = new QLabel("00");
    Control_Sum_Label->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    Sent_NMEA = new QPushButton(tr("Send"));
    Sent_NMEA->setMinimumHeight(HEIGHT);
    Sent_NMEA->setDisabled(true);

    File_Label = new QLabel("_________________");

    ptabWidget = new QTabWidget(this);
    ptabWidget->setTabBarAutoHide(true);
    addTab("Main");
    connect(ptabWidget, SIGNAL(currentChanged(int)), this, SLOT(isPortOpened_Slot(int)));
    connect(this, SIGNAL(portStateChanged_Signal(int)), this, SLOT(changeSendStatus_Slot(int)));
    portsOpenStatus[0] = false;

    Save_File_Button = new QPushButton(tr("&Save"));
    QPalette pal_blau = Save_File_Button->palette();
    pal_blau.setColor(QPalette::Button, QColor(0,154,255));
    Save_File_Button->setPalette(pal_blau);

    Open_File_Button  = new QPushButton(tr("&Open_File"));
    QSpinBox    *Read_Spinbox     = new QSpinBox();
    Read_File_Button              = new QPushButton(tr("&Read_File"));
    Read_File_Button->setDisabled(true);
    Read_File_Button->setCheckable(true);
    Read_All_Button              = new QPushButton(tr("Read_&All"));
    Read_All_Button->setDisabled(true);
    Read_All_Button->setCheckable(true);

    Clear_Button = new QPushButton(tr("&Clear"));

    Read_Spinbox->setMaximum(10000);
    Read_Spinbox->setMinimum(1);
    Read_Spinbox->setValue(10);
    Read_Spinbox->setSuffix(" ms");

    QHBoxLayout *QHB = new QHBoxLayout;
    QHB->addWidget(ComboBox);
    QHB->addWidget(Protocol_Znak);
    QHB->addWidget(Control_Sum_Label);
    QHB->addWidget(Sent_NMEA);

    QHBoxLayout *QHB_1 = new QHBoxLayout;
    QHB_1->addWidget(Save_File_Button);
    QHB_1->addWidget(Open_File_Button);
    QHB_1->addWidget(Read_Spinbox);
    QHB_1->addWidget(Read_File_Button);
    QHB_1->addWidget(Read_All_Button);
    QHB_1->addWidget(Clear_Button);

    slider = new QSlider(Qt::Horizontal);
    slider->setDisabled(true);
    spin = new QSpinBox();
    spin->setDisabled(true);
    spin->setMinimum(0);
    spin->setMaximum(100);
    spin->setSuffix(" %");

    QHBoxLayout *QhB = new QHBoxLayout;
    QhB->addWidget(slider);
    QhB->addWidget(spin);

    QVBoxLayout *QVB = new QVBoxLayout;
    QVB->addLayout(QHB);
    QVB->addWidget(File_Label);
    QVB->addWidget(ptabWidget);
    QVB->addLayout(QHB_1);
    QVB->addLayout(QhB);
    setLayout(QVB);

    read_thread = new Read_File_Thread();

    connect(Open_File_Button,   SIGNAL(pressed()), this,    SLOT(Open_File_Slot()));
    connect(Save_File_Button,   SIGNAL(pressed()), this,    SLOT(Save_File_Slot()));
    connect(Read_File_Button,   SIGNAL(clicked(bool)), this, SLOT(Read_Stop_Slot(bool)));
    connect(Read_All_Button,    SIGNAL(clicked()), this,    SLOT(Read_All()));
    connect(Clear_Button,   SIGNAL(clicked()), this,        SLOT(Clear_Slot()));
    connect(Read_Spinbox,   SIGNAL(valueChanged(int)), read_thread, SLOT(Set_Period(int)));
    connect(read_thread,    SIGNAL(Read_Line()), this,      SLOT(Read_File_Slot()));
    connect(Sent_NMEA,  SIGNAL(clicked()), this,            SLOT(putData()));
    connect(ComboBox,   SIGNAL(editTextChanged(const QString &)), this, SLOT(Control_Sum(const QString &)));
    connect(slider,     SIGNAL(valueChanged(int)), this,    SLOT(Peremotka_Slot(int)));
    connect(spin,   SIGNAL(valueChanged(int)), this,        SLOT(Change_Slider(int)));
}

Dialog::~Dialog()
{
    Settings.beginWriteArray("/Dialog_ComboBox");
    for (int i = 0; i < ComboBox->maxVisibleItems(); ++i) {
        Settings.setArrayIndex(i);
        Settings.setValue("/Send_NMEA_"+QString::number(i), ComboBox->itemText(i));
    }
    Settings.endArray();
}

void Dialog::Read_Dialog_ComboBox()
{
    int size = Settings.beginReadArray("/Dialog_ComboBox");
    for (int i = 0; i < size; ++i) {
        Settings.setArrayIndex(i);
        QString str = Settings.value("/Send_NMEA_"+QString::number(i), "").toString();
        if(str.size() > 0) {
            ComboBox->addItem(str);
        }
    }
    Settings.endArray();
    ComboBox->setCurrentIndex(-1);
}

void Dialog::Control_Sum(const QString &str)
{
        qint8 nmeaCRC = 0;

        for (int i = 0; i < str.length(); i++)
        {
            nmeaCRC ^= str.toLocal8Bit()[i];
        }

        QString B = QString::number(nmeaCRC, 16).toUpper();

        if(B.length()<2)
            B = "0"+B;

        B.resize(2);
        Control_Sum_Label->setText(B);
        NMEA_Text ="$"+str+"*"+B+"\r\n";
}

void Dialog::putData(void)
{
    emit Write_NMEA_Data_SIGNAL(NMEA_Text, ptabWidget->currentIndex() - 1);
}

void Dialog::Show_NMEA_Text(const QByteArray &Byte, int index)
{
    QString Text  = QString::fromLocal8Bit(Byte);

    tabs[index]->insertPlainText(Text);
    tabs[index]->moveCursor(QTextCursor::End);

    if(Save_File.exists())
       Save_File.write(Text.toLocal8Bit());

    emit Parse_NMEA_Signal(Text);
}

/*      Not used anywhere   *\
 * void Dialog::Show_NMEA_Text_1(const QByteArray &Byte)
 * {
 *     QString Text  = QString::fromLocal8Bit(Byte);
 *
 *     tabs.first()->insertPlainText(Text);
 *     tabs.first()->moveCursor(QTextCursor::End);
 * }
\*                          */

void Dialog::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Dialog::Disable_Enable_Send(bool S)
{
    Sent_NMEA->setEnabled(S);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(75, 14, 58, 255));
    if(S) {
        p.setColor(QPalette::Text, Qt::white);
    }
    else {
        p.setColor(QPalette::Text, Qt::red);
    }
    ComboBox->setPalette(p);
}

void Dialog::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

void Dialog::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
        case Qt::Key_Return:
            Sent_NMEA->animateClick();
            break;
        case Qt::Key_R:
            Read_File_Button->animateClick();
            break;
        case Qt::Key_A:
            Read_All_Button->animateClick();
            break;
        case Qt::Key_S:
            Save_File_Button->animateClick();
            break;
        case Qt::Key_O:
            Open_File_Button->animateClick();
            break;
        case Qt::Key_C:
            Clear_Button->animateClick();
            break;
    }
}

void Dialog::Read_Stop_Slot(bool checked)
{
    ptabWidget->setCurrentIndex(0);
    if(checked)
    {
        read_thread->start();
        Read_All_Button ->setEnabled(false);
        Open_File_Button->setEnabled(false);
        Clear_Button    ->setEnabled(false);
    }
    else
    {
        read_thread->quit();
        Read_All_Button ->setEnabled(true);
        Open_File_Button->setEnabled(true);
        Clear_Button    ->setEnabled(true);
    }

    emit Push_Read_Buton_Signal(checked);
}

void Dialog::Read_All()
{
    QByteArray data = Open_File.readAll();

    QString Text_NMEA = QString::fromLocal8Bit(data);

    tabs.first()->insertPlainText(Text_NMEA);

    QStringList Text_NMEA_List = Text_NMEA.split("\n");

    emit Show_Hide_Signal(false);
    ptabWidget->setCurrentIndex(0);

    for(int i = 0; i < Text_NMEA_List.size(); i++)
    {
        if(Text_NMEA_List[i].size() > 0)
        {
            QString Z = *(Text_NMEA_List[i].end() -1);

            if(Z != "\r")
            {
                Text_NMEA_List[i] += "\r";
            }

            emit Parse_NMEA_Signal(Text_NMEA_List[i] + "\n");
        }
    }

    Read_All_Button->setChecked(false);
    spin->setValue(100);
    slider->setValue(Open_File.pos());

    emit Read_All_Button_Signal();
    emit Show_Hide_Signal(true);
}

void Dialog::Read_File_Slot()
{
    QByteArray data = Open_File.readLine();

    int R = data.size();

    if(R >2 && data[R-2] != '\r')
        data.insert(R-1, '\r');

    slider->setValue(Open_File.pos());

    if(File_Size > 0)
    {
        int k = (Open_File.pos()*100)/File_Size;

        spin->setValue(k);

        if(k == 100)
            Read_File_Button->click();
    }
    else
    {
        Read_File_Button->click();
    }

    this->Show_NMEA_Text(data, 0);
}

void Dialog::Open_File_Slot()
{
    this->setWindowTitle("");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
    tr(".nme (*.nme)"));
    if (fileName != "")
    {
        Open_File.setFileName(fileName);
        if (!Open_File.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(0, "Warning", "Error");
        }
        else
        {
            Read_File_Button->setEnabled(true);
            slider->setEnabled(true);
            spin->setEnabled(true);

            if(!Read_File_Button->isChecked())
                Read_All_Button->setEnabled(true);

            slider->setRange(0, Open_File.size());

            File_Size = Open_File.size();

            File_Label->setText(fileName);
        }
    }
}

void Dialog::Save_File_Funk(QString fileName)
{
    if (fileName != "")
    {
        Save_File.setFileName(fileName);
        if (!Save_File.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(0, "Warning", "Error");
        }
        else
        {
            QTextStream stream(&Save_File);
            int tabIndex = ptabWidget->currentIndex();
            stream << tabs.at(tabIndex)->toPlainText();
        }
    }
}

void Dialog::Save_File_Slot()
{
    QString date = QDate::currentDate().toString("yyyy.MM.dd");

#ifdef linux
    QString time = QTime::currentTime().toString("hh:mm");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + "_" + time + ".nme",
    tr("NMEA File (*.nme)"));
#else
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + ".nme",
    tr("NMEA File (*.nme)"));
#endif

    if (fileName != "") {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".nme");
    }

    Save_File_Funk(fileName);

    if(Save_File.isOpen())
    {
        QPalette pal_blau = Read_File_Button->palette();
        Save_File_Button->setPalette(pal_blau);
    }

    File_Label->setText(fileName);
}

void Dialog::Peremotka_Slot(int S)
{
    Open_File.seek(S);
}

void Dialog::Change_Slider(int S)
{
    if(!Read_File_Button->isChecked() && !Read_All_Button->isChecked())
    {
        int k = S*File_Size/100;
        slider->setValue(k);
    }
}

void Dialog::Clear_Slot()
{
    slider->setValue(0);
    spin->setValue(0);
    slider->setEnabled(false);
    spin->setEnabled(false);
    Read_File_Button->setEnabled(false);
    Read_All_Button->setEnabled(false);
    int tabIndex = ptabWidget->currentIndex();
    tabs.at(tabIndex)->clear();

    File_Label->setText("_________________");

    emit Clear_Signal();
}

void Dialog::Find_Signal_Slot(double Cents)
{
    int tabIndex = ptabWidget->currentIndex();
    tabs.at(tabIndex)->Find_Slot(Cents);
}

//void Dialog::Read_Button_Settings(bool Enable)
//{
//    if(Open_File.isOpen())
//    {
//        Read_All_Button->setEnabled(Enable);
//        Read_File_Button->setEnabled(Enable);
//        slider->setEnabled(Enable);
//        spin->setEnabled(Enable);
//    }
//}

void Dialog::Get_NMEA_SLOT(const QString &NMEA_String)
{
    ComboBox->setCurrentText(NMEA_String);
    Sent_NMEA->animateClick();
}

void Dialog::isPortOpened_Slot(int i)
{
    Disable_Enable_Send(portsOpenStatus[i]);
}

void Dialog::changeSendStatus_Slot(int i)
{
    portsOpenStatus[i] = !portsOpenStatus[i];
    if (i == ptabWidget->currentIndex()) {
        Disable_Enable_Send(portsOpenStatus[i]);
    }
}

void Dialog::addTab(const QString& str)
{
    Show_Text_QW* newTab = new Show_Text_QW;
    tabs.push_back(newTab);
//    QPalette Show_Text_Palette = newTab->palette();
//    Show_Text_Palette.setColor(QPalette::Base, QColor(100, 200, 255, 255));
//    newTab->setPalette(Show_Text_Palette);
    ptabWidget->addTab(newTab, str);
    portsOpenStatus.push_back(false);
}

void Dialog::deleteTab()
{
    ptabWidget->removeTab(ptabWidget->count() - 1);
    delete tabs.last();
    tabs.pop_back();
    portsOpenStatus.pop_back();
}
