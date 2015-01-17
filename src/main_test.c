#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
TextLayer *timer_layer;

//static AppTimer* timer = NULL;
static double remaining_time = 0;
static bool timer_on = false;
static bool is_work = false; 

//Standard times
static int work_duration = 25 * 60 * 1000;
static int brk_duration = 5 * 60 * 1000;

//functions
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
//static void select_reset_handler(ClickRecognizerRef recognizer, void *context);
static void click_config_provider(Window *window);
  
static void setup_timer();
static void pause_timer(void);
static void resume_timer(void);
static void update_timer(void* content);

static void handle_init(void);
static void handle_deinit(void);
static void window_load(Window *window);
static void window_unload(Window *window);

//function definitions
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
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

/*static void select_reset_handler(ClickRecognizerRef recognizer, void *context) {
    end_timer();
}*/

static void click_config_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_long_click_subscribe(BUTTON_ID_SELECT, 500, (ClickHandler)select_reset_handler, NULL);

}
static void setup_timer() {
  text_layer_set_text(text_layer, "setup");
  remaining_time = (brk_duration + work_duration);
  timer_on = true;
  is_work = true;
}

static void pause_timer() {
  text_layer_set_text(text_layer, "Pause");
  timer_on = false;
}

static void resume_timer() {
  text_layer_set_text(text_layer, "Resume");
  timer_on = true;
}
  
static void update_timer(void* content) {
  if (timer_on) {
    remaining_time -= 100;
    
    //////////// Update the timer display
    static char timeText[] = "00:00";

    //double totalDuration = (double)time(NULL) - startTime;
    int seconds = (int)remaining_time / 1000 % 60;
    int minutes = (int)remaining_time / 60000 % 60;
  
    if(is_work == true) {
      snprintf(timeText, 6, "%02d:%02d", minutes - 5, seconds); 
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
    }
  }
  app_timer_register(100, update_timer, NULL);
}

static void handle_init(void) {
  my_window = window_create();
  //window_set_background_color(my_window, GColorBlack);
  window_set_fullscreen(my_window, false);
  
  window_set_click_config_provider(my_window, (ClickConfigProvider) click_config_provider);

  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_text(text_layer, "Start timer");
  window_stack_push(my_window, true);
  update_timer(NULL);
}

static void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

static void window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  
  text_layer = text_layer_create(GRect(5, 25, 105, 35));
 // text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_text(text_layer, "RockTomatoe");
  layer_add_child(root_layer, (Layer*)text_layer);
  
  timer_layer = text_layer_create(GRect(50, 100, 50, 35));
  text_layer_set_text(timer_layer, "25:00");
  layer_add_child(root_layer, (Layer*)timer_layer);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
