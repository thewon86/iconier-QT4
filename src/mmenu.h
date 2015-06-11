#ifndef MMENU_H
#define MMENU_H

#include <QMenu>
#include <QAction>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class mMenu : public QMenu
{
    Q_OBJECT
public:
    explicit mMenu(QWidget *parent = 0);
    void addSubMenuActsFilter(QAction *act);
    void addSubMenuActsFilter(QList<QAction*> *act);
    void clearSubMenuActsFilter();

protected:
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void itemChanged();
    void removeItem(QAction *act);

public slots:
    void removeCurrAct();
    void removeCurrAct(QAction *act);

private:
    QMenu *m_contextMenu;
    QAction *m_delAct;
    QAction *m_curAct;
    QList<QAction*> *m_actsList;
    bool m_enableSubMenu;
    bool m_subMenuActived;
};

#endif // MMENU_H
