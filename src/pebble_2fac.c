/*
pebble-2fac - A program to calculate and display Google 2-factor
authentication codes on the Pebble smartwatch
Copyright (C) 2014 Martin Pugh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <pebble.h>
#include <stdio.h>
#include <stdlib.h>
#include "hmac.h"
#include "base32.h"
#include "gen_code.h"

static Window *window;
static TextLayer *text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    //const char* code = "JBSWY3DPEHPK3PXP";
    const char* code = "uq3q5qw5rmzcqt77f5i4pfzxnpgs7hfh";
    const uint8_t* base32 = (uint8_t*)code;
    char* out = (char*)malloc(sizeof(char)*7);
    get_code(base32,16,(int)time(NULL),out);
 	text_layer_set_text(text_layer, out);
    free(out);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 40 }, .size = { bounds.size.w, 100 } });
  text_layer_set_font(text_layer,fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text(text_layer, "------");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();

}
