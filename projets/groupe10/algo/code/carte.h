// ======================= carte.h =======================
#ifndef CARTE_H
#define CARTE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QObject>
#include <QMouseEvent>

class Carte : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Carte(const QString &v, const QString &c, QGraphicsItem *parent = nullptr); // Constructeur
    QString getvalue();                     // Retourne la valeur de la carte
    void turn();                            // Retourne la carte (face visible/cachée)
    //bool estLibre() const { return bloquantes == 0 && refe; } // Carte libre si non bloquée et face visible
    ~Carte();                               // Destructeur

    QList<Carte*> dependantes;              // Cartes libérées si celle-ci est jouée
    int bloquantes = 0;                     // Nombre de cartes qui la bloquent
    bool refe = false;                      // true si face visible


signals:
    void carteCliquee(Carte *c);            // Signal émis lors d'un clic

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override ;     // Gestion clic
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;      // Gestion déplacement

private:
    QString valeur;                         // Valeur de la carte
    QPixmap retro;                          // Image face visible
    QPixmap verso;                          // Image face cachée
};

#endif // CARTE_H


