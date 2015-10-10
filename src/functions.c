#include <pebble.h>
#include "main.h"
 
static void defineColors() {
  #if defined(PBL_COLOR) 
    foregrndColor = GColorBrightGreen;
    backgrndColor = GColorBlack;
  #elif defined(PBL_BW)
    foregrndColor = GColorWhite;
    backgrndColor = GColorBlack;
  #endif
}
  
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

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

