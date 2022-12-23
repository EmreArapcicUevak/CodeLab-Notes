#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

#include "codeeditor.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    CodeEditor w;
    w.show();
    return a.exec();
}
