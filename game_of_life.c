/*
 * Made by Boiiterra
 * Trying to implement game of life again.
 * Inspired by:
 * 1. https://github.com/tsoding/la/ ball.c
 * 2. https://youtu.be/kLj-H1K317U
 *
 */

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 96
#define HEIGHT 32
#define LEN (WIDTH * HEIGHT)
#define ABOVE (LEN - WIDTH)
#define BELOW (LEN + WIDTH)

static char screen[WIDTH * HEIGHT];

static int clamp(int i) {
  // Prob needs better name. :P
  // Cycle or loop? from 0 to LEN ignore negative
  if (i < 0) {
    printf("Please don't use negative numbers with this function. It wasn't "
           "designed for that.\n");
    exit(1);
  }
  return i % (LEN);
}

static void init(void) {
  char *ptr = screen;
  size_t n = LEN;
  while (n-- > 0)
    *ptr++ = (rand() % 6 > 3) ? '@' : ' ';
}

static void update(void) {
  char copy[LEN];
  strcpy(copy, screen);
  for (int p = 0; p < LEN; p++) {
    int alive_neighbors = 0;
    // Check upper neighbors
    if (copy[clamp(p + ABOVE - 1)] == '@')
      ++alive_neighbors;
    if (copy[clamp(p + ABOVE)] == '@')
      ++alive_neighbors;
    if (copy[clamp(p + ABOVE + 1)] == '@')
      ++alive_neighbors;

    // Check side ones
    if (copy[clamp(p + LEN - 1)] == '@')
      ++alive_neighbors;
    if (copy[clamp(p + LEN + 1)] == '@')
      ++alive_neighbors;

    // Check bottom
    if (copy[clamp(p + BELOW - 1)] == '@')
      ++alive_neighbors;
    if (copy[clamp(p + BELOW)] == '@')
      ++alive_neighbors;
    if (copy[clamp(p + BELOW + 1)] == '@')
      ++alive_neighbors;

    // RULES
    if (((alive_neighbors == 3 || alive_neighbors == 2) && copy[p] == '@') ||
        (alive_neighbors == 3 && copy[p] == ' '))
      screen[p] = '@';
    else
      screen[p] = ' ';
  }
}

static void show(void) {
  for (int y = 0; y < HEIGHT; ++y) {
    fwrite(&screen[y * WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
}

static void back(void) { // Move cursor to the top left angle of the frame
  printf("\x1b[%dD", WIDTH);
  printf("\x1b[%dA", HEIGHT);
}

#define FPS 15

int main(void) {
  srand(time(NULL));
  init();
  while (1) {
    update();
    show();
    back();
    usleep(1000 * 1000 / FPS);
  }
  return 0;
}
