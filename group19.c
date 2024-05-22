#include "stdlib.h"
#include <19.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>
#include <stdint.h>
#include <stdio.h>

static uint8_t Read_Buffer;
static uint8_t Print_Buffer[100];
static uint8_t Clear_Screen[] = "\033c";

extern nrfx_uarte_t uarte_instance;
extern const nrfx_rtc_t rtc_instance;

static int x = 0, y = 0;
static int target_x = 5;
static int target_y = 5;
static int score = 0;
static int currentTime = 0;       // time in ms
static int highscore = 9999999999;  // time in ms
// taken from https://stackoverflow.com/a/14997413
static int positive_modulo(int i, int n) { return (i % n + n) % n; }

static void waitTX(nrfx_uarte_t *instance) {
  while (nrfx_uarte_tx_in_progress(instance)) {
    // do nothing, just wait
  }
  return;
}

void drawGrid(int x_pos, int y_pos, int target_x, int target_y,
              uint8_t backround, uint8_t player, uint8_t target) {

  static uint8_t grid[HEIGHT][WIDTH];
  nrfx_uarte_tx(&uarte_instance, Clear_Screen, sizeof(Clear_Screen), 0);
  waitTX(&uarte_instance);
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (x == x_pos && y == y_pos) {
        grid[y][x] = player;
      } else if (x == target_x && y == target_y) {
        grid[y][x] = target;
      } else {
        grid[y][x] = backround;
      }
    }
  }
  for (int row = HEIGHT - 1; row >= 0; row--) {
    nrfx_uarte_tx(&uarte_instance, grid[row], sizeof(grid[row]), 0);
    waitTX(&uarte_instance);
    static uint8_t newRow[] = "\n\r";
    nrfx_uarte_tx(&uarte_instance, newRow, sizeof(newRow), 0);
    waitTX(&uarte_instance);
  }
}
void uarte_handler_game_group19(nrfx_uarte_event_t const *p_event,
                                void *p_context) {
  nrfx_uarte_t *p_inst = p_context;
  if (p_event->type == NRFX_UARTE_EVT_RX_DONE) {
    switch (Read_Buffer) {
    case 'w':
      y = positive_modulo(y + 1, HEIGHT);
      nrfx_rtc_enable(&rtc_instance);
      break;
    case 's':
      y = positive_modulo(y - 1, HEIGHT);
      nrfx_rtc_enable(&rtc_instance);
      break;
    case 'a':
      x = positive_modulo(x - 1, WIDTH);
      nrfx_rtc_enable(&rtc_instance);
      break;
    case 'd':
      x = positive_modulo(x + 1, WIDTH);
      nrfx_rtc_enable(&rtc_instance);
      break;
    }

    if (target_x == x && target_y == y) {
      score++;
      do {
        target_y = (rand() % 9) + 1;
        target_x = (rand() % 9) + 1;
      } while (target_x == x && target_y == y);
    }
    if (score >= MAX_SCORE) {
      currentTime = nrfx_rtc_counter_get(&rtc_instance) / 32.768;
      score = 0;
      nrfx_rtc_counter_clear(&rtc_instance);
      nrfx_rtc_disable(&rtc_instance);
      if (highscore > currentTime) {
        highscore = currentTime;
      }
    }
    srand(score + Read_Buffer);
    nrfx_uarte_rx(&uarte_instance, &Read_Buffer, sizeof(Read_Buffer));
  }
}

void start_game_group19() {
  // clear the screen
  nrfx_rtc_disable(&rtc_instance);
  nrfx_uarte_tx(&uarte_instance, Clear_Screen, sizeof(Clear_Screen), 0);
  waitTX(&uarte_instance); // wait for the TX event to be done \
  // (its nonblocking, so we make it blocking)

  // start user input (it is self starting after first start)
  nrfx_uarte_rx(&uarte_instance, &Read_Buffer, sizeof(Read_Buffer));

  while (1) {

    drawGrid(x, y, target_x, target_y, '-', 'X', 'O');

    currentTime = nrfx_rtc_counter_get(&rtc_instance) / 32.768;
    int len = sprintf(Print_Buffer, "Time: %d\n\r", currentTime);
    nrfx_uarte_tx(&uarte_instance, Print_Buffer, len, 0);
    waitTX(&uarte_instance);

    len = sprintf(Print_Buffer, "highscore  %d\n\r", highscore);
    nrfx_uarte_tx(&uarte_instance, Print_Buffer, len, 0);
    waitTX(&uarte_instance);

    len = sprintf(Print_Buffer, "Score: %d\n\r", score);
    nrfx_uarte_tx(&uarte_instance, Print_Buffer, len, 0);
    waitTX(&uarte_instance);

    nrfx_systick_delay_ms(50);
  }
}
