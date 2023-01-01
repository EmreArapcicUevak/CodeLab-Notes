#include "openedfiletab.h"
#include <QStyle>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
OpenedFileTab::OpenedFileTab(activeFileInformation &activeFileInfo) {

    this->fileInfo = activeFileInfo;

    pressed = false;
    code = "";
    filePath = activeFileInfo.fileInstance->fileName();
    fileExtension = activeFileInfo.fileExtension;
    layout = new QHBoxLayout();
    iconHolder = new QLabel("", this);
    label = new QLabel(activeFileInfo.fileName, this);
    deleteBtn = new QPushButton(QIcon(":/Resources/Resources/Icons/remove_icon.svg"), "", this);

    layout->addWidget(iconHolder);
    layout->addWidget(label);
    layout->addWidget(deleteBtn);

    this->setLayout(layout);

    connect(this, &OpenedFileTab::onPressed, this, &OpenedFileTab::tabPressed);
    connect(deleteBtn, &QPushButton::released, this, &OpenedFileTab::deletePressed);



    layout->setSpacing(0);
    this->setObjectName("openedTab");
    deleteBtn->setIconSize(QSize(25,25));
    this->setMaximumSize(QSize(10000, 36));
    this->setStyleSheet("[clicked=false] {"
                        "background-color : #353638;"
                        "color : #c5c5c5;"
                        "border-top-left-radius : 5px;"
                        "border-top-right-radius : 5px;"
                        "margin : 0;"
                        "padding : 0;"
                        "}"
                        "[clicked=true] {"
                        "background-color : #27282a;"
                        "color : white;"
                        "border-top-left-radius : 5px;"
                        "border-top-right-radius : 5px;"
                        "margin : 0;"
                        "padding : 0;"
                        "}");
    deleteBtn->setStyleSheet("border : none;");
    iconHolder->setStyleSheet("padding-right : 5px;");

    this->setProperty("clicked", false);
    deleteBtn->setProperty("clicked", false);
    iconHolder->setProperty("clicked", false);
    label->setProperty("clicked", false);
}

void OpenedFileTab::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void OpenedFileTab::changeFile(activeFileInformation &activeFileInfo)
{
    this->label->setText(activeFileInfo.fileName);
    this->fileInfo = activeFileInfo;
    this->filePath = activeFileInfo.fileInstance->fileName();
    this->fileExtension = activeFileInfo.fileExtension;
}

void OpenedFileTab::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit onPressed();
}

void OpenedFileTab::tabPressed() {
    emit thisTabPressed(this);
}

void OpenedFileTab::deletePressed() {
    emit thisTabClosed(this);
    emit tabClosed(filePath);
    delete this;
}

void OpenedFileTab::changeColor() {

    pressed = !pressed;

    if (pressed) {
        this->setProperty("clicked", true);
        deleteBtn->setProperty("clicked", true);
        iconHolder->setProperty("clicked", true);
        label->setProperty("clicked", true);
    }
    else {
        this->setProperty("clicked", false);
        deleteBtn->setProperty("clicked", false);
        iconHolder->setProperty("clicked", false);
        label->setProperty("clicked", false);
    }

    this->style()->unpolish(this);
    this->style()->polish(this);
    deleteBtn->style()->unpolish(deleteBtn);
    deleteBtn->style()->polish(deleteBtn);
    iconHolder->style()->unpolish(iconHolder);
    iconHolder->style()->polish(iconHolder);
    label->style()->unpolish(label);
    label->style()->polish(label);
}
