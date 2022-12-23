#include "openedfiletab.h"
#include <QStyle>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

OpenedFileTab::OpenedFileTab(QString text) {

    layout = new QHBoxLayout();
    iconHolder = new QLabel("");
    label = new QLabel(text);
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
    this->setStyleSheet("background-color : #353638;"
                        "color : #c5c5c5;"
                        "border-radius : 5px;"
                        "margin : 0;"
                        "padding : 0;");

    deleteBtn->setStyleSheet("border : none;");
    iconHolder->setStyleSheet("padding-right : 5px;"
                              "padding-bottom : 3px;");
    label->setStyleSheet("padding-bottom : 3px");
}

void OpenedFileTab::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void OpenedFileTab::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit onPressed();
}

void OpenedFileTab::tabPressed() {
    this->setStyleSheet("background-color : #27282a;"
                        "border-radius : 5px;"
                        "color : white;"
                        "margin : 0;"
                        "padding : 0;");
}

void OpenedFileTab::deletePressed() {
    delete this;
}
