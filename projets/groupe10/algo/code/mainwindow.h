#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QBoxLayout>
#include "jeutripeaks.h"

#include <QPixmap>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QPoint>
#include <QLabel>
#include <QStackedWidget>
#include "splashscreen.h"
#include "acceuilwidget.h"

#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void game();

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    Ui::MainWindow *ui;


    QPixmap fondEcran;
    bool musiqueActive;
    bool effetsActifs;

    void mettreAJourMusique();
    void mettreAJourEffets();
    QMediaPlayer *lecteurMusique;
    QAudioOutput *audiosortie;
    QAction *actionMusique;
    QGraphicsView* vue;

    JeuTripeaks * jeu;

    void setupUI();

    bool monMenu = false;

    QStackedWidget *stack;
    SplashScreen *splash;
    AccueilWidget *accueil;

    void showAccueil();
};
#endif // MAINWINDOW_H
