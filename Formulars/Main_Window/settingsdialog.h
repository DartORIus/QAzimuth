
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QSettings>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    struct portSettings {
        QString name;
        qint32 baudRate;
//        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
//        QString stringDataBits;
        QSerialPort::Parity parity;
//        QString stringParity;
        QSerialPort::StopBits stopBits;
//        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
//        QString stringFlowControl;
        bool localEchoEnabled;
    };

    explicit SettingsDialog(QSettings*, unsigned int n, QWidget* parent = 0);
    ~SettingsDialog();

    const portSettings& settings() const { return currentSettings; }
    QSerialPort* getPort() { return port; }
    QSerialPort::SerialPortError openSerialPort() const;
//    void initPort(void);

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
//    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort* port;
    const QString portName;
    portSettings currentSettings;
    QSettings* savedSettings;
    QIntValidator* intValidator;
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
