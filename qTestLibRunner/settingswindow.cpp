#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>
#include <QDebug>

SettingsWindow::SettingsWindow(Settings *settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->settings = settings;
    ui->edtTestsSearchDir->setText(settings->searchPathForTestExecutable);
    ui->edtTestsSearchMask->setText(settings->searchMaskForTestExecutable);
    ui->edtTestWorkingDir->setText(settings->workingPathForTestExecutable);
    ui->edtSourceRootDir->setText(settings->sourceRootDirectory);
    ui->edtQTCreatorPath->setText(settings->qtCreatorPath);
    ui->chbRunOnChange->setChecked(settings->runTestOnExecutableChange);
    ui->txtPathEnvironment->clear();
    ui->txtPathEnvironment->append(settings->pathEnvironment.join("\n"));
    testExecutables = new TestExecutables(settings->testExecutables);
    loadTestExecutablesInList();
}

SettingsWindow::~SettingsWindow()
{
    delete testExecutables;
    delete ui;
}


void SettingsWindow::on_buttonBox_accepted()
{
    settings->searchPathForTestExecutable =  ui->edtTestsSearchDir->text();
    settings->searchMaskForTestExecutable =  ui->edtTestsSearchMask->text();
    settings->workingPathForTestExecutable =  ui->edtTestWorkingDir->text();
    settings->sourceRootDirectory =  ui->edtSourceRootDir->text();
    settings->qtCreatorPath =  ui->edtQTCreatorPath->text();
    settings->runTestOnExecutableChange = ui->chbRunOnChange->isChecked();
    settings->pathEnvironment.clear();
    settings->pathEnvironment.append(ui->txtPathEnvironment->toPlainText().split('\n'));
    settings->testExecutables.load(*testExecutables);
}

void SettingsWindow::loadTestExecutablesInList(){
    ui->listTestExecutbles->clear();

#if 1
    for (int i=0;i<testExecutables->executables.count();i++){
        ui->listTestExecutbles->addItem(testExecutables->executables[i].pathOfExectuable);
    }
#endif
}

void SettingsWindow::on_toolButton_clicked()
{
    testExecutables->executableFilter = ui->edtTestsSearchMask->text();
    testExecutables->rootPath = ui->edtTestsSearchDir->text();

    testExecutables->scanDirectory();
    loadTestExecutablesInList();

#if 0
    qDebug() << testExecutables->executablesChanged();
    qDebug() << testExecutables->executablesChanged();
#endif

}

void SettingsWindow::on_pushButton_clicked()
{

}
