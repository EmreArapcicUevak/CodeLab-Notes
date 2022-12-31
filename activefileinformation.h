#ifndef ACTIVEFILEINFORMATION_H
#define ACTIVEFILEINFORMATION_H

#include <QFile>

class activeFileInformation
{
public:
    QFile* fileInstance;
    QString fileName, fileExtension;

    activeFileInformation(const QString&, QFile*);
    activeFileInformation(activeFileInformation&);
    activeFileInformation();
    ~activeFileInformation();

    activeFileInformation& operator=(activeFileInformation&);
};

#endif // ACTIVEFILEINFORMATION_H
