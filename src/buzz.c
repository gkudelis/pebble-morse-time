#include <pebble.h>

// dot duration in miliseconds
#define DD 100

static Window *window;
//static TextLayer *text_layer;

static uint32_t *durations;

static size_t append_digit(uint32_t *position, char digit) {
    // takes a position where it can start writing elements and a digit
    // returns the number of elements appended so position can be advanced
    int d = digit - '0';
    if (d == 0) {
        // append dashes
        for (int i=0; i<5; i++) {
            position[2*i] = 3*DD;
            position[2*i + 1] = DD;
        }
    } else if (d < 6) {
        // append dots
        for (int i=0; i<5; i++) {
            if (i < d) {
                position[2*i] = DD;
                position[2*i + 1] = DD;
            } else {
                position[2*i] = 3*DD;
                position[2*i + 1] = DD;
            }
        }
    } else {
        // append dashes
        for (int i=0; i<d-5; i++) {
            if (i < d) {
                position[2*i] = 3*DD;
                position[2*i + 1] = DD;
            } else {
                position[2*i] = DD;
                position[2*i + 1] = DD;
            }
        }
    }
    // pause after digit is longer
    position[9] = 3*DD;
    // we added 5 new ./- and 4 spaces, next element is 10 items down
    return 10;
}

// fill durations based on time string, return the number of segments
static size_t fill_durations_array(uint32_t *durations, char *time) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, time);
    // track how many elements were written to the durations array
    // NOTE: spaces count as elements
    size_t elements_written = 0;
    if (time[1] == ':') {
        // hour only has a single digit
        //elements_written += append_digit(&durations[elements_written], '0');
        elements_written += append_digit(&durations[elements_written], time[0]);
        // pause after hours is longer
        durations[elements_written - 1] = 7*DD;
        elements_written += append_digit(&durations[elements_written], time[2]);
        elements_written += append_digit(&durations[elements_written], time[3]);
    } else {
        // hour has two digits
        elements_written += append_digit(&durations[elements_written], time[0]);
        elements_written += append_digit(&durations[elements_written], time[1]);
        // pause after hours is longer
        durations[elements_written - 1] = 7*DD;
        elements_written += append_digit(&durations[elements_written], time[3]);
        elements_written += append_digit(&durations[elements_written], time[4]);
    }
    // the last element in durations is a pause, hence subtract 1
    return elements_written - 1;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    // get current time and fill durations array based on the time
    char *timestr = malloc(10);
    clock_copy_time_string(timestr, 8);
    size_t durations_size = fill_durations_array(durations, timestr);
    free(timestr);

    // create and queue the vibrations pattern
    // this call is async so we can't just free durations array memory
    VibePattern pat = {
        .durations = durations,
        .num_segments = durations_size,
    };
    vibes_enqueue_custom_pattern(pat);
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void init(void) {
    durations = (uint32_t*) malloc(40 * sizeof(uint32_t));
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
