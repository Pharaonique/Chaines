#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>

#define TailleHUD 222
//Notre minimap est 10.3 plus petite que la map totale
//1900/10.3 = 185 et 1120/10.3 = 109
#define RadarW 185
#define RadarH 109

///Prototypes

//Scrolling de la map au clavier ou a la souris
void scrolling(BITMAP *map, int *screenx, int *screeny);
