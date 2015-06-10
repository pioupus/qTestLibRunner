#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"

#include <QProcess>
#include <QtDebug>
#include <QDomDocument>
#include <QFont>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->actionEditSettings, SIGNAL(triggered(bool)),
                        this, SLOT(on_actionEditSettings_Triggered(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRun_clicked()
{
    runTests("");
}

void MainWindow::on_actionEditSettings_Triggered(bool checked)
{
    (void)checked;
    settingsWindow = new SettingsWindow(this);
    settingsWindow->show();
}

#if 0
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

#endif

bool MainWindow::runTests(QString directory){
    (void) directory;
    QProcess testProcess;
    QStringList env = QProcess::systemEnvironment();
    QStringList paths;
    QList<TestCaseEntry> testResults;

    paths << "../src/debug/";
    paths << "../../../libs/PythonQt3.0/lib";
    for (int i=0;i<env.count();i++){
        if (env[i].indexOf("PATH") == 0){
            //qDebug() << env.at(i);
            for(int n = 0;n<paths.count();n++){
                env[i] = env[i]+';'+paths[n];
            }
            break;
        }
    }
    testProcess.setEnvironment(env);
    testProcess.setWorkingDirectory("C:\\Users\\ark\\entwicklung\\qt\\crystalTestFrameworkApp\\builds\\crystalTestFrameworkApp-Desktop_Qt_5_4_1_MinGW_32bit-Debug\\tests");
    testProcess.start("C:/Users/ark/entwicklung/qt/crystalTestFrameworkApp/builds/crystalTestFrameworkApp-Desktop_Qt_5_4_1_MinGW_32bit-Debug/tests/debug/tests.exe", QStringList() << "-xml");
    if (!testProcess.waitForStarted()){
        qDebug() << "finished problem";
        return false;
    }
    qDebug() << "Test Started";
    if (!testProcess.waitForFinished()){
        qDebug() << "finished problem";
        return false;
    }
    qDebug() << "Test Finished";
    QByteArray result = testProcess.readAllStandardOutput();
    QByteArray err = testProcess.readAllStandardError();

    qDebug() << "stdErr: " << err;
    //qDebug() << result;

    testResults = parseXML(QString(result),testProcess.workingDirectory());
    listTestResults(testResults);
    return true;
}
#if  0
QString getXMLAttribute(QXmlStreamReader xml, QString Name){
    foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
        if (attr.name().toString() == QLatin1String("QStringName")) {
            QString attribute_value = attr.value().toString();
            return(attribute_value);
        }
    }
    return "";
}

#endif
QString removeXMLEncodingTag(QString in){
    QString xmlEcodingTag;
    while (in.indexOf("<?xml") > -1){
        int tagStart = in.indexOf("<?xml");
        int tagEnd = in.indexOf("?>",tagStart) ;
        xmlEcodingTag = in.mid(tagStart,tagEnd-tagStart+2);
        //qDebug() << xmlEcodingTag;
        in.remove(tagStart,tagEnd-tagStart+2);
    }
    return xmlEcodingTag+"<root>"+in+"</root>";

//<?xml version="1.0" encoding="UTF-8"?>
}

QList<TestCaseEntry> MainWindow::parseXML(QString xmlinput,QString testExecutableWorkingDirectory){
#if 0
    <?xml version="1.0" encoding="UTF-8"?>
    <TestCase name="TestScriptEngine">
        <Environment>
            <QtVersion>5.4.1</QtVersion>
            <QtBuild>Qt 5.4.1 (i386&#x002D;little_endian&#x002D;ilp32 shared (dynamic) debug build; by GCC 4.9.1)</QtBuild>
            <QTestVersion>5.4.1</QTestVersion>
        </Environment>
        <TestFunction name="initTestCase">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.015179"/>
        </TestFunction>
        <TestFunction name="getFilesInDirectory">
        <Incident type="fail" file="..\..\..\tests\testScriptEngine.cpp" line="35">
            <Description><![CDATA[Compared values are not the same
           Actual   (filelist.count()): 4
           Expected (2)               : 2]]></Description>
        </Incident>
            <Duration msecs="0.438558"/>
        </TestFunction>
        <TestFunction name="runScriptGettingStarted">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.010229"/>
        </TestFunction>
        <TestFunction name="cleanupTestCase">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.004949"/>
        </TestFunction>
        <Duration msecs="0.518086"/>
    </TestCase>



    <?xml version="1.0" encoding="UTF-8"?>
    <TestCase name="TestQString">
        <Environment>
            <QtVersion>5.4.1</QtVersion>
            <QtBuild>Qt 5.4.1 (i386&#x002D;little_endian&#x002D;ilp32 shared (dynamic) debug build; by GCC 4.9.1)</QtBuild>
            <QTestVersion>5.4.1</QTestVersion>
        </Environment>
        <TestFunction name="initTestCase">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.007919"/>
        </TestFunction>
        <TestFunction name="toUpper">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.015509"/>
        </TestFunction>
        <TestFunction name="cleanupTestCase">
        <Incident type="pass" file="" line="0" />
            <Duration msecs="0.004619"/>
        </TestFunction>
        <Duration msecs="0.053458"/>
    </TestCase>
#endif

    QDomDocument doc("mydocument");
    xmlinput = removeXMLEncodingTag(xmlinput);
    doc.setContent(xmlinput);
    QDomElement docElem = doc.documentElement();

    QDomNode testCases = docElem.firstChild();
    QList<TestCaseEntry> testCasesList;

    while(!testCases.isNull()) {
        QDomElement testCase = testCases.toElement(); // try to convert the node to an element.
        if(!testCase.isNull()) {
            TestCaseEntry testCaseEntry;
            testCaseEntry.name = testCase.attribute("name");
            testCaseEntry.duration = testCase.firstChildElement("Duration").attribute("msecs");
            testCaseEntry.testSourceDirectory = testExecutableWorkingDirectory;
            if (testCase.tagName() == "TestCase"){
                QDomNode testFunctions = testCase.firstChild();

                while(!testFunctions.isNull()) {
                    TestFunctionEntry testFunctionEntry;
                    QDomElement testFunction = testFunctions.toElement();
                    if(!testFunction.isNull()) {
                        if (testFunction.tagName() == "TestFunction"){
                           testFunctionEntry.name = testFunction.attribute("name");
                           QDomElement testIncident = testFunction.firstChildElement("Incident");
                           testFunctionEntry.passed = testIncident.attribute("type") == "pass";
                           testFunctionEntry.fileName = testIncident.attribute("file");
                           testFunctionEntry.lineNumber = testIncident.attribute("line");
                           QDomNodeList testIncidentDurations = testFunction.elementsByTagName("Duration");
                           if (testIncidentDurations.count() > 0){
                               QDomElement testIncidentDuration = testIncidentDurations.at(0).toElement();
                               testFunctionEntry.duration = testIncidentDuration.attribute("msecs");

                           }
                           QDomNodeList testIncidentDescriptions = testFunction.elementsByTagName("Description");
                           if (testIncidentDescriptions.count() > 0){
                               QDomElement testIncidentDescription = testIncidentDescriptions.at(0).toElement();
                               testFunctionEntry.description = testIncidentDescription.text();

                           }

                           testCaseEntry.testFunctionList.append(testFunctionEntry);

                        }
                        //qDebug() << qPrintable(testFunction.tagName()) << qPrintable(testFunction.attribute("name")) << endl; // the node really is an element.
                    }
                    testFunctions = testFunctions.nextSibling();

                }
            }
            testCasesList.append(testCaseEntry);
#if 0
            <Incident type="pass" file="" line="0" />
                <Duration msecs="0.007589"/>
            </TestFunction>
#endif
        }
        testCases = testCases.nextSibling();
    }

    return testCasesList;
}

void MainWindow::listTestResults(QList<TestCaseEntry> testResults)
{
    QFont font("Courier");
    this->ui->treeWidget->clear();
    this->ui->treeWidget->setColumnCount(4);
    //0:name
    //1:OK/FAIL
    //2:Duration
    //3:Description
    for( int i=0; i<testResults.count(); i++ )
    {
        bool testCasePassed = true;
        TestCaseEntry testCase = testResults.at(i);
        QTreeWidgetItemWithData *testCaseItem =  new QTreeWidgetItemWithData(this->ui->treeWidget);
        testCaseItem->setText(0,testCase.name);
        testCaseItem->setText(2,testCase.duration+"ms");
        this->ui->treeWidget->addTopLevelItem(testCaseItem);
        for( int n=0; n<testCase.testFunctionList.count(); n++ ){

            TestFunctionEntry testFunctionEntry = testCase.testFunctionList.at(n);
            QTreeWidgetItemWithData *testFunctionItem =  new QTreeWidgetItemWithData(testCaseItem);
            //qDebug() << "testname:" << testFunctionEntry.name;
            testFunctionItem->setText(0,testFunctionEntry.name);
            testFunctionItem->setText(2,testFunctionEntry.duration+"ms");
            testFunctionItem->testFunctionEntry = testFunctionEntry;
            testFunctionItem->testCaseEntry = testCase;
            //testFunctionItem->setData(3,);
            if ((testFunctionEntry.fileName != "") || (testFunctionEntry.description != "")){
                testFunctionItem->setFont(3,font);
                testFunctionItem->setText(3,testFunctionEntry.fileName+':'+testFunctionEntry.lineNumber +'\n'+ testFunctionEntry.description);
            }
            if (testFunctionEntry.passed){
                testFunctionItem->setText(1,"OK");
                testFunctionItem->setBackground(1,QBrush(Qt::green));
            }else{
                testCasePassed = false;
                testFunctionItem->setText(1,"Fail");
                testFunctionItem->setBackground(0,QBrush(Qt::red));
                testFunctionItem->setBackground(1,QBrush(Qt::red));
                testCaseItem->setExpanded(true);

            }

            testCaseItem->addChild(testFunctionItem);
        }
        if (testCasePassed){
            testCaseItem->setBackground(1,QBrush(Qt::green));
            testCaseItem->setText(1,"OK");
        }else{
            testCaseItem->setBackground(0,QBrush(Qt::red));
            testCaseItem->setBackground(1,QBrush(Qt::red));
            testCaseItem->setText(1,"Fail");
        }
    }
    for (int i = 0;i<this->ui->treeWidget->columnCount();i++){
        this->ui->treeWidget->resizeColumnToContents(i);
    }
    this->ui->treeWidget->sortItems(1,Qt::AscendingOrder);//Qt::DescendingOrder

}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    (void)column;
    if( QTreeWidgetItemWithData* itemWithData = dynamic_cast< QTreeWidgetItemWithData* >( item ) )
    {
        if(itemWithData->testFunctionEntry.fileName != ""){
            QStringList arguments;
            QString path = "C:/Qt/Qt5.4.1/Tools/QtCreator/bin/";
            QString sourceDirectory = itemWithData->testCaseEntry.testSourceDirectory;
            QDir qpath(sourceDirectory);
            QString sourceFileAbs = sourceDirectory+'/'+itemWithData->testFunctionEntry.fileName;//qpath.relativeFilePath(itemWithData->testFunctionEntry.fileName);
            //qDebug() << sourceFileAbs;
            arguments <<"-client" <<sourceFileAbs+':'+itemWithData->testFunctionEntry.lineNumber;
            QProcess::startDetached(path+"qtcreator.exe", arguments);
        }
    }
}

