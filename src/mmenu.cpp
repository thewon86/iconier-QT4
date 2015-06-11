#include "mmenu.h"
#include <QMouseEvent>
#include <QDebug>

mMenu::mMenu(QWidget *parent) :
    QMenu(parent),
    m_contextMenu(new QMenu(this)),
    m_delAct(new QAction(this)),
    m_actsList(new QList<QAction*>),
    m_enableSubMenu(false),
    m_subMenuActived(false)
{
    m_contextMenu->addAction(m_delAct);
//    connect(m_delAct, SIGNAL(triggered()), this, SLOT(removeCurrAct));
//    connect(m_delAct, SIGNAL(triggered(1)), this, SLOT(removeCurrAct()));
    connect(m_contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(removeCurrAct(QAction *)));
}

void mMenu::removeCurrAct()
{
//    qDebug() << "removeCurrAct()";
//    this->removeAction(m_curAct);
}

void mMenu::removeCurrAct(QAction *act)
{
//    qDebug() << "removeCurrAct(act)";
//    this->removeAction(m_curAct);
    if(act == m_delAct){
        emit removeItem(m_curAct);
    }
}

void mMenu::addSubMenuActsFilter(QAction *act)
{
//    m_enableSubMenu = true;
    m_actsList->append(act);
}

void mMenu::addSubMenuActsFilter(QList<QAction*> *actList)
{
//    m_enableSubMenu = true;
    m_actsList->append(*actList);
}

void mMenu::clearSubMenuActsFilter()
{
//    m_enableSubMenu = true;
    m_actsList->clear();
}

void mMenu::mouseReleaseEvent(QMouseEvent *e)
{
    if(/*m_enableSubMenu && */Qt::RightButton == e->button() && !m_subMenuActived){
        m_subMenuActived = true;
        m_curAct = this->activeAction();
//        QList<QAction *>::iterator i;
//        for(i=this->m_actsList->begin();i!=this->m_actsList->end();i++){
//            qDebug() << "act" << *i;
//        }
        if(m_curAct){
            if(m_actsList->contains(m_curAct)){
                QString actText = m_curAct->text();
                m_delAct->setText(QString("Del %1").arg(actText));
                m_contextMenu->exec(QCursor::pos());
            }
            else{
                QMenu::mouseReleaseEvent(e);
            }
        }
        m_subMenuActived = false;
    }
    else{
        QMenu::mouseReleaseEvent(e);
    }
}
