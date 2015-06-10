#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "settingswindow.h"

namespace Ui {
class MainWindow;
}


class TestFunctionEntry
{
public:
    QString name;
    QString duration;
    bool passed;
    QString fileName;
    QString description;
    QString lineNumber;
};

class TestCaseEntry
{
public:
    QString name;
    QString duration;
    QString testSourceDirectory;
    QList<TestFunctionEntry> testFunctionList;
};

class QTreeWidgetItemWithData: public QTreeWidgetItem
{
public:
    QTreeWidgetItemWithData(QTreeWidget * parent, int type = Type): QTreeWidgetItem(parent,type){}
    QTreeWidgetItemWithData(QTreeWidgetItem * parent, int type = Type): QTreeWidgetItem(parent,type){}

    TestFunctionEntry   testFunctionEntry;
    TestCaseEntry       testCaseEntry;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    bool runTests(QString directory);
    QList<TestCaseEntry> parseXML(QString xmlinput, QString testExecutablePath);
    void listTestResults(QList<TestCaseEntry> testResults);


private slots:
    void on_btnRun_clicked();
    void on_actionEditSettings_Triggered(bool checked = false);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;

};



#endif // MAINWINDOW_H
