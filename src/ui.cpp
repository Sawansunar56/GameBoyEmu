#include "SDL.h"
#include "ui.h"
#include "emu.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "SDL_video.h"

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

void ui_init()
{
 SDL_Init(SDL_INIT_VIDEO);
 printf("SDL INIT\n");
 TTF_Init();
 printf("TTF INIT\n");

 SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             0,
                             &sdlWindow,
                             &sdlRenderer);
}

void ui_handle_events()
{
 SDL_Event e;
 while (SDL_PollEvent(&e) > 0)
 {
  // TODO SDL_UpdateWindowSurface(sdlWindow);
  // TODO SDL_UpdateWindowSurface(sdlTraceWindow);
  // TODO SDL_UpdateWindowSurface(sdlDebugWindow);

  if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
  {
   emu_get_context()->die = true;
  }
 }
}
