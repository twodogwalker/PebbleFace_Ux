#include <pebble.h>
#include "main.h"
#include "functions.c"

static void main_window_load(Window *window) {
  // construct custom fonts
  s_custom_font_veramono_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_VERAMONO_BOLD_20));
  s_custom_font_veramono_44 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_VERAMONO_BOLD_44));

  // create time prompt layer 
  s_timeprompt_layer = text_layer_create(GRect(2, 2, 140, 25));
  text_layer_set_background_color(s_timeprompt_layer, backgrndColor);
  text_layer_set_text_color(s_timeprompt_layer, foregrndColor);
  text_layer_set_text(s_timeprompt_layer, "root~$ time");
  text_layer_set_font(s_timeprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_timeprompt_layer, GTextAlignmentLeft);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(2, 44, 140, 55));
  text_layer_set_background_color(s_time_layer, backgrndColor);
  text_layer_set_text_color(s_time_layer, foregrndColor);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_custom_font_veramono_44);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

  // create date prompt layer 
  s_dateprompt_layer = text_layer_create(GRect(2, 2, 140, 25));
  text_layer_set_background_color(s_dateprompt_layer, backgrndColor);
  text_layer_set_text_color(s_dateprompt_layer, foregrndColor);
  text_layer_set_text(s_dateprompt_layer, "root~$ date");
  text_layer_set_font(s_dateprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_dateprompt_layer, GTextAlignmentLeft);
  
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(2, 29, 140, 25));
  text_layer_set_background_color(s_date_layer, backgrndColor);
  text_layer_set_text_color(s_date_layer, foregrndColor);
  text_layer_set_text(s_date_layer, " 1 Jan. 99");
  text_layer_set_font(s_date_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  
  // create load prompt layer 
  s_loadprompt_layer = text_layer_create(GRect(2, 56, 140, 25));
  text_layer_set_background_color(s_loadprompt_layer, backgrndColor);
  text_layer_set_text_color(s_loadprompt_layer, foregrndColor);
  text_layer_set_text(s_loadprompt_layer, "root~$ top");
  text_layer_set_font(s_loadprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_loadprompt_layer, GTextAlignmentLeft);
  
  // Create load TextLayer
  s_load_layer = text_layer_create(GRect(2, 83, 140, 25));
  text_layer_set_background_color(s_load_layer, backgrndColor);
  text_layer_set_text_color(s_load_layer, foregrndColor);
  text_layer_set_text(s_load_layer, "99%");
  text_layer_set_font(s_load_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_load_layer, GTextAlignmentLeft);
 
  
  // create network prompt layer 
  s_networkprompt_layer = text_layer_create(GRect(2, 110, 140, 25));
  text_layer_set_background_color(s_networkprompt_layer, backgrndColor);
  text_layer_set_text_color(s_networkprompt_layer, foregrndColor);
  text_layer_set_text(s_networkprompt_layer, "root~$ lsof");
  text_layer_set_font(s_networkprompt_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_networkprompt_layer, GTextAlignmentLeft);
  
  // Create network TextLayer
  s_network_layer = text_layer_create(GRect(2, 137, 140, 25));
  text_layer_set_background_color(s_network_layer, backgrndColor);
  text_layer_set_text_color(s_network_layer, foregrndColor);
  text_layer_set_text(s_network_layer, "eth0 LISTEN");
  text_layer_set_font(s_network_layer, s_custom_font_veramono_20);
  text_layer_set_text_alignment(s_network_layer, GTextAlignmentLeft);
  
  // Add it as a child layer to the Window's root layer, some commented for debug purposes
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeprompt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dateprompt_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_loadprompt_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_load_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_networkprompt_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_network_layer));
  
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
  text_layer_destroy(s_loadprompt_layer);
  text_layer_destroy(s_load_layer);
  text_layer_destroy(s_networkprompt_layer);
  text_layer_destroy(s_network_layer);
  // destroy custom font
  fonts_unload_custom_font(s_custom_font_veramono_20);
}

void change_visual_state(bool showtime) {
  //struct TextLayer *timeLayers[] = {s_timeprompt_layer, s_time_layer};
  //struct TextLayer *otherLayers[] = {s_dateprompt_layer, s_date_layer, s_loadprompt_layer, s_load_layer,
  //                                 s_networkprompt_layer, s_network_layer};
  // if time is currently shown, remove each timeLayer, and add all others
  if(showtime) {
    layer_remove_from_parent(text_layer_get_layer(s_dateprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_date_layer));
    layer_remove_from_parent(text_layer_get_layer(s_loadprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_load_layer));
    layer_remove_from_parent(text_layer_get_layer(s_networkprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_network_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_timeprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_time_layer));
  } else {
    layer_remove_from_parent(text_layer_get_layer(s_timeprompt_layer));
    layer_remove_from_parent(text_layer_get_layer(s_time_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_dateprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_date_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_loadprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_load_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_networkprompt_layer));
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_network_layer));
  }
}

static void timer_callback(void *data) {
  // change back to time
  change_visual_state(true);
}

void tap_handler(AccelAxisType axis, int32_t direction){
  // change the visual state to show date (and other) info
  change_visual_state(false);
  // register a timer to return back to the time state
  s_timer = app_timer_register(2000, timer_callback, NULL);
}

static void init() {
  // define fore- and background colors
  defineColors();
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, backgrndColor);

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
