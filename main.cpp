#include "codeeditor.h"

#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CodeEditor w;
    w.show();
    return a.exec();
}
