#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "qfilesystemmodel.h"
#include <QMainWindow>
#include <QFont>

#include "highlighter.h"
#include "openedfiletab.h"
#include "activefileinformation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CodeEditor; }
QT_END_NAMESPACE

class CodeEditor : public QMainWindow {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void createTab(activeFileInformation& ,QString&, bool = 0);
    void setWorkingDirectory(const QString &newWorkingDirectory);
    void aboutCodeLabNotes();
    void setHighlighting();
    void setupEditor();
signals:
    void workingDirectoryChanged();
private slots:
    /*Trew view based slots */
    void updateTreeView();
    void on_treeView_doubleClicked(const QModelIndex &index);
    void tabChangedProcess(OpenedFileTab*);
    void tabClosedProcess(OpenedFileTab*);

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
    void saveAllFiles();
    void saveFileAs();
    void autoSaveToggle(const bool);
    void deleteFile();
    void on_actionChange_Font_Size_triggered();
private:
    Ui::CodeEditor *ui;
    QFileSystemModel* dirModel;
    QString workingDirectory, rootFileName;

    QList<activeFileInformation*> activeFiles;
    QList<OpenedFileTab*> activeTabs;

    Highlighter* highlighter;
    OpenedFileTab* currentTab;

    QHash<QString, QString> icons;

    bool blockChange, contentChanged;


    void setUpMenu();
    void setUpTreeView();
    void openFile(const QString&, const QString &);
    void displayFile();
    void checkEditor();
    void saveFile(activeFileInformation& filePath, const QString& fileContent);
};
#endif // CODEEDITOR_H
