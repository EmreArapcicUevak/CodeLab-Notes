#ifndef ACTIVEFILEINFORMATION_H
#define ACTIVEFILEINFORMATION_H

#include <QFile>

class activeFileInformation {

public:

    QFile* fileInstance;
    QString fileName, fileExtension;

    activeFileInformation(const QString&, QFile*);
    activeFileInformation(activeFileInformation&);
    activeFileInformation();
    ~activeFileInformation();

    activeFileInformation& operator=(activeFileInformation&);
    bool operator==(activeFileInformation& objToCompare);
    bool operator==(activeFileInformation* objToCompare);

};

#endif // ACTIVEFILEINFORMATION_H
