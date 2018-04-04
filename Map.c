#include "Map.h"
#define XBAT  98
#define YBAT  88

///Scrolling de la map au clavier et a la souris avec detection des bords de la map
void scrolling(BITMAP *map, int *screenx, int *screeny)
{
    // GESTION DU SCROLLING AU CLAVIER (Touches de direction)
    if (key[KEY_RIGHT])   *screenx+=5;
    if (key[KEY_LEFT] )   *screenx-=5;
    if (key[KEY_DOWN] )   *screeny+=5;
    if (key[KEY_UP]   )   *screeny-=5;

    // GESTION DU SCROLLING A LA SOURIS
    if (mouse_x > SCREEN_W-50)   *screenx+=5;
    if (mouse_x < TailleHUD+50)   *screenx-=5;
    if (mouse_y > SCREEN_H-50)   *screeny+=5;
    if (mouse_y < 0+50)   *screeny-=5;

    // Bloquer le scrolling si il est ammené trop loin !
    if ( *screenx < 0 ) *screenx=0;
    if ( *screenx > map->w - SCREEN_W + TailleHUD) *screenx=map->w - SCREEN_W + TailleHUD;
    if ( *screeny < 0 ) *screeny=0;
    if ( *screeny > map->h - SCREEN_H) *screeny=map->h - SCREEN_H;
}
