#include <SDL2/SDL.h>
#include <complex.h>

#define WIDTH 1920
#define HEIGHT 1280
#define MAX_ITER 600

double scale = 3.0;
double offsetX = -2.0;
double offsetY = -1.5;

int mandelbrot(double complex c) {
    double complex z = 0;
    int n = 0;
    while (cabs(z) <= 2 && n < MAX_ITER) {
        z = z * z + c;
        n++;
    }
    return n;
}

void render(SDL_Renderer *renderer) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            double real = offsetX + (x / (double)WIDTH) * scale;
            double imag = offsetY + (y / (double)HEIGHT) * scale;
            int iter = mandelbrot(real + imag * I);
            int color = (255 * iter) / MAX_ITER;
            SDL_SetRenderDrawColor(renderer, color, color / 2, color / 4, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    double clickX = offsetX + (event.button.x / (double)WIDTH) * scale;
                    double clickY = offsetY + (event.button.y / (double)HEIGHT) * scale;
                    scale *= 0.5;
                    offsetX = clickX - (scale / 2);
                    offsetY = clickY - (scale / 2);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    scale *= 2;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) offsetX -= scale * 0.1;
                if (event.key.keysym.sym == SDLK_RIGHT) offsetX += scale * 0.1;
                if (event.key.keysym.sym == SDLK_UP) offsetY -= scale * 0.1;
                if (event.key.keysym.sym == SDLK_DOWN) offsetY += scale * 0.1;
            }
        }
        render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

