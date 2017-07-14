#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "enum.h"

using namespace std;
void initialiser(int**&);
void erreur(string a);
void blitter(int** vCase,SDL_Surface*& ecran, SDL_Surface**& pieces);
void swapr(int &a, int &b);
void positionner(Uint16 x,Uint16 y,int& _i,int& _j);
int processMove(int**&vCase,int& _i,int& _j,int& __i,int& __j,int& coups);
void jouer(int**& vCase,SDL_Surface *ecran,SDL_Surface* table,SDL_Surface**& pieces);
void annuler(int**&vCase,int iDepart,int jDepart,int iArrivee,int jArrivee,int& coups,int ancien,int siPion);
void sauvegarder(int**& vCase);
void charger(int**& vCase);
bool tourExact(int**& vCase,int _i,int _j,int& coups, int& ok);

#endif // FONCTIONS_H_INCLUDED
