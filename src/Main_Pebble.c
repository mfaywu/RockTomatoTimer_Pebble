#include <pebble.h>
#include "TaskMenu.h"
#include "Timer.h"
  
int main(void) {
  init_menu();
  app_event_loop();
  deinit_menu();
}