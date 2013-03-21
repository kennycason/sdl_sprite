#include "Main.h"

using namespace std;

int main ( int argc, char* argv[] ) {
    bool running = true;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError());
        running = false;
    }
    // ensure a clean exit
    atexit(SDL_Quit);
    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(400, 230, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_WM_SetCaption( "Sprite Example - Samus and RPG Character", "Sprite Example - Samus and RPG Character" );
    if ( !screen ) {
        printf("Unable to set 400x300 video: %s\n", SDL_GetError());
        running = false;
    }
    SDL_Event keyevent;

    Sprite* s1 = new Sprite("sprites/samus_normal_run.bmp",10,60); // load a BMP that contains 10 frames
                                                // set the animation speed to 60 milliseconds
    s1->setTransparency(255,0,255);      // set RGB(255,0,255) as transparent
                                // NOTE: setTransparency(SDL_Surface->format->colorkey) also works
    Sprite* s2 = new Sprite(s1->getSurface(),10,60);    // load another copy of the previously loaded BMP
                                            // transparency is also copied over
    s2->flipHorizontal()->reverseAnimation();// flip the whole BMP horizontally
                                // then reverse the animations, if you don't do this the sprite
                                // will appear to be walking backwards. Observe s6 to see this

    Sprite* s3 = new Sprite(s1->getSurface(),10,60); // make another copy FROM s1
    s3->flipVertical();

    Sprite* s4 = new Sprite("sprites/samus_normal_run.bmp",10,20);
    s4->rotate90()->setTransparency(255,0,255);

    Sprite* s5 = new Sprite("sprites/samus_normal_run.bmp",10,360);
    s5->rotate180()->setTransparency(255,0,255);

    Sprite* s6 = new Sprite("sprites/samus_normal_run.bmp",10,60);
    s6->rotate270()->setTransparency(255,0,255);

    Sprite* s7 = new Sprite("sprites/samus_normal_run.bmp",10,60);
    s7->zoom(50)->setTransparency(255,0,255);


    Sprite* s8 = new Sprite("sprites/samus_normal_run.bmp",10,300);
    s8->setTransparency(255,0,255)->zoom(150);


    Sprite* s9 = new Sprite("sprites/samus_normal_run.bmp",10,68);
    s9->setTransparency(255,0,255)->stretchX(300);

    Sprite* s10 = new Sprite("sprites/samus_normal_run.bmp",10,100);
    s10->setTransparency(255,0,255)->stretchY(200);

    Sprite* s11 = new Sprite("sprites/samus_normal_run.bmp",10,100);
    s11->setTransparency(255,0,255);
    Sprite* s12 = new Sprite("sprites/samus_normal_run.bmp",10,100);
    s12->setTransparency(255,0,255);

    Sprite* s15 = new Sprite("sprites/samus_normal_run.bmp",10,0);
    s15->setTransparency(255,0,255)->stop();

        for(int y = 0; y < 16; y+=2) {
            for(int x = 0; x < 32; x+=2) {
                for(int i = 0; i < s15->getMaxFrames(); i ++) {
                    s15->setPixel(i*s15->getWidth()+x,y,0xFF0000);
                }
            }
        }
        for(int y = 0; y < 32; y+=2) {
            for(int x = 0; x < 32; x+=2) {
                for(int i = 0; i < s15->getMaxFrames(); i ++) {
                   s15->setTransparentPixel(i*s15->getWidth()+x,y);
                }
            }
        }

      /* load a different character sheet*/
    Sprite* characterSheet = new Sprite("sprites/character.bmp",1,0);
    characterSheet->setTransparency(255, 0, 255);
    Sprite** character = new Sprite*[8];
    for(int i = 0; i < 8; i++) {
        character[i] = new Sprite(characterSheet->getRect(0, i*32, 6*32, 32), 6, 160);
        cout << "STUFF: "<<character[i]->isSprite()<< endl;
    }




    while(running) {

        // clear background to black, RGB(0,0,0)
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 255, 0));

        s1->animate()->draw(screen,0,0);
        s2->animate()->draw(screen,50,0);

        s3->animate()->draw(screen,90,0);
        s4->animate()->draw(screen,140,0);

// draw s4 and s5 such that they are overlapping
        s5->animate()->draw(screen,185,0);
        s6->animate()->draw(screen,220,0);


        s7->animate()->draw(screen,0,70);
        s8->animate()->draw(screen,30,70);
        s9->animate()->draw(screen,70,70);
        s10->animate()->draw(screen,170,70);


        s11->animate()->draw(screen,210,70);
        s12->animate()->draw(screen,220,70);


        s15->animate()->draw(screen,270,0);

        // Now Draw characters
        for(int i = 0; i < 8; i++) {
            character[i]->animate()->draw(screen, i*32, 170);
        }

        SDL_Flip(screen);

        if(s11->pixelCollide(210,70,*s12,220,70)) {
         //   cout << "s11 and s12 pixel Collision = true!" << endl;
        } else {
           // cout << "s11 and s12 pixel Collision = false!" << endl;
        }

     SDL_PollEvent(&keyevent);
        switch(keyevent.type) {
            case SDL_KEYDOWN:
                printf("key down\n");
                switch(keyevent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                 running = false;
                 break;
            default:
                break;
        }
    }

    return 0;
}
