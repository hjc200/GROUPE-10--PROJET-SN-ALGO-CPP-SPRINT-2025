#ifndef ACCEUILWIDGET_H
#define ACCEUILWIDGET_H

#include "jeutripeaks.h"

#include <QtCore/qglobal.h>
#include <QMessageBox>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRandomGenerator>

class AccueilWidget : public QWidget {
    Q_OBJECT

public:
    explicit AccueilWidget(QGraphicsView* vue);
private:
    QGraphicsView *vue;
    QGraphicsScene *scene;
    QList<QGraphicsPixmapItem*> cartes;
    QTimer *timer;

    void initCartesAnimees();

    JeuTripeaks * jeu;
signals:
    void lancementDuJeu();

private slots:
    void animerCartes() ;
};

#endif // ACCUEILWIDGET_H
