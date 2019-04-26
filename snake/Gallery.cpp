#include "Gallery.h"
#include "SDL_utils.h"

#include <SDL_image.h>

Gallery::Gallery(SDL_Renderer* renderer_)
    : renderer(renderer_)
{
    loadGamePictures();
}

Gallery::~Gallery()
{
    for (SDL_Texture* texture : pictures)
        SDL_DestroyTexture(texture);
}

SDL_Texture* Gallery::loadTexture(std::string path )
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
        logSDLError(std::cout, "Unable to load image " + path + " SDL_image Error: " + IMG_GetError());
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            logSDLError(std::cout, "Unable to create texture from " + path + " SDL Error: " + SDL_GetError());
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void Gallery::loadGamePictures()
{
    pictures[PIC_SCISSORS] = loadTexture("scissors.png");
//    pictures[PIC_SNAKE_VERTICAL] = loadTexture("snake_vertical.png");
//    pictures[PIC_SNAKE_HORIZONTAL] = loadTexture("snake_horizontal.png");
    pictures[PIC_BACKGROUND] = loadTexture("background.png");
    pictures[PIC_SNAKE_BODY] = loadTexture("ball.png");
    pictures[PIC_SNAKE_HEAD] = loadTexture("ball.png");
    pictures[PIC_BLOCK_GAMEOVER] = loadTexture("Block.png");
    pictures[PIC_BLOCK_ADDHEAD1] = loadTexture("circle1.png");
    pictures[PIC_BLOCK_ADDHEAD2] = loadTexture("circle2.png");
    pictures[PIC_BLOCK_ADDHEAD3] = loadTexture("circle3.png");
    pictures[PIC_BLOCK_ADDHEAD4] = loadTexture("circle4.png");
    pictures[PIC_BLOCK_ADDHEAD5] = loadTexture("circle5.png");
    pictures[PIC_PLAYBUTTON] = loadTexture("playbutton.png");
    pictures[PIC_EXITBUTTON] = loadTexture("exitbutton.png");
    pictures[PIC_RESTARTBUTTONWON] = loadTexture("restartWon.png");
    pictures[PIC_RESTARTBUTTONOVER] = loadTexture("restartOver.png");
    pictures[PIC_EXITBUTTONOVER] = loadTexture("exitOver.png");
    pictures[PIC_EXITBUTTONWON] = loadTexture("exitWon.png");
    pictures[PIC_O] = loadTexture("O1.png");
}


