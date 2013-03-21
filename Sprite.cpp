#include "Sprite.h"

/*
    Sprite library for SDL - using bitmaps
    This library was put together by Kenny Cason and is designed to
    be easily implemented into any C++ program using SDL
    Feel free to do what ever you want with it. enjoy!
    Please report any bugs
    kenneth [DOT] cason [AT] gmail [DOT] com
    v1.0
    2009 Jan 7
*/

Sprite::Sprite() {
    sprite = NULL;
    loaded = false;
    width = 0;
    height = 0;
    speed = 0;
    run = false;
    loopToBeginning = true;
    indexIterator = 0;
    index = 0;
}

Sprite::Sprite(std::string file, int frames, int speed) {
    SDL_Surface *temp = SDL_LoadBMP(file.c_str());
    sprite = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    if(sprite == NULL) {
        std::cout << "failed to load sprite " << file << std::endl;
        loaded = false;
        width = 0;
        height = 0;
        this->speed = 0;
    } else {
      //  std::cout << "successfully loaded sprite " << file << std::endl;
        loaded = true;
        width = sprite->w/frames;
        height = sprite->h;
        this->speed = speed;
        run = true;
    }
    maxFrames = frames;
    if(maxFrames > 1) {
        run = true;
    } else {
        run = false;
    }
    lastAnimated = SDL_GetTicks();
    index = 0;
    indexIterator = 1;
    loopToBeginning = true;
}

Sprite::Sprite(SDL_Surface* surface, int frames, int speed) {
    if(surface == NULL) {
        std::cout << "failed to load sprite" << std::endl;
        sprite = NULL;
        loaded = false;
        width = 0; height = 0;
        this->speed = 0;
    } else {
       // std::cout << "successfully loaded sprite" << std::endl;
        // create a new surface
        if(surface->flags & SDL_SRCCOLORKEY) {
            sprite = SDL_CreateRGBSurfaceFrom(surface->pixels, surface->w, surface->h, surface->format->BitsPerPixel, surface->pitch,
                surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
        } else {
            sprite = SDL_CreateRGBSurfaceFrom(surface->pixels, surface->w, surface->h, surface->format->BitsPerPixel, surface->pitch,
                surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        }
        if(surface->flags & SDL_SRCCOLORKEY) {
            SDL_SetColorKey(sprite, SDL_RLEACCEL|SDL_SRCCOLORKEY, surface->format->colorkey );
        }
        loaded = true;
        width = sprite->w/frames;
        height = sprite->h;
        this->speed = speed;
    }
    maxFrames = frames;
    if(maxFrames > 1) {
        run = true;
    } else {
        run = false;
    }
    lastAnimated = SDL_GetTicks();
    indexIterator = 1;
    index = 0;
    loopToBeginning = true;
}

Sprite* Sprite::draw(SDL_Surface* buffer, int x, int y) {
     if(!isSprite()) {
         std::cout << "Failed to draw, Sprite not initialized!"<< std::endl;
         return this;
     }
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    // this blits the current frame from the sprite sheet
    SDL_Rect animRect;
    animRect.x = width*index;
    animRect.y = 0;
    animRect.w = width;
    animRect.h = height;
    SDL_BlitSurface(sprite, &animRect, buffer,&dstrect);
    return this;
}

Sprite::~Sprite() {
    destroy();
}


Sprite* Sprite::setTransparency(Uint8 r, Uint8 g, Uint8 b) {
    if(!isSprite()) {
         std::cout << "Failed to set Transparency, Sprite not initialized!"<< std::endl;
         return this;
     }
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY,SDL_MapRGB(sprite->format, r, g, b));
    return this;
}

Sprite* Sprite::setTransparency(Uint32 colorkey) {
    if(!isSprite()) {
         std::cout << "Failed to set Transparency, Sprite not initialized!"<< std::endl;
         return this;
     }
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, colorkey);
    return this;
}

Sprite* Sprite::setSpeed(int i) {
    speed = i;
    return this;
}

int Sprite::getSpeed() {
    return speed;
}

Sprite* Sprite::start() {
    run = true;
    return this;
}

Sprite* Sprite::restart() {
    if(run) {
        index = 0;
        lastAnimated = SDL_GetTicks();
    }
    return this;
}


Sprite* Sprite::animate() {
    if(run) {
        if(SDL_GetTicks() - lastAnimated > speed) {
            lastAnimated = SDL_GetTicks();
            index += indexIterator;
            if(index >= maxFrames) {
                if(loopToBeginning) {
                    indexIterator = 1;
                    index = 0;
                } else {
                    indexIterator = -1;
                    index = maxFrames - 1;
                }
            } else if(index < 0) {
                indexIterator = 1;
                index = 0;
            }
        }
    }
    return this;
}

Sprite* Sprite::setLoopToBegin(bool loop) {
    loopToBeginning = loop;
    return this;
}

bool Sprite::running() {
   return run;
}

Sprite* Sprite::stop() {
    run = false;
    index = 0;
    return this;
}

bool Sprite::isSprite() {
    return loaded;
}


Sprite* Sprite::setTransparentPixel(int x, int y) {
    setPixel(x, y, sprite->format->colorkey);
    return this;
}

bool Sprite::isTransparentPixel(int x, int y) {
    Uint32 pixelcolor = getPixel(x, y);
	//test whether pixels color == color of transparent pixels for that surface
	return (pixelcolor == sprite->format->colorkey);
}

int Sprite::setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    Uint32 color = 0;
    color += b*256*256;
    color += g*256;
    color += r;
    return setPixel(x,y,color);
}

int Sprite::setPixel(int x, int y, Uint32 pixel) {
    return SpriteEffects::setPixel(sprite, x, y, pixel);
}

Uint32 Sprite::getPixel(int x, int y) {
   return SpriteEffects::getPixel(sprite, x, y);
}

Uint8 Sprite::getPixel8(int x, int y) {
    if(!isSprite()) {
         std::cout << "Failed to get pixel, Sprite not initialized!"<< std::endl;
         return 0;
     }
    Uint8* pixels = (Uint8*)sprite->pixels;
    return pixels[y * sprite->w + x];
}

int Sprite::setPixel8(int x, int y, Uint8 pixel) {
    if(!isSprite()) {
         std::cout << "Failed to set pixel, Sprite not initialized!"<< std::endl;
         return -1;
     }
    Uint8* pixels = (Uint8*)sprite->pixels;
    pixels[y * sprite->w + x] = pixel;
    return 0;
}

Uint16 Sprite::getPixel16(int x, int y) {
    if(!isSprite()) {
         std::cout << "Failed to get pixel, Sprite not initialized!"<< std::endl;
         return 0;
     }
    Uint16* pixels = (Uint16*)sprite->pixels;
    return pixels[y * sprite->w + x];
}

int Sprite::setPixel16(int x, int y, Uint16 pixel) {
    if(!isSprite()) {
         std::cout << "Failed to set pixel, Sprite not initialized!"<< std::endl;
         return -1;
     }
    Uint16* pixels = (Uint16*)sprite->pixels;
    pixels[y * sprite->w + x] = pixel;
    return 0;
}

Uint32 Sprite::getPixel32(int x, int y) {
    if(!isSprite()) {
         std::cout << "Failed to get pixel, Sprite not initialized!"<< std::endl;
         return 0;
     }
    Uint32* pixels = (Uint32*)sprite->pixels;
    return pixels[y * sprite->w + x];
}

int Sprite::setPixel32(int x, int y, Uint32 pixel) {
    if(!isSprite()) {
         std::cout << "Failed to set pixel, Sprite not initialized!"<< std::endl;
         return -1;
     }
    Uint32* pixels = (Uint32*)sprite->pixels;
    pixels[y * sprite->w + x] = pixel;
    return 0;
}


int Sprite::getFrame() {
    return index;
}

int Sprite::getFrameIterator() {
    return indexIterator;
}

int Sprite::getMaxFrames() {
    return maxFrames;
}


int Sprite::getWidth() { // this is most likely the width a user will need to know
    return width;
}

Sprite* Sprite::setWidth(int width) {
    this->width = width;
    return this;
}

int Sprite::getSpriteWidth() { // sprites Actual width
    return sprite->w;
}

int Sprite::getHeight() { // in this Class the frame height is the same as the sprites Actual height
    return height;
}

Sprite* Sprite::setHeight(int height) {
    this->height = height;
    return this;
}

int Sprite::getSpriteHeight() { // sprites Actual height
    return sprite->h;
}

bool Sprite::equals(Sprite cmp) {
     if(sprite == cmp.getSurface()) {
         return true;
     }
     return false;
}

SDL_Surface* Sprite::getSurface() {
     return sprite;
}

Sprite* Sprite::setSurface(SDL_Surface* surface) {
    sprite = surface;
    return this;
}

SDL_Surface* Sprite::getRect(int x, int y, int w, int h) {
    return SpriteEffects::getRect(*this, x, y, w, h);
}

bool Sprite::rectCollide(int x1, int y1, Sprite &spriteB, int x2, int y2) {
    return SpriteEffects::rectCollide(*this, x1, y1, spriteB, x2, y2);
}

bool Sprite::pixelCollide(int x1, int y1, Sprite &spriteB, int x2, int y2) {
    return SpriteEffects::pixelCollide(*this, x1, y1, spriteB, x2, y2);
}

Sprite* Sprite::rotate90() {
    SpriteEffects::rotate90(*this);
    return this;
}

Sprite* Sprite::rotate180() {
    SpriteEffects::rotate180(*this);
    return this;
}

Sprite* Sprite::rotate270() {
    SpriteEffects::rotate270(*this);
    return this;
}

Sprite* Sprite::flipHorizontal() {
    SpriteEffects::flipHorizontal(*this);
    return this;
}

Sprite* Sprite::flipVertical() {
    SpriteEffects::flipVertical(*this);
    return this;
}

Sprite* Sprite::fade(float f) {
    SpriteEffects::fade(*this, f);
    return this;
}

Sprite* Sprite::reverseAnimation() {
    SpriteEffects::reverseAnimation(*this);
    return this;
}

Sprite* Sprite::zoom(float z) {
    SpriteEffects::zoom(*this, z);
    return this;
}

Sprite* Sprite::stretchX(float x) {
    SpriteEffects::stretchX(*this, x);
    return this;
}

Sprite* Sprite::stretchY(float y) {
    SpriteEffects::stretchY(*this, y);
    return this;
}

Sprite* Sprite::stretch(float x,float y) {
    SpriteEffects::stretch(*this, x, y);
    return this;
}

Sprite* Sprite::destroy() {
    if(isSprite()) {
        SDL_FreeSurface(sprite);
    }
    return this;
}


/*
  SpriteEffects functions
  */


SDL_Surface* SpriteEffects::getRect(Sprite &sprite, int x, int y, int w, int h) {
    if(!sprite.isSprite()) {
         std::cout << "Failed to get Rectangle, Sprite not initialized!"<< std::endl;
         return NULL;
     }
    SDL_Surface* newrect;
    // create a new surface
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        newrect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
            sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask );
    } else {
        newrect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
            sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0 );
    }

    for(int j = 0; j < h; j++) {
        for(int i = 0; i < w; i++) {
            setPixel(newrect, i, j, sprite.getPixel(x+i,y+j));
        }
    }
    //Copy color key
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        SDL_SetColorKey(newrect, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
    }
    return newrect;
}


    /*
       test
	    testX,testY____ testX+width
		|   	       |
		| sprite	   |
		|  thisX,thisY |__ thisX+width
		|  |	       |  |
		|__|___________|  |
		testY+height	  |
			|_____________|
			thisY+height
    */
bool SpriteEffects::rectCollide(Sprite &spriteA, int aX, int aY, Sprite &spriteB, int bX, int bY) {
    if(!spriteA.isSprite() || !spriteB.isSprite()) {
         std::cout << "Failed to perfrom Rectangle Collision test, Sprite not initialized!"<< std::endl;
         return false;
    }
	if((aX + spriteA.getWidth() < bX) || (bX + spriteB.getWidth() < aX)) {
		return false;
	}
	if((aY + spriteA.getHeight() < bY) || (bY + spriteB.getHeight() < aY)) {
		return false;
	}
    return true;
}

bool SpriteEffects::pixelCollide(Sprite &spriteA, int aX, int aY, Sprite &spriteB, int bX, int bY) {
	/*check if bounding boxes intersect*/
	if(!rectCollide(spriteA, aX, aY, spriteB, bX, bY)) {
         return false;
    }

    // get the overlaping box
	int inter_x0 = SPRITE_MAX(bX,aX);
	int inter_x1 = SPRITE_MIN(bX+spriteB.getWidth(),aX+spriteA.getWidth());

	int inter_y0 = SPRITE_MAX(bY,aY);
	int inter_y1 = SPRITE_MIN(bY+spriteB.getHeight(),aY+spriteA.getHeight());

	for(int y = inter_y0 ; y <= inter_y1 ; y++) {
		for(int x = inter_x0 ; x <= inter_x1 ; x++) {
		    /*compute offsets for surface, but dont forget to account for the current animation*/
            if((!spriteB.isTransparentPixel(x-aX + spriteB.getFrame() * spriteB.getWidth() , y-bY))
			&& (!spriteA.isTransparentPixel(x-aX + spriteA.getFrame() * spriteA.getWidth(), y-aY))) {/*before pass to isTransparentPixel*/
				return true;
			}
		}
	}
    return false;
}

void SpriteEffects::flip(Sprite &sprite, int val) {
    if(!sprite.isSprite()) {
         std::cout << "Failed to flip, Sprite not initialized!"<< std::endl;
         return;
     }
    SDL_Surface* flipped;
    // create a new surface
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask  );
    } else {
        flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0 );
    }
    // check to see if the surface must be locked
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_LockSurface(sprite.getSurface());
    }
    for(int y = 0; y < sprite.getSurface()->h; y++) {
        for(int x = 0; x < sprite.getSurface()->w; x++) {
            if(val == FLIP_HORIZONTAL) {
                setPixel(flipped, sprite.getSurface()->w - 1 - x, y, sprite.getPixel(x,y));
            } else if(val == FLIP_VERTICAL) {
                setPixel(flipped, x, sprite.getSurface()->h - 1 - y, sprite.getPixel(x,y));
            }
        }
    }
    //Copy color key
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        SDL_SetColorKey(flipped, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_UnlockSurface(sprite.getSurface());
    }
    SDL_FreeSurface(sprite.getSurface());
    sprite.setSurface(flipped);
}

void SpriteEffects::flipVertical(Sprite &sprite) {
    return flip(sprite, FLIP_VERTICAL);
}

void SpriteEffects::flipHorizontal(Sprite &sprite) {
    return flip(sprite, FLIP_HORIZONTAL);
}


void SpriteEffects::rotate90(Sprite &sprite) {
    rotate(sprite, 90);
}

void SpriteEffects::rotate180(Sprite &sprite) {
    rotate(sprite, 180);
}

void SpriteEffects::rotate270(Sprite &sprite) {
    rotate(sprite, 270);
}

void SpriteEffects::rotate(Sprite &sprite, int deg) {
    if(!sprite.isSprite()) {
         std::cout << "Failed to rotate animation, Sprite not initialized!"<< std::endl;
         return;
     }
    SDL_Surface* rotated;
    int w,h;
    if(deg == 90 || deg == 270) {
        w = sprite.getHeight() * sprite.getMaxFrames();
        h = sprite.getWidth();
    } else if(deg == 180) {
        w = sprite.getWidth() * sprite.getMaxFrames();
        h = sprite.getHeight();
    } else {
        return;
    }
    // create a new surface
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        rotated = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask  );
    } else {
        rotated = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0 );
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_LockSurface(sprite.getSurface());
    }


    for(int i = 0; i < sprite.getMaxFrames(); i++) {
        for(int y = 0; y < sprite.getHeight(); y++) {
            for(int x = 0; x < sprite.getWidth(); x++) {
                if(deg == 90) {
                    setPixel(rotated, sprite.getHeight() * i + sprite.getHeight() - y - 1, x, sprite.getPixel((i * sprite.getWidth()) + x,y));
                } else if(deg == 180) {
                    setPixel(rotated, sprite.getWidth() * i + sprite.getWidth() - x - 1, rotated->h - y - 1, sprite.getPixel((i * sprite.getWidth()) + x,y));
                } else if(deg == 270) {
                    setPixel(rotated, sprite.getHeight() * i + y, rotated->h - x - 1, sprite.getPixel((i * sprite.getWidth()) + x,y));
                } else {
                    return;
                }
            }
        }
    }
    sprite.setWidth( rotated->w/sprite.getMaxFrames() );
    sprite.setHeight( rotated->h );
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        SDL_SetColorKey(rotated, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_UnlockSurface(sprite.getSurface());
    }
    SDL_FreeSurface(sprite.getSurface());
    sprite.setSurface(rotated);
}



void SpriteEffects::fade(Sprite &sprite, float fade) {
    return;
}

void SpriteEffects::reverseAnimation(Sprite &sprite) {
    if(!sprite.isSprite()) {
         std::cout << "Failed to reverse animation, Sprite not initialized!" << std::endl;
         return;
    }
    SDL_Surface* reversed = NULL;
    // create a new surface
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        reversed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask );
    } else {
        reversed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0);
    }
    // check to see if the surface must be locked
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_LockSurface(sprite.getSurface());
    }

    for(int f = 0; f < sprite.getMaxFrames(); f++) {
        for(int y = 0; y < sprite.getSurface()->h; y++) {
            for(int x = 0; x < sprite.getWidth(); x++) {
                setPixel(reversed, (sprite.getMaxFrames() - f - 1)*sprite.getWidth() + x , y, sprite.getPixel(f*sprite.getWidth()+x,y));
            }
        }
    }
    //Copy color key
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        SDL_SetColorKey(reversed, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_UnlockSurface(sprite.getSurface());
    }
    SDL_FreeSurface(sprite.getSurface());
    sprite.setSurface(reversed);
}



void SpriteEffects::stretchX(Sprite &sprite, float stretchX) {
    return stretch(sprite, stretchX, 100);
}

void SpriteEffects::stretchY(Sprite &sprite, float stretchY) {
    return stretch(sprite, 100, stretchY);
}

void SpriteEffects::zoom(Sprite &sprite, float zoom) {
    return stretch(sprite, zoom,zoom);
}


void SpriteEffects::stretch(Sprite &sprite, float stretchX, float stretchY) {
    if(!sprite.isSprite()) {
         std::cout << "Failed to zoom, Sprite not initialized!"<< std::endl;
         return;
     }
    SDL_Surface* zoomed;
    if(stretchX < 1 || stretchY < 1) {
        std::cout << "Failed to zoom, value must be greater than zero!"<< std::endl;
        return;
    }
    stretchX /= 100;
    stretchY /= 100;
    // create a new surface
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
        zoomed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w*stretchX, sprite.getSurface()->h*stretchY, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask  );
    } else {
        zoomed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w*stretchX, sprite.getSurface()->h*stretchY, sprite.getSurface()->format->BitsPerPixel,
        sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0);
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_LockSurface(sprite.getSurface());
    }
    int zoomedWidth = zoomed->w/sprite.getMaxFrames();

    for(int y = 0; y < zoomed->h; y++) {
        for(int x = 0; x < zoomedWidth; x++) {
            // iterate over each animation as opposed to the whole sprite, to ensure that each animation is resized properly
            for(int i = 0; i < sprite.getMaxFrames(); i++) {
                setPixel(zoomed, (zoomedWidth * i) + x, y, sprite.getPixel((sprite.getWidth() * i) + (int)(x/stretchX),(int)(y/stretchY) ));
            }
        }
    }

    sprite.setWidth( zoomedWidth );
    sprite.setHeight( zoomed->h );
    if(sprite.getSurface()->flags & SDL_SRCCOLORKEY ) {
        SDL_SetColorKey(zoomed, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
    }
    if(SDL_MUSTLOCK(sprite.getSurface())) {
        SDL_UnlockSurface(sprite.getSurface());
    }
    SDL_FreeSurface(sprite.getSurface());
    sprite.setSurface(zoomed);
}



int SpriteEffects::setPixel(SDL_Surface* sprite, int x, int y, Uint32 pixel) {
    if(sprite == NULL) {
         std::cout << "Failed to set pixel, Sprite not initialized!"<< std::endl;
         return -1;
     }
    int bpp = sprite->format->BytesPerPixel;
    /* p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)sprite->pixels + y * sprite->pitch + x * bpp;
    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
    return 0;
}

Uint32 SpriteEffects::getPixel(SDL_Surface* sprite, int x, int y) {
   if(sprite == NULL) {
         std::cout << "Failed to get pixel, Sprite not initialized!"<< std::endl;
         return 0;
    }
    int bpp = sprite->format->BytesPerPixel;
    /* p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)sprite->pixels + y * sprite->pitch + x * bpp;
    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                return p[0] << 16 | p[1] << 8 | p[2];
            } else {
                return p[0] | p[1] << 8 | p[2] << 16;
            }
            break;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}
