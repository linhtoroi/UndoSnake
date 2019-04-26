#ifndef GALLERY_H
#define GALLERY_H

#include <vector>
#include <string>
#include <SDL.h>

enum PictureID {
    PIC_SCISSORS = 0, PIC_SNAKE_VERTICAL, PIC_SNAKE_HEAD, PIC_SNAKE_BODY, PIC_BLOCK_GAMEOVER, PIC_BLOCK_ADDHEAD1, PIC_BLOCK_ADDHEAD2,
    PIC_BLOCK_ADDHEAD3, PIC_BLOCK_ADDHEAD4, PIC_BLOCK_ADDHEAD5, PIC_BACKGROUND, PIC_PLAYBUTTON, PIC_EXITBUTTON, PIC_RESTARTBUTTONWON, PIC_RESTARTBUTTONOVER,
    PIC_EXITBUTTONOVER, PIC_EXITBUTTONWON, PIC_O, PIC_COUNT
};

class Gallery
{
    SDL_Texture* pictures[PIC_COUNT];
    SDL_Renderer* renderer;
    SDL_Texture* loadTexture(std::string path);
public:
    Gallery(SDL_Renderer* renderer_);
    ~Gallery();

    void loadGamePictures();
    SDL_Texture* getImage(PictureID id) const { return pictures[id]; }
};

#endif // GALLERY_H
