#include "settings.h"
#include <QSettings>
#include <QDebug>


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
    qsetting.setValue("pathEnvironment",pathEnvironment.join('\n'));

    qsetting.setValue("searchMaskForTestExecutables",searchMaskForTestExecutable);
    qsetting.setValue("searchPathForTestExecutables",searchPathForTestExecutable);

    qsetting.setValue("workingPathForTestExecutable",workingPathForTestExecutable);
    qsetting.setValue("sourceRootDirectory",sourceRootDirectory);
    return true;
}

bool Settings::saveToFileAs(QString fileName)
{
    this->fileName = fileName;
    return saveToFile();
}

