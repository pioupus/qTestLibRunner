#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"

#include <QRegExp>
#include <QProcess>
#include <QtDebug>
#include <QDomDocument>
#include <QFont>
#include <QDir>
#include <QFileDialog>

#define MAXRECENTFILESCOUNT 10

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString actualFileName;
    ui->setupUi(this);
    statusLabel = new QLabel(ui->statusBar);

    statusLabel->setMinimumHeight(20);
    ui->statusBar->setMinimumHeight(30);
    sysSettings = new QSettings{"config.conf", QSettings::IniFormat};
    actualFileName = sysSettings->value("lastProjectsettingsFile","myProjectSetting.qtr").toString();
    settings = new Settings{actualFileName};
    recentFiles = new RecentFiles(sysSettings->value("recentFiles","").toStringList());
    recentFiles->addToRecentFiles(actualFileName);
    fillRecenFileMenu(recentFiles);
    settings->readFromFile();
    QObject::connect(ui->actionEditSettings, SIGNAL(triggered(bool)),
                        this, SLOT(on_actionEditSettings_Triggered(bool)));
    QObject::connect(ui->actionLoad_from_File, SIGNAL(triggered(bool)),
                        this, SLOT(on_actionLoad_from_File_Triggered(bool)));
    QObject::connect(ui->actionSave_as, SIGNAL(triggered(bool)),
                        this, SLOT(on_actionSave_as_Triggered(bool)));
    QObject::connect(ui->actionRun, SIGNAL(triggered(bool)),
                        this, SLOT(on_btnRun_clicked()));

    timerId = startTimer(1000);

}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    sysSettings->setValue("recentFiles",recentFiles->recentFiles);
    settings->saveToFile();
    delete recentFiles;
    delete settings;
    delete sysSettings;
    delete statusLabel;
    delete ui;
}

void MainWindow::on_btnRun_clicked()
{
    runTests();
}

void MainWindow::openFile(QString fileName)
{
    settings->openNewFile(fileName);
    recentFiles->addToRecentFiles(fileName);
    fillRecenFileMenu(recentFiles);
    sysSettings->setValue("lastProjectsettingsFile",fileName);
}

void MainWindow::saveAs(QString fileName)
{
    settings->saveToFileAs(fileName);
    recentFiles->addToRecentFiles(fileName);
    fillRecenFileMenu(recentFiles);
    sysSettings->setValue("lastProjectsettingsFile",fileName);
}

void MainWindow::on_actionLoad_from_File_Triggered(bool checked){
    (void)checked;
    QFileDialog dial(this);
    dial.setNameFilter(tr("QTest Runner Settings (*.qtr)"));
    dial.setFileMode(QFileDialog::ExistingFile);
    if (dial.exec()) {
        openFile(dial.selectedFiles()[0]);
    }
}


void MainWindow::fillRecenFileMenu(RecentFiles *recentFiles)
{
    QList<QAction *> actionList;

    actionList = ui->menuRecent_Files->actions();
    for (int i =0;i<actionList.count();i++){
        ui->menuRecent_Files->removeAction(actionList[i]);
    }

    for (int i =1;i<recentFiles->recentFiles.count();i++){
        QAction * action = ui->menuRecent_Files->addAction(recentFiles->recentFiles[i]);
        QObject::connect(action, SIGNAL(triggered(bool)),
                            this, SLOT(on_actionRecentFile_Triggered(bool)));
    }


}

void MainWindow::timerEvent(QTimerEvent *event)
{
    (void)event;
    if (settings->getRunTestOnExecutableChange()){
        if (settings->testExecutables.executablesChanged()){
            runTests();
        }
    }
}

void MainWindow::on_actionSave_as_Triggered(bool checked){
    (void)checked;
    QFileDialog dial(this);
    dial.setNameFilter(tr("QTest Runner Settings (*.qtr)"));
    dial.setFileMode(QFileDialog::AnyFile);
    if (dial.exec()) {
        QFileInfo fname(dial.selectedFiles()[0]);
        if (fname.suffix() != "qtr"){
            saveAs(fname.absoluteFilePath()+".qtr");
        }else{
            saveAs(fname.absoluteFilePath());
        }
    }
}

void MainWindow::on_actionRecentFile_Triggered(bool checked)
{
    (void)checked;
    QObject * senderObj = QObject::sender();
    if( QAction* senderAction = dynamic_cast< QAction* >( senderObj ) ){
        openFile(senderAction->text());
    }
}

void MainWindow::on_actionEditSettings_Triggered(bool checked)
{
    (void)checked;
    settingsWindow = new SettingsWindow{settings};
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

QString findUnXMLedString(QString in){
    (void)in;
    QRegExp reg("</.+>");
    QString result="";
    int pos;
    pos = reg.lastIndexIn(in);
    if (pos>-1){
        result = in.mid(pos+reg.matchedLength(),in.length());
        result = result.trimmed();
    }
    return result;
}

bool MainWindow::runTests(){

    QProcess testProcess;
    QStringList env = QProcess::systemEnvironment();
    QStringList paths = settings->getPathEnvironment();
    QList<TestCaseEntry> testResults;
    bool totalSuccess = true;


    //paths << "../src/debug/";
    //paths << "../../../libs/PythonQt3.0/lib";
    ui->txtLog->clear();
    ui->txtLog->appendHtml("<b>start: </b>"+QDateTime::currentDateTime().toString());
    ui->txtLog->appendHtml("");
    if (paths.count() > 0){
        bool pathNotfound = true;
        ui->txtLog->appendHtml("<b>Adding to Path:</b>");
        for(int i=0;i<paths.count();i++){
            //qDebug() << paths[i];
            ui->txtLog->appendHtml("<font color=\"black\">"+paths[i]+"</font>");
        }
        for (int i=0;i<env.count();i++){
            if (env[i].indexOf("PATH=") == 0){
                pathNotfound = false;
               // qDebug() << env.at(i);
                for(int n = 0;n<paths.count();n++){
                    env[i] = env[i]+';'+paths[n];
                }
                break;
            }
        }
        if(pathNotfound){
            QString path="";
            for(int n = 0;n<paths.count();n++){
                if (n > 0){
                    path = path+';';
                }
                path = path+paths[n];
            }
            env.append("PATH="+path);
        }
        ui->txtLog->appendPlainText("");
    }
    testProcess.setEnvironment(env);
    QByteArray testOutPut;

    for(int i = 0;i<settings->testExecutables.executables.count();i++){

        QString program = settings->testExecutables.executables[i].pathOfExectuable;
        QString workingdir = settings->testExecutables.executables[i].workingDirectory;
        if (workingdir == ""){
            workingdir = settings->getWorkingPathForTestExecutableAbsolute(program);
        }
        ui->txtLog->appendHtml("<b>Running:</b> <tab id=t1>\t\t\t\t"+program);
        ui->txtLog->appendHtml("<b>in Directory:</b> <tab to=t1>\t\t\t\t"+workingdir);
        testProcess.setWorkingDirectory(workingdir);
        testProcess.start(program, QStringList() << "-xml");
        if (!testProcess.waitForStarted()){
            ui->txtLog->appendHtml("<font color=\"red\">program didnt start well</font>");
            totalSuccess = false;
        }
        if (!testProcess.waitForFinished()){
            ui->txtLog->appendHtml("<font color=\"red\">program didnt finish within timeout</font>");
            totalSuccess = false;
            //qDebug() << "finished problem";
        }
        QByteArray stdout_ = testProcess.readAllStandardOutput();
        //qDebug() << stdout_;
        if (QString(stdout_)==""){
            ui->txtLog->appendHtml("<font color=\"red\">program didnt generate output on stdout</font>");
            totalSuccess = false;
        }

        if (testProcess.exitCode() != QProcess::NormalExit){
            ui->txtLog->appendHtml("<font color=\"red\">program exited with code " +QString::number(testProcess.exitCode())+"</font>");
            totalSuccess = false;
        }

        testOutPut += stdout_;

        QByteArray err = testProcess.readAllStandardError();
        QString errorOutput=QString(err);
        errorOutput.replace("\n","<br>");
        if (errorOutput!=""){
            ui->txtLog->appendHtml("<font color=\"red\">program wrote on stderr: <br>" +errorOutput+"</font>");
            totalSuccess = false;
        }

        QString unXMLedText = findUnXMLedString(QString(stdout_));
        testResults.append(parseXML(QString(stdout_),program,workingdir));
        ui->listLinks->clear();
        if (unXMLedText != ""){
            //qDebug() << "hallo";
            ui->txtLog->appendHtml("<font color=\"black\"><b>it printed:</b></font>");
            unXMLedText.replace("\n","<br>");
            ui->txtLog->appendHtml("<font color=\"red\">" +QString(unXMLedText)+"</font>");
        }
        if ((unXMLedText != "") || (errorOutput != "" )){
            addFileLinks(unXMLedText+errorOutput,settings->getSourceRootDirectoryAbsolute(program,workingdir));
        }
        ui->listLinks->setVisible(ui->listLinks->count()>0);
        ui->lblLink->setVisible(ui->listLinks->count()>0);
        ui->txtLog->appendPlainText("");
    }
    listTestResults(testResults,totalSuccess);
    return true;
}

    QStringList searchFileLinks(QString xmlinput){
        //qDebug() << xmlinput;
       QRegExp rx(".cpp:\\d+: ");//QRegExp rx(".cpp:\\d+: ");
       QString str = "<br>"+xmlinput;
       QStringList list;
       int pos = 0;

       while ((pos = rx.indexIn(str, pos)) != -1) {

           int posstart = str.lastIndexOf("<br>",pos);
           int deleteoffset = 4;
           if (posstart < str.lastIndexOf(":",pos)){
               posstart = str.lastIndexOf(":",pos);
               deleteoffset=1;
           }
           int poslength = pos-posstart+rx.matchedLength();
           list << str.mid(posstart+deleteoffset,poslength-6).trimmed();
           pos += rx.matchedLength();

       }

        return list;
    }

void MainWindow::addFileLinks(QString xmlinput,QString rootpath){
   QStringList list;
   list = searchFileLinks(xmlinput);
   ui->listLinks->clear();
   for(int i=0;i< list.count();i++){
       QListWidgetItem *item;
       ui->listLinks->addItem(list[i]);
       item = ui->listLinks->item(ui->listLinks->count()-1);
       item->setData(Qt::ToolTipRole, rootpath+'/'+list[i]);
   }
}

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

QList<TestCaseEntry> MainWindow::parseXML(QString xmlinput, QString testExecutable, QString testExecutableWorkingDir){
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
            <Message type="qwarn" file="" line="0">
                <Description><![CDATA[..\..\..\tests\testScriptEngine.cpp:86: Failure
            Actual function call count doesnt match EXPECT_CALL(m_pysys, print(QVariant("Test")))...
                     Expected: to be called at least once
                       Actual: never called - unsatisfied and active]]></Description>
            </Message>
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
            testCaseEntry.testSourceDirectory = settings->getSourceRootDirectoryAbsolute(testExecutable,testExecutableWorkingDir);
            if (testCase.tagName() == "TestCase"){
                QDomNode testFunctions = testCase.firstChild();

                while(!testFunctions.isNull()) {
                    TestFunctionEntry testFunctionEntry;
                    QDomElement testFunction = testFunctions.toElement();
                    if(!testFunction.isNull()) {
                        if (testFunction.tagName() == "TestFunction"){
                           testFunctionEntry.name = testFunction.attribute("name");
                           testFunctionEntry.description = "";
                           QDomElement testMessage = testFunction.firstChildElement("Message");
                           QDomNodeList testMessageDescriptions = testMessage.elementsByTagName("Description");
                           if (testMessageDescriptions.count() > 0){
                               QDomElement testMessageDescription = testMessageDescriptions.at(0).toElement();
                               //testFunctionEntry.description = testMessageDescription.text();

                           }

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
                               testFunctionEntry.description += testIncidentDescription.text();

                           }
                           if (testFunctionEntry.description.contains("fail",Qt::CaseInsensitive)){
                               testFunctionEntry.passed = false;
                           }
                           if ((testFunctionEntry.passed == false)&&(testFunctionEntry.fileName=="")){
                                QStringList filelist = searchFileLinks(testFunctionEntry.description);
                                if (filelist.count()){
                                    QString fn =  filelist[0];
                                    testFunctionEntry.fileName = fn.left(fn.indexOf(":"));
                                    testFunctionEntry.lineNumber = fn.mid(fn.indexOf(":")+1,fn.length());
                                }
                           }
                           testCaseEntry.testFunctionList.append(testFunctionEntry);


                        }
                        //qDebug() << qPrintable(testFunction.tagName()) << qPrintable(testFunction.attribute("name")) << endl; // the node really is an element.
                    }
                    testFunctions = testFunctions.nextSibling();

                }
            }
            testCasesList.append(testCaseEntry);

        }
        testCases = testCases.nextSibling();
    }

    return testCasesList;
}

void MainWindow::listTestResults(QList<TestCaseEntry> testResults, bool totalSuccess)
{
    QFont font("consolas");
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
                totalSuccess = false;
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

    if (totalSuccess){
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, Qt::green);
        sample_palette.setColor(QPalette::WindowText, Qt::black);

        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(sample_palette);

        statusLabel->setText("OK");
        //QIcon icon = QIcon(":/icons/smile-happy.ico");
        QIcon icon = QIcon(":/icons/smile-happy.ico");
        setWindowIcon(icon);
    }else{
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, Qt::red);
        sample_palette.setColor(QPalette::WindowText, Qt::black);

        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(sample_palette);
        statusLabel->setText("Fail");
        QIcon icon = QIcon(":/icons/smile-sad.ico");
        setWindowIcon(icon);

    }


}

void MainWindow::runqtCreator(QString link){
    QStringList arguments;
    arguments <<"-client" << link;
    QProcess::startDetached(settings->getQtCreatorPath(), arguments);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    (void)column;
    if( QTreeWidgetItemWithData* itemWithData = dynamic_cast< QTreeWidgetItemWithData* >( item ) )
    {
        if(itemWithData->testFunctionEntry.fileName != ""){

            //QString path = "C:/Qt/Qt5.4.1/Tools/QtCreator/bin/";
            QString sourceDirectory = itemWithData->testCaseEntry.testSourceDirectory;
            QDir qpath(sourceDirectory);
            QString sourceFileAbs = sourceDirectory+'/'+itemWithData->testFunctionEntry.fileName;//qpath.relativeFilePath(itemWithData->testFunctionEntry.fileName);
            //qDebug() << sourceFileAbs;
            QString link = sourceFileAbs+':'+itemWithData->testFunctionEntry.lineNumber;
            runqtCreator(link);
        }
    }
}

void MainWindow::on_listLinks_itemDoubleClicked(QListWidgetItem *item)
{
    QString link(item->data(Qt::ToolTipRole).toString());
    runqtCreator(link);
}

RecentFiles::RecentFiles(QStringList initList)
{
    this->recentFiles.append(initList);
}

RecentFiles::~RecentFiles()
{

}

void RecentFiles::addToRecentFiles(QString fileName)
{
    int index = recentFiles.indexOf(fileName);
    if (index > -1){
        recentFiles.removeAt(index);
    }
    recentFiles.insert(0,fileName);
    if (recentFiles.count() > MAXRECENTFILESCOUNT){
        recentFiles.removeAt(recentFiles.count()-1);
    }

}







