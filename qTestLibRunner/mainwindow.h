#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    bool runTests(QString directory);
    void printTests(QString xmlinput);

private slots:
    void on_btnRun_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
