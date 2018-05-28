#include <sepia.hpp>  //  Alex
#include "SDL2/SDL.h"


SDL_Renderer    *renderer;
SDL_Window      *window;
int     ImageSizeX = 640, ImageSizeY = 480;
int     scale = 2;
long    et_thr=0;





void handleEvent(sepia::Event event)  // handleEvent is an event callback for an event stream observable.
{
    int     ex, ey;
    SDL_Rect    fillRect;

    if (event.isThresholdCrossing) return;

    ex = event.x;
    ey = event.y;
    
    fillRect.x = ex*scale;   fillRect.y = ey*scale;   fillRect.w = scale;   fillRect.h = scale;
    if (event.polarity==0) SDL_SetRenderDrawColor(renderer, 0,0,220, 255);
    if (event.polarity==1) SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
    SDL_RenderFillRect(renderer, &fillRect);

    if (event.timestamp>et_thr) {
        SDL_RenderPresent(renderer);
        et_thr += 5000;
    }

}


/// handleException does nothing.
void handleException(std::exception_ptr) {}

int main()
{
    int   x, y;
    int   result;
    int   WinSizeX = 800, WinSizeY = 600;

 ///////////////////////////////


    result = SDL_Init(SDL_INIT_VIDEO);   if (result<0) return(0);
    printf("SDL Init OK.\n");

    window = SDL_CreateWindow("SDL_RendererClear",0,20, WinSizeX,WinSizeY, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 212,208,200, 255);
    SDL_RenderClear(renderer);


    auto eventStreamObservable = sepia::make_eventStreamObservable("../Data/sepiaTest.es", handleEvent01, handleException);

    SDL_Delay(10000);
    SDL_Quit();
    return (0);
}
