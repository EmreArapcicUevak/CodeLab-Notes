#ifndef OPENEDFILETAB_H
#define OPENEDFILETAB_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>
#include <QAbstractButton>
#include <QImage>

class OpenedFileTab : public QWidget {

    Q_OBJECT
public:
    void changeColor(bool pressed);

    QLabel* iconHolder;
    QHBoxLayout* layout;
    QLabel* label;
    QPushButton* deleteBtn;
    OpenedFileTab(QString text);
    void paintEvent(QPaintEvent* event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void onPressed();
public slots:

    void tabPressed();
    void deletePressed();

};

#endif // OPENEDFILETAB_H
