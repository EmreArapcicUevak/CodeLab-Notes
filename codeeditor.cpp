#include "codeeditor.h"
#include "./ui_codeeditor.h"
#include <QAction>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QAbstractScrollArea>
#include <QToolBar>

#include "openedfiletab.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CodeEditor) {
    ui->setupUi(this);


    createTab("main.cpp");
    createTab("cppBetterThanC.c");
    createTab("header.h");
    createTab("document.txt");
    createTab("ddkadjddj");



    setUpMenu();


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

void CodeEditor::createTab(QString text) {
    OpenedFileTab* tab = new OpenedFileTab(text);
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

void CodeEditor::setUpMenu()
{
    connect(ui->actionAbout_QT, &QAction::triggered, this, QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, QApplication::quit);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &CodeEditor::openFolder);

    connect(this, &CodeEditor::workingDirectoryChanged, [this]()->void{
        qDebug() << "Working directory changed!";
    });
}

void CodeEditor::openFolder(){
    QString selectedDirectory =  QFileDialog::getExistingDirectory(this,"Select Working Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDirectory.isEmpty()){
        this->workingDirectory = selectedDirectory;
        this->dirModel->setRootPath(this->workingDirectory);
        this->updateTreeView();
        emit this->workingDirectoryChanged();
    }
}

void CodeEditor::updateTreeView()
{
    ui->treeView->setModel(this->dirModel);
}
