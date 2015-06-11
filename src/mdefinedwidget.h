#ifndef MDEFINEDWIDGET_H
#define MDEFINEDWIDGET_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class mDefinedWidget : public QDialog
{
    Q_OBJECT
public:
    explicit mDefinedWidget(QWidget *parent = 0);
    ~mDefinedWidget();
    void changeSkin(const QString &skinName);

private:
    void drawWindow(QPainter &painter);
    void initSkinLayout();
    bool readSettings();
    int  readSettingsValue(QSettings *settings_l, const QString &group_l, const QString &key_l);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

signals:
//    void windowActive();
public slots:

private:
    QPoint m_moveOldPos;        // 移动之前的位置坐标
    bool m_mouseLeftPress;      // 鼠标左键按下
    bool m_winactive;
    QString *m_skinName;

    int m_TopCornerW;
    int m_TopCornerH;
    int m_TopBorderH;          // may be always equale to m_topCornerHeight
    int m_MiddleBorderW;
    int m_BottomCornerW;
    int m_BottomCornerH;
    int m_BottomBorderH;

    int m_InActTopCornerW;
    int m_InActTopCornerH;
    int m_InActTopBorderH;          // may be always equale to m_topCornerHeight
    int m_InActMiddleBorderW;
    int m_InActBottomCornerW;
    int m_InActBottomCornerH;
    int m_InActBottomBorderH;

    int m_topCW;
    int m_topCH;
    int m_topBH;          // may be always equale to m_topCornerHeight
    int m_middleBW;
    int m_bottomCW;
    int m_bottomCH;
    int m_bottomBH;
};

#endif // MDEFINEDWIDGET_H
