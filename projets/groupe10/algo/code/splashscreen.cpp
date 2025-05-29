#include "splashscreen.h"

#include <QLabel>
#include <QVBoxLayout>

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Chargement de Tripeak...", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 24px; font-weight: bold;");

    layout->addStretch();
    layout->addWidget(label);
    layout->addStretch();
}
