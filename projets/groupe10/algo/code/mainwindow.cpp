#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jeutripeaks.h"
#include <QHBoxLayout>
#include <QPushButton>

#include "fenetrerules.h"
#include "fenetrepropos.h"
#include "acceuilwidget.h"
#include "splashscreen.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QIcon>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , musiqueActive(true)
    , effetsActifs(true)
{
    ui->setupUi(this);

    vue = new QGraphicsView(this);

    // Supprimer les scrollbars
    vue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vue->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vue);


    stack = new QStackedWidget(this);
    splash = new SplashScreen;
    accueil = new AccueilWidget(vue);
    connect(accueil, &AccueilWidget::lancementDuJeu, this, &MainWindow::game);

    stack->addWidget(splash);   // index 0
    stack->addWidget(accueil);  // index 1

    setCentralWidget(stack);
    stack->setCurrentIndex(0);

    // Affiche l'accueil après 3 secondes
    QTimer::singleShot(3000, this, &MainWindow::showAccueil);

    setWindowIcon(QIcon(":/images/images/joker3.jpeg"));
    setWindowTitle("TRIPEAKS");
    resize(600, 400);
}


void MainWindow::mettreAJourMusique()
{
    if (musiqueActive) {
        qDebug() << "Musique activée";
        // Code pour démarrer la musique ici
        QMessageBox::information(this, "Musique", "Musique activée !");
    } else {
        qDebug() << "Musique désactivée";
        // Code pour arrêter la musique ici
        QMessageBox::information(this, "Musique", "Musique désactivée !");
    }
}

void MainWindow::mettreAJourEffets()
{
    if (effetsActifs) {
        qDebug() << "Effets sonores activés";
        // Code pour activer les effets ici
        QMessageBox::information(this, "Effets sonores", "Effets activés !");
    } else {
        qDebug() << "Effets sonores désactivés";
        // Code pour désactiver les effets ici
        QMessageBox::information(this, "Effets sonores", "Effets désactivés !");
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter .drawPixmap(rect(), fondEcran);
    QMainWindow::paintEvent(event);
}

void MainWindow::showAccueil() {
    stack->setCurrentIndex(1);
    monMenu = true;

}

void MainWindow::game(){
    delete vue;
    vue = new QGraphicsView(this);
    // Supprimer les scrollbars
    if (monMenu){
        QMenu *menuJeu = menuBar()->addMenu("MENU");
        fondEcran = QPixmap(":/img/background.jpg");

        // Comment jouer
        QAction *actionCommentJouer = new QAction(QIcon(":/images/images/iconcommentjouer.jpg"),"Comment jouer", this);
        connect(actionCommentJouer,&QAction::triggered,this, [=](){

            FenetreRules dialog;
            dialog.exec();

        });
        // Recommencer
        QAction *actionRecommencer = new QAction(QIcon(":/images/images/iconretry.jpg"),"Recommencer", this);
        connect(actionRecommencer, &QAction::triggered, this, [=]() {
            QTimer::singleShot(0, this, [=]() {
                delete jeu;
                jeu = new JeuTripeaks(vue);
                jeu->game();
            });
        });

        // Sauvegarde de la partie
        QAction *sauvegardePartie = new QAction ("Sauvergarder de la Partie" , this);
        connect(sauvegardePartie, &QAction::triggered, this , [=](){

        });

        // Sous-menu Option de jeu
        QMenu *menuOptions = new QMenu("Option de jeu", this);

        QAction *actionMusique = new QAction("Musique", this);
        actionMusique->setCheckable(true);
        actionMusique->setChecked(musiqueActive);

        QAction *actionEffets = new QAction("Effets sonores", this);
        actionEffets->setCheckable(true);
        actionEffets->setChecked(effetsActifs);

        connect(actionMusique, &QAction::toggled, this, [=](bool checked) {
            musiqueActive = checked;
            mettreAJourMusique();
        });

        connect(actionEffets, &QAction::toggled, this, [=](bool checked) {
            effetsActifs = checked;
            mettreAJourEffets();
        });

        menuOptions->addAction(actionMusique);
        menuOptions->addAction(actionEffets);

        // A propos
        QAction *actionAPropos = new QAction(QIcon(":/images/images/iconpropos.jpg"),"A Propos", this);
        //connect(actionAPropos, &QAction::triggered, this, []() {
        //QMessageBox::information(nullptr, "A propos",
        //  "texte");
        connect(actionAPropos,&QAction::triggered,this,[=](){
            //FenetrePropos *fenetrep = new FenetrePropos(this);
            //fenetrep->exec();
            FenetrePropos dialog;
            dialog.exec();

        });

        // Quitter
        QAction *actionQuitter = new QAction(QIcon(":/images/images/iconquit.jpg"),"Quitter", this);
        actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
        connect(actionQuitter, &QAction::triggered, this, &MainWindow::close);

        // Ajouter au menu
        menuJeu->addAction(actionCommentJouer);
        menuJeu->addSeparator();
        menuJeu->addAction(actionRecommencer);
        menuJeu->addSeparator();
        menuJeu->addMenu(menuOptions);
        menuJeu->addSeparator();
        menuJeu->addAction(actionAPropos);
        menuJeu->addSeparator();
        menuJeu->addAction(actionQuitter);

        // Style visuel
        setStyleSheet(R"(
            QMenuBar {
                background-color: #1e1e1e;
                color: white;
                font-size: 18px;
                font-family: "Arial Black";
            }

            QMenu {
                background-color: #2c2c2c;
                color: white;
                font-size: 16px;
            }
            QMenu::item:selected {
                background-color: #444444;
            }
        )");
    }
    vue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vue->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vue);
    jeu = new JeuTripeaks(vue);
    jeu->game();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete jeu;
}
