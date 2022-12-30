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



CodeEditor::CodeEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CodeEditor) {
    ui->setupUi(this);



    highlighter = new Highlighter();
    setHighlighting(false);

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
    connect(ui->actionSave, &QAction::triggered, this, &CodeEditor::saveFile);
    connect(ui->actionSave_All, &QAction::triggered, this, &CodeEditor::saveAllFiles);
    connect(ui->actionSave_as, &QAction::triggered, this, &CodeEditor::saveFileAs);


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


/* ACTIVE FILE INFORMATION STRUCT SET UP */

activeFileInformation::activeFileInformation(const QString &fileName, QFile *fileInstance){
    this->fileInstance = fileInstance;

    int dotIndex = fileName.size();
    while (dotIndex > 0 && fileName[--dotIndex] != '.');
    this->fileName = fileName;

    if (dotIndex != 0)
        this->fileExtension = fileName.sliced(dotIndex+1);

    qDebug() << fileInstance->fileName() << this->fileName << this->fileExtension;
}

activeFileInformation::~activeFileInformation()
{
    qDebug() << this->fileInstance->fileName() << "Freed";
    if (this->fileInstance->isOpen())
        this->fileInstance->close();
    delete this->fileInstance;
}

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

void CodeEditor::createTab(QString text, bool pressed = 0, QString _filePath = "") {
    OpenedFileTab* tab = new OpenedFileTab(text, _filePath, true);
    tab->changeColor();

    QString iconLocation = ":/Resources/Resources/Logos/text_logo_icon.svg";
    if (text.contains(".cpp")) {
        iconLocation = ":/Resources/Resources/Logos/cpp_logo_icon.svg";
    }
    else if (text.contains(".c")) {
        iconLocation = ":/Resources/Resources/Logos/c_logo_icon.svg";
    }
    else if (text.contains(".h")) {
        iconLocation = ":/Resources/Resources/Logos/header_logo_icon.svg";
    }

    QPixmap icon(iconLocation);
    tab->iconHolder->setPixmap(icon.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    ui->tabContainer->addWidget(tab, 0, Qt::AlignLeft);

    connect(tab, &OpenedFileTab::tabClosed, this, &CodeEditor::fileCloseSlot);
}

void CodeEditor::displayFile() {
    if (!activeFiles[activeFiles.size() - 1]->fileInstance->open(QFile::ReadWrite)) {
        qDebug() << "File did not open";
        return;
    }

    QTextStream textStream(activeFiles[activeFiles.size() - 1]->fileInstance);
    QString text = textStream.readAll();
    ui->editor->setPlainText(text);
    createTab(activeFiles[activeFiles.size() - 1]->fileName, 1, activeFiles[activeFiles.size() - 1]->fileInstance->fileName());
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

void CodeEditor::setHighlighting(bool set) {
    if (set == true)
        highlighter->setDocument(ui->editor->document());
    else
        highlighter->setDocument(nullptr);
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

void CodeEditor::saveFile()
{

}

void CodeEditor::saveAllFiles()
{

}

void CodeEditor::saveFileAs()
{

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

