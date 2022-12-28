#include "codeeditor.h"
#include "./ui_codeeditor.h"
#include <QAction>
#include <QFileDialog>
#include <QFileSystemModel>
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


    highlighter = new Highlighter(ui->editor->document());


    QToolBar* toolBar = new QToolBar();
    ui->toolbarHolder->addWidget(toolBar);
    toolBar->addAction(ui->actionRemove);
    toolBar->addAction(ui->actionNew_Folder);
    toolBar->addAction(ui->actionNew_File);
    toolBar->addAction(ui->actionSave);
    toolBar->addAction(ui->actionBuild);


    this->dirModel = new QFileSystemModel(this);
    this->dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    this->dirModel->setRootPath(QString());
    this->updateTreeView();
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

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
    connect(ui->actionAbout_CodeLab_Notes, &QAction::triggered, this, &CodeEditor::aboutCodeLabNotes);

    connect(this, &CodeEditor::workingDirectoryChanged, [this]()->void{
        qDebug() << "Working directory changed!";
    });
}

void CodeEditor::openFolder() {
    QString selectedDirectory =  QFileDialog::getExistingDirectory(this,"Select Working Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDirectory.isEmpty()){
        this->workingDirectory = selectedDirectory;
        this->dirModel->setRootPath(this->workingDirectory);
        this->updateTreeView();
        emit this->workingDirectoryChanged();
    }
}

void CodeEditor::updateTreeView() {
    ui->treeView->setModel(this->dirModel);
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

