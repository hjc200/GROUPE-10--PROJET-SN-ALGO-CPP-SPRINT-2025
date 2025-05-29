#include "carte.h"

Carte::Carte(const QString &v, const QString &c, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), valeur(v) {
                                                                // Chargement des images
    retro.load(":/images/images/" + v + c + ".gif");
    verso.load(":/images/images/dos1.jpeg");

                                                                // Redimensionner les images
    retro = retro.scaled(72, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    verso = verso.scaled(72, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                                                                // Initialisation avec l'image verso (carte cachée)
    setPixmap(verso);
}

QString Carte::getvalue() {
    return valeur;                                              // Retourne la valeur
}

void Carte::turn() {
    refe = !refe;                                               // Inverse l'état de la carte (face visible ou non)
    setPixmap(refe ? retro : verso);                            // Affiche l'image appropriée
}

void Carte::mousePressEvent(QGraphicsSceneMouseEvent *) {
    emit carteCliquee(this);                                    // Émet le signal de clic
}

/*void Carte::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPixmapItem::mouseMoveEvent(event);                 // Appel à la bas
    setPos(mapToScene(event->pos()));                           // Déplace la carte dans la scène

}*/

Carte::~Carte() {
                                                                // Rien à libérer manuellement
}
