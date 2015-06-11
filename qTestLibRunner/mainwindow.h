#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QMenu>
#include "settingswindow.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class RecentFiles
{
public:
    explicit RecentFiles(QStringList initList);
    ~RecentFiles();

    QStringList recentFiles;
    void addToRecentFiles(QString fileName);
};

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
    QList<TestCaseEntry> parseXML(QString xmlinput);
    void listTestResults(QList<TestCaseEntry> testResults);




private slots:
    void on_btnRun_clicked();
    void on_actionEditSettings_Triggered(bool checked = false);
    void on_actionLoad_from_File_Triggered(bool checked = false);
    void on_actionSave_as_Triggered(bool checked);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionRecentFile_Triggered(bool checked);

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    Settings *settings;
    QSettings *sysSettings;
    RecentFiles *recentFiles;

    void saveAs(QString fileName);
    void openFile(QString fileName);
    void fillRecenFileMenu(RecentFiles *recentFiles);
};



#endif // MAINWINDOW_H
