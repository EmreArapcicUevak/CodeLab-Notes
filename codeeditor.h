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
    void setHighlighting(bool set);
signals:
    void workingDirectoryChanged();
private slots:
    /*Trew view based slots */
    void updateTreeView();
    void on_treeView_doubleClicked(const QModelIndex &index);

    /* Text editor based slots */
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();

    /* File/Folder based slots */
    void openFolder();
    void fileCloseSlot(QString);
    void createNewFile();
    void createNewFolder();
    void saveFile();
    void saveAllFiles();
    void saveFileAs();
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
