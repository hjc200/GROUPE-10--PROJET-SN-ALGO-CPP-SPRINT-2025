#include "fenetrerules.h"
#include <QVBoxLayout>
//#include <QLabel>
#include <QPushButton>
#include <QTextEdit>


FenetreRules::FenetreRules(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Comment jouer ?");
    setMinimumSize(400, 300);

    setStyleSheet("background-color:#2b2d42; color:white; font-size:14px;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QTextEdit *textEdit = new QTextEdit(this);
    //label->setWordWrap(true);
    textEdit->setReadOnly(true);
    //label->setText();
    textEdit->setText(
        "Bienvenue dans le jeu !\n\n"
        "Comment y jouer ?\n\n"
        "Pour commencer la partie, "
        "nous retirons une carte de la pile de réserve et la plaçons face visible sur la défausse (qui commence vide). "
        "Cette carte servira de première base pour commencer à déconstruire les pyramides.\n"
        "Vous devez l'associer à une carte de la pyramide, face visible, "
        "qui lui est supérieure ou inférieure d'un point. "
        "Par exemple, si votre carte de base est un 5, vous pouvez l'associer à un 4 ou un 6. "
        "La couleur et les couleurs n'ont aucune importance dans une partie de Tripeaks. "
        "Seuls les chiffres comptent.\n"
        "La dernière carte que vous ajoutez à la défausse reste sur le dessus, face à l'avant, et devient la prochaine base pour la paire suivante. "
        "En réalité, plutôt que de créer des paires, vous devez construire des séquences en allant plus haut ou plus bas à chaque coup.\n"
        "S'il n'y a pas de carte sur le tableau à placer au-dessus de la dernière carte défaussée, la séquence s'arrête. "
        "Dans ce cas, vous pouvez prendre une ou plusieurs cartes du stockage pour remplacer celle de la pile de défausse. "
        "Dans ce Tripeaks en ligne, il vous suffit de cliquer sur la pile de stockage pour retourner une carte.\n"
        "Vous perdrez la partie lorsqu'il n'y aura plus de coups disponibles (et que la pile de stockage sera vide) mais que certaines cartes resteront sur le tableau.\n\n"
        "Comment gagner au tripeaks ?\n\n"
        "Malgré sa simplicité de jeu, gagner au Tripeak n'est pas aussi simple qu'il n'y paraît. "
        "Bien que vous n'ayez pas besoin d'être aussi stratégique que dans d'autres jeux de Solitaire tels que Klondike ou Spider, vous devez tout de même réfléchir soigneusement à vos mouvements. \n"
        "Par exemple, le fait d'associer les cartes de façon distraite donne rarement lieu à de longues séquences. "
        "Cela signifie que vous devrez utiliser souvent le stockage, ce qui augmente les chances de ne plus avoir de coups disponibles même si plusieurs cartes restent sur le tableau.\n"
        "Si vous voulez gagner une partie de Tripeaks, voici quelques trucs et astuces qui vous aideront.\n"
        "•	Rangée par rangée\n"
        "Au lieu d'essayer de déconstruire chaque pyramide à la fois, attaquez-vous au tableau ligne par ligne. "
        "En procédant ainsi, vous aurez moins de cartes qui se chevauchent à chaque fois, ce qui augmentera vos chances de construire des séquences puisque vous aurez plus de cartes orientées vers l'avant.\n"
        "•	Découvrez la carte face cachée lorsque vous en avez la possibilité\n"
        "Chaque fois que vous devez choisir entre deux cartes ou plus pour construire une séquence, vérifiez si l'une d'entre elles peut débloquer une carte retournée. Si c'est le cas, c'est celle que vous devez choisir.\n"
        "Alors que les autres vous laisseront avec le même tableau pour le coup suivant, celui qui révèle une carte ajoute une nouvelle voie au jeu. "
        "Même s'il n'y a pas d'avantage immédiat, vous pouvez avoir besoin de la carte maintenant découverte pour votre prochaine séquence et peut-être cela vous aidera-t-il à éviter d'utiliser la pile de réserve.\n"
        "•	Essayez d'établir de longues séquences en alternant entre le haut et le bas.\n"
        "Plus la séquence est longue, plus vous devez révéler et défausser de cartes sans utiliser la pile de réserve, que vous voulez garder aussi grande que possible jusqu'à la fin. \n"
        "Lorsque vous construisez une séquence, gardez à l'esprit que vous ne devez pas vous en tenir à une seule direction. Vous pouvez aller plus haut ou plus bas à tout moment. Essayez de planifier vos mouvements à l'avance pour les maximiser. De plus, essayez de jouer avec différentes combinaisons de chiffres pour vérifier si vous pouvez étendre votre séquence. \n"
        "•	Se souvenir des cartes utilisées\n"
        "C'est l'une des astuces les plus utiles et les plus importantes du Solitaire Tripeaks si vous voulez gagner. N'oubliez pas que ce jeu n'utilise qu'un seul jeu de 52 cartes. Par conséquent, si vous gardez une trace des cartes rejetées, vous pouvez avoir une meilleure idée de celles qui peuvent encore être retournées ou dans la pile de réserve et planifier vos séquences en conséquence.\n"
        "Par exemple, si tous les 5 ont déjà été écartés et que vous avez un 6 sur le tableau, cela signifie que vous ne pouvez l'enlever qu'en l'appariant avec un 7. Si vous continuez à utiliser vos 7 pour monter plus haut et les apparier avec des 8, vous finirez par manquer de cartes pour enlever le 6 de la pyramide. Et une fois que vous ne pourrez plus le retirer, vous perdrez la partie."

        );
    QPushButton *fermerBtn = new QPushButton("Fermer");
    connect(fermerBtn,&QPushButton::clicked, this, &FenetreRules::accept);
    layout->addWidget(textEdit);
    setMinimumSize(400, 300);
    layout->addWidget(fermerBtn);
}
