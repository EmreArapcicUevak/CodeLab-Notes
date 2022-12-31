#include "codeeditor.h"
#include "./ui_codeeditor.h"
#include <QAction>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDir>
#include <QTreeWidgetItemIterator>
#include <QTreeView>
#include <QAbstractScrollArea>
#include <QToolBar>
#include <QMessageBox>

#include "openedfiletab.h"
#include <QHash>



CodeEditor::CodeEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CodeEditor) {
    ui->setupUi(this);


    // Set default values of variables
    currentTab = nullptr;
    highlighter = new Highlighter();
    this->blockChange = this->contentChanged = false;

    // Set up hash map for icons
    icons.insert("cpp", ":/Resources/Resources/Logos/cpp_logo_icon.svg");
    icons.insert("c",":/Resources/Resources/Logos/c_logo_icon.svg");
    icons.insert("h", ":/Resources/Resources/Logos/header_logo_icon.svg");

    connect(ui->editor,&QPlainTextEdit::textChanged, [this]()->void{
        if (!this->blockChange)
            this->contentChanged = true;
    });

    QToolBar* toolBar = new QToolBar();
    ui->toolbarHolder->addWidget(toolBar);
    toolBar->addAction(ui->actionRemove);
    toolBar->addAction(ui->actionNew_Folder);
    toolBar->addAction(ui->actionNew_File);
    toolBar->addAction(ui->actionSave);
    toolBar->addAction(ui->actionBuild);


    // Set up all the connections for actions
    setUpMenu();

    // Set up default values for the tree directory view
    setUpTreeView();

    // Checks the editor for status
    checkEditor();
}


CodeEditor::~CodeEditor() {
    for (QList<activeFileInformation*>::ConstIterator i = activeFiles.constBegin() ; i < activeFiles.constEnd(); i++)
        delete *i;
    activeFiles.clear();
    delete highlighter;
    delete ui;
}

/* SET UPS */

void CodeEditor::setUpMenu() {
    connect(ui->actionAbout_QT, &QAction::triggered, this, QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, QApplication::quit);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &CodeEditor::openFolder);
    connect(this, &CodeEditor::workingDirectoryChanged, this, &CodeEditor::updateTreeView);
    connect(ui->actionAbout_CodeLab_Notes, &QAction::triggered, this, &CodeEditor::aboutCodeLabNotes);
    connect(ui->actionNew_File, &QAction::triggered, this, &CodeEditor::createNewFile);
    connect(ui->actionNew_Folder, &QAction::triggered, this, &CodeEditor::createNewFolder);
    //connect(ui->actionSave, &QAction::triggered, this, &CodeEditor::saveFile);
    connect(ui->actionSave_All, &QAction::triggered, this, &CodeEditor::saveAllFiles);
    connect(ui->actionSave_as, &QAction::triggered, this, &CodeEditor::saveFileAs);
    connect(ui->actionAuto_Save, &QAction::toggled, this, &CodeEditor::autoSaveToggle);

    connect(this, &CodeEditor::workingDirectoryChanged, [this]()->void{
        qDebug() << "Working directory changed!";
    });
}


void CodeEditor::setUpTreeView()
{
    this->dirModel = new QFileSystemModel(this);
    this->dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    this->dirModel->setRootPath(QString());
    this->dirModel->setReadOnly(true);
    this->workingDirectory = QString();

    ui->treeView->setAnimated(true);
    ui->treeView->setModel(this->dirModel);

    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

void CodeEditor::checkEditor() {
    if (activeFiles.size() == 0) {
        ui->editor->setPlaceholderText("");
        ui->editor->hide();
    }else if (activeFiles.size() > 0) {
        ui->editor->show();
    }
}

void CodeEditor::setupEditor() {
    this->blockChange = true;
    ui->editor->setPlainText(currentTab->code);
    this->blockChange = false;
}


/* ACTIVE FILE INFORMATION STRUCT SET UP */



/* Other slots */

void CodeEditor::aboutCodeLabNotes() {

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About CodeLab Notes");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("This program is made for the CS103 project at IUS.<br>"
                   "Made by : <br>"
                   "<a href= \"https://github.com/VedadSiljic\">Vedad Siljic</a><br>"
                   "<a href= \"https://github.com/EmreArapcicUevak\">Emre Arapcic Uevak</a><br>"
                   "Source code : <br><a href= \"https://github.com/EmreArapcicUevak/CodeLab-Notes\">https://github.com/EmreArapcicUevak/CodeLab-Notes</a>");
    msgBox.exec();
}

/*
 *
 *
 * CODE EDITOR BASED SLOTS
 *
 *
*/
/*
void CodeEditor::createTab(QString text, bool pressed = 0, QString filePath = "", QString fileExtension = "", QString code = "") {
    OpenedFileTab* tab = new OpenedFileTab(text, filePath, fileExtension);
    tab->changeColor();
    tab->code = code;

    qDebug() << "Current Extension is : " << tab->fileExtension;
    QString iconLocation = ":/Resources/Resources/Logos/text_logo_icon.svg";
    if (tab->fileExtension == "cpp") {
        iconLocation = ":/Resources/Resources/Logos/cpp_logo_icon.svg";
    }
    else if (tab->fileExtension == "c") {
        iconLocation = ":/Resources/Resources/Logos/c_logo_icon.svg";
    }
    else if (tab->fileExtension == "h") {
        iconLocation = ":/Resources/Resources/Logos/header_logo_icon.svg";
    }

    QPixmap icon(iconLocation);
    tab->iconHolder->setPixmap(icon.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    ui->tabContainer->addWidget(tab, 0, Qt::AlignLeft);

    connect(tab, &OpenedFileTab::tabClosed, this, &CodeEditor::fileCloseSlot);
    connect(tab, &OpenedFileTab::thisTabPressed, this, &CodeEditor::tabChangedProcess);
    connect(tab, &OpenedFileTab::thisTabClosed, this, &CodeEditor::tabClosedProcess);
    if (currentTab != nullptr)
        currentTab->changeColor();

    currentTab = tab;
    activeTabs.append(tab);

    setupEditor();
    setHighlighting();
}*/

void CodeEditor::createTab(activeFileInformation& fileInfo, QString& code, bool pressed) {
    OpenedFileTab* tab = new OpenedFileTab(fileInfo);
    tab->changeColor();
    tab->code = code;

    qDebug() << "Current Extension is : " << tab->fileExtension;
    QString iconLocation = icons.contains(fileInfo.fileExtension) ? icons.value(fileInfo.fileExtension) : ":/Resources/Resources/Logos/text_logo_icon.svg";
    QPixmap icon(iconLocation);
    tab->iconHolder->setPixmap(icon.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    ui->tabContainer->addWidget(tab, 0, Qt::AlignLeft);

    connect(tab, &OpenedFileTab::tabClosed, this, &CodeEditor::fileCloseSlot);
    connect(tab, &OpenedFileTab::thisTabPressed, this, &CodeEditor::tabChangedProcess);
    connect(tab, &OpenedFileTab::thisTabClosed, this, &CodeEditor::tabClosedProcess);

    connect(tab, &OpenedFileTab::thisTabPressed, [&]()->void{
        qDebug() << this->ui->actionAuto_Save->isChecked();
        //if (this->ui->actionAuto_Save->isChecked())
        //    qDebug() << "Save the file " + fileInfo.fileName;
    });

    if (currentTab != nullptr)
        currentTab->changeColor();

    currentTab = tab;
    activeTabs.append(tab);

    setupEditor();
    setHighlighting();
}

void CodeEditor::tabChangedProcess(OpenedFileTab* tab) {
    if (tab == currentTab)
        return;

    QString code = ui->editor->toPlainText();

    if (ui->actionAuto_Save->isChecked() && this->contentChanged){
        this->saveFile(currentTab->fileInfo, code);
        qDebug() << "File saved" << currentTab->fileInfo.fileName;
    }

    currentTab->code = code;
    currentTab->changeColor();
    currentTab = tab;
    currentTab->changeColor();

    this->contentChanged = false;
    setupEditor();
    setHighlighting();
}

void CodeEditor::tabClosedProcess(OpenedFileTab* tab) {

    for (int i = 0; i < activeTabs.count(); i++) {
        if (activeTabs[i] == tab) {
            activeTabs.removeAt(i);
            break;
        }
    }

    if (tab != currentTab)
        return;



    if (activeTabs.count() == 0) {
        currentTab = nullptr;
        return;
    }


    currentTab = activeTabs[0];
    currentTab->changeColor();

    setupEditor();
    setHighlighting();

}

void CodeEditor::displayFile() {
    if (!activeFiles[activeFiles.size() - 1]->fileInstance->open(QFile::ReadWrite)) {
        qDebug() << "File did not open";
        return;
    }

    QTextStream textStream(activeFiles[activeFiles.size() - 1]->fileInstance);
    QString text = textStream.readAll();

    this->blockChange = true;
    ui->editor->setPlainText(text);
    this->blockChange = false;

    createTab(*activeFiles[activeFiles.size() - 1], text);
    checkEditor();
}

void CodeEditor::on_actionUndo_triggered() {
    ui->editor->undo();
}


void CodeEditor::on_actionRedo_triggered() {
    ui->editor->redo();
}


void CodeEditor::on_actionCut_triggered() {
    ui->editor->cut();
}


void CodeEditor::on_actionCopy_triggered() {
    ui->editor->copy();
}


void CodeEditor::on_actionPaste_triggered() {
    ui->editor->paste();
}

/*
 *
 *  FILE BASED SLOTS
 *
*/

void CodeEditor::setWorkingDirectory(const QString &newWorkingDirectory){
    workingDirectory = newWorkingDirectory;
    emit this->workingDirectoryChanged();
}

void CodeEditor::fileCloseSlot(QString filePath) {
    qDebug() << filePath;
    for (QList<activeFileInformation*>::ConstIterator i = activeFiles.constBegin() ; i < activeFiles.constEnd(); i++)
        if ((*i)->fileInstance->fileName() == filePath) {
            activeFiles.erase(i);
            checkEditor();
            break;
        }
}

void CodeEditor::setHighlighting() {

    if (currentTab->fileExtension == "cpp" || currentTab->fileExtension == "c" || currentTab->fileExtension == "h") {
        highlighter->setDocument(ui->editor->document());
    }
    else {
        highlighter->setDocument(nullptr);
    }

}

void CodeEditor::openFile(const QString &filePath, const QString &fileName) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile()){
        for (unsigned int i = 0; i < activeFiles.count(); i++)
            if (activeFiles[i]->fileInstance->fileName() == filePath){
                QMessageBox::warning(this,"File couldn't open", "This file is already open", QMessageBox::Ok,QMessageBox::Ok);
                return;
            }
        activeFiles.append(new activeFileInformation(fileName,new QFile(filePath)));
        displayFile();

    }else
        QMessageBox::warning(this,"File couldn't open", "There was a problem trying to open a file", QMessageBox::Ok,QMessageBox::Ok);
}

void CodeEditor::openFolder(){
    QString selectedDirectory = QFileDialog::getExistingDirectory(this,"Select Working Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDirectory.isEmpty()){
        this->workingDirectory = selectedDirectory;

        unsigned int i = selectedDirectory.size();
        while (selectedDirectory[--i] != '/');
        this->rootFileName = selectedDirectory.sliced(i+1);

        emit this->workingDirectoryChanged();
    }
}

void CodeEditor::createNewFile(){
    QString newFilePath = QFileDialog::getSaveFileName(this,"Select how you want to save your file",this->workingDirectory);
    qDebug() << "New file name is " << newFilePath;
    if (newFilePath.isEmpty())
        return;
    else{
        QFile file(newFilePath);
        qDebug() << file.exists();

        if (!file.exists())
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                file.close();
            else
                QMessageBox::warning(this,"Can not make wanted file","Something happened while trying to make the file.");
        else if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            file.close();
        else
            QMessageBox::warning(this,"Can not replace wanted file","This file is currently being used by another application.");


    }
}

void CodeEditor::createNewFolder()
{
    QString newFolderPath = QFileDialog::getSaveFileName(this,"Select how you want to save your file",this->workingDirectory, "Folder (*)");
    qDebug() << newFolderPath;

    if (newFolderPath.isEmpty())
        return;
    else{
        QDir dir(newFolderPath);

        if (dir.exists())
            QMessageBox::warning(this,"Can not make this directory","A folder with this name already exists");
        else if (!dir.mkpath(newFolderPath))
            QMessageBox::warning(this,"Can not make this directory","Something went wrong while creating the directory");
    }
}

void CodeEditor::saveFile(activeFileInformation& filePath, const QString& fileContent){
    filePath.fileInstance->close();

    if (filePath.fileInstance->open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(filePath.fileInstance);
        out << fileContent;

        filePath.fileInstance->close();
        filePath.fileInstance->open(QFile::ReadWrite);
    }else
        QMessageBox::warning(this,"File failed to save", filePath.fileInstance->fileName() + " failed to save");
}

void CodeEditor::saveAllFiles(){

}

void CodeEditor::saveFileAs(){

}

void CodeEditor::autoSaveToggle(const bool state){
    if (state)
        this->statusBar()->showMessage("Auto saved turned on!");
    else
        this->statusBar()->clearMessage();
}


/*
 *
 * TREE VIEW BASED SLOTS
 *
*/


void CodeEditor::on_treeView_doubleClicked(const QModelIndex &index){
    openFile(dirModel->filePath(index), index.data().toString());
}


void CodeEditor::updateTreeView(){
    ui->treeView->setRootIndex(this->dirModel->index(this->workingDirectory));
}

/* */

