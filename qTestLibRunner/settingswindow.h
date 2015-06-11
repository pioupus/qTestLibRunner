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
    TestExecutables *testExecutables;

    void loadTestExecutablesInList();
signals:

public slots:
private slots:
    void on_buttonBox_accepted();
    void on_toolButton_clicked();
    void on_pushButton_clicked();


};

#endif // SETTINGSWINDOW_H
