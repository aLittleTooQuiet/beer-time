#include <pebble.h>

#define GRADIENT_EDGE_WIDTH 4
#define FOAM_HEIGHT 12
#define MAX_BUBBLE_COUNT 20

/* Variable definitions */
struct Bubble {
    int16_t x;
    int16_t y;
    int16_t rad;
};

/* Function definitions */
void drawBubbles(Layer *bubbles_layer, GContext *ctx);
void contextDrawBubbles(GContext *ctx, GRect bounds);
void contextDrawBubble(GContext *ctx, int16_t x, int16_t y, int16_t rad);
void drawTimeLayer(TextLayer *time_layer, TextLayer *shadow_layer);

/* Functions */
void drawBubbles(Layer *bubbles_layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(bubbles_layer);
    contextDrawBubbles(ctx, bounds);
}

void contextDrawBubbles(GContext *ctx, GRect bounds) {
    int16_t x, y, rad;
    
    for (int i=0; i<MAX_BUBBLE_COUNT; i++) {
        x = (rand() % bounds.size.w) + 1;
        y = (rand() % bounds.size.h) + 1;
        rad = (rand() % 3) + 1;
        contextDrawBubble(ctx, x, y, rad);
    }
}

void contextDrawBubble(GContext *ctx, int16_t x, int16_t y, int16_t rad) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorPastelYellow);
    graphics_fill_circle(ctx, GPoint(x, y), rad);
    graphics_draw_circle(ctx, GPoint(x, y), rad);
}

void drawTimeLayer(TextLayer *time_layer, TextLayer *shadow_layer) {
    // Shadow
    text_layer_set_background_color(shadow_layer, GColorClear);
    text_layer_set_text_color(shadow_layer, GColorBulgarianRose);
    text_layer_set_font(shadow_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(shadow_layer, GTextAlignmentCenter);
    
    // Time
    text_layer_set_background_color(time_layer, GColorClear);
    text_layer_set_text_color(time_layer, GColorWhite);
    text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
}