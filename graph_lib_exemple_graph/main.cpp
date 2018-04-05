#include "grman/grman.h"
#include <iostream>
#include <fstream>
#include "Sous-p.h"
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /*///Initialisation d'Allegro
    srand(time(NULL));
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen);*/

    ///D�claration des bitmaps
    BITMAP* ecran=create_bitmap(1600,1200); //Bitmap d'affichage
    BITMAP* Menu=load_bitmap("Nemo.bmp",NULL); //Bitmaps du menu
    BITMAP* Graphe=load_bitmap("Graphe.bmp",NULL); //Bitmap d'acc�s aux informations explicatives
    BITMAP* Reseau=load_bitmap("Reseau.bmp",NULL); //Bitmap pour acc�der � nos trois r�seaux
    BITMAP* Quitter=load_bitmap("Quitter1.bmp",NULL); //Bitmap pour quitter

    int fin=0; // Variable pour le menu
    menu(ecran,Menu,Graphe,Reseau,Quitter,fin);

    /*return 0;*/



    /*/// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
    /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        g.sauvgarder();
    }

    grman::fermer_allegro();*/

    return 0;
}
END_OF_MAIN();


