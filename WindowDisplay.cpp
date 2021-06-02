#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
// Uncomment below to use SDL_image
// #include "SDL_image.h"

#define windowWidth 800
#define windowHeight 600
#define fps 60

using namespace std;

class Sprite
{

protected:
    SDL_Surface *image;
    SDL_Rect rect;

    int originX, originY;

public:
    Sprite(Uint32 color, int x, int y, int w = 48, int h = 64)
    {
        image = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

        SDL_FillRect(image, NULL, color);

        rect = image->clip_rect;

        originX = rect.w / 2;
        originY = rect.h / 2;

        rect.x = x;
        rect.y = y;
    }

    void update()
    {
    }

    void draw(SDL_Surface *destination)
    {
        SDL_BlitSurface(image, NULL, destination, &rect);
    }

    SDL_Surface *getImage() const
    {
        return image;
    }

    bool operator==(const Sprite &other) const
    {
        return (image == other.image);
    };
};

class SpriteGroup
{

private:
    vector<Sprite *> sprites;
    int spritesSize;

public:
    SpriteGroup copy()
    {
        SpriteGroup newGroup;

        for (int i = 0; i < spritesSize; i++)
        {
            newGroup.add(sprites[i]);
        }
        return newGroup;
    }

    void add(Sprite *sprite)
    {
        sprites.push_back(sprite);

        spritesSize = sprites.size();
    }

    void remove(Sprite spriteObject)
    {
        for (int i = 0; i < spritesSize; i++)
        {
            if (*sprites[i] == spriteObject)
            {
                sprites.erase(sprites.begin() + i);
            }
        }

        spritesSize = sprites.size();
    }

    bool has(Sprite spriteObject)
    {
        for (int i = 0; i < spritesSize; i++)
        {
            if (*sprites[i] == spriteObject)
            {
                return true;
            }
        }

        return false;
    }

    void update()
    {

        if (!sprites.empty())
        {
            for (int i = 0; i < spritesSize; i++)
            {
                sprites[i]->update();
            }
        }
    }

    void draw(SDL_Surface *destination)
    {

        if (!sprites.empty())
        {
            for (int i = 0; i < spritesSize; i++)
            {
                sprites[i]->draw(destination);
            }
        }
    }

    void empty()
    {
        sprites.clear();
        spritesSize = sprites.size();
    }

    int size()
    {
        return spritesSize;
    }

    vector<Sprite *> getSprites()
    {
        return sprites;
    }
};

class Block : public Sprite
{

public:
    Block(Uint32 color, int x, int y, int w = 48, int h = 64) : Sprite(color, x, y, w, h)
    {
        updateProperties();
    }

    void updateProperties()
    {
        originX = 0;
        originY = 0;

        setPosition(rect.x, rect.y);
    }

    void setPosition(int x, int y)
    {
        rect.x = x - originX;
        rect.y = y - originY;
    }

    void setImage(const char filename[] = NULL)
    {
        if (filename != NULL)
        {
            SDL_Surface *loadedImage = NULL;

            loadedImage = SDL_LoadBMP(filename);

            if (loadedImage != NULL)
            {
                image = loadedImage;

                int oldX = rect.x;
                int oldY = rect.y;

                rect = image->clip_rect;

                rect.x = oldX;
                rect.y = oldY;

                updateProperties();
            }
        }
    }
};

void fpsCap(Uint32 clock)
{
    if ((1000 / fps) > SDL_GetTicks() - clock)
    {
        SDL_Delay(1000 / fps - (SDL_GetTicks() - clock));
    }
}

int main(int arc, char *argv[])
{

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Window Display using SDL2 Library",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth,
                                          windowHeight,
                                          0);

    if (window == NULL)
    {
        cout << "There was an error while loading this application" << endl
             << SDL_GetError() << endl;
    }

    SDL_Surface *screen = SDL_GetWindowSurface(window);
    Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 blue = SDL_MapRGB(screen->format, 0, 0, 255);

    SDL_FillRect(screen, NULL, white);

    Sprite object(red, windowWidth / 2, windowHeight / 2);
    Sprite anotherObject(blue, windowWidth / 2 - 100, windowHeight / 2 + 20);

    Block block(red, windowWidth / 2, windowHeight / 2);

    // Comment the line below to see red rectangle object created using Sprite Class
    block.setImage("sprite.bmp");

    SpriteGroup activeSprites;
    activeSprites.draw(screen);
    block.draw(screen);
    SDL_UpdateWindowSurface(window);

    Uint32 clock;
    SDL_Event event;
    bool running = true;

    while (running)
    {
        clock = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }

        fpsCap(clock);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}