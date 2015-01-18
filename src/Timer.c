#include <pebble.h>
#include "Timer.h"
#include "TaskMenu.h"

  
Window *my_window;
TextLayer *text_layer;
TextLayer *timer_layer;

//static AppTimer* timer = NULL;
double remaining_time = 0;
bool timer_on = false;
bool is_work = false; 

//Standard times
int work_duration = 25 * 1000; //CHANGE BACK TO MINUTES
int brk_duration = 5 * 1000; //CHANGE BACK TO MINUTES

//function definitions
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(timer_on) {
    pause_timer();
  } 
  else {
    if (remaining_time <= 0) {
      setup_timer();
    } 
    else {
      resume_timer();
    }
  }
}

/* static void select_reset_handler(ClickRecognizerRef recognizer, void *context) {
    end_timer();
} */

void click_config_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_long_click_subscribe(BUTTON_ID_SELECT, 500, (ClickHandler)select_reset_handler, NULL);

}
void setup_timer() {
  text_layer_set_text(text_layer, "setup");
  remaining_time = (brk_duration + work_duration);
  timer_on = true;
  is_work = true;
}

void pause_timer() {
  text_layer_set_text(text_layer, "Pause");
  timer_on = false;
}

void resume_timer() {
  text_layer_set_text(text_layer, "Resume");
  timer_on = true;
}
  
static AppTimer *timer;

void update_timer(void* content) {
  if (timer_on) {
    remaining_time -= 100;
    
    //////////// Update the timer display
    static char timeText[] = "00:00";

    //double totalDuration = (double)time(NULL) - startTime;
    int seconds = (int)remaining_time / 1000 % 60;
    int minutes = (int)remaining_time / 60000 % 60;
  
    if(is_work == true) {
      snprintf(timeText, 6, "%02d:%02d", minutes, seconds - 5); //CHANGE THIS 
      text_layer_set_text(text_layer, "run timer");
    }
    if(!is_work) {
      snprintf(timeText, 6, "%02d:%02d", minutes, seconds);
    }
    text_layer_set_text(timer_layer, timeText);
    /////////////
    
    if (remaining_time == brk_duration) {
      text_layer_set_text(text_layer, "break time");
      vibes_double_pulse();
      is_work = false;
    }
    if(remaining_time <= 0) {
      text_layer_set_text(text_layer, "end timer");
      timer_on = false;
      vibes_double_pulse();
      window_stack_pop(true);
    }
  }
  timer = app_timer_register(100, update_timer, NULL);
}

void timer_init(void) {
  if (my_window == NULL) {
    my_window = window_create();
    //window_set_background_color(my_window, GColorBlack);
    window_set_fullscreen(my_window, false);
    window_set_click_config_provider(my_window, (ClickConfigProvider) click_config_provider);
  
    window_set_window_handlers(my_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });    
  }
  window_stack_push(my_window, true);
  update_timer(NULL);
  setup_timer();
}

void window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  
  text_layer = text_layer_create(GRect(5, 25, 105, 35));
 // text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_text(text_layer, "RockTomatoe");
  layer_add_child(root_layer, (Layer*)text_layer);
  
  timer_layer = text_layer_create(GRect(50, 100, 50, 35));
  text_layer_set_text(timer_layer, "25:00");
  layer_add_child(root_layer, (Layer*)timer_layer);
}

void window_unload(Window *window) {
  app_timer_cancel(timer);
  text_layer_destroy(text_layer);
  text_layer_destroy(timer_layer);
}


