// ======================= jeutripeaks.h =======================
#ifndef JEUTRIPEAKS_H
#define JEUTRIPEAKS_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <stack>
#include "carte.h"
#include <QList>
#include <vector>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class JeuTripeaks : public QObject {
    Q_OBJECT

public:
    JeuTripeaks(QGraphicsView *vueParent);                  // Constructeur
    ~JeuTripeaks();                                         // Destructeur

    void game();                                            // Lance une partie
    void melangeCarte();                                    // Mélange les cartes
    void chargerCartes();                                   // Charge les cartes depuis les ressources
    void genererPyramides();                                // Génère les pyramides du jeu
    void genererPiles();                                    // Génère les cartes restantes en pioche
    void genererLaCarteDeJeu();                             // Crée la carte de départ jouable
    int valeurEnInt(const QString &val);                    // Convertit une valeur de carte en entier
    Carte* trouverCarteParValeur(const QString &valeur);    // renvoie une carte

    QList<QList<Carte*>> Dependantes;                       // Liste de toute les Cartes et leurs dependantes
    int score = 0;
    QGraphicsTextItem* scoreText;                           // Affichage du score
    QList <Carte*> LaDeckList;                              // La pile de carte restante
    QList <QList <Carte *>> LesAllCarteParPic;                // Les diffentes carte pour chaque piques
    QMediaPlayer * sonClic;
    QMediaPlayer * sonError;
    QAudioOutput * audio;



public slots:
    void gererCarteCliquer(Carte *);                        // Réagit aux clics sur les cartes
    void sauvegarderPartie(const QString &nomFichier);      // Sauvegarde l'état du jeu
    void chargerPartie(const QString &nomFichier);          // Recharge une partie sauvegardée

private:
    QGraphicsScene *scene = nullptr;                        // Scène graphique
    QGraphicsView *vue = nullptr;                           // Vue associée
    std::stack<Carte*> pile;                                // Pile des cartes restantes
    std::vector<std::vector<int>> LesPositions;             // Positions pour recréer la pyramide
    QList<Carte*> LesCartes;                                // Liste de toutes les cartes
    Carte *carteEnJeu = nullptr;                            // Carte actuellement jouée
};

#endif // JEUTRIPEAKS_H
