#include <pebble.h>
#include "main.h"


// declarations
static Window *s_main_window;
static GFont s_custom_font_veramono_20;
static TextLayer *s_timeprompt_layer;
static TextLayer *s_time_layer;
static TextLayer *s_dateprompt_layer;
static TextLayer *s_date_layer1;
static TextLayer *s_date_layer2;
bool showtime;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create two long-lived buffers
  static char timeBuffer[] = "00:00";
  static char date1Buffer[] = " 1 Jan.";
  static char date2Buffer[] = " 1 Jan.";

  // Write the current hours and minutes into the timebuffer
  if(clock_is_24h_style() == true) {
    // Use 2h hour format
    strftime(timeBuffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(timeBuffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  // put the date into the dateBuffer
  strftime(date1Buffer, sizeof(" 1 Jan."), "%e %b.", tick_time);
  strftime(date2Buffer, sizeof("1999"), "%Y", tick_time);

  // Display this time and date on the TextLayers
  text_layer_set_text(s_time_layer, timeBuffer);
  text_layer_set_text(s_date_layer1, date1Buffer);
  text_layer_set_text(s_date_layer2, date2Buffer);
}

static void main_window_load(Window *window) {
  // construct custom font
  s_custom_font_veramono_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_VERAMONO_BOLD_20));

  // create time prompt layer 
  s_timeprompt_layer = text_layer_create(GRect(2, 2, 140, 25));
  text_layer_set_background_color(s_timeprompt_layer, GColorBlack);
  text_layer_set_text_color(s_timeprompt_layer, GColorWhite);
  text_layer_set_text(s_timeprompt_layer, "root~$ time");
  text_layer_set_font(s_timeprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_timeprompt_layer, GTextAlignmentLeft);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(2, 44, 140, 55));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

  // create date prompt layer 
  s_dateprompt_layer = text_layer_create(GRect(2, 2, 140, 25));
  text_layer_set_background_color(s_dateprompt_layer, GColorBlack);
  text_layer_set_text_color(s_dateprompt_layer, GColorWhite);
  text_layer_set_text(s_dateprompt_layer, "root~$ date");
  text_layer_set_font(s_dateprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_dateprompt_layer, GTextAlignmentLeft);
  
  // Create date TextLayer
  s_date_layer1 = text_layer_create(GRect(2, 29, 140, 25));
  text_layer_set_background_color(s_date_layer1, GColorBlack);
  text_layer_set_text_color(s_date_layer1, GColorWhite);
  text_layer_set_text(s_date_layer1, " 1 Jan.");
  text_layer_set_font(s_date_layer1, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_date_layer1, GTextAlignmentLeft);
  
  
  // Create date TextLayer 2
  s_date_layer2 = text_layer_create(GRect(2, 56, 140, 25));
  text_layer_set_background_color(s_date_layer2, GColorBlack);
  text_layer_set_text_color(s_date_layer2, GColorWhite);
  text_layer_set_text(s_date_layer2, "1999");
  text_layer_set_font(s_date_layer2, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_date_layer2, GTextAlignmentLeft);
  
  // Add it as a child layer to the Window's root layer, some commented for debug purposes
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeprompt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dateprompt_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer1));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer2));  
  // Make sure the time is displayed from the start
  showtime = true;
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_timeprompt_layer);
  text_layer_destroy(s_dateprompt_layer);
  text_layer_destroy(s_date_layer1);
  text_layer_destroy(s_date_layer2);
  // destroy custom font
  fonts_unload_custom_font(s_custom_font_veramono_20);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void tap_handler(AccelAxisType axis, int32_t direction){
  if(showtime) {
    layer_remove_from_parent(text_layer_get_layer(s_timeprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_time_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_dateprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_date_layer1));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_date_layer2));
    showtime = false;
  } else {
    layer_remove_from_parent(text_layer_get_layer(s_dateprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_date_layer1));
    layer_remove_from_parent(text_layer_get_layer(s_date_layer2));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_timeprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_time_layer));
    showtime = true;
  }
}
  

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  accel_tap_service_subscribe(tap_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  accel_tap_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
