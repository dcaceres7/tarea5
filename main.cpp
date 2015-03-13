
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<stdio.h>
#include <fstream>
#include <string.h>
#include <ctime>
#include<vector>
#include<list>
#include "Sho.h"
#include "EnemigoAzul.h"
#include "EnemigoVerde.h"
#include "EnemigoRojo.h"
#include <cstdlib>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background;
SDL_Rect rect_background;

SDL_Rect rect_perder;
SDL_Texture* texture_perder;

SDL_Rect rect_ganar;
SDL_Texture* texture_ganar;

SDL_Rect rect_inst;
SDL_Texture* instrucciones;


void frameWrite(int f)
{
    ofstream out("frames");
 //   out.seekg(0);
    out.write((char*)&f,4);
    out.close();
}
void frameRead(int f)
{
    ifstream in("frames");
    in.seekg(0);
    int fram;
    in.read((char*)&fram,4);
    if(f>fram){
    frameWrite(f);
    cout<<"MAYOR: "<<f<<endl;
    }
    in.close();
}

void loopJuego()
{
    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    texture_ganar= IMG_LoadTexture(renderer,"ganaste.png");
    texture_perder= IMG_LoadTexture(renderer,"perdiste.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    SDL_QueryTexture(texture_ganar, NULL, NULL, &w, &h);
    rect_ganar.x = rect_background.x + (rect_background.w/2) -(w/2);
    rect_ganar.y = rect_background.y + (rect_background.h/2) -(h/2);
    rect_ganar.w = w;
    rect_ganar.h = h;

    SDL_QueryTexture(texture_perder, NULL, NULL, &w, &h);
    rect_perder.x = rect_background.x + (rect_background.w/2) -(w/2);
    rect_perder.y = rect_background.y + (rect_background.h/2) -(h/2);
    rect_perder.w = w;
    rect_perder.h = h;

    list<Personaje*> personajes;
    personajes.push_back(new Sho(renderer,&personajes));

    //Main Loop
    int frame=0;
    int animacion_sho = 0;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    bool continuar=true,perder=true, empezar=false,juego=true;
    while(true)
    {

     if(juego){
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
        }

if(currentKeyStates[SDL_SCANCODE_E])
    {
        empezar=true;
    }

 if(empezar){
        if(frame%550==0)
        {
          personajes.push_back(new EnemigoRojo(renderer,&personajes));
          personajes.push_back(new EnemigoAzul(renderer,&personajes));
        }

        if(frame%200==0)
        {
          personajes.push_back(new EnemigoVerde(renderer,&personajes));
        }
 }


        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++){
               (*p)->act();
            if((*p)->ganar>=3){
               frameRead(frame);
               continuar=false;
               juego=false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);
SDL_RenderCopy(renderer, background, NULL, &rect_background);

if(continuar){
        if(perder){
        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->draw(renderer);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            if((*p)->muerto)
            {
                personajes.erase(p);
              if((*p)->tipoPersonaje=="sho"){
                    cout<<""<<endl;
              background = IMG_LoadTexture(renderer,"perdiste.png");
                frameRead(frame);
                perder=false;
                continuar=false;
                juego=false;
              }
                break;
            }
        }
}
else if(continuar==false&&perder==true){
    SDL_RenderCopy(renderer, texture_ganar, NULL, &rect_ganar);
    //mySleep(1);
    frameRead(frame);
    juego=false;
}

        SDL_RenderPresent(renderer);

        frame++;
        cout<<"FRAME:"<<frame<<endl;

    }
    else
    break;
  }
}

class MenuButton
{
public:
    SDL_Texture *selected;
    SDL_Texture *unselected;
    SDL_Rect rect;
    bool is_selected;
    MenuButton(string selected, string unselected, int x, int y)
    {
        this->selected = IMG_LoadTexture(renderer,selected.c_str());
        this->unselected = IMG_LoadTexture(renderer,unselected.c_str());
        SDL_QueryTexture(this->selected, NULL, NULL, &rect.w, &rect.h);
        rect.x = x;
        rect.y = y;
        is_selected = false;
    }

    void render()
    {
        if(is_selected)
            SDL_RenderCopy(renderer,selected,NULL,&rect);
        else
            SDL_RenderCopy(renderer,unselected,NULL,&rect);
    }

    void select()
    {
        is_selected = true;
    }

    void unselect()
    {
        is_selected = false;
    }
};

void instruc(){

    instrucciones=IMG_LoadTexture(renderer,"instrucciones.png");

    SDL_QueryTexture(instrucciones, NULL, NULL, &rect_inst.w, &rect_inst.h);
    rect_inst.x = 150;
    rect_inst.y = 120;

    while(true){
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 252, 80, 15, 255);
        SDL_RenderCopy(renderer,instrucciones,NULL,&rect_inst);
        SDL_RenderPresent(renderer);

         while(SDL_PollEvent(&Event))
        {
         if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
            if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }
    }
};

void mainMenu()
{
    int opcion = 1;
    SDL_Texture *menu_fondo = IMG_LoadTexture(renderer,"menu_fondo.png");
    SDL_Rect menu_rect;

    vector<MenuButton*>buttons;
    buttons.push_back(new MenuButton("button1_selected.png","button1_unselected.png",600,300));
    buttons.push_back(new MenuButton("button2_selected.png","button2_unselected.png",600,400));
    buttons.push_back(new MenuButton("button3_selected.png","button3_unselected.png",600,500));

    SDL_QueryTexture(menu_fondo, NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;


    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_2)
                {
                    exit(0);
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego();
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_DOWN)
                {
                    opcion++;
                    if(opcion > 3)
                        opcion = 3;
                }
                if(Event.key.keysym.sym == SDLK_UP)
                {
                    opcion--;
                    if(opcion < 1)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            loopJuego();
                        break;
                        case 2:
                             instruc();
                        break;
                        case 3:
                            exit(0);
                        break;
                    }
                }
            }
        }



        SDL_RenderCopy(renderer,menu_fondo,NULL,&menu_rect);


        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->unselect();
        }
        buttons[opcion-1]->select();
        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->render();
        }

        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1024/*WIDTH*/, 768/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    mainMenu();

	return 0;
}







