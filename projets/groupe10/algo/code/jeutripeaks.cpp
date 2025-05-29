// ======================= jeutripeaks.cpp =======================
#include "jeutripeaks.h"
#include <QRandomGenerator>
#include <QDebug>

JeuTripeaks::JeuTripeaks(QGraphicsView *vueParent) : vue(vueParent) {
    scene = new QGraphicsScene(this);        // Création de la scène

    vue->setScene(scene);        // Associe la scène à la vue
    vue->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0, 0, 1280, 660);                   // Taille de la scène

    QPixmap image(":/images/images/fontgame.jpg");          // Image de fond
    QPixmap redim = image.scaled(scene->width(), scene->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *font = new QGraphicsPixmapItem(redim);
    font->setZValue(-1000);                                 // Fond derrière tout
    scene->addItem(font);
    sonClic = new QMediaPlayer(this);
    sonError = new QMediaPlayer(this);
    audio = new QAudioOutput(this);
    sonClic->setAudioOutput(audio);
    sonError->setAudioOutput(audio);
    sonClic->setSource(QUrl("qrc:/sons/son/son1.mp3"));
    sonError->setSource(QUrl(":/sons/son/son1.mp3"));
    audio->setVolume(100);
}

JeuTripeaks::~JeuTripeaks() {
                                                            // Rien à supprimer manuellement grâce à Qt parent-child
    scene->clear();
}

void JeuTripeaks::game() {
    scoreText = new QGraphicsTextItem;
    scoreText->setPlainText("Score: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 16));
    scoreText->setPos(10, 10); // Position en haut à gauche
    scene->addItem(scoreText);

    chargerCartes();
    melangeCarte();
    genererPyramides();
    genererLaCarteDeJeu();
    genererPiles();
}

void JeuTripeaks::gererCarteCliquer(Carte *c) {
    qDebug() << "HELLO"<<valeurEnInt(c->getvalue());
    if (!carteEnJeu || (!LaDeckList.contains(c) && !c->refe)) {
        sonError->play();
        return;               // Vérifie les conditions de jouabilité
    }
    qDebug()<<"Passer....";

    QString v1 = carteEnJeu->getvalue();
    QString v2 = c->getvalue();

    int val1 = valeurEnInt(v1);
    int val2 = valeurEnInt(v2);

    sonClic->play();
    if (sonClic->isPlaying()){
        qDebug() << "Sound...................";
    }

                                                            // Vérifie si les valeurs sont consécutives ou A-K/K-A
    if (std::abs(val1 - val2) == 1 || (v1 == "A" && v2 == "K") || (v1 == "K" && v2 == "A")) {
        score += 10; // ou tout autre système de points
        scoreText->setPlainText("Score: " + QString::number(score));
        carteEnJeu->hide();
        carteEnJeu = c;                                     // Nouvelle carte en jeu
        c->setZValue(10);
        c->setPos(450, 450);                                // Position centrale
        LesCartes.removeOne(c);

        if (LesCartes.isEmpty()){
            scoreText->setPos(620, 200);
            scoreText->setPlainText("Winner ...");

        }
        for (const QList<Carte*>& lotsdeDep : Dependantes) {
            if (lotsdeDep.size() < 3) continue;

            Carte* carteHaut = lotsdeDep[0];
            Carte* carteBas1 = lotsdeDep[1];
            Carte* carteBas2 = lotsdeDep[2];

            // Vérifier si les deux cartes du bas sont retournées
            if (carteBas1->refe && carteBas2->refe) {
                qreal yCarte1 = carteBas1->pos().y();
                qreal yCarte2 = carteBas2->pos().y();
                qreal yEnJeu  = carteEnJeu->pos().y();

                // Vérifie si elles sont au même niveau que la carte en jeu (avec marge de 2 pixels)
                if (std::abs(yCarte1 - yEnJeu) < 2 && std::abs(yCarte2 - yEnJeu) < 2) {
                    qDebug() << "Les deux cartes sont au niveau de la carte en jeu, on retourne la carte du haut.";
                    carteHaut->turn();

                    // Supprime l'ensemble de dépendance après action
                    Dependantes.removeOne(lotsdeDep);
                }
            }
        }
    }
    if (LaDeckList.contains(c)){
        score -=5 ; // ou tout autre système de points
        scoreText->setPlainText("Score: " + QString::number(score));
        carteEnJeu->hide();
        c->setZValue(10);
        c->turn();
        carteEnJeu = c;                                     // Nouvelle carte en jeu
        c->setZValue(10);
        c->setPos(450, 450);                                // Position centrale
        LaDeckList.removeOne(c);                            // Retirer la carte de la deck liste restante
        if (LaDeckList.isEmpty()){
            scoreText->setPos(620, 200);
            scoreText->setPlainText("Game Over ...");
        }
    }

}

int JeuTripeaks::valeurEnInt(const QString &val) {
    if (val == "A") return 1;
    if (val == "J") return 11;
    if (val == "Q") return 12;
    if (val == "K") return 13;
    return val.toInt();
}




void JeuTripeaks::sauvegarderPartie(const QString &nomFichier) {
    QFile fichier(nomFichier);
    if (!fichier.open(QIODevice::WriteOnly)) {
        qWarning() << "Impossible d’ouvrir le fichier pour écriture :" << nomFichier;
        return;
    }

    QJsonObject racine;

    // Sauvegarde des cartes dans LesCartes (sur les pics)
    QJsonArray cartesArray;
    for (Carte *c : LesCartes) {
        QJsonObject obj;
        obj["valeur"] = c->getvalue();
        obj["x"] = c->pos().x();
        obj["y"] = c->pos().y();
        obj["visible"] = c->refe;  // ou un getter ex: c->isVisible()
        cartesArray.append(obj);
    }
    racine["cartes"] = cartesArray;

    // Sauvegarde des cartes de la pioche (LaDeckList)
    QJsonArray deckArray;
    for (Carte *c : LaDeckList) {
        QJsonObject obj;
        obj["valeur"] = c->getvalue();
        deckArray.append(obj);
    }
    racine["deck"] = deckArray;

    // Carte en jeu (carteEnJeu)
    if (carteEnJeu) {
        QJsonObject carteEnJeuObj;
        carteEnJeuObj["valeur"] = carteEnJeu->getvalue();
        carteEnJeuObj["x"] = carteEnJeu->pos().x();
        carteEnJeuObj["y"] = carteEnJeu->pos().y();
        carteEnJeuObj["visible"] = carteEnJeu->refe;
        racine["carteEnJeu"] = carteEnJeuObj;
    }

    // Dépendances des cartes
    QJsonArray dependancesArray;
    for (const QList<Carte*> &dep : Dependantes) {
        if (dep.size() == 3) {
            QJsonObject depObj;
            depObj["carte"] = dep[0]->getvalue();
            depObj["gauche"] = dep[1] ? dep[1]->getvalue() : "";
            depObj["droite"] = dep[2] ? dep[2]->getvalue() : "";
            dependancesArray.append(depObj);
        }
    }
    racine["dependances"] = dependancesArray;

    // Sauvegarde dans le fichier
    QJsonDocument doc(racine);
    fichier.write(doc.toJson(QJsonDocument::Indented));
    fichier.close();
}

void JeuTripeaks::chargerPartie(const QString &nomFichier) {
    QFile fichier(nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d’ouvrir le fichier pour lecture :" << nomFichier;
        return;
    }

    QByteArray donnees = fichier.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(donnees);
    if (!doc.isObject()) {
        qWarning() << "Le fichier n'est pas un document JSON valide.";
        return;
    }

    QJsonObject racine = doc.object();

    // Nettoyage de l'ancien état
    for (Carte *c : LesCartes) {
        scene->removeItem(c);
        delete c;
    }
    LesCartes.clear();

    for (Carte *c : LaDeckList) {
        scene->removeItem(c);
        delete c;
    }
    LaDeckList.clear();

    if (carteEnJeu) {
        scene->removeItem(carteEnJeu);
        delete carteEnJeu;
        carteEnJeu = nullptr;
    }

    Dependantes.clear();

    // Chargement des cartes sur les pics
    QJsonArray cartesArray = racine["cartes"].toArray();
    for (const QJsonValue &val : cartesArray) {
        QJsonObject obj = val.toObject();
        QString valeur = obj["valeur"].toString();
        QString couleur = valeur.right(1);
        QString v = valeur.left(valeur.length() - 1);

        Carte *c = new Carte(v, couleur);
        c->setPos(obj["x"].toDouble(), obj["y"].toDouble());
        if (obj["visible"].toBool()) c->turn();

        scene->addItem(c);
        connect(c, &Carte::carteCliquee, this, &JeuTripeaks::gererCarteCliquer);
        LesCartes.append(c);
    }

    // Chargement de la pioche
    QJsonArray deckArray = racine["deck"].toArray();
    for (const QJsonValue &val : deckArray) {
        QString valeur = val.toObject()["valeur"].toString();
        QString couleur = valeur.right(1);
        QString v = valeur.left(valeur.length() - 1);
        Carte *c = new Carte(v, couleur);
        LaDeckList.append(c);
    }

    // Chargement de la carte en jeu
    if (racine.contains("carteEnJeu")) {
        QJsonObject obj = racine["carteEnJeu"].toObject();
        QString valeur = obj["valeur"].toString();
        QString couleur = valeur.right(1);
        QString v = valeur.left(valeur.length() - 1);

        carteEnJeu = new Carte(v, couleur);
        carteEnJeu->setPos(obj["x"].toDouble(), obj["y"].toDouble());
        if (obj["visible"].toBool()) carteEnJeu->turn();

        scene->addItem(carteEnJeu);
    }

    // Chargement des dépendances
    QJsonArray dependancesArray = racine["dependances"].toArray();
    for (const QJsonValue &val : dependancesArray) {
        QJsonObject obj = val.toObject();

        QString v1 = obj["carte"].toString();
        QString v2 = obj["gauche"].toString();
        QString v3 = obj["droite"].toString();

        Carte *c1 = trouverCarteParValeur(v1);
        Carte *c2 = trouverCarteParValeur(v2);
        Carte *c3 = trouverCarteParValeur(v3);

        Dependantes.append({c1, c2, c3});
    }
}

Carte* JeuTripeaks::trouverCarteParValeur(const QString &valeur) {
    for (Carte *c : LesCartes) {
        if (c->getvalue() == valeur)
            return c;
    }
    return nullptr;
}

void JeuTripeaks::chargerCartes()
{
    QStringList couleur = {"h" , "d" , "c" , "s"};
    QStringList valeurs = {"A", "2","3","4","5","6","7","8","9","10","J","Q","K"};

    for (const auto& c : couleur){
        for (const auto& v : valeurs){
            Carte *carte = new Carte (v,c);
            connect(carte, &Carte::carteCliquee, this, &JeuTripeaks::gererCarteCliquer);
            bool ok = connect(carte, &Carte::carteCliquee, this, &JeuTripeaks::gererCarteCliquer);
            qDebug() << "Connexion réussie ? " << ok;
            pile.push(carte);
        }
    }
}

void JeuTripeaks::genererPyramides()
{
    int baseY = 50;                             // Distance entre Hauteur et somment premier Catre
    int carteH = 96 ,  carteW = 72;             // Dimension de la carte
    LesAllCarteParPic.clear();
    for (int pic = 0; pic < 3; ++pic) {
        QList <int> IdParPic;          // Recuperation des indice des cartes par pic
        QList <Carte*> CarteParPic;
        int i=0;
        int startX = 300 + pic * (4 * carteW);  // Augmenté pour bien espacer les pyramides
        for (int ligne = 0; ligne < 4; ++ligne) {
            for (int col = 0; col <= ligne; ++col) {
                int x = startX + col * carteW - (ligne * carteW) / 2;
                int y = baseY + ligne * (carteH * 0.4);
                Carte *c = pile.top();
                pile.pop();
                c->setPos(x, y);
                std::vector<int> position = {x,y};
                LesPositions.push_back(position);       // Memorisation des position
                CarteParPic.append(c);
                LesCartes.push_back(c);
                IdParPic.push_back(i);
                i++;
                if (ligne == 3) {
                    c->turn();
                }
                scene->addItem(c);
            }

        }
        LesAllCarteParPic.append(CarteParPic);
    }

    // ==== Liaison des dépendances (carte au-dessus -> carte(s) en dessous) ====

    Dependantes.clear();

    for (QList <Carte *> pic : LesAllCarteParPic) {
        int index = 0;
        for (int ligne = 0; ligne < 3; ++ligne) {
            for (int col = 0; col <= ligne; ++col) {
                int carte_index = index;
                int gauche_index = index + ligne + 1;
                int droite_index = gauche_index + 1;

                QList<Carte*> dependantes;

                dependantes.append(pic[carte_index]);
                dependantes.append(pic[gauche_index]);
                dependantes.append(pic[droite_index]);

                Dependantes.append(dependantes);

                index++;
            }
        }
    }
}


void JeuTripeaks::genererPiles()
{
    int i = 0;
    while (!pile.empty()){
        Carte *c = pile.top();
        LaDeckList.append(c);
        pile.pop();
        c->setPos(350 +0.1*i,450);
        scene->addItem(c);
        i++;
    }
}

void JeuTripeaks::melangeCarte(){  // Melange les Cartes dans la pile de Carte de depart
    std::vector<Carte*> temp={};
    while(!pile.empty()){
        temp.push_back(pile.top());
        pile.pop();
    }
    std::random_device rd;              // Creation d'un operateur de gestion aleatoire rd
    std::mt19937 gen(rd());             // Generateur de chiffre aleatoire a partir de l'operateur gen
    std::shuffle(temp.begin(), temp.end(), gen);    // Melange le lot de Carte de maniere aleatoire
        // En suite donne a chaque joueur 2 carte de manier aleatoire
    for (auto& c : temp){
        pile.push(c);
    }
}

void JeuTripeaks::genererLaCarteDeJeu()
{
    if (!pile.empty()){
        auto *carte = pile.top();
        pile.pop();
        carte->setPos(450,450);
        carte->turn();
        scene->addItem(carte);
        carteEnJeu = carte;
    }
}
