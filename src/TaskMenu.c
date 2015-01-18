#include <pebble.h>
#include "TaskMenu.h"
#include "Timer.h"

  
Window *window;
MenuLayer *menu_layer;
char str1[] = "Noay";

void draw_row_callback (GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    // Which row is it?
    switch (cell_index->row) {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, str1, "is good at hacking pebble watches for fun", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "2. Orange", "Peel first!", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "3. Pear", "Teardrop shaped!", NULL);
        break;
    case 3:
        menu_cell_basic_draw(ctx, cell_layer, "4. Banana", "Can be a gun!", NULL);
        break;
    case 4:
        menu_cell_basic_draw(ctx, cell_layer, "5. Tomato", "Extremely versatile!", NULL);
        break;
    case 5:
        menu_cell_basic_draw(ctx, cell_layer, "6. Grape", "Bunches of 'em!", NULL);
        break;
    case 6:
        menu_cell_basic_draw(ctx, cell_layer, "7. Melon", "Only three left!", NULL);
        break;
    }
}

uint16_t num_rows_callback (MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 7;
}


void select_click_callback (MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
/*  int which = cell_index->row;
  uint32_t segments[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for (int i = 0; i <= which; ++i) {
    segments[2*i] = 200;
    segments[(2*i)+1] = 100;
  }
  VibePattern pattern = {
    .durations = segments,
    .num_segments = 16
  };
  vibes_enqueue_custom_pattern(pattern);
*/
  timer_init();
}

void window_load1 (Window *window){
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
  menu_layer_set_click_config_onto_window(menu_layer, window);
  MenuLayerCallbacks callbacks = {
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

void window_unload1 (Window *window) {
  menu_layer_destroy(menu_layer);
}

void init_menu() {
  window = window_create();
  WindowHandlers handlers = {
    .load = window_load1,
    .unload = window_unload1
  };
  window_set_window_handlers (window, (WindowHandlers) handlers);
  window_stack_push (window, true);
}

void deinit_menu() {
  window_destroy(window);
}
