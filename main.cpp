#include "enum.h"
using namespace std;

int main (int argc, char* argv[]){
/// Donnees
    SDL_Surface *ecran=NULL,*table=NULL;
    int** vCase(0),i(0),j(0);
/// Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO)==-1){
        fprintf(stderr,"Erreur d'initialisation de la SDL: %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

/// Initialisation des pieces
    SDL_Surface **pieces(0);
    pieces=new (nothrow) SDL_Surface* [12];
    pieces[RB]=IMG_Load("roiBlanc.png");
    pieces[RN]=IMG_Load("roiNoir.png");
    pieces[DB]=IMG_Load("dameBlanc.png");
    pieces[DN]=IMG_Load("dameNoir.png");
    pieces[TB]=IMG_Load("tourBlanc.png");
    pieces[TN]=IMG_Load("tourNoir.png");
    pieces[FB]=IMG_Load("fouBlanc.png");
    pieces[FN]=IMG_Load("fouNoir.png");
    pieces[CB]=IMG_Load("cvlBlanc.png");
    pieces[CN]=IMG_Load("cvlNoir.png");
    pieces[PB]=IMG_Load("pionBlanc.png");
    pieces[PN]=IMG_Load("pionNoir.png");

/// Initialisation de l'ecran et de l'echiquier
    SDL_Rect positionTable;
    positionTable.x=0;
    positionTable.y=0;

    // Ecran principal de la SDL
    ecran=SDL_SetVideoMode(1024,1024,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Chess game",NULL);

    // Pour l'echiquier
    table=IMG_Load("chessboardBig.png");
    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,136,136));
    SDL_BlitSurface(table,NULL,ecran,&positionTable);

/// Initialisation des pieces sur l'ecran

    initialiser(vCase);
    blitter(vCase,ecran,pieces);
    SDL_Flip(ecran);
    jouer(vCase,ecran,table,pieces);
    SDL_FreeSurface(table);
    SDL_Quit();

    return EXIT_SUCCESS;
}
