#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QSettings>

class TestExecutableEntry{
public:


    QString workingDirectory;
    QString pathOfExectuable;
    QDateTime modificationDate;
    bool active;
};

class TestExecutables {
private:


public:
    TestExecutables();
    TestExecutables(QString rootPath, QString executablePath);
    TestExecutables(TestExecutables &testExecutables);

    void load(TestExecutables &testExecutables);
    bool scanDirectory();
    bool executablesChanged();
    void saveToQSettings(QSettings &qsetting);

    QList<TestExecutableEntry> executables;
    void loadFromQSettings(QSettings &qsetting);
    QString executableFilter;
    QString rootPath;
};



class Settings
{
private:
    QString fileName;
public:
    Settings(QString fileName);

    ~Settings();

    bool readFromFile(void);

    bool openNewFile(QString fileName);

    bool saveToFile(void);

    bool saveToFileAs(QString fileName);

    QString     qtCreatorPath;
    QStringList pathEnvironment;

    QString     searchMaskForTestExecutable;
    QString     searchPathForTestExecutable;

    QString     workingPathForTestExecutable;
    QString     sourceRootDirectory;
    bool        runTestOnExecutableChange;

    TestExecutables testExecutables;

};

#endif // SETTINGS_H
