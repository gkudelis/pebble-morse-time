#include <pebble.h>

static Window *window;
//static TextLayer *text_layer;

static const uint32_t const morse_digits[][9] = {
    { 300, 100, 300, 100, 300, 100, 300, 100, 300 },
    { 100, 100, 300, 100, 300, 100, 300, 100, 300 },
    { 100, 100, 100, 100, 300, 100, 300, 100, 300 },
    { 100, 100, 100, 100, 100, 100, 300, 100, 300 },
    { 100, 100, 100, 100, 100, 100, 100, 100, 300 },
    { 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    { 300, 100, 100, 100, 100, 100, 100, 100, 100 },
    { 300, 100, 300, 100, 100, 100, 100, 100, 100 },
    { 300, 100, 300, 100, 300, 100, 100, 100, 100 },
    { 300, 100, 300, 100, 300, 100, 300, 100, 100 },
};

static uint32_t *segments;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Select button pushed\n");

    const size_t element_size = sizeof(uint32_t);

    memcpy(&segments[0], &morse_digits[0][0], 9 * element_size);
    segments[9] = 300;
    memcpy(&segments[10], &morse_digits[7][0], 9 * element_size);
    segments[19] = 300;
    memcpy(&segments[20], &morse_digits[2][0], 9 * element_size);
    segments[29] = 300;
    memcpy(&segments[30], &morse_digits[4][0], 9 * element_size);

    VibePattern pat = {
        .durations = segments,
        .num_segments = 39,
    };
    vibes_enqueue_custom_pattern(pat);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void init(void) {
    segments = (uint32_t*) malloc((4*9 + 3) * sizeof(uint32_t));
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    const bool animated = false;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
    free(segments);
}

int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}
