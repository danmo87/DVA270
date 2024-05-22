#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>

#define WIDTH 10
#define HEIGHT 10
#define MAX_SCORE 4

void start_game_group19();

void uarte_handler_game_group19(nrfx_uarte_event_t const *p_event, void *p_context);
