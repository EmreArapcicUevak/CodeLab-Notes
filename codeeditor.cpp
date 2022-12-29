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



    createTab("main.cpp", 1);
    createTab("cppBetterThanC.c", 0);
    createTab("header.h", 0);
    createTab("document.txt", 0);
    createTab("ddkadjddj", 0);



    setUpMenu();


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
}


CodeEditor::~CodeEditor() {
    delete ui;
}

void CodeEditor::createTab(QString text, bool pressed = 0) {
    OpenedFileTab* tab = new OpenedFileTab(text);
    tab->changeColor(pressed);

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
}

void CodeEditor::setWorkingDirectory(const QString &newWorkingDirectory)
{
    workingDirectory = newWorkingDirectory;
    emit this->workingDirectoryChanged();
}

void CodeEditor::setUpMenu() {
    connect(ui->actionAbout_QT, &QAction::triggered, this, QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, QApplication::quit);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &CodeEditor::openFolder);
    connect(this, &CodeEditor::workingDirectoryChanged, this, &CodeEditor::updateTreeView);
    connect(ui->actionAbout_CodeLab_Notes, &QAction::triggered, this, &CodeEditor::aboutCodeLabNotes);


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
}

void CodeEditor::openFolder(){

    QString selectedDirectory =  QFileDialog::getExistingDirectory(this,"Select Working Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDirectory.isEmpty()){
        this->workingDirectory = selectedDirectory;

        unsigned int i = selectedDirectory.size();
        while (selectedDirectory[--i] != '/');
        this->rootFileName = selectedDirectory.sliced(i+1);

        emit this->workingDirectoryChanged();
    }
}


void CodeEditor::updateTreeView()
{
    ui->treeView->setRootIndex(this->dirModel->index(this->workingDirectory));

}

void CodeEditor::on_treeView_doubleClicked(const QModelIndex &index){
    if (this->workingDirectory.isEmpty())
        return;

    QString path = QString();
    QModelIndex cur = index;
    while (cur.data().toString() != this->rootFileName){
        path = QString("%1/%3").arg(cur.data().toString()).arg(path);
        cur = cur.parent();
    }
    path = this->workingDirectory + '/' + path.sliced(0,path.size()-1);
    QFileInfo fileInfo(path);

    qDebug() << this->workingDirectory << path << fileInfo.exists() << fileInfo.isDir() << fileInfo.isFile();
}

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

