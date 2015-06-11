#ifndef MICONWIDGET_H
#define MICONWIDGET_H

#include <QLabel>
#include "mbitmap.h"

class mIconWidget : public QLabel
{
    Q_OBJECT
public:
    explicit mIconWidget(QWidget *parent = 0);
    ~mIconWidget();
    void clearSizeList();
    void addSize(QList <int> &list);

private:
    void getOpenFiles();
    void makeIcoFile();

protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);

signals:

public slots:

private:
    QStringList *imgFileList;
    QString *m_HistoryDir;
    QList <int> *m_sizeList;
};

#endif // MICONWIDGET_H
