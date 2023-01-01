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

private:
    QPoint dragStartPosition;
    int oldX, oldY;
    int mouseClickX, mouseClickY;
    QList<OpenedFileTab*> activeTabs;

public:
    void changeColor();

    bool pressed;
    QString filePath;
    QString fileExtension;
    QString code;
    QLabel* iconHolder;
    QHBoxLayout* layout;
    QLabel* label;
    QPushButton* deleteBtn;
    OpenedFileTab(QString text, QString _filePath, QString _fileExtension);
    void paintEvent(QPaintEvent* event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void moveInLayout(int direction);
    bool IsMinimumDistanceReached(QMouseEvent *event);


signals:
    void onPressed();
    void tabClosed(QString);
    void thisTabPressed(OpenedFileTab*);
    void thisTabClosed(OpenedFileTab*);
public slots:

    void tabPressed();
    void deletePressed();
};

#endif // OPENEDFILETAB_H
