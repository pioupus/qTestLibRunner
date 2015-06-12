#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>
#include <QDebug>

SettingsWindow::SettingsWindow(Settings *settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->settings = settings;
    ui->pushButton->setVisible(false);
    ui->edtTestsSearchDir->setText(settings->getSearchPathForTestExecutableRelative());
    ui->edtTestsSearchMask->setText(settings->getSearchMaskForTestExecutable());
    ui->edtTestWorkingDir->setText(settings->getWorkingPathForTestExecutableRelative());
    ui->edtSourceRootDir->setText(settings->getSourceRootDirectoryRelative());
    ui->edtQTCreatorPath->setText(settings->getQtCreatorPath());
    ui->chbRunOnChange->setChecked(settings->getRunTestOnExecutableChange());
    ui->txtPathEnvironment->clear();
    ui->txtPathEnvironment->append(settings->getPathEnvironment().join("\n"));
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
    QStringList pathEnvironment;
    settings->setSearchPathForTestExecutable(ui->edtTestsSearchDir->text());
    settings->setSearchMaskForTestExecutable(ui->edtTestsSearchMask->text());
    settings->setWorkingPathForTestExecutable(ui->edtTestWorkingDir->text());
    settings->setSourceRootDirectory(ui->edtSourceRootDir->text());
    settings->setQtCreatorPath(ui->edtQTCreatorPath->text());
    settings->setRunTestOnExecutableChange(ui->chbRunOnChange->isChecked());
    //settings->setPathEnvironment.clear();
    pathEnvironment.append(ui->txtPathEnvironment->toPlainText().split('\n'));
    //qDebug() << pathEnvironment.count();
    settings->setPathEnvironment(pathEnvironment);
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
    testExecutables->rootPath = settings->getAbsolutePath(ui->edtTestsSearchDir->text(),"",ui->edtTestWorkingDir->text());

    testExecutables->scanDirectory();
    loadTestExecutablesInList();

#if 0
    qDebug() << testExecutables->executablesChanged();
    qDebug() << testExecutables->executablesChanged();
#endif

}

void SettingsWindow::on_pushButton_clicked()
{
    settings->testAbsolutePath();
}
