#include <pebble.h>
#include <beer_time.h>

static Window *s_main_window;
static GBitmap *s_bitmap;
static BitmapLayer *s_background_layer;
static Layer *s_bubbles_layer;
static TextLayer *s_time_layer;
static TextLayer *s_time_shadow_layer;

static void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
    if (s_buffer[0] == '0') {
        for (uint i=0; i<strlen(s_buffer); i++) {
            s_buffer[i] = s_buffer[i+1];
        }
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_shadow_layer, s_buffer);
    text_layer_set_text(s_time_layer, s_buffer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_load(Window *window) {
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    // Create the background layer
    s_background_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
    s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BEER_TIME_BG_IMG);
    bitmap_layer_set_bitmap(s_background_layer, s_bitmap);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
    
    s_bubbles_layer = layer_create(bounds);
    layer_set_update_proc(s_bubbles_layer, drawBubbles);
    layer_add_child(window_layer, s_bubbles_layer);

    // Display the time
    s_time_shadow_layer = text_layer_create(GRect(1, PBL_IF_ROUND_ELSE(59, 53), bounds.size.w, 50));
    s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
    drawTimeLayer(s_time_layer, s_time_shadow_layer);
    layer_add_child(window_layer, text_layer_get_layer(s_time_shadow_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    
    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_time_shadow_layer);
    gbitmap_destroy(s_bitmap);
    layer_destroy(s_bubbles_layer);
    bitmap_layer_destroy(s_background_layer);
}

void init(void) {
    s_main_window = window_create();
    
    window_set_background_color(s_main_window, GColorBlack);

    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    window_stack_push(s_main_window, true);
    
    // Make sure the time is displayed from the start
    update_time();
}

void deinit(void) {
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
