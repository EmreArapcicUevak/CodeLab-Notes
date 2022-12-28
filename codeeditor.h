#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "qfilesystemmodel.h"
#include <QMainWindow>

#include "highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CodeEditor; }
QT_END_NAMESPACE

class CodeEditor : public QMainWindow {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void createTab(QString text, bool pressed);
    void setWorkingDirectory(const QString &newWorkingDirectory);
    void aboutCodeLabNotes();
signals:
    void workingDirectoryChanged();
private slots:
    void openFolder();
    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

private:
    Ui::CodeEditor *ui;
    QFileSystemModel* dirModel;
    QString workingDirectory;

    Highlighter* highlighter;

    void setUpMenu();
    void updateTreeView();
};
#endif // CODEEDITOR_H
