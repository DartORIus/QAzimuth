#ifndef SERIALPORTACTION_H
#define SERIALPORTACTION_H

#include <QAction>
#include <QIcon>
//#include <QSerialPort>
#include "settingsdialog.h"

class SerialPortAction : public QAction
{
    Q_OBJECT
public:
    enum ACT_TYPES {CONNECT = 0, DISCONNECT, SETTINGS, ADD, DELETE};

    explicit SerialPortAction(const QIcon&, const QString&, ACT_TYPES, SettingsDialog* s = nullptr, QObject* parent = nullptr);

    SettingsDialog* getSettingsDialog(void) { return port; }
    const ACT_TYPES& getType(void) const { return type; }
    void setType(ACT_TYPES t) { type = t; }

public slots:
    void wasTriggered_slot();

signals:
    void wasTriggered_signal(SerialPortAction*);

private:
    ACT_TYPES type;
    SettingsDialog* port;
};

#endif // SERIALPORTACTION_H
