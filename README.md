sdl_sprite
==========
This is a simple(and old) Sprite class that I used for game development in C++ using SDL (www.libsdl.org).
Features include sprite animation, rotation, stretching, transparencies, and other commonly used sprite functions.

I hope this is somewhat useful. The source is available and should be fairly simple to work with.
Feel free to modify it however you want. please create an issue about any bugs are suggestions that you have. Thanks.
Original blog post: http://ken-soft.com/2009/01/02/sprite-class-in-c-using-sdl/

<b>Compile & Run</b>
```
g++ *.cpp -lSDL
./a.out
```
<b>Usage</b>

```c++
    // Initialize SDL, etc
   ...
   ...
   Sprite* s1 = new Sprite("sprites/samus_normal_run.bmp",10,60); // load a BMP that contains 10 frames
                                                // set the animation speed to 60 milliseconds
      // set RGB(255,0,255) as transparent, rotate 180 degrees, flip horizontal and reverse animation
  s1->setTransparency(255,0,255)->rotate180()->flipHorizontal()->reverseAnimation();
   // etc
    ...
    ...
 
    // Main loop
    // clear background to black, RGB(0,0,0)
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
    // animate and draw the sprite
    s1->animate()->draw(screen,0,0);
    SDL_Flip(screen);
```

<table>
<tr>
<td><img src="http://www.ken-soft.com/code/c/SDL/Sprite/samus_normal_run.bmp"/></td>
<td><img src="http://www.ken-soft.com/code/c/SDL/Sprite/sshot.PNG"/></td>
</tr>
</table>
