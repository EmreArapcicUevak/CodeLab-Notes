#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "qfilesystemmodel.h"
#include <QMainWindow>

#include "highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CodeEditor; }
QT_END_NAMESPACE


struct activeFileInformation {
    QFile* fileInstance;
    QString fileName, fileExtension;

    activeFileInformation(const QString&, QFile*);
    ~activeFileInformation();
};

class CodeEditor : public QMainWindow {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void createTab(QString text, bool pressed, QString _filePath);
    void setWorkingDirectory(const QString &newWorkingDirectory);
    void aboutCodeLabNotes();
signals:
    void workingDirectoryChanged();
private slots:
    void openFolder();
    void updateTreeView();
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();

    void fileCloseSlot(QString);
private:
    Ui::CodeEditor *ui;
    QFileSystemModel* dirModel;
    QString workingDirectory, rootFileName;
    QList<activeFileInformation*> activeFiles;

    Highlighter* highlighter;

    void setUpMenu();
    void setUpTreeView();
    void openFile(const QString&, const QString &);
    void displayFile();
    void checkEditor();
};
#endif // CODEEDITOR_H
