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
    QString settingsFileName;


    QString     qtCreatorPath;
    QStringList pathEnvironment;

    QString     searchMaskForTestExecutable;
    QString     searchPathForTestExecutable;

    QString     workingPathForTestExecutable;
    QString     sourceRootDirectory;
    bool        runTestOnExecutableChange;
public:
    Settings(QString fileName);

    ~Settings();

    bool readFromFile(void);

    bool openNewFile(QString fileName);

    bool saveToFile(void);

    bool saveToFileAs(QString fileName);



    TestExecutables testExecutables;

    QString getQtCreatorPath() const;
    void setQtCreatorPath(const QString &value);
    QStringList getPathEnvironment() const;
    void setPathEnvironment(const QStringList &value);
    QString getSearchMaskForTestExecutable() const;
    void setSearchMaskForTestExecutable(const QString &value);

    QString getSearchPathForTestExecutableRelative() const;
    QString getSearchPathForTestExecutableAbsolute(QString testExecutable, QString testExecutableWorkingPath) const;
    void setSearchPathForTestExecutable(const QString &value);

    QString getWorkingPathForTestExecutableRelative() const;
    QString getWorkingPathForTestExecutableAbsolute(QString testExecutable) const;
    void setWorkingPathForTestExecutable(const QString &value);

    QString getAbsolutePath(QString relative, QString testExecutable, QString testExecutableWorkingPath) const;
    QString getSourceRootDirectoryRelative() const;
    QString getSourceRootDirectoryAbsolute(QString testExecutable, QString testExecutableWorkingPath) const;
    void setSourceRootDirectory(const QString &value);
    bool getRunTestOnExecutableChange() const;
    void setRunTestOnExecutableChange(bool value);

    QString getAbsolutePathRelativeToSettingsFile(QString relative) const;

    void testAbsolutePath();

};

#endif // SETTINGS_H
