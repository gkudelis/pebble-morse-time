#include <pebble.h>

// dot duration in miliseconds
#define DD 100

static Window *window;
//static TextLayer *text_layer;

// size of one time element
const size_t element_size = sizeof(uint32_t);
// array of sequences represening vibration patterns for digits
static const uint32_t const morse_digits[][9] = {
    { 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD },
    { 1*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD },
    { 1*DD, 1*DD, 1*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD },
    { 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 3*DD, 1*DD, 3*DD },
    { 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 3*DD },
    { 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD },
    { 3*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD },
    { 3*DD, 1*DD, 3*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD, 1*DD },
    { 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 1*DD, 1*DD, 1*DD },
    { 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 3*DD, 1*DD, 1*DD },
};

static uint32_t *durations;

static void fill_durations_array(uint32_t *durations, char *time) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, time);
    // read time string and use the numerical characters
    memcpy(&durations[0], &morse_digits[0][0], 9 * element_size);
    durations[9] = 3 * DD;
    memcpy(&durations[10], &morse_digits[7][0], 9 * element_size);
    durations[19] = 7 * DD;
    memcpy(&durations[20], &morse_digits[2][0], 9 * element_size);
    durations[29] = 3 * DD;
    memcpy(&durations[30], &morse_digits[4][0], 9 * element_size);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    fill_durations_array(durations, "07:24");
    VibePattern pat = {
        .durations = durations,
        .num_segments = 39,
    };
    vibes_enqueue_custom_pattern(pat);
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void init(void) {
    durations = (uint32_t*) malloc((4*9 + 3) * sizeof(uint32_t));
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    const bool animated = false;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
    free(durations);
}

int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}
