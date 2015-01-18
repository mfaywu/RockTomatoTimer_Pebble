#include <pebble.h>
#include "TaskMenu.h"
#include "Timer.h"
#include "comm.h"
  
Window *window;
MenuLayer *menu_layer;
char str1[] = "Buy milk";

/*int zero_poms = 0;
int one_poms = 0;
int two_poms = 0;
int three_poms = 0;
int four_poms = 0;
int five_poms = 0;
int six_poms = 0;*/

void draw_row_callback (GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
  //BASE ON WHICH CASE, SET TASK STRING AND POMS_LEFT and POMS_REMAINING TODO: FOR FAY
    // Which row is it?
  
 // task_t* temp_task = get_task(cell_index->row);
  //menu_cell_basic_draw(ctx, cell_layer, temp_task->name, "Completed: ", NULL); //TODO correct the "Completed: "
  
    switch (cell_index->row) {
    case 0:
        //static char complete[] = "Completed: ";
        //snprintf(complete, 17, "Completed: %01 / 2", zero_poms);
        menu_cell_basic_draw(ctx, cell_layer, str1, "Completed: 1 / 2", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Read the WSJ", "Completed: 3 / 4", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Read book", "Completed: 1 / 5", NULL);
        break;
    case 3:
        menu_cell_basic_draw(ctx, cell_layer, "Send emails", "Completed: 2 / 3", NULL);
        break;
    case 4:
        menu_cell_basic_draw(ctx, cell_layer, "Study algorithms", "Completed: 5 / 7", NULL);
        break;
    case 5:
        menu_cell_basic_draw(ctx, cell_layer, "Walk the dog", "Completed: 0 / 1", NULL);
        break;
    case 6:
        menu_cell_basic_draw(ctx, cell_layer, "Finish photonics homework", "Completed: 1 / 7", NULL);
        break;
    }
}

uint16_t num_rows_callback (MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 7; //get_num_tasks(); //returns how many rows aka how many times to run draw_rows_callback
}


void select_click_callback (MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  int which = cell_index->row;
  
  //char* task_name = get_task(which)->name;
  //int poms_target = get_task(which)->nTarget;
  //int poms_remaining = poms_target - get_task(which)->nCompleted;
  
 /* uint32_t segments[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
  
  switch(which) {
    case 0:
        poms_remaining = 1;
      poms_target = 2;
    task_name = "Buy milk";
        break;
    case 1:
    poms_remaining = 1;
    poms_target = 4;
    task_name = "Read the WSJ";
       // menu_cell_basic_draw(ctx, cell_layer, "Read the WSJ", "Completed: 3 / 4", NULL);
        break;
    case 2:
    poms_remaining = 4;
    poms_target = 5;
    task_name = "Read book";
        //menu_cell_basic_draw(ctx, cell_layer, "Read book", "Completed: 1 / 5", NULL);
        break;
    case 3:
    poms_remaining = 1;
    poms_target = 3;
    task_name = "Send emails";
        //menu_cell_basic_draw(ctx, cell_layer, "Send emails", "Completed: 2 / 3", NULL);
        break;
    case 4:
    poms_remaining = 2;
    poms_target = 7;
    task_name = "Study algorithms";
        //menu_cell_basic_draw(ctx, cell_layer, "Study algorithms", "Completed: 5 / 7", NULL);
        break;
    case 5:
    poms_remaining = 1;
    poms_target = 1;
    task_name = "Walk the dog";
        //menu_cell_basic_draw(ctx, cell_layer, "Walk the dog", "Completed: 0 / 1", NULL);
        break;
    case 6:
    poms_remaining = 6;
    poms_target = 7;
    task_name = "Finish photonics homework";
        //menu_cell_basic_draw(ctx, cell_layer, "Finish photonics homework", "Completed: 1 / 7", NULL);
        break;
    }
  }
  pass_variables(poms_remaining, poms_target, task_name);
  timer_init();
}

void window_load1 (Window *window){
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  menu_layer = menu_layer_create(bounds);
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
  //free(tasks); remove comments when you have actual variable holding this
}