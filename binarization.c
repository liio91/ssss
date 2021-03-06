#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include <err.h>
#include "toolstr.h"
//#include "pixel.h"
#include "detect.h"

static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}

t_binary_image *bitmap_to_binaryimage(SDL_Surface *image)
{
  int i;
  int j;
  Uint32 black;
  t_binary_image *pic;

  black = SDL_MapRGB(image->format,0x00,0x00,0x00);
  pic = NULL;
  pic = malloc(sizeof(t_binary_image));
  // pic->name = filename;
  pic->name = "hgvhgv";
  pic->width = image->w;
  pic->height = image->h;
  pic->data = malloc(pic->height * pic->width * sizeof(int));
  for (i = 0; i<pic->width;i++ )
    {
      pic->data[i] = malloc(pic->width * pic->height * sizeof(int));
      
    }
  
  SDL_LockSurface(image);
  /*
  printf("image.w = %d\n", image->w);
  printf("pic.w = %d\n", pic->width);
  printf("image.h = %d\n", image->h);
  printf("pic.h = %d\n", pic->height);
  
  */
  for(i = 0; i < image->h; i++)
      {
	 printf("i = %d\n",i);
       for(j = 0; j < image->w;j++)
	{
	   printf("j = %d\n",j);
	  if (getpixel(image,j,i) == black)
	    {
	       printf("black\n");
	      pic->data[j][i] = 1;
	    }
	  else
	    {
	       printf("white\n");
	      pic->data[j][i] = 0;
	    }
	    }
  
	}
  pic->hproj = NULL;
  SDL_UnlockSurface(image);
 
  //free(pic);
  //free(pic->data);
  return(pic);
}
void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

void free_pic(t_binary_image *pic)
{
  int i;

  if (pic != NULL)
    {
      free(pic->hproj);
      pic->hproj = NULL;
      for (i=0; i < pic->height; i++)
	{
	  free(pic->data[i]);
	  pic->data[i] = NULL;
	}
      free(pic->data);
      pic->data= NULL;
      free(pic);
      pic = NULL;
    }
}

int main()
{
  
  SDL_Surface *image;
  image = NULL;
  t_binary_image *pic;
  pic = NULL;
  // display_image(image);
   image = SDL_LoadBMP("Lorem-Droid.bmp");
  // image = SDL_LoadBMP("incruster-image-texte-02.bmp");
  // image = SDL_LoadBMP("happy.bmp");
  pic = bitmap_to_binaryimage(image);
  
  init_sdl();
  display_image(image);
  
  if (pic != NULL){
  
    for(int i = 0; i < pic->height; i++)
      {
	for(int j = 0; j < pic->width;j++)
	  {
	  
	    printf("%d ",pic->data[i][j]);
	  
	  }
	printf("\n");
	printf("\n");
      }
  }
  free_pic(pic);
  return 0;
}
