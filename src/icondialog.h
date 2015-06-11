#ifndef ICONDIALOG_H
#define ICONDIALOG_H

#include <QDialog>
#include "mdefinedwidget.h"
#include "mmenu.h"

QT_BEGIN_NAMESPACE
class QMenu;
//template <typename int>QList;
QT_END_NAMESPACE
typedef struct sizelist{
    int size;
    bool enable;
}__attribute__((__packed__))SIZELIST;

namespace Ui {
    class IconDialog;
}

class IconDialog : public mDefinedWidget
{
    Q_OBJECT

public:
    explicit IconDialog(QWidget *parent = 0);
    ~IconDialog();

private:
//    void initGuiStyle();
    void loadConfig();
    void loadDefaultConfig();
    bool readConfig();
    void saveConfig();
    void updateSizeList();
    void createContextMenu();
    void showContextMenu();
    void enabledSizeList();
    void setSkin(const QString &skinName);

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void showNormal();

private slots:
//    void testslot();
//    void testslot(QAction *act);
    void contextMenuTriggered(QAction *act);
    void removeCurrItem(QAction *act);
    void initGuiStyle();
    void on_btnMenu_clicked();
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();

private:
    Ui::IconDialog *ui;
    mMenu *m_contextMenu;
    QPoint *m_pos;
    QString *m_skin;
    QList <SIZELIST> *m_sizeList;
    QList <SIZELIST> *m_customSizeList;
    QList<QAction *> *m_sizeActs;
    QList<QAction *> *m_customSizeActs;
    QAction *m_customAct;
};

#endif // ICONDIALOG_H
