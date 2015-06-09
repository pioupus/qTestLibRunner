#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>
#include <QtDebug>
#include <QXmlStreamReader>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRun_clicked()
{
    runTests("");
}
/*
<?xml version="1.0" encoding="ISO-8859-1"?>
<TestCase name="TestQString">
    <Environment>
        <QtVersion>4.8.6</QtVersion>
        <QTestVersion>4.8.6</QTestVersion>
    </Environment>
    <TestFunction name="initTestCase">
        <Incident type="pass" file="" line="0" />
    </TestFunction>
    <TestFunction name="toUpper">
        <Incident type="pass" file="" line="0" />
    </TestFunction>
    <TestFunction name="cleanupTestCase">
        <Incident type="pass" file="" line="0" />
    </TestFunction>
</TestCase>
<?xml version="1.0" encoding="ISO-8859-1"?>
<TestCase name="TestScriptEngine">
    <Environment>
        <QtVersion>4.8.6</QtVersion>
        <QTestVersion>4.8.6</QTestVersion>
    </Environment>
    <TestFunction name="initTestCase">
        <Incident type="pass" file="" line="0" />
    </TestFunction>
    <TestFunction name="getFilesInDirectory">
        <Incident type="pass" file="" line="0" />
    </TestFunction>
        <TestFunction name="cleanupTestCase">
    <Incident type="pass" file="" line="0" />
    </TestFunction>
</TestCase>
"
*/

bool MainWindow::runTests(QString directory){
    (void) directory;
    QProcess testProcess;
    testProcess.setWorkingDirectory("/home/arne/programming/qt/crystalTestFrameworkApp/builds/crystalTestFrameworkApp-Desktop-Debug/tests/");
    testProcess.start("/home/arne/programming/qt/crystalTestFrameworkApp/builds/crystalTestFrameworkApp-Desktop-Debug/tests/tests", QStringList() << "-xml");
    if (!testProcess.waitForStarted()){
        qDebug() << "finished problem";
        return false;
    }
    qDebug() << "started";
    if (!testProcess.waitForFinished()){
        qDebug() << "finished problem";
        return false;
    }
    qDebug() << "finished";
    QByteArray result = testProcess.readAllStandardOutput();
//qDebug() << result;
    printTests(QString(result));
}

QString getXMLAttribute(QXmlStreamReader xml, QString Name){
    foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
        if (attr.name().toString() == QLatin1String("QStringName")) {
            QString attribute_value = attr.value().toString();
            return(attribute_value);
        }
    }
    return "";
}

void MainWindow::printTests(QString xmlinput){
    QXmlStreamReader xml(xmlinput);
    while (!xml.atEnd()) {
          xml.readNext();
          QString tagName=xml.name().toString();
          //qDebug() <<
          if(tagName == "TestFunction"){
              QXmlStreamReader xmlFunction=xml;
              QString functionName = getXMLAttribute(xml,"name");
              qDebug() << functionName;
              while (!xmlFunction.atEnd()) {
                  xmlFunction.readNext();
                  QString tagName=xmlFunction.name().toString();
                  if(tagName == "Incident"){
                        QString status = getXMLAttribute(xmlFunction,"type");
                        qDebug() << status;
                  }
              }

          }
                          //... // do processing
    }

}
