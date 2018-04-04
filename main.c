#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>


#include "Map.h"
#include "StructuresJoueur.h"
#include "StructuresEnnemis.h"


#define NBMENU 5

BITMAP *load_bitmap_check(char *nomImage);
void deplacement(int destx, int desty, int *x, int *y);
void MenuPause(BITMAP *REGLES[NBMENU]);
void chargerImgMenu(BITMAP *REGLES[NBMENU]);
void chargerImgMap(BITMAP **map, BITMAP **minimap, BITMAP **mapcolli, int Nmap);
void MenuPrincipal(int *jeu, int *map, BITMAP *REGLES[NBMENU], BITMAP *Menu, int *difficulte);


// Chargement "sécurisé" d'une image :
// interrompt le programme avec un message si problème...
BITMAP *load_bitmap_check(char *nomImage){
    BITMAP *bmp;
    bmp=load_bitmap(nomImage,NULL);
    if (!bmp)
    {
        allegro_message("pas pu trouver %s",nomImage);
        exit(EXIT_FAILURE);
    }
    return bmp;
}

void MenuPause(BITMAP *REGLES[NBMENU])
{
    int a = 0;
    while (!key[KEY_F11])
    {
        if (key[KEY_RIGHT] && a != NBMENU)    a++;
        if (key[KEY_LEFT] && a != 0)   a--;
        if (key[KEY_F10] || a < 0)   a = 0;
        if (a == NBMENU) a = NBMENU-1;
        rest(100);

        masked_blit(REGLES[a], screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
}

void chargerImgMenu(BITMAP *REGLES[NBMENU])
{
    int i;
    char img[50];

    for (i=0;i<NBMENU;i++)
    {
        sprintf(img,"images/menu%d.bmp",i);
        REGLES[i] = load_bitmap_check(img);
    }
}


void chargerImgMap(BITMAP **map, BITMAP **minimap, BITMAP **mapcolli, int Nmap)
{
    char img[100];

    sprintf(img,"images/map%d.bmp",Nmap);
    *map = load_bitmap_check(img);
    sprintf(img,"images/minimap%d.bmp",Nmap);
    *minimap = load_bitmap_check(img);
    sprintf(img,"images/map%dcolli.bmp",Nmap);
    *mapcolli = load_bitmap_check(img);

}

void MenuPrincipal(int *jeu, int *map, BITMAP *REGLES[NBMENU], BITMAP *Menu, int *difficulte)
{
    int menu = 1;
    BITMAP *page;
    page = create_bitmap(SCREEN_W, SCREEN_H);
    while (menu == 1)
    {
        clear_bitmap(page);
        blit(Menu, page, 0,0,0,0, SCREEN_W, SCREEN_H);
        textprintf_ex(page,font,30,500,makecol(255,255,255),-1,"Difficulte : %d sec", *difficulte);

        if (mouse_b&1 && getpixel(Menu, mouse_x, mouse_y) == makecol(255,0,0))  *difficulte = 480;
        if (mouse_b&1 && getpixel(Menu, mouse_x, mouse_y) == makecol(0,255,0))  *difficulte = 900;
        if (mouse_b&1 && getpixel(Menu, mouse_x, mouse_y) == makecol(255,128,0))  *difficulte = 600;
        if (mouse_b&1 && getpixel(Menu, mouse_x, mouse_y) == makecol(0,0,255))  *difficulte = 10;

        //Bouton "Quitter"
        if (mouse_b&1 && mouse_x < 75 && mouse_y < 42)
        {
            *jeu = 0;
            menu = 0;
        }
        //Charger map 1
        if (mouse_b&1 && mouse_x > 109 && mouse_x < 293 && mouse_y > 214 && mouse_y < 323)
        {
            *map = 1;
            *jeu = 1;
            menu = 0;
        }
        //Charger map 2
        if (mouse_b&1 && mouse_x > 490 && mouse_x < 675 && mouse_y > 214 && mouse_y < 323)
        {
            *map = 2;
            *jeu = 1;
            menu = 0;
        }
        //Charger map 3
        if (mouse_b&1 && mouse_x > 300 && mouse_x < 485 && mouse_y > 356 && mouse_y < 465)
        {
            *map = 3;
            *jeu = 1;
            menu = 0;
        }

        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    }
}

int main()
{
    srand(time(NULL));

    // Lancer allegro et le mode graphique
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    // Montrer la souris à l'écran
    show_mouse(screen);

    ///BITMAP
    BITMAP *page;//bitmap buffer d'affichage
    BITMAP *map;
    BITMAP *HUD;
    BITMAP *minimap;
    BITMAP *minimapBuff;
    BITMAP *caserne;
    BITMAP *base;
    BITMAP *mapcolli;
    BITMAP *REGLES[NBMENU];
    BITMAP *Menu;
    BITMAP *victoire;
    BITMAP *defaite;

    // CREATION DU BUFFER D'AFFICHAGE à la taille de l'écran
    page = create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    // Buffer de la minimap(radar)
    minimapBuff = create_bitmap(RadarW, RadarH);
    clear_bitmap(minimapBuff);

    //Chargement des bitmaps (avec verification du chargement a chaque fois dans le sous programme)
    HUD = load_bitmap_check("images/hudpetit.bmp");
    caserne = load_bitmap_check("images/caserne.bmp");
    base = load_bitmap_check("images/base.bmp");
    Menu = load_bitmap_check("images/accueil.bmp");
    victoire = load_bitmap_check("images/victoire.bmp");
    defaite = load_bitmap_check("images/defaite.bmp");
    chargerImgMenu(REGLES);

    ///SAMPLE et MIDI
    MIDI *zelda;
    MIDI *test;
    SAMPLE *youwin;
    SAMPLE *youlose;

    zelda = load_midi("son/z1_exter2.mid");
    test = load_midi("son/z4_fin.mid");
    youwin = load_wav("son/youwin.wav");
    youlose = load_wav("son/youlose.wav");

    int Nmap = 1;
    int jeu = 1;
    int difficulte = 600;


    while (jeu != 0)
    {
        play_midi(zelda, TRUE);
        time_t temps = 0;
        time_t reset = 0;

        t_listeJ *joueurs = creerListejoueur();
        t_listeE *ennemis = creerListeEnnemi();

        //Variables Selection
        int a = 0;
        int xs,ys;
        int xs1,ys1;
        xs = xs1 = ys = ys1 = 0;

        //Variables Ressources
        float ressource1 = 7500;
        float ressource2 = 12250;

        //Variable Batiment
        int b = 0;
        int xb = 0;
        int yb = 0;
        int b1 = 0;
        int xb1 = 0;
        int yb1 = 0;
        int EnnemiBat = 30000;

        //Variable Scrolling
        int screenx = 0;
        int screeny = 0;

        //Variable Scrolling Minimap
        int radarx = 0;
        int radary = 0;

        //int gameover = 0;

        MenuPrincipal(&jeu, &Nmap, REGLES, Menu, &difficulte);
        chargerImgMap(&map, &minimap, &mapcolli, Nmap);
        placerEnnemis(ennemis, mapcolli);
        reset = clock();

        // Boucle de jeu
        while (!key[KEY_ESC] && jeu == 1)
        {
            // EFFACER LES BUFFER
            clear_bitmap(page);
            clear_bitmap(minimapBuff);

            scrolling(map, &screenx, &screeny);


            /// Affichage de la map sur le buffer
            blit(map,page,screenx-TailleHUD, screeny, 0,0, SCREEN_W, SCREEN_H);
            //blit(mapcoli,page,screenx-TailleHUD, screeny, 0,0, SCREEN_W, SCREEN_H);

            ///Ressources

            recolte(joueurs, mapcolli, &ressource1, &ressource2);

            ///Pose batiment

            if (key[KEY_B] && b == 0)
            {
                b = 1;
            }

            if (b == 1)
            {
                masked_blit(caserne, page, 0, 0, mouse_x, mouse_y, caserne->w, caserne->h);

                if (mouse_b&1 && ressource2 >= 1750 && placeOk(mouse_x, mouse_y, mapcolli, 0))
                {
                    xb = mouse_x+screenx;
                    yb = mouse_y+screeny;
                    ressource2 = ressource2 - 1750;
                    b = 0;
                }
                if (mouse_b&2)
                {
                    b = 0;
                }
            }
            if (key[KEY_N] && b1 == 0)
            {
                b1 = 1;
            }

            if (b1 == 1)
            {
                masked_blit(base, page, 0, 0, mouse_x, mouse_y, base->w, base->h);

                if (mouse_b&1 && ressource2 >= 1000 && placeOk(mouse_x, mouse_y, mapcolli, 0))
                {
                    xb1 = mouse_x+screenx;
                    yb1 = mouse_y+screeny;
                    ressource2 = ressource2 - 1000;
                    b1 = 0;
                }
                if (mouse_b&2)
                {
                    b1 = 0;
                }
            }

            ///Gestion Batiment

            if (xb != 0 && yb != 0)
            {
                //Affichage de la caserne
                masked_blit(caserne, page, 0, 0, xb-screenx, yb-screeny, caserne->w, caserne->h);

                //Messages d'erreur si pas assez de ressources, pas de batiment etc
                if (ressource1 < 100 && key[KEY_1_PAD])    textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Pas assez de ressources !");
                if (joueurs->utilise == joueurs->nbmax && key[KEY_1_PAD])    textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Vous avez deja le maximum d'unités !");

                //Generation des joueurs et enlevement des ressources utilisé a la creation
                if (ressource1 >= 100 && joueurs->utilise != joueurs->nbmax && key[KEY_1_PAD])
                {
                    ajouterjoueur(joueurs, (xb+caserne->w/2), (yb+caserne->h), 1);
                    ressource1 = ressource1-100;
                }
            }
            if (xb1 != 0 && yb1 != 0)
            {
                //Affichage de la caserne
                masked_blit(base, page, 0, 0, xb1-screenx, yb1-screeny, caserne->w, caserne->h);

                //Messages d'erreur si pas assez de ressources, pas de batiment etc
                if (ressource1 < 25 && ressource2 < 75 && key[KEY_2_PAD])    textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Pas assez de ressources !");
                if (joueurs->utilise == joueurs->nbmax && key[KEY_2_PAD])    textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Vous avez deja le maximum d'unités !");

                //Generation des joueurs et enlevement des ressources utilisé a la creation
                if (ressource1 >=25 && ressource2 >= 75 && joueurs->utilise !=joueurs->nbmax && key[KEY_2_PAD])
                {
                    ajouterjoueur(joueurs, (xb1+caserne->w/2), (yb1+caserne->h), 0);
                    ressource1 = ressource1-25;
                    ressource2 = ressource2-75;
                }
            }
            if (xb == 0 && yb == 0 && key[KEY_1_PAD])   textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Vous n'avez pas de Caserne !");
            if (xb1 == 0 && yb1 == 0 && key[KEY_2_PAD])   textprintf_centre_ex(page,font,(SCREEN_W+TailleHUD)/2,SCREEN_H/2,makecol(255,0,0),-1,"Vous n'avez pas de Base !");

            ///Selection
            //Entree mode selection d'unité
            if (mouse_b&1 && a == 0)
            {
                //On stock les coordonnées du coin en haut a gauche du rectangle de selection
                xs = mouse_x;
                ys = mouse_y;
                //Variable qui permet d'eviter l'utilisation d'un while pour faire la selection d'unité
                //(Une boucle while au milieu de la boucle de jeu mettrais en pause le jeu donc on evite)
                a = 1;
            }

            //Sortie mode selection d'unité
            if (mouse_b&1 && a == 1)
            {
                //On stocke les coordonnées du coin en bas a droite du rectangle de selection
                //Et on affiche le rectangle de selection a l'utilisateur
                xs1 = mouse_x;
                ys1 = mouse_y;
                rect(page,xs,ys,xs1, ys1,makecol(0,255,0));
            }

            //Reinitialisation du mode selection d'unité si le clic gauche est laché et verification de chaque perso
            if (!(mouse_b&1) && a == 1)
            {
                selectionJoueurs(joueurs, xs+screenx, ys+screeny, xs1+screenx, ys1+screeny);
                a = 0;
            }

            selectionParClasse(joueurs);

            ///Deplacement de la selection
            //Deplacement des objets selectionnnées vers l'emplacement du clic droit
            if (mouse_b&2)
            {

                verifSelection(joueurs, screenx, screeny);
                //On remet le rectangle de selection en (0;0) apres avoir effectuer le deplacement
                xs=xs1=ys=ys1=0;
            }

            detection_obstacle(joueurs, mapcolli);
            deplacementJoueurs(joueurs);
            detectionEnnemis(ennemis, joueurs);

            combat(ennemis, joueurs, page);
            BatEnnemisAtq(joueurs, mapcolli, &EnnemiBat);

            ///Affichage Unitées
            actualiserEnnemis(ennemis, page, screenx, screeny);
            actualiserAffichage(joueurs, page, screenx, screeny);

            ///HUD
            draw_sprite(page, HUD, 0,0);
            textprintf_ex(page,font,28,186,makecol(255,255,255),-1,"Pokeball : %.0f", ressource1);
            textprintf_ex(page,font,28,201,makecol(255,255,255),-1,"Roche : %.0f", ressource2);
            textprintf_ex(page,font,28,216,makecol(255,255,255),-1,"Vos unites : %d", joueurs->utilise);
            textprintf_ex(page,font,28,231,makecol(255,255,255),-1,"Unites ennemies : %d", ennemis->utilise);
            textprintf_ex(page,font,28,246,makecol(255,255,255),-1,"Vie du batiment ennemi :");
            textprintf_ex(page,font,28,261,makecol(255,255,255),-1,"%d", EnnemiBat);
            textprintf_ex(page,font,28,276,makecol(255,255,255),-1,"Temps : %.0f sec", (double) (temps-reset)/CLOCKS_PER_SEC );



            ///Minimap
            blit(minimap, minimapBuff, 0,0,0,0, minimapBuff->w, minimapBuff->h);

            radarx = screenx*0.097;
            radary = screeny*0.1;

            //point d'unité sur la minimap
            pixelMinimap(joueurs, minimapBuff);
            pixelEnnemis(ennemis, minimapBuff);

            //rectangle de vision sur la minimap
            rect(minimapBuff, radarx, radary, ((SCREEN_W-TailleHUD)/10.3) + radarx, (SCREEN_H/10.3) + radary, makecol(0,255,0));
            blit(minimapBuff, page, 0,0,18,25, minimapBuff->w, minimapBuff->h);

            ///Detection de victoire
            while (EnnemiBat <= 0 && ennemis->utilise == 0 && !mouse_b&1)
            {
                stop_midi();
                play_sample(youwin, 255, 128, 1000, 0);
                masked_blit(victoire, page, 0,0,0,0, SCREEN_W, SCREEN_H);
                blit(page,screen,0,0,0,0,SCREEN_W, SCREEN_H);
                if (mouse_b&1)  jeu = 2;
            }

            if (EnnemiBat > 0 && ennemis->utilise != 0 && (temps-reset)/CLOCKS_PER_SEC == difficulte && !mouse_b&1)
            {
                stop_midi();
                play_sample(youlose, 255, 128, 1000, 0);
                rest(30);
                //play_midi(test, TRUE);
            }
            while (EnnemiBat > 0 && ennemis->utilise != 0 && (temps-reset)/CLOCKS_PER_SEC == difficulte && !mouse_b&1)
            {
                masked_blit(defaite, page, 0,0,0,0, SCREEN_W, SCREEN_H);
                blit(page,screen,0,0,0,0,SCREEN_W, SCREEN_H);
                if (mouse_b&1)  jeu = 2;
            }

            ///Affichage du buffer sur l'ecran
            blit(page,screen,0,0,0,0,SCREEN_W, SCREEN_H);

            /*int i;
            for(i=0; i<EMAX; i++)
            {
                if (ennemis->tab[i] != NULL)
                {
                    ennemis->tab[i]->vie = ennemis->tab[i]->vie - 0.4;
                    printf("%d : %.0f vie\n", i, ennemis->tab[i]->vie);
                }
            }*/

            if (key[KEY_F10])   MenuPause(REGLES);
            temps = clock();

            rest(20);
         }
    }
    stop_midi();
    return 0;
}END_OF_MAIN();
