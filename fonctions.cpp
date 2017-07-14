#include "enum.h"
using namespace std;

void jouer(int**& vCase,SDL_Surface *ecran,SDL_Surface* table,SDL_Surface**& pieces){
    SDL_Rect positionTable,position;
    FMOD_SYSTEM* system;
    FMOD_System_Create(&system);
    positionTable.x=0;
    positionTable.y=0;
    int continuer(1),a(6),clickEnCours(0),iDepart(0),jDepart(0),iArrivee(0),jArrivee(0),valide(1),coups(0),ancien(0),siPion(0);
    while(continuer){
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    case SDLK_q:
                        continuer=0;
                        break;
                    case SDLK_UP:
                        swapr(vCase[a][7],vCase[a-1][7]);
                        a--;
                    case SDLK_BACKSPACE:
                        annuler(vCase,iDepart,jDepart,iArrivee,jArrivee,coups,ancien,siPion);
                        break;
                    case SDLK_s:
                        sauvegarder(vCase);
                        break;
                    case SDLK_c:
                        charger(vCase);
                        break;
                    case SDLK_r:
                        initialiser(vCase);
                        coups=0;
                        break;
                    default:
                        break;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                valide=1;
                if(clickEnCours==0){
                    /*cout << "event.button.x=" << event.button.x << endl;
                    cout << "event.button.y=" << event.button.y << endl;
                    cout << "i=" << iDepart << "\nj=" << jDepart << endl;*/
                    positionner(event.button.x,event.button.y,iDepart,jDepart);
                    if(iDepart==-1 || jDepart==-1 || vCase[iDepart][jDepart]==VIDE)
                        valide=0; // test de correction d'un bug de mouvement
                    else siPion=vCase[iDepart][jDepart];
                }
                if(clickEnCours==1){
                    //cout << "i=" << iArrivee << "\nj=" << jArrivee << endl;
                    positionner(event.button.x,event.button.y,iArrivee,jArrivee);
                    //if(iArrivee==-1 || jArrivee==-1)
                      //  valide=0;
                    ancien=processMove(vCase,iDepart,jDepart,iArrivee,jArrivee,coups);
                }
                if(clickEnCours && valide){
                    clickEnCours=0;
                }
                else if(!clickEnCours && valide){
                    clickEnCours=1;
                }
                break;
            /*case SDL_MOUSEMOTION:
                if(clickEnCours){
                    position.x=event.motion.x;
                    position.y=event.motion.y;
                    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
                    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,136,136));
                    SDL_BlitSurface(table,NULL,ecran,&positionTable);

                    SDL_Flip(ecran);
                }
                break;*/
        }
        SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
        if(clickEnCours)
            SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,225,136,136));
        else
            SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,255,255));
        SDL_BlitSurface(table,NULL,ecran,&positionTable);
        blitter(vCase,ecran,pieces);
        //SDL_BlitSurface(ecran,NULL,pieces[RB],&position);
        SDL_Flip(ecran);

    }
}
void annuler(int**& vCase,int iDepart,int jDepart,int iArrivee,int jArrivee,int& coups,int ancien,int siPion){
    int tmp(0);
    if(siPion==PB)
        vCase[iArrivee][jArrivee]=PB;
    tmp=vCase[iArrivee][jArrivee];
    vCase[iArrivee][jArrivee]=ancien;
    vCase[iDepart][jDepart]=tmp;
    coups--;
}
int processMove(int**&vCase,int& _i,int& _j,int& __i,int& __j,int& coups){
    /*
        Cette fonction processMove est la fonction principale qui teste tous les mouvements
        Elle est la base de tout ce jeu d'echec.
        C'est ici qu'on ajoute des fonctionnalites, pour limiter les mouvements par exemple
    */
    int tmp(0),ok=0,ancien(0);
    if(_i!=-1 && _j!=-1 && __i!=-1 && __j!=-1){
        switch(vCase[_i][_j]){
            case VIDE:
                break;
            case PB:
                // Promotion des pions
                if(__i==0 && !(vCase[__i][__j]==RB || vCase[__i][__j]==RN))
                    vCase[_i][_j]=DB;
                ok=1;
                break;
            case PN:
                if(__i==7 && !(vCase[__i][__j]==RB || vCase[__i][__j]==RN))
                    vCase[_i][_j]=DN;
                ok=1;
                break;
            default:
                ok=1;
                break;
        }
            if(vCase[__i][__j]==RB || vCase[__i][__j]==RN)
            ok=0;
        for(int i=RB;i<RN;i++){
            for(int j=RB;j<RN;j++){
                if(vCase[_i][_j]==i && vCase[__i][__j]==j)
                    ok=0;
            }
        }
        for(int i=RN;i<=PN;i++){
            for(int j=RN;j<=PN;j++){
                if(vCase[_i][_j]==i && vCase[__i][__j]==j)
                    ok=0;
            }
        }
        ancien=vCase[__i][__j];
        if(ok && tourExact(vCase,_i,_j,coups,ok)){
            // les instructions qui effectuent les permutations produisant mouvement reel
            tmp=vCase[_i][_j];
            vCase[_i][_j]=VIDE;
            vCase[__i][__j]=tmp;
            coups++;
        }
    }
    cout << ancien << endl;
    return ancien;
}
bool tourExact(int**& vCase,int _i,int _j,int& coups, int& ok){
    /* Fonction qui se charge que l'on procede les coups par tour */
    bool exact;
    if(coups%2==0){
        for(int i=RB;i<=PB;i++){
            if(vCase[_i][_j]==i && ok){
                exact=true;
            }
        }
    }
    if(coups%2==1){
        for(int i=RN;i<=PN;i++){
            if(vCase[_i][_j]==i && ok){
                exact=true;
            }
        }
    }

    return exact;
}

void charger(int**& vCase){
    ifstream file;
    file.open("table.txt");
    if(!file) erreur("Probleme d'ouverture du fichier...");
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            file >> vCase[i][j];
        }
    }
    file.close();
}
void sauvegarder(int**& vCase){
    ofstream file;
    file.open("table.txt");
    if(!file) erreur("Probleme d'ouverture du fichier...");
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            file << vCase[i][j] << "\t";
        }
        file << endl;
    }
    file.close();
}
void positionner(Uint16 x,Uint16 y,int& _i,int& _j){
    /*
        La fonction positionner sert à convertir la position de la souris sur l'échiquier en i et j
        tel que vCase[i][j] correspond à la case sélectionnée
    */
    int i=0;
    while(y>80*i+43){
        _i=i;
        i++;
    }
    int j=0;
    while(x>80*j+42){
        _j=j;
        j++;
    }
    if(x<43 || x>683)
        _i=-1; // le retour -1 marque que la position selectionnee n'est pas sur l'echiquier
    if(y<42 || y>682)
        _j=-1;
}
void swapr(int &a, int &b){
    int t(0);
    t=a;a=b;b=t;
}
void blitter(int** vCase,SDL_Surface*& ecran, SDL_Surface**& pieces){
    ///Define the cells
    SDL_Rect cell[8][8];//cell[i][j] is i-ligne and j-column

    int i=0,j=0;

    //Initialisation des positions
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            cell[i][j].x=43;
            cell[i][j].y=42;
        }
    }
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            cell[i][j].x+=j*80;
            cell[i][j].y+=i*80;
        }
    }
/// Blits
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            SDL_BlitSurface(pieces[vCase[i][j]],NULL,ecran,&cell[i][j]);
        }
    }
}
void initialiser(int**& table){

    int i(0),j(0);
    table=new (nothrow) int* [8];
    if(!table) erreur("Erreur d'allocation de l'echiquier...");
    for(i=0;i<8;i++)
        table[i]=new (nothrow) int [8];

    // Pions
    for(i=0;i<8;i++){
        table[6][i]=PB;
        table[1][i]=PN;
    }
    // Rois
    table[0][4]=RN;
    table[7][4]=RB;
    // Dames
    table[0][3]=DN;
    table[7][3]=DB;
    // Tours
    table[0][0]=TN;
    table[0][7]=TN;
    table[7][0]=TB;
    table[7][7]=TB;
    // Fous
    table[0][2]=FN;
    table[0][5]=FN;
    table[7][2]=FB;
    table[7][5]=FB;
    // Cavaliers
    table[0][1]=CN;
    table[0][6]=CN;
    table[7][1]=CB;
    table[7][6]=CB;
    // Cases vides
    for(i=2;i<=5;i++){
        for(j=0;j<=8;j++){
            table[i][j]=VIDE;
        }
    }
}
void erreur(string a){
    cout << "Erreur : " << a << endl;
    exit(1);
}
