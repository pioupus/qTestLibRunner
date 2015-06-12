#include "settings.h"
#include <QSettings>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>

Settings::Settings(QString fileName)
{
    this->settingsFileName = fileName;
}

Settings::~Settings(){

}

bool Settings::readFromFile(void)
{
    QSettings qsetting(settingsFileName,QSettings::IniFormat);
    QString env = qsetting.value("pathEnvironment","").toString();
    qtCreatorPath = qsetting.value("qtCreator","qtcreator.exe").toString();
    pathEnvironment.append(env.split('\n'));
    searchMaskForTestExecutable = qsetting.value("searchMaskForTestExecutables","test*.exe").toString();
    searchPathForTestExecutable = qsetting.value("searchPathForTestExecutables",".").toString();

    workingPathForTestExecutable = qsetting.value("workingPathForTestExecutable",".").toString();
    sourceRootDirectory = qsetting.value("sourceRootDirectory",".").toString();

    runTestOnExecutableChange = qsetting.value("runTestOnExecutableChange",true).toBool();

    testExecutables.loadFromQSettings(qsetting);
    return true;
}

bool Settings::openNewFile(QString fileName)
{
    this->settingsFileName = fileName;
    return readFromFile();
}

bool Settings::saveToFile(void)
{
    QSettings qsetting(settingsFileName,QSettings::IniFormat);

    qsetting.setValue("qtCreator",qtCreatorPath);
    qsetting.setValue("pathEnvironment",pathEnvironment.join("\n"));

    qsetting.setValue("searchMaskForTestExecutables",searchMaskForTestExecutable);
    qsetting.setValue("searchPathForTestExecutables",searchPathForTestExecutable);

    qsetting.setValue("workingPathForTestExecutable",workingPathForTestExecutable);
    qsetting.setValue("sourceRootDirectory",sourceRootDirectory);

    qsetting.setValue("runTestOnExecutableChange",runTestOnExecutableChange);

    testExecutables.saveToQSettings(qsetting);
    return true;
}

bool Settings::saveToFileAs(QString fileName)
{
    this->settingsFileName = fileName;
    return saveToFile();
}

bool Settings::getRunTestOnExecutableChange() const
{
    return runTestOnExecutableChange;
}

void Settings::setRunTestOnExecutableChange(bool value)
{
    runTestOnExecutableChange = value;
}

QString Settings::getSourceRootDirectoryRelative() const{
    return sourceRootDirectory;
}

QString Settings::getSourceRootDirectoryAbsolute(QString testExecutable, QString testExecutableWorkingPath) const
{
    return getAbsolutePath(sourceRootDirectory,testExecutable,testExecutableWorkingPath);
}

void Settings::setSourceRootDirectory(const QString &value)
{
    sourceRootDirectory = value;
}

QString Settings::getWorkingPathForTestExecutableRelative() const
{
    return workingPathForTestExecutable;
}

QString Settings::getWorkingPathForTestExecutableAbsolute(QString testExecutable) const
{
    return getAbsolutePath(workingPathForTestExecutable,testExecutable,workingPathForTestExecutable);
}

void Settings::setWorkingPathForTestExecutable(const QString &value)
{
    workingPathForTestExecutable = value;
}

QString Settings::getAbsolutePath(QString relative, QString testExecutable, QString testExecutableWorkingPath) const
{
    bool variableFound = true;
    if (testExecutableWorkingPath.contains("%TEST_EXE_WORKING_PATH%")){
        QMessageBox::warning(0,"testExecutableWorkingPath mus not contain variable %TEST_EXE_WORKING_PATH%"  ,
                                      "testExecutableWorkingPath mus not contain variable %TEST_EXE_WORKING_PATH%",
                                      QMessageBox::Ok,QMessageBox::Ok);
    }
    if (testExecutable.contains("%")){
        QMessageBox::warning(0,"testExecutable must not contain variables"  ,
                                      "testExecutable must not contain variables",
                                      QMessageBox::Ok,QMessageBox::Ok);
    }
    QDir settingPath(QFileInfo(settingsFileName).dir());
    QDir testExecutablePath(QFileInfo(testExecutable).dir());

    if (QFileInfo(relative).isRelative() && !relative.contains("%")){
        relative = settingPath.absoluteFilePath(relative);
    }
    while (variableFound){
        variableFound = false;


        if (relative.contains("%SETTINGSFILE_PATH%")){
            relative.replace("%SETTINGSFILE_PATH%",settingPath.absolutePath());
            variableFound = true;
        }else if (relative.contains("%TEST_EXE_PATH%")){
            relative.replace("%TEST_EXE_PATH%",testExecutablePath.absolutePath());
            variableFound = true;
        }else if (relative.contains("%TEST_EXE_WORKING_PATH%")){
            relative.replace("%TEST_EXE_WORKING_PATH%",testExecutableWorkingPath);
            variableFound = true;
        }
    }
    QDir resultDir(relative);
    QString result = resultDir.absolutePath();
    return result;
}
void Settings::testAbsolutePath()
{

    QString testresult;
    QString settingsFileNameold = settingsFileName;
    settingsFileName = "C:/ich/bin/ein/settings.qtr";



    testresult = getAbsolutePath("%SETTINGSFILE_PATH%/relativer/Pfad","C:/ich/bin/ein/test.exe","%TEST_EXE_PATH%" );
    if ( testresult == "C:/ich/bin/ein/relativer/Pfad"){
        qDebug() << "OK:"+testresult;
    }else{
        qWarning() << "Wrong result " << testresult;
    }

    testresult = getAbsolutePath("%TEST_EXE_WORKING_PATH%/relativer/Pfad","C:/pfad/zu/einem/executable/test.exe","%TEST_EXE_PATH%" );
    if ( testresult == "C:/pfad/zu/einem/executable/relativer/Pfad"){
        qDebug() << "OK:" + testresult;
    }else{
        qWarning() << "Wrong result " << testresult;
    }
    testresult = getAbsolutePath("%TEST_EXE_WORKING_PATH%/relativer/Pfad","C:/pfad/zu/einem/executable/test.exe","%TEST_EXE_PATH%/.." );
    if ( testresult == "C:/pfad/zu/einem/relativer/Pfad"){
        qDebug() << "OK:" + testresult;
    }else{
        qWarning() << "Wrong result " << testresult;
    }

#if 0
    testresult = getAbsolutePath("%TEST_EXE_WORKING_PATH%/relativer/Pfad","C:/pfad/zu/einem/executable/test.exe","%TEST_EXE_WORKING_PATH%" );
    if ( testresult == "C:/pfad/zu/einem/executable/relativer/Pfad"){
        qDebug() << "OK:" + testresult;
    }else{
        qWarning() << "Wrong result " << testresult;
    }
#endif
    testresult = getAbsolutePath("relativer/Pfad","C:/pfad/zu/einem/executable/test.exe","%TEST_EXE_PATH%/.." );
    if ( testresult == "C:/ich/bin/ein/relativer/Pfad"){
        qDebug() << "OK:" + testresult;
    }else{
        qWarning() << "Wrong result " << testresult;
    }
    settingsFileName = settingsFileNameold;
}



QString Settings::getSearchPathForTestExecutableRelative() const
{
    return searchPathForTestExecutable;
}

QString Settings::getSearchPathForTestExecutableAbsolute(QString testExecutable, QString testExecutableWorkingPath) const
{
    return getAbsolutePath(searchPathForTestExecutable,testExecutable,testExecutableWorkingPath);
}

void Settings::setSearchPathForTestExecutable(const QString &value)
{
    searchPathForTestExecutable = value;
}

QString Settings::getSearchMaskForTestExecutable() const
{
    return searchMaskForTestExecutable;
}

void Settings::setSearchMaskForTestExecutable(const QString &value)
{
    searchMaskForTestExecutable = value;
}



QStringList Settings::getPathEnvironment() const
{
    return pathEnvironment;
}

void Settings::setPathEnvironment(const QStringList &value)
{
    pathEnvironment = value;
}

QString Settings::getQtCreatorPath() const
{
    return qtCreatorPath;
}

void Settings::setQtCreatorPath(const QString &value)
{
    qtCreatorPath = value;
}




TestExecutables::TestExecutables()
{

}

TestExecutables::TestExecutables(QString rootPath, QString executablePath)
{
    this->rootPath = rootPath;
    this->executableFilter = executablePath;
}

TestExecutables::TestExecutables(TestExecutables &testExecutables)
{
    load(testExecutables);
}

void TestExecutables::load(TestExecutables &testExecutables)
{
    this->rootPath = testExecutables.rootPath;
    this->executableFilter = testExecutables.executableFilter;
    this->executables.clear();
    for(int i=0;i<testExecutables.executables.count();i++){
        TestExecutableEntry entry = testExecutables.executables[i];
        executables.append(entry);
    }
}

bool TestExecutables::scanDirectory()
{
    QStringList filter;
    filter.append(executableFilter);
    //filter, QDir::Executable,
    QDirIterator it(rootPath, filter, QDir::Files | QDir::Executable, QDirIterator::Subdirectories);
    executables.clear();
    while (it.hasNext()) {
        it.next();
        TestExecutableEntry entry;
        entry.active = true;
        entry.pathOfExectuable=it.filePath();

        entry.workingDirectory="";
        entry.modificationDate = it.fileInfo().lastModified();
        executables.append(entry);
        qDebug() << "filenme:" +entry.pathOfExectuable;
    }
    return true;
}

bool TestExecutables::executablesChanged()
{
    bool result = false;
    for(int i=0;i<executables.count();i++){
        QFileInfo fileinfo(executables[i].pathOfExectuable);
        if (fileinfo.lastModified() != executables[i].modificationDate){
            result = true;
            executables[i].modificationDate = fileinfo.lastModified();
            //qDebug() << executables[i].pathOfExectuable;
        }
    }
    return result;
}

void TestExecutables::saveToQSettings(QSettings &qsetting)
{
    qsetting.beginGroup("TestExecutables");
    QStringList groups = qsetting.childGroups();
    for(int i=0;i<groups.count();i++){
        qsetting.beginGroup(groups[i]);
        qsetting.remove("FileName");
        qsetting.remove("WorkingDir");
        qsetting.remove("active");
        qsetting.endGroup();
    }

    for(int i=0;i<executables.count();i++){
        qsetting.beginGroup("entry"+QString::number(i));
        qsetting.setValue("FileName",executables[i].pathOfExectuable);
        qsetting.setValue("WorkingDir",executables[i].workingDirectory);
        qsetting.setValue("active",executables[i].active);
        qsetting.endGroup();
    }
    qsetting.endGroup();
}

void TestExecutables::loadFromQSettings(QSettings &qsetting)
{
    qsetting.beginGroup("TestExecutables");
    QStringList groups = qsetting.childGroups();
    executables.clear();
    for(int i=0;i<groups.count();i++){
        TestExecutableEntry entry;
        qsetting.beginGroup(groups[i]);
        entry.pathOfExectuable = qsetting.value("FileName","").toString();
        entry.workingDirectory = qsetting.value("WorkingDir","").toString();
        entry.active = qsetting.value("active","").toBool();
        executables.append(entry);
        qsetting.endGroup();
    }
    qsetting.endGroup();
}
