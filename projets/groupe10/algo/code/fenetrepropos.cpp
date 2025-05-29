#include "fenetrepropos.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>


FenetrePropos::FenetrePropos(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("A Propos");
    setMinimumSize(400, 300);

    setStyleSheet("background-color:#2b2d42; color:white; font-size:14px;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setText(
        "Bienvenue dans le jeu!\n\n"
        "A Propos\n\n"
        "Tripeaks est un jeu de cartes à un joueur dont le gameplay est facile, mais non moins stimulant. "
        "Son nom vient de la forme des cartes affichées dans le tableau. Celles-ci sont positionnées de manière à créer trois pyramides de cartes superposées, et comme chaque pyramide a un sommet, nous avons les Tri-Peaks ou triples pics. \n"
        "Comme pour les autres jeux de Solitaire, le but est ici aussi de faire disparaître toutes les cartes du tableau. Cependant, la manière d'y parvenir est sensiblement différente de celle du Klondike ou du Spider Solitaire. Au lieu de créer des séquences complètes dans l'ordre et par couleur, les joueurs doivent maintenant créer des séquences mixtes en alternant des cartes d'un numéro supérieur et/ou inférieur à la précédente.\n\n"
        "QUELLES SONT LES REGLES DU JEU ?\n\n"
        "Solitaire Tripeaks utilise un jeu de 52 cartes. Le but est de rassembler toutes les cartes affichées dans le tableau en déconstruisant les pyramides. Les cartes ne peuvent être collectées que si elles ont un point de plus ou de moins que la carte supérieure de la défausse.\n"
        "Dans Tripeaks, le plateau est divisé en trois zones principales : les pyramides (sur le tableau), la pile de stockage et la pile de défausse.\n"
        "Sur le tableau, vous trouverez trois pyramides. Chacune a 4 cartes comme base et se termine par une seule carte comme sommet. Cela signifie qu'au total, vous avez 28 cartes disposées sur 4 rangées. Les cartes de la rangée inférieure (la base de chaque pyramide) sont orientées vers le haut, tandis que les autres sont orientées vers le bas. Il n'est possible de retourner une carte que lorsqu'il n'y a pas de chevauchement avec les autres.\n"
        "Les cartes restantes sont regroupées, face cachée, dans la pile de réserve. La pile de défausse est l'endroit où les joueurs doivent mettre les cartes collectées.\n\n"
        "DEVELOPPEURS\n\n"
        "BAYIGA BOGMIS IVAN\n  "
        "    CONTACT: +237697221899\n "
        "    EMAIL: ivanbayigabogmis0@gmail.com\n"
        "HANGBAN JEAN CLAUDE \n"
        "    CONTACT: 659413486\n  "
        "    EMAIL: hangbanwesley@gmail.com\n"
        "GLORIA CHIKOAM TCHAKOUNTE \n"
        "    CONTACT:658269542\n  "
        "    EMAIL: glorychikoam@gmail.com\n"
        "BELLE EBULLE MADELEINE SHEBANIA\n"
        "DEGNON KOMI PRINCE EMILE"


        );
    QPushButton *fermerBtn = new QPushButton("Fermer");
    connect(fermerBtn,&QPushButton::clicked, this, &FenetrePropos::accept);
    layout->addWidget(textEdit);
    setMinimumSize(400, 300);
    layout->addWidget(fermerBtn);
}
