#define JMAX 100
#define XPERSO 35
#define YPERSO 38

///Structures

typedef struct joueur {

    int posx;
    int posy;
    int destx;
    int desty;
    int mouvement;
    int actif;

    float vie;
    int atq;

    int recolte;

    BITMAP *img;
}t_joueur;

typedef struct listeJ{

    int nbmax;
    int utilise;
    t_joueur **tab;

}t_listeJ;

///Prototypes

//Ajoute un joueur a liste en fonction de sa classe aux coordoonées x,y
t_joueur * ajouterjoueur(t_listeJ *liste, int x, int y, int classe);
//Creer et initialise la liste de joueurs
t_listeJ * creerListejoueur();
//Creer et initialise un joueur
t_joueur * creerjoueur();
//Supprime un joueur de la liste quand il n'a plus de vie et le fait disparaitre de l'ecran
void supprJoueur(t_listeJ *joueurs, int i);
//Actualise l'affichage de chaque joueurs sur la map ainsi que leur vie au dessus de leur tete
void ActualiserAffichage(t_listeJ *joueurs, BITMAP *page, int screenx, int screeny);
//Verifie la selection de chaque joueur actif et leur donne une destination
void verifSelection(t_listeJ *joueurs, int screenx, int screeny);
//Deplace les joueurs pixel par pixel en direction de leur destination tant que celle ci est differente de leur position actuelle
void deplacementJoueurs(t_listeJ *joueurs);
//Selectionne les joueurs actif se trouvant dans le rectangle de selection et active leur "mouvement" (selection) si c'est le cas
void selectionJoueurs(t_listeJ *joueurs,int x1, int y1, int x2, int y2);
//Determine une "formation" de marche et de positionnement a l'arrivé et desactive leur selection (ils n'en ont plus besoin, ils ont deja leur destination)
void depFormation(t_listeJ *joueurs);
//Affiche un pixel de couleur blanche sur la Minimap en fonction de leur position actuelle sur la Map
void pixelMinimap(t_listeJ *joueurs, BITMAP *minimapBuff);
//Selection toute les unités d'un type identique (soldat ou ouvrier)
void selectionParClasse(t_listeJ *joueurs);
//Detecte les collisions avec les obstacles ou les bord de la map
void detection_obstacle(t_listeJ *joueurs, BITMAP *mapcoli);
//Recolte des ressources si il y a un ouvrier dans la bonne zone
void recolte(t_listeJ *joueurs, BITMAP *collisions, float *r1, float *r2);
//Gere la vie du batiment ennemi
void BatEnnemisAtq(t_listeJ *joueurs, BITMAP *collisions, int *BatVie);

///Sous programmes

t_joueur * ajouterjoueur(t_listeJ *liste, int x, int y, int classe)
{
    int i;
    t_joueur *joueur;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (liste->utilise >= liste->nbmax) return NULL;

    // Allouer un acteur initialisé
    joueur = creerjoueur(x,y, classe);

    // Chercher un emplacement libre
    i = 0;

    while (liste->tab[i]!= NULL && i<liste->nbmax)  i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i < liste->nbmax){
        // Accrocher le acteur à l'emplacement trouvé
        liste->tab[i] = joueur;
        liste->utilise++;
    }

    return joueur;
}

t_listeJ * creerListejoueur()
{
    t_listeJ *liste;
    int i;

    liste=(t_listeJ *)malloc(1*sizeof(t_listeJ));

    liste->nbmax = JMAX;
    liste->utilise=0;
    liste->tab=(t_joueur **)malloc(JMAX*sizeof(t_joueur*));

    for (i=0;i<JMAX;i++)
        liste->tab[i]=NULL;

    return liste;
}

t_joueur * creerjoueur(int x, int y, int classe)
{
    t_joueur *unit;

    unit=(t_joueur *)malloc(1*sizeof(t_joueur));

    unit->actif = 1;
    unit->posx = x;
    unit->posy = y;
    unit->destx = 0;
    unit->desty = 0;
    unit->mouvement = 0;
    if (classe == 1)
    {
        unit->vie = 200;
        unit->atq = 3;
        unit->recolte = 0;
        unit->img = load_bitmap("images/junko.bmp", NULL);
    }
    else
    {
        unit->vie = 80;
        unit->atq = 1;
        unit->recolte = 1;
        unit->img = load_bitmap("images/chen.bmp", NULL);
    }

    return unit;
}

void supprJoueur(t_listeJ *joueurs, int i)
{
    joueurs->tab[i]->actif = 0;
    joueurs->tab[i] = NULL;
    joueurs->utilise--;
}

void actualiserAffichage(t_listeJ *joueurs, BITMAP *page, int screenx, int screeny)
{
    int i;
    int couleur = makecol(0,255,0);

    for(i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL)
        {
            if (joueurs->tab[i]->actif == 1 && joueurs->tab[i]->vie != 0)
            {
                masked_blit(joueurs->tab[i]->img,page,0,0,(joueurs->tab[i]->posx)-screenx,(joueurs->tab[i]->posy)-screeny, XPERSO, YPERSO);
                textprintf_centre_ex(page, font, joueurs->tab[i]->posx+XPERSO/2-screenx,joueurs->tab[i]->posy-screeny, couleur,-1,"%.0f", joueurs->tab[i]->vie);
            }
            if (joueurs->tab[i]->vie <= 0)  supprJoueur(joueurs, i);
        }

    }
}

void verifSelection(t_listeJ *joueurs, int screenx, int screeny)
{
    int i;
    for(i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL)
        {
            if(joueurs->tab[i]->mouvement == 1 && joueurs->tab[i]->actif == 1)
            {
                joueurs->tab[i]->destx = mouse_x + screenx;
                joueurs->tab[i]->desty = mouse_y + screeny;
                //joueurs->tab[i]->mouvement = 0;
            }
        }
    }
    depFormation(joueurs);
}

void deplacementJoueurs(t_listeJ *joueurs)
{
    int i;
    for(i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL && joueurs->tab[i]->destx != 0 && joueurs->tab[i]->desty != 0)
        {
            if (joueurs->tab[i]->posx <= joueurs->tab[i]->destx)   joueurs->tab[i]->posx = joueurs->tab[i]->posx+2;
            if (joueurs->tab[i]->posx >= joueurs->tab[i]->destx)   joueurs->tab[i]->posx = joueurs->tab[i]->posx-2;
            if (joueurs->tab[i]->posy <= joueurs->tab[i]->desty)   joueurs->tab[i]->posy = joueurs->tab[i]->posy+2;
            if (joueurs->tab[i]->posy >= joueurs->tab[i]->desty)   joueurs->tab[i]->posy = joueurs->tab[i]->posy-2;

            //deplacement(joueurs->tab[i]->destx, joueurs->tab[i]->desty, &(joueurs->tab[i]->destx), &(joueurs->tab[i]->desty));
            if (joueurs->tab[i]->posx == joueurs->tab[i]->destx && joueurs->tab[i]->posy == joueurs->tab[i]->desty)
            {
                joueurs->tab[i]->destx = 0;
                joueurs->tab[i]->desty = 0;
            }
        }
    }
}

void selectionJoueurs(t_listeJ *joueurs,int x1, int y1, int x2, int y2)
{
    int i;
    for(i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL)
        {
            if (joueurs->tab[i]->actif == 1 && joueurs->tab[i]->posx > x1 && joueurs->tab[i]->posx < x2 && joueurs->tab[i]->posy > y1 && joueurs->tab[i]->posy < y2)
            {
                joueurs->tab[i]->mouvement = 1;
            }
        }
    }
}

void depFormation(t_listeJ *joueurs)
{
    int i,j,k;

    int a=0;
    int b=0;
    int l=0;

    for (i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL && joueurs->tab[i]->mouvement == 1)    a++;
    }

    b = ceil(sqrt(a));

    for (j=0; j<b; j++)
    {
        for(k=0; k<b; k++)
        {
            while (joueurs->tab[l] != NULL && l != JMAX-1 && joueurs->tab[l]->mouvement != 1)
            {
                l++;
            }
            if (joueurs->tab[l] != NULL && joueurs->tab[l]->mouvement == 1)
            {
                joueurs->tab[l]->destx = k*XPERSO + (joueurs->tab[l]->destx) - XPERSO/2;
                joueurs->tab[l]->desty = j*YPERSO + (joueurs->tab[l]->desty) - YPERSO/2;
                joueurs->tab[l]->mouvement = 0;
            }
        }
    }
}

void pixelMinimap(t_listeJ *joueurs, BITMAP *minimapBuff)
{
    int i;
    for (i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL)
        {
           if (joueurs->tab[i]->actif == 1) putpixel(minimapBuff, (joueurs->tab[i]->posx - TailleHUD)/10.3, joueurs->tab[i]->posy/10.3, makecol(255,255,255));
        }
    }
}

void selectionParClasse(t_listeJ *joueurs)
{
    int i;
    for (i=0;i<JMAX;i++)
    {
        if (joueurs->tab[i] != NULL)
        {
            if (key[KEY_F2] && joueurs->tab[i]->recolte == 1)
            {
                joueurs->tab[i]->mouvement = 1;
            }
            if (key[KEY_F3] && joueurs->tab[i]->recolte == 0)
            {
                joueurs->tab[i]->mouvement = 1;
            }

        }
    }
}

void detection_obstacle(t_listeJ *joueurs, BITMAP *mapcoli)
{
    int i;
    int x,y;
    for(i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL && joueurs->tab[i]->destx != 0 && joueurs->tab[i]->desty != 0)
        {
            ///Collisions avec les bords de la map
            if (joueurs->tab[i]->posx+XPERSO > mapcoli->w   )   joueurs->tab[i]->posx = mapcoli->w-XPERSO;
            if (joueurs->tab[i]->posy+YPERSO > mapcoli->h)   joueurs->tab[i]->posy = mapcoli->h-YPERSO;
            if (joueurs->tab[i]->posx < TailleHUD)   joueurs->tab[i]->posx = TailleHUD;
            if (joueurs->tab[i]->posy < 0)   joueurs->tab[i]->posy = 0;

            ///Collision avec les obstacles sur la map
            for (x = 0; x < XPERSO; x++ )
            {
                //On verifie le bord haut de la hitbox du personnagposy
                if (getpixel(mapcoli, joueurs->tab[i]->posx+x, joueurs->tab[i]->posy) == makecol(0,0,0))  joueurs->tab[i]->posy++;
                //On verifie le bord bas de la hitbox du personnagposy
                if (getpixel(mapcoli, joueurs->tab[i]->posx+x, joueurs->tab[i]->posy+YPERSO) == makecol(0,0,0))  joueurs->tab[i]->posy--;
            }
            for (y = 0; y < YPERSO; y++ )
            {
                //On verifie le coté gauche de la hitbox du personnagposy
                if (getpixel(mapcoli, joueurs->tab[i]->posx, joueurs->tab[i]->posy+y) == makecol(0,0,0))  joueurs->tab[i]->posx++;
                //On verifie le coté droit de la hitbox du personnagposy
                if (getpixel(mapcoli, joueurs->tab[i]->posx+XPERSO, joueurs->tab[i]->posy+y) == makecol(0,0,0))  joueurs->tab[i]->posx--;
            }
        }
    }
}

void recolte(t_listeJ *joueurs, BITMAP *collisions, float *r1, float *r2)
{
    int i;
    for (i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL && joueurs->tab[i]->recolte == 1)
        {
            if (getpixel(collisions, joueurs->tab[i]->posx+XPERSO/2, joueurs->tab[i]->posy+YPERSO/2) == makecol(0,255,0)) *r1 = *r1 + 0.05;
            if (getpixel(collisions, joueurs->tab[i]->posx+XPERSO/2, joueurs->tab[i]->posy+YPERSO/2) == makecol(0,0,255)) *r2 = *r2 + 0.05;
        }
    }
}

void BatEnnemisAtq(t_listeJ *joueurs, BITMAP *collisions, int *BatVie)
{
    int i;
    for (i=0; i<JMAX; i++)
    {
        if (joueurs->tab[i] != NULL)
        {
            if (getpixel(collisions, joueurs->tab[i]->posx+XPERSO/2, joueurs->tab[i]->posy+YPERSO/2) == makecol(255,128,0))
            {
                *BatVie = *BatVie - joueurs->tab[i]->atq;
                if (*BatVie <= 0)   *BatVie = 0;
            }
        }
    }
}
