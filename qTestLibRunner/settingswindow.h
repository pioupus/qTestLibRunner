#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
    class Settings;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::Settings *ui;

signals:

public slots:
};

#endif // SETTINGSWINDOW_H
