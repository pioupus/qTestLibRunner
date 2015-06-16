#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QSettings>
#include <QMenu>
#include <QLabel>
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


    bool runTests();
    QList<TestCaseEntry> parseXML(QString xmlinput, QString testExecutable, QString testExecutableWorkingDir);
    void listTestResults(QList<TestCaseEntry> testResults, bool totalSuccess);




    void addFileLinks(QString xmlinput, QString rootpath);
    void runqtCreator(QString link);
private slots:
    void on_btnRun_clicked();
    void on_actionEditSettings_Triggered(bool checked = false);
    void on_actionLoad_from_File_Triggered(bool checked = false);
    void on_actionSave_as_Triggered(bool checked);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionRecentFile_Triggered(bool checked);

    void on_listLinks_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    Settings *settings;
    QSettings *sysSettings;
    RecentFiles *recentFiles;
    int timerId;
    QLabel *statusLabel;

    void saveAs(QString fileName);
    void openFile(QString fileName);
    void fillRecenFileMenu(RecentFiles *recentFiles);

protected:
    void timerEvent(QTimerEvent *event);
};



#endif // MAINWINDOW_H
