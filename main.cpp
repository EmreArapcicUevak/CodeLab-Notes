#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

#include "codeeditor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setWindowIcon(QIcon(":/Resources/Resources/Icons/main_icon.ico"));
    CodeEditor codeEditor;
    codeEditor.show();
    return app.exec();
}
