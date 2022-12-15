#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CodeEditor; }
QT_END_NAMESPACE

class CodeEditor : public QMainWindow
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

private:
    Ui::CodeEditor *ui;
};
#endif // CODEEDITOR_H
