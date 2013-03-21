#ifndef Sprite_h
#define Sprite_h

#include <cstdlib>

#include <iostream>
#include <string>
#include <math.h>

#include <SDL/SDL.h>



/*
    Sprite library for SDL - using bitmaps
    This library was put together by Kenny Cason and is designed to
    be easily implemented into any C++ program using SDL
    Feel free to do what ever you want with it. enjoy!
    Please report any bugs
    kenneth [DOT] cason [AT] gmail [DOT] com
    v1.0
    2009 Sep 20
*/


#define SPRITE_MAX(a,b)	((a > b) ? a : b)
#define SPRITE_MIN(a,b)	((a < b) ? a : b)

#define FLIP_HORIZONTAL 1
#define FLIP_VERTICAL   2

class SpriteEffects;
class Sprite {
public:
    Sprite();
    virtual ~Sprite();
    Sprite(std::string fileName,int maxFrames, int animationSpeed);
    Sprite(SDL_Surface* surface, int maxFrames,int animationSpeed);

    Sprite* draw(SDL_Surface* buffer, int x, int y);
    Sprite* setTransparency(Uint32 color);
    Sprite* setTransparency(Uint8 r, Uint8 g, Uint8 b);
    Sprite* setSpeed(int animationSpeed);
    int getSpeed();
    Sprite* start();
    Sprite* restart();
    Sprite* animate();
    Sprite* setLoopToBegin(bool loop);
    bool running();
    Sprite* stop();
    bool isSprite();
    int getFrame();
    int getFrameIterator();
    int getMaxFrames();
    int getWidth();  // frame width
    int getHeight(); // frame height
    Sprite* setWidth(int width);
    Sprite* setHeight(int height);
    int getSpriteWidth();
    int getSpriteHeight();
    bool equals(Sprite compare);
    SDL_Surface* getSurface();
    Sprite* setSurface(SDL_Surface* surface);
    Sprite* destroy();

    bool isTransparentPixel(int x, int y);
    Sprite* setTransparentPixel(int x, int y);
    Uint32 getPixel(int x, int y);
    int setPixel(int x, int y, Uint32 color);
    int setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
    Uint8 getPixel8(int x, int y);
    int setPixel8(int x, int y, Uint8 color);
    Uint16 getPixel16(int x, int y);
    int setPixel16(int x, int y, Uint16 color);
    Uint32 getPixel32(int x, int y);
    int setPixel32(int x, int y, Uint32 color);

    SDL_Surface* getRect(int x, int y, int width, int height);
    bool rectCollide(int x1, int y1, Sprite &spriteB, int x2, int y2);
    bool pixelCollide(int x1, int y1, Sprite &spriteB, int x2, int y2);
    Sprite* rotate90();
    Sprite* rotate180();
    Sprite* rotate270();
    Sprite* flipHorizontal();
    Sprite* flipVertical();
    Sprite* fade(float fade); // fade from 0 to 100%
    Sprite* reverseAnimation();

    Sprite* zoom(float z);  // percentage to zoom in
    Sprite* stretchX(float x); // percentage to stretchheight
    Sprite* stretchY(float y);  // percentage to stretchwidth
    Sprite* stretch(float x,float y); // percentage to strech X and Y

private:

    bool loaded;
    bool run;
    Uint32 speed;
    Uint32 width;
    Uint32 height;
    Uint32 index;
    Uint32 indexIterator;
    bool loopToBeginning;  // if loop = true iterate through animations from 0 to N, then reset to 0
                // if loop = false iterate through animations from 0 to N, then from N to 0, and repeat
    Uint32 maxFrames;
    Uint32 lastAnimated;

    SDL_Surface* sprite;
};

class Sprite;
class SpriteEffects {
public:
    static SDL_Surface* getRect(Sprite &sprite, int x, int y, int width, int height);
    static bool rectCollide(Sprite &spriteA, int x1, int y1, Sprite &spriteB, int x2, int y2);
    static bool pixelCollide(Sprite &spriteA, int x1, int y1, Sprite &spriteB, int x2, int y2);
    static void rotate90(Sprite &sprite);
    static void rotate180(Sprite &sprite);
    static void rotate270(Sprite &sprite);
    static void flipHorizontal(Sprite &sprite);
    static void flipVertical(Sprite &sprite);
    static void rotate(Sprite &sprite, int dir); // helper function for rotate90(),rotate180(), and rotate270()
    static void flip(Sprite &sprite, int dir);  // helper function for flipHorizontal() and flipVeritcal()
    static void fade(Sprite &sprite, float fade); // fade from 0 to 100%
    static void reverseAnimation(Sprite &sprite);

    static void zoom(Sprite &sprite, float x);  // percentage to zoom in
    static void stretchX(Sprite &sprite, float x); // percentage to stretchheight
    static void stretchY(Sprite &sprite, float y);  // percentage to stretchwidth
    static void stretch(Sprite &sprite, float x,float y); // percentage to strech X and Y

    static Uint32 getPixel(SDL_Surface* sprite, int x, int y);
    static int setPixel(SDL_Surface* sprite, int x, int y, Uint32 color);

private:


};
#endif
