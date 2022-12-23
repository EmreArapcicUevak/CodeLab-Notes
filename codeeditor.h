#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "qfilesystemmodel.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CodeEditor; }
QT_END_NAMESPACE

class CodeEditor : public QMainWindow {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void createTab(QString text);
    void setWorkingDirectory(const QString &newWorkingDirectory);
signals:
    void workingDirectoryChanged();
private slots:
    void openFolder();
private:
    Ui::CodeEditor *ui;
    QFileSystemModel* dirModel;
    QString workingDirectory;

    void setUpMenu();
    void updateTreeView();
};
#endif // CODEEDITOR_H
