#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QObject>
#include <QMutex>
#include <QFont>
#include <QLabel>
#include <QPushButton>

class FontAws : public QObject
{
private:
    explicit FontAws(QObject *parent = 0);
    QFont iconFont;
    static FontAws* _instance;

public:
    static FontAws* Instance()
    {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new FontAws;
            }
        }
        return _instance;
    }

    void SetIcon(QLabel* lab, QChar c, int size = 10);
    void SetIcon(QPushButton* btn, QChar c, int size = 10);

};

#endif // ICONHELPER_H
