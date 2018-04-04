///*****************************///
///          A FINIR           ///
///*****************************///

#define EMAX 100
#define XBAT  98
#define YBAT  88

///Structures

typedef struct ennemi {

    int posx;
    int posy;
    int destx;
    int desty;
    int actif;

    int visible;
    int rayonDetection;

    float vie;
    int atq;

    int rayonAtq; // rayon d'aggresion de l'ennemi

    BITMAP *img;
}t_ennemi;

typedef struct listeE{

    int nbmax;
    int utilise;
    t_ennemi **tab;

}t_listeE;

///Prototypes

//Ajoute un ennemi a liste aux coordoonées x,y
t_ennemi * ajouterEnnemi(t_listeE *liste, int x, int y);
//Creer et initialise une liste d'ennemis
t_listeE * creerListeEnnemi();
//Creer et initialise un ennemi
t_ennemi * creerEnnemi(int x, int y);
//Supprime un ennemi de la liste quand il n'a plus de vie et le fait disparaitre de l'ecran
void supprEnnemi(t_listeE *ennemis, int i);
//Actualise l'affichage de chaque ennemis sur la map ainsi que leur vie au dessus de leur tete
void actualiserEnnemis(t_listeE *ennemis, BITMAP *page, int screenx, int screeny);
//Affiche un pixel de couleur rouge sur la Minimap en fonction de leur position actuelle sur la Map si ils ont été révélé par un joueur
void pixelEnnemis(t_listeE *ennemis, BITMAP *minimapBuff);
//Revele un ennemi si un joueur se trouve dans un certain rayon autour de lui
void detectionEnnemis(t_listeE *ennemis, t_listeJ *joueurs);
//Renvoi le carré du nombre x recu
int carre(int x);
//Verifie que la place prise par un perso est libre au coordonnées x,y recu par le sous programme
int placeOk(int x, int y, BITMAP *mapcol, int type);
//Place tout les ennemis dans les 3/4 droit de la map si la place est libre
void placerEnnemis(t_listeE *ennemis, BITMAP *mapcol);
//Renvoi un nombre aléatoire entre a et b
int randBorne(int a, int b);

void combat(t_listeE *ennemis, t_listeJ *joueurs, BITMAP *page);

///Sous programmes

t_ennemi * ajouterEnnemi(t_listeE *liste, int x, int y)
{
    int i;
    t_ennemi *ennemi;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (liste->utilise >= liste->nbmax) return NULL;

    // Allouer un acteur initialisé
    ennemi = creerEnnemi(x,y);

    // Chercher un emplacement libre
    i = 0;

    while (liste->tab[i]!= NULL && i<liste->nbmax)  i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i < liste->nbmax){
        // Accrocher le acteur à l'emplacement trouvé
        liste->tab[i] = ennemi;
        liste->utilise++;
    }

    return ennemi;
}

t_listeE * creerListeEnnemi()
{
    t_listeE *liste;
    int i;

    liste=(t_listeE *)malloc(1*sizeof(t_listeE));

    liste->nbmax = EMAX;
    liste->utilise=0;
    liste->tab=(t_ennemi **)malloc(EMAX*sizeof(t_ennemi*));

    for (i=0;i<EMAX;i++)
        liste->tab[i]=NULL;

    return liste;
}

t_ennemi * creerEnnemi(int x, int y)
{
    t_ennemi *unit;
    int a = 0;

    unit=(t_ennemi *)malloc(1*sizeof(t_ennemi));

    unit->actif = 1;

    unit->posx = x;
    unit->posy = y;
    unit->destx = 0;
    unit->desty = 0;

    unit->rayonAtq = XPERSO/2+YPERSO/2;

    unit->rayonDetection = 50;
    unit->visible = 0;

    unit->img = load_bitmap("images/kirby.bmp", NULL);

    a = rand()%100;
    if (a == 4 || a == 13 || a == 14)
    {
        unit->vie = 500;
        unit->atq = 7;
    }
    else
    {
        unit->vie = 300;
        unit->atq = 2;
    }
    return unit;
}

void supprEnnemi(t_listeE *ennemis, int i)
{
    ennemis->tab[i]->actif = 0;
    ennemis->tab[i] = NULL;
    ennemis->utilise--;
}

void actualiserEnnemis(t_listeE *ennemis, BITMAP *page, int screenx, int screeny)
{
    int i;
    int couleur = makecol(255,255,255);

    for(i=0; i<EMAX; i++)
    {
        if (ennemis->tab[i] != NULL)
        {
            if (ennemis->tab[i]->actif == 1 && ennemis->tab[i]->vie != 0)
            {
                masked_blit(ennemis->tab[i]->img,page,0,0,(ennemis->tab[i]->posx)-screenx,(ennemis->tab[i]->posy)-screeny, XPERSO, YPERSO);
                if (ennemis->tab[i]->vie > 300)
                {
                    couleur = makecol(255,0,0);
                }
                else
                {
                    couleur = makecol(255,255,255);
                }
                textprintf_centre_ex(page, font, ennemis->tab[i]->posx+XPERSO/2-screenx,ennemis->tab[i]->posy-screeny, couleur,-1,"%.0f", ennemis->tab[i]->vie);
            }
            if (ennemis->tab[i]->vie <= 0)  supprEnnemi(ennemis, i);
        }

    }
}

void pixelEnnemis(t_listeE *ennemis, BITMAP *minimapBuff)
{
    int i;
    for (i=0; i<EMAX; i++)
    {
        if (ennemis->tab[i] != NULL)
        {
           if (ennemis->tab[i]->actif == 1 && ennemis->tab[i]->visible == 1) putpixel(minimapBuff, (ennemis->tab[i]->posx - TailleHUD)/10.3, ennemis->tab[i]->posy/10.3, makecol(255,0,127));
        }
    }
}

void detectionEnnemis(t_listeE *ennemis, t_listeJ *joueurs)
{
    int i,j;
    int a,c,b;
    for (i=0; i<EMAX; i++)
    {
        if (ennemis->tab[i] != NULL)
        {
            for(j=0; j<JMAX; j++)
            {
                if (joueurs->tab[j] != NULL)
                {
                    a = (joueurs->tab[j]->posx) - (ennemis->tab[i]->posx);
                    b = (joueurs->tab[j]->posy) - (ennemis->tab[i]->posy);
                    c = ennemis->tab[i]->rayonDetection;
                    if (carre(a) + carre(b) <= carre(c))
                    {
                        ennemis->tab[i]->visible = 1;
                    }
                }
            }
        }
    }
}

int carre(int x)
{
    return x*x;
}

int placeOk(int x, int y, BITMAP *mapcol, int type)
{
    int i,j;
    int a = 0;
    int tailleX = 0;
    int tailleY = 0;

    if (type == 1)
    {
        tailleX = XPERSO;
        tailleY = YPERSO;
    }
    else if (type == 0)
    {
        tailleX = XBAT;
        tailleY = YBAT;
    }

    for (i=0; i<tailleY; i++)
    {
        for (j=0; j<tailleX; j++)
        {
            if (getpixel(mapcol, x+j, y+i) != makecol(0,0,0))   a++;
        }
    }

    if (a == tailleX*tailleY) return 1;
    return 0;
}

int randBorne(int a, int b)
{
    return rand()%(b-a) + a;
}

void placerEnnemis(t_listeE *ennemis, BITMAP *mapcol)
{
    int x,y;
    while (ennemis->utilise != ennemis->nbmax)
    {
        x = randBorne(mapcol->w/3, mapcol->w-XPERSO);
        y = rand()%(mapcol->h-YPERSO);
        if (placeOk(x, y, mapcol, 1) == 1)
        {
            ajouterEnnemi(ennemis, x, y);
        }
    }
}

void combat(t_listeE *ennemis, t_listeJ *joueurs, BITMAP *page)
{
    int i,j;
    int a,c,b;
    for (i=0; i<EMAX; i++)
    {
        if (ennemis->tab[i] != NULL)
        {
            for(j=0; j<JMAX; j++)
            {
                if (joueurs->tab[j] != NULL)
                {
                    a = (joueurs->tab[j]->posx) - (ennemis->tab[i]->posx);
                    b = (joueurs->tab[j]->posy) - (ennemis->tab[i]->posy);
                    c = ennemis->tab[i]->rayonAtq;
                    if (carre(a) + carre(b) <= carre(c))
                    {
                        textprintf_ex(page,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,255),-1,"COMBAT !");
                        ennemis->tab[i]->vie = ennemis->tab[i]->vie - joueurs->tab[j]->atq;
                        joueurs->tab[j]->vie = joueurs->tab[j]->vie - ennemis->tab[i]->atq;
                    }
                }
            }
        }
    }
}
