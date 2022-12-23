#include "codeeditor.h"
#include "./ui_codeeditor.h"

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

