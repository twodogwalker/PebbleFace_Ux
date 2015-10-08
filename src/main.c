#include <pebble.h>
#include "main.h"


// declarations
static Window *s_main_window;
static GFont s_custom_font_veramono_20;
static TextLayer *s_timeprompt_layer;
static TextLayer *s_time_layer;
static TextLayer *s_dateprompt_layer;
static TextLayer *s_date_layer;
static TextLayer *s_loadprompt_layer;
static TextLayer *s_load_layer;
static TextLayer *s_networkprompt_layer;
static TextLayer *s_network_layer;
bool showtime;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create two long-lived buffers
  static char timeBuffer[] = "00:00";
  static char dateBuffer[] = " 1 Jan. 99";

  // Write the current hours and minutes into the timebuffer
  if(clock_is_24h_style() == true) {
    // Use 2h hour format
    strftime(timeBuffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(timeBuffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  // put the date into the dateBuffer
  strftime(dateBuffer, sizeof(" 1 Jan. 99"), "%e %b. %y", tick_time);

  // Display this time and date on the TextLayers
  text_layer_set_text(s_time_layer, timeBuffer);
  text_layer_set_text(s_date_layer, dateBuffer);
}

static void battery_handler(BatteryChargeState new_state) {
  // Write to buffer and display
  static char s_battery_buffer[32];
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%%CPU %d", new_state.charge_percent);
  text_layer_set_text(s_load_layer, s_battery_buffer);
}

static void bt_handler(bool connected) {
  // Show current connection state
  if (connected) {
    text_layer_set_text(s_network_layer, "eth0 UP");
  } else {
    text_layer_set_text(s_network_layer, "eth0 DOWN");
  }
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
  s_date_layer = text_layer_create(GRect(2, 29, 140, 25));
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, " 1 Jan. 99");
  text_layer_set_font(s_date_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  
  // create load prompt layer 
  s_loadprompt_layer = text_layer_create(GRect(2, 56, 140, 25));
  text_layer_set_background_color(s_loadprompt_layer, GColorBlack);
  text_layer_set_text_color(s_loadprompt_layer, GColorWhite);
  text_layer_set_text(s_loadprompt_layer, "root~$ top");
  text_layer_set_font(s_loadprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_loadprompt_layer, GTextAlignmentLeft);
  
  // Create load TextLayer
  s_load_layer = text_layer_create(GRect(2, 83, 140, 25));
  text_layer_set_background_color(s_load_layer, GColorBlack);
  text_layer_set_text_color(s_load_layer, GColorWhite);
  text_layer_set_text(s_load_layer, "99%");
  text_layer_set_font(s_load_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_load_layer, GTextAlignmentLeft);
 
  
  // create network prompt layer 
  s_networkprompt_layer = text_layer_create(GRect(2, 110, 140, 25));
  text_layer_set_background_color(s_networkprompt_layer, GColorBlack);
  text_layer_set_text_color(s_networkprompt_layer, GColorWhite);
  text_layer_set_text(s_networkprompt_layer, "root~$ lsof");
  text_layer_set_font(s_networkprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_networkprompt_layer, GTextAlignmentLeft);
  
  // Create network TextLayer
  s_network_layer = text_layer_create(GRect(2, 137, 140, 25));
  text_layer_set_background_color(s_network_layer, GColorBlack);
  text_layer_set_text_color(s_network_layer, GColorWhite);
  text_layer_set_text(s_network_layer, "eth0 LISTEN");
  text_layer_set_font(s_network_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_network_layer, GTextAlignmentLeft);
  
  // Add it as a child layer to the Window's root layer, some commented for debug purposes
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeprompt_layer));
 // layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dateprompt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_loadprompt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_load_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_networkprompt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_network_layer));
  
  // Make sure the time is displayed from the start
  showtime = true;
  update_time();
  
  // Get the current battery level
  battery_handler(battery_state_service_peek());
  
  // Show current connection state
  bt_handler(bluetooth_connection_service_peek());
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_timeprompt_layer);
  text_layer_destroy(s_dateprompt_layer);
  text_layer_destroy(s_date_layer);
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
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_date_layer));
    showtime = false;
  } else {
    layer_remove_from_parent(text_layer_get_layer(s_dateprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_date_layer));
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
  
  // Subscribe to the Battery State Service
  battery_state_service_subscribe(battery_handler);
  
  // Subscribe to Bluetooth updates
  bluetooth_connection_service_subscribe(bt_handler);
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
