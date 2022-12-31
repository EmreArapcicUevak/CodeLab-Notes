#include "activefileinformation.h"
#include <QDebug>

activeFileInformation::activeFileInformation(const QString &fileName, QFile *fileInstance){
    this->fileInstance = fileInstance;

    int dotIndex = fileName.size();
    while (dotIndex > 0 && fileName[--dotIndex] != '.');
    this->fileName = fileName;

    if (dotIndex != 0)
        this->fileExtension = fileName.sliced(dotIndex+1);

    qDebug() << fileInstance->fileName() << this->fileName << this->fileExtension;
}

activeFileInformation::activeFileInformation(activeFileInformation &objToCopy){
    this->fileName = objToCopy.fileName;
    this->fileExtension = objToCopy.fileExtension;
    this->fileInstance = objToCopy.fileInstance;
}

activeFileInformation::activeFileInformation()
{
    this->fileName = this->fileExtension = QString();
    this->fileInstance = nullptr;
}

activeFileInformation::~activeFileInformation()
{
    qDebug() << this->fileInstance->fileName() << "Freed";
    if (this->fileInstance->isOpen())
        this->fileInstance->close();
    delete this->fileInstance;
}

activeFileInformation &activeFileInformation::operator=(activeFileInformation &objToCopy)
{
    this->fileName = objToCopy.fileName;
    this->fileExtension = objToCopy.fileExtension;
    this->fileInstance = objToCopy.fileInstance;
    return *this;
}
