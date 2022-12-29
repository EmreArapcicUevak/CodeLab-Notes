#include "codeeditor.h"
#include "./ui_codeeditor.h"
#include <QAction>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDir>
#include <QTreeWidgetItemIterator>
#include <QTreeView>
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


    // Set up all the connections for actions
    setUpMenu();

    // Set up default values for the tree directory view
    setUpTreeView();
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
    connect(this, &CodeEditor::workingDirectoryChanged, this, &CodeEditor::updateTreeView);

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
    path = this->workingDirectory + path.sliced(0,path.size()-1);
    QFileInfo fileInfo(path);

    qDebug() << path << fileInfo.exists() << fileInfo.isDir() << fileInfo.isFile();
}

