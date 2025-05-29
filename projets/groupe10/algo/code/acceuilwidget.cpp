#include "acceuilwidget.h"
#include <QRandomGenerator>
#include "mainwindow.h"

AccueilWidget::AccueilWidget(QGraphicsView* vueParent):  vue(vueParent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    scene = new QGraphicsScene(this);
    QPixmap image(":/images/images/poker1.jpg");          // Image de fond
    QPixmap redim = image.scaled(scene->width(), scene->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *font = new QGraphicsPixmapItem(redim);
    font->setZValue(-1000);                                 // Fond derrière tout
    scene->addItem(font);
    vue->setScene(scene);
    vue->setRenderHint(QPainter::Antialiasing);
    vue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vue->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vue->setSceneRect(0, 0, 800, 600);  // Taille par défaut
    layout->addWidget(vue);

    // Cartes animées
    initCartesAnimees();

    // Superposer les boutons
    QWidget *boutonsWidget = new QWidget(this);
    QHBoxLayout *vbox = new QHBoxLayout(boutonsWidget);
    vbox->setAlignment(Qt::AlignCenter);

    QPushButton *bt2 = new QPushButton("Reprendre");
    bt2->setStyleSheet("font-size: 20px; padding: 12px 20px;");
    vbox->addWidget(bt2);
    connect(bt2,&QPushButton::clicked,this,[=]() {
        QMessageBox::information(this, "Information", "Pas de reprise possible…");
    });

    QPushButton *bt3 = new QPushButton("Connexion");
    bt3->setStyleSheet("font-size: 20px; padding: 12px 20px;");
    vbox->addWidget(bt3);
    connect(bt3,&QPushButton::clicked,this,[=]() {
        QMessageBox::information(this, "Information", "Pas de reprise possible…");

    });


    QPushButton *bt4 = new QPushButton("Quiter");
    bt4->setStyleSheet("font-size: 20px; padding: 12px 20px;");
    vbox->addWidget(bt4);
    connect(bt4,&QPushButton::clicked,this,[=]() {
        QMessageBox::information(this, "Information", "Pas de reprise possible…");

    });

    QPushButton *bt1 = new QPushButton("Nouvelle Partie");
    bt1->setStyleSheet("font-size: 20px; padding: 12px 20px;");
    vbox->addWidget(bt1);
    connect(bt1,&QPushButton::clicked,this,[this,bt1,bt2,bt3,bt4,layout]() {
        emit lancementDuJeu();
        layout->removeWidget(bt1);
        bt1->hide();         // Cache le bouton
        bt1->deleteLater();  // Supprime le bouton proprement (évite les fuites mémoire)

        layout->removeWidget(bt2);
        bt2->hide();         // Cache le bouton
        bt2->deleteLater();  // Supprime le bouton proprement (évite les fuites mémoire)

        layout->removeWidget(bt3);
        bt3->hide();         // Cache le bouton
        bt3->deleteLater();  // Supprime le bouton proprement (évite les fuites mémoire)

        layout->removeWidget(bt4);
        bt4->hide();         // Cache le bouton
        bt4->deleteLater();  // Supprime le bouton proprement (évite les fuites mémoire)

    });



    boutonsWidget->setStyleSheet("background-color: rgba(0, 0, 0, 120);");
    layout->addWidget(boutonsWidget, 0, Qt::AlignCenter);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AccueilWidget::animerCartes);
    timer->start(40);
}

void AccueilWidget::initCartesAnimees() {
    for (int i = 0; i < 12; ++i) {
        QPixmap carte(":/images/images/Ac.gif"); // Remplace avec ton image
        QGraphicsPixmapItem *item = scene->addPixmap(carte.scaled(60, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qreal x = QRandomGenerator::global()->bounded(800);
        qreal y = QRandomGenerator::global()->bounded(600);
        item->setPos(x, y);
        cartes.append(item);
    }
}

void AccueilWidget::animerCartes() {
    for (auto *carte : cartes) {
        QPointF pos = carte->pos();
        pos.ry() += 1.2;
        if (pos.y() > 600)
            pos.setY(0);
        carte->setPos(pos);
    }
}
