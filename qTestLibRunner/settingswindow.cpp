#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Settings *settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->settings = settings;
    ui->edtTestsSearchDir->setText(settings->searchPathForTestExecutable);
    ui->edtTestsSearchMask->setText(settings->searchMaskForTestExecutable);
    ui->edtTestWorkingDir->setText(settings->workingPathForTestExecutable);
    ui->edtSourceRootDir->setText(settings->sourceRootDirectory);
    ui->edtQTCreatorPath->setText(settings->qtCreatorPath);
    ui->txtPathEnvironment->clear();
    ui->txtPathEnvironment->append(settings->pathEnvironment.join('\n'));

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


void SettingsWindow::on_buttonBox_accepted()
{
    settings->searchPathForTestExecutable =  ui->edtTestsSearchDir->text();
    settings->searchMaskForTestExecutable =  ui->edtTestsSearchMask->text();
    settings->workingPathForTestExecutable =  ui->edtTestWorkingDir->text();
    settings->sourceRootDirectory =  ui->edtSourceRootDir->text();
    settings->qtCreatorPath =  ui->edtQTCreatorPath->text();
    settings->pathEnvironment.clear();
    settings->pathEnvironment.append(ui->txtPathEnvironment->toPlainText().split('\n'));
}
