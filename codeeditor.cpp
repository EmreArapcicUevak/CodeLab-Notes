#include "codeeditor.h"
#include "./ui_codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CodeEditor)
{
    ui->setupUi(this);
}

CodeEditor::~CodeEditor()
{
    delete ui;
}

