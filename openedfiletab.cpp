#include "openedfiletab.h"
#include <QStyle>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
OpenedFileTab::OpenedFileTab(activeFileInformation &activeFileInfo) {
    this->fileInfo = activeFileInfo;

    pressed = false;
    code = "";
    filePath = activeFileInfo.fileInstance->fileName();
    fileExtension = activeFileInfo.fileExtension;
    layout = new QHBoxLayout();
    iconHolder = new QLabel("", this);
    label = new QLabel(activeFileInfo.fileName, this);
    QSpacerItem *spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    deleteBtn = new QPushButton(QIcon(":/Resources/Resources/Icons/remove_icon.svg"), "", this);


    layout->addWidget(iconHolder);
    layout->addWidget(label);
    layout->addSpacerItem(spacer);
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
                        "border-radius : 5px;"
                        "margin : 0;"
                        "padding : 0;"
                        "}"
                        "[clicked=true] {"
                        "background-color : #27282a;"
                        "color : white;"
                        "border-radius : 5px;"
                        "margin : 0;"
                        "padding : 0;"
                        "}");
    deleteBtn->setStyleSheet("border : none;");
    iconHolder->setStyleSheet("padding-right : 5px;");

    this->setProperty("clicked", false);
    deleteBtn->setProperty("clicked", false);
    iconHolder->setProperty("clicked", false);
    label->setProperty("clicked", false);

    iconHolder->setAlignment(Qt::AlignLeft);
    label->setAlignment(Qt::AlignLeft);


    this->setMaximumSize(QSize(200, this->height()));
    this->setMinimumSize(QSize(200, this->height()));
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
    this->raise();

    // Tab drag
    if (event->buttons() & Qt::LeftButton)
        dragStartPosition = event->pos();
    oldX = this->geometry().x();
    oldY = this->geometry().y();
    mouseClickX = event->globalPosition().x();
    mouseClickY = event->globalPosition().y();
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
void OpenedFileTab::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton) || !IsMinimumDistanceReached(event))
        return;

    int x = event->globalPosition().x() - mouseClickX + oldX;
    int RightBorder = this->parentWidget()->geometry().width() - this->geometry().width();
    x = x < 0 ? 0 : x > RightBorder ? RightBorder : x;
    move(x, oldY);
}

bool OpenedFileTab::IsMinimumDistanceReached(QMouseEvent* event) {
    return (event->pos() - dragStartPosition).manhattanLength() >= QApplication::startDragDistance();
}


// Direction -1 is Left && 1 is Right
void OpenedFileTab::moveInLayout(int direction) {
    QHBoxLayout* myLayout = qobject_cast<QHBoxLayout*>(this->parentWidget()->findChild<QHBoxLayout*>("tabContainer"));
    const int index = myLayout->indexOf(this);

    int newIndex = index + direction;
    newIndex = newIndex < 0 ? 0 : newIndex >= myLayout->count() ? myLayout->count() - 1 : newIndex;

    if (index == -1 || index == newIndex)
        return;
    this->parentWidget()->layout()->removeWidget(this);
    myLayout->insertWidget(newIndex, this);
}

void OpenedFileTab::mouseReleaseEvent(QMouseEvent*) {
    this->lower();

    int x = geometry().x();
    int direct = oldX > x ? -1 : 1;
    int offset = (x - oldX) * direct;

    int count = float(offset) / this->width() + .34;
    moveInLayout(direct * count);


    update();
    QHBoxLayout* myLayout = qobject_cast<QHBoxLayout*>(this->parentWidget()->layout());
    myLayout->update();
    this->saveGeometry();
}

