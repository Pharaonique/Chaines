#include "Sous-p.h"
#include <iostream>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <vector>
#include "graph.h"



///Sous-programme du menu principale
void menu (BITMAP* ecran, BITMAP* Menu, BITMAP* Graphe, BITMAP* Reseau, BITMAP* Quitter, int fin)
{

    fin = 0; // variable de la boucle
    int v = 0 ; //valeur utilisée pour temporisation
    while (!fin && v==0) //tant qu'on est dans la boucle intérative
    {
        blit(Menu,ecran,0,0,0,0,1024,768); //on affiche le menu qui est l'écran principale

        if (mouse_x>=50 && mouse_x<=415 && mouse_y>=40 && mouse_y<=200) //si la souris se trouvent dans cette zone
        {
            blit(Graphe,ecran,0,0,0,0,1024,768); //on affiche la bitmap des infos
            if(mouse_b&1)
            {
                 ///Chargement de chaque bitmap
            BITMAP* Graphe2 = load_bitmap("Graphe2.bmp", NULL);
            BITMAP* Graphe21 = load_bitmap("Graphe21.bmp", NULL);
            BITMAP* Graphe22 = load_bitmap("Graphe22.bmp", NULL);
            /*BITMAP* Choice = load_bitmap("Choice.bmp", NULL);*/
            choix(ecran,Graphe2,Graphe21,Graphe22);
                v = 1 ;
            }
        }

        if (mouse_x>=50 && mouse_x<=415 && mouse_y>=220 && mouse_y<=380)
        {
            blit(Reseau,ecran,0,0,0,0,1024,768); //on affiche la bitmap des réseaux
            if (mouse_b&1) //si on clique sur le bouton gauche
            {
                BITMAP* Reseau2=load_bitmap("Reseau2.bmp",NULL); //Chargement bitmap pour affichage des informations
                blit(Reseau2,ecran,0,0,0,0,1024,728); //Affichage du bitmap explicatif de nos réseaux
                v = 2;
            }
        }

        if (mouse_x>=50 && mouse_x<=415 && mouse_y>=400 && mouse_y<=560)
        {
            blit(Quitter,ecran,0,0,0,0,1024,768); //on affiche la bitmap pour quitter
            if(mouse_b&1)
            {
                BITMAP* Quitter12=load_bitmap("Quitter12.bmp",NULL); //Chargement bitmap pour affichage des informations
                blit(Quitter12,ecran,0,0,0,0,1024,768); //Affichage du bitmap explicatif de nos réseaux
                v = 3;
            }
        }

        blit(ecran,screen,0,0,0,0,1024,768);
        //clear(ecran);

        if(v==1)
        {
            /*///Chargement de chaque bitmap
            BITMAP* Graphe2 = load_bitmap("Graphe2.bmp", NULL);
            BITMAP* Graphe21 = load_bitmap("Graphe21.bmp", NULL);
            BITMAP* Graphe22 = load_bitmap("Graphe22.bmp", NULL);
            /*BITMAP* Choice = load_bitmap("Choice.bmp", NULL);
            choix(ecran,Graphe2,Graphe21,Graphe22);*/
        }

        if(v==2)
        {
            rest(2000); //temporisation pour pouvoir lire les infos explicatifs des réseaux
            v=0; //remettre v à zéro nous permet de rester dans le boucle
        }

        if (v==3)
        {
            rest(2000); //temporisation avant de sotir du programme
            allegro_exit(); //sortie du programme
            exit(EXIT_FAILURE);
        }

        if (mouse_b&2) //On sort de la boucle grace au clique droit
            fin = 1 ;
    }
}

void choix (BITMAP* ecran, BITMAP* Graphe2,BITMAP* Graphe21, BITMAP* Graphe22) //sous-p du choix du réseau à afficher BITMAP* Choice,
{
    int choice;
    int v = 0;
    bool fin =true  ; //booléen pour rester dans la boucle
    while (fin) //tant qu'on est dans la boucle
    {
        blit(Graphe2,ecran,0,0,0,0,1024,768); //on affiche le bitmap de choix du réseau

        if (mouse_x>=200 && mouse_x<=300 && mouse_y>=270 && mouse_y<=370) //en fonction de la position de la souris
            {
                blit(Graphe21,ecran,0,0,0,0,1024,768); //on affiche le bitmap pour le cycle de l'eau
                if(mouse_b&1)
                {
                    v=1;
                    choice=1;
                }
            }

        if (mouse_x>=720 && mouse_x<=820 && mouse_y>=270&& mouse_y<=370)
            {
                blit(Graphe22,ecran,0,0,0,0,1024,768);  //on affiche le bitmap pour le réseau de la savane
                if(mouse_b&1)
                {
                    v=1;
                    choice=2;
                }
            }

        if(key[KEY_SPACE])
        {
            v=2;
        }


        /*if (mouse_x>=242 && mouse_x<=559 && mouse_y>=386 && mouse_y<=485)
        {
            blit(Foret,ecran,0,0,0,0,800,600);  //on affiche le bitmap pour les chaines alimentaires
        }*/

        blit(ecran,screen,0,0,0,0,1024,768);

        if (v==1)
        {
            Selectiongraph(choice);
            v=0;
        }

        if(v==2)
        {
            BITMAP* ecran=create_bitmap(1600,1200); //Bitmap d'affichage
            BITMAP* Menu=load_bitmap("Nemo.bmp",NULL); //Bitmaps du menu
            BITMAP* Graphe=load_bitmap("Graphe.bmp",NULL); //Bitmap d'accès aux informations explicatives
            BITMAP* Reseau=load_bitmap("Reseau.bmp",NULL); //Bitmap pour accéder à nos trois réseaux
            BITMAP* Quitter=load_bitmap("Quitter1.bmp",NULL); //Bitmap pour quitter
            menu(ecran,Menu,Graphe,Reseau,Quitter,fin);

        }
        ///clear(ecran);
    }
}

void Selectiongraph(int choice)
{
    std::string milieu;
    Graph g;

    if(choice==1)
    {
        milieu = "Fish.txt";
    }

    if(choice==2)
    {
        milieu = "Mer.txt";
    }


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    g.make_example(milieu);
    while ( !key[KEY_ESC] )
    {
    /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();



        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        g.sauvgarder(milieu);
    }
}

