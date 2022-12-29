#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

#include "codeeditor.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setWindowIcon(QIcon(":/Resources/Resources/Icons/main_icon.ico"));
    CodeEditor w;
    w.show();
    return a.exec();
}
