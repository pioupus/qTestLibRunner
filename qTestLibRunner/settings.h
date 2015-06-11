#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QDateTime>

class TestExecutableEntry{
public:
    QString workingDirectory;
    QString pathOfExectuable;
    QDateTime modificationDate;
    bool active;
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

    QList<TestExecutableEntry> testExecutableList;

};

#endif // SETTINGS_H
