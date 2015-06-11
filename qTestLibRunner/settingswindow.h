#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "settings.h"

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    SettingsWindow(Settings *settings, QWidget *parent = 0);

    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    Settings *settings;

signals:

public slots:
private slots:
    void on_buttonBox_accepted();
};

#endif // SETTINGSWINDOW_H
