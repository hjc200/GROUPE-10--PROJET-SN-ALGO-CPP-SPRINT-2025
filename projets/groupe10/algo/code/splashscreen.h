#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QtCore/qglobal.h>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMovie>

class SplashScreen : public QWidget {
    Q_OBJECT
public:
    SplashScreen(QWidget *parent = nullptr);
};

#endif // SPLASHSCREEN_H
