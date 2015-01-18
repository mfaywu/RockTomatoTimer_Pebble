#include <pebble.h>
#include "Timer.h"
#include "TaskMenu.h"

  
Window *my_window;
TextLayer *work_break_textlayer;
TextLayer *timer_layer;

static int poms_remain;
static int poms_targ;
static char* name_of_task;

//UI generated code
static GFont s_res_bitham_34_medium_numbers;
static GFont s_res_gothic_14;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_play_image;
static GBitmap *s_res_pause_image;
static TextLayer *task_textlayer;
static TextLayer *poms_left_textlayer;
//static TextLayer *work_break_textlayer;
static TextLayer *poms_num_textlayer;
static BitmapLayer *play_pause_img;

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
  //text_layer_set_text(work_break_textlayer, "Start");
  remaining_time = (brk_duration + work_duration);
  timer_on = true;
  is_work = true;
}

static void update_feedback(GBitmap *bitmap, char *text) {
  text_layer_set_text(work_break_textlayer, text);
  bitmap_layer_set_bitmap(play_pause_img, bitmap);
}

void pause_timer() {
  update_feedback(s_res_play_image, "Pause");
  timer_on = false;
}

void resume_timer() {
  //text_layer_set_text(work_break_textlayer, "Resume");
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
      update_feedback(s_res_pause_image, "Work");
    }
    if(!is_work) {
      snprintf(timeText, 6, "%02d:%02d", minutes, seconds);
    }
    text_layer_set_text(timer_layer, timeText);
    /////////////
    
    if (remaining_time == brk_duration) {
      update_feedback(s_res_pause_image, "Break");

      vibes_double_pulse();
      is_work = false;
    }
    if(remaining_time <= 0) {
      update_feedback(s_res_play_image, "Done!");

      timer_on = false;
      vibes_double_pulse();
      window_stack_pop(true); // .disappear, .unload -> cancel_timer == no effect
    }
  }
  timer = app_timer_register(100, update_timer, NULL);
}

static void window_disappear(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_disappear");
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
      .disappear = window_disappear,
    });    
  }
  window_stack_push(my_window, true);

  setup_timer();
  if (timer == NULL) {
    update_timer(NULL);
  }
}

void window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load");
  //Layer *root_layer = window_get_root_layer(window); //TODO: Not sure if I need this
  
  /////////////////DELETE THIS CODE
  //text_layer = text_layer_create(GRect(5, 25, 105, 35));
  //text_layer_set_text(text_layer, "RockTomatoe");
  //layer_add_child(root_layer, (Layer*)text_layer);
  /////////////////DELETE THIS CODE
  
  //auto-generated code
  //s_window = window_create();
  window_set_background_color(my_window, GColorBlack);
  window_set_fullscreen(my_window, false);
  
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_play_image = gbitmap_create_with_resource(RESOURCE_ID_play_image);
  s_res_pause_image = gbitmap_create_with_resource(RESOURCE_ID_pause_image);
  // timer_layer
  timer_layer = text_layer_create(GRect(18, 75, 100, 49));
  text_layer_set_background_color(timer_layer, GColorBlack);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text(timer_layer, "25:00");
  text_layer_set_text_alignment(timer_layer, GTextAlignmentLeft);
  text_layer_set_font(timer_layer, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(my_window), (Layer *)timer_layer);
  
  //TODO: Need to put in //CURRENTLY STATIC NEED TO MAKE DYNAMIC
  // task_textlayer
  task_textlayer = text_layer_create(GRect(6, 7, 131, 32));
  text_layer_set_background_color(task_textlayer, GColorBlack);
  text_layer_set_text_color(task_textlayer, GColorWhite);
  text_layer_set_text(task_textlayer, name_of_task);
  text_layer_set_font(task_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(my_window), (Layer *)task_textlayer);
  
  // poms_left_textlayer
  poms_left_textlayer = text_layer_create(GRect(34, 122, 56, 20));
  text_layer_set_background_color(poms_left_textlayer, GColorBlack);
  text_layer_set_text_color(poms_left_textlayer, GColorWhite);
  text_layer_set_text(poms_left_textlayer, "Poms Left:");
  text_layer_set_font(poms_left_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(my_window), (Layer *)poms_left_textlayer);
  
  // work_break_textlayer
  work_break_textlayer = text_layer_create(GRect(17, 38, 100, 30));
  text_layer_set_background_color(work_break_textlayer, GColorBlack);
  text_layer_set_text_color(work_break_textlayer, GColorWhite);
  //text_layer_set_text(work_break_textlayer, "Work");
  text_layer_set_text_alignment(work_break_textlayer, GTextAlignmentCenter);
  text_layer_set_font(work_break_textlayer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(my_window), (Layer *)work_break_textlayer);
  
  //TODO: Need to fit in //CURRENTLY STATIC - HOW TO GET IT DYNAMIC
  // poms_num_textlayer 
  poms_num_textlayer = text_layer_create(GRect(94, 122, 24, 20));
  text_layer_set_background_color(poms_num_textlayer, GColorBlack);
  text_layer_set_text_color(poms_num_textlayer, GColorWhite);
  char* poms_nums = "0/0";

   snprintf(poms_nums, 6, "%01d/%01d", poms_remain, poms_targ);
  text_layer_set_text(poms_num_textlayer, poms_nums); 
  text_layer_set_font(poms_num_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(my_window), (Layer *)poms_num_textlayer);
  
  //TODO: Need to fit in ASK: What's the best way to change the image without reloading every frame?
  // play_pause_img
  play_pause_img = bitmap_layer_create(GRect(123, 64, 20, 20));
  bitmap_layer_set_bitmap(play_pause_img, s_res_play_image);
  layer_add_child(window_get_root_layer(my_window), (Layer *)play_pause_img);
}

void window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_unload");
  app_timer_cancel(timer);
  timer = NULL;
 // text_layer_destroy(text_layer);
  text_layer_destroy(timer_layer);
  
  //text_layer_destroy(time_display_textlayer);
  text_layer_destroy(task_textlayer);
  text_layer_destroy(poms_left_textlayer);
  text_layer_destroy(work_break_textlayer);
  text_layer_destroy(poms_num_textlayer);
  bitmap_layer_destroy(play_pause_img);
  gbitmap_destroy(s_res_play_image);
  gbitmap_destroy(s_res_pause_image);
}

void pass_variables(int poms_remaining, int poms_target, char* task_name) {
  poms_remain = poms_remaining;
  poms_targ = poms_target;
  name_of_task = task_name; //MESSING THE POINTER UP TODO
}


