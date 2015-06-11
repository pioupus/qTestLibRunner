#include "settings.h"
#include <QSettings>
#include <QDebug>
#include <QDirIterator>


Settings::Settings(QString fileName)
{
    this->fileName = fileName;
}

Settings::~Settings(){

}

bool Settings::readFromFile(void)
{
    QSettings qsetting(fileName,QSettings::IniFormat);
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
    this->fileName = fileName;
    return readFromFile();
}

bool Settings::saveToFile(void)
{
    QSettings qsetting(fileName,QSettings::IniFormat);

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
    this->fileName = fileName;
    return saveToFile();
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
