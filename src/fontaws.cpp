#include "fontaws.h"
#include <QFontDatabase>
#include <QApplication>

FontAws* FontAws::_instance = 0;
FontAws::FontAws(QObject*):
    QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/fontaws");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);
}

void FontAws::SetIcon(QLabel* lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void FontAws::SetIcon(QPushButton* btn, QChar c, int size)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}
