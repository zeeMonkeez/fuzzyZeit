#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

// The auto-generated header file with resource handle definitions
#include "resource_ids.auto.h"

#define MY_UUID {0x48, 0x9d, 0x61, 0x4c, 0xf8, 0x91, 0x40, 0x33, 0x86, 0x8b, 0x71, 0xc8, 0x51, 0xbf, 0x7c, 0x02}
PBL_APP_INFO(MY_UUID, "FuzzyZeit", "Jonas Zimmermann", 0x4, 0x0, \
    RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE /* App version */);

char *myHours[12] = {"Zwölf", "Eins", "Zwei", "Drei", "Vier", "Fünf", "Sechs",
    "Sieben", "Acht", "Neun", "Zehn", "Elf"};

Window window;

TextLayer min_layer;
TextLayer hour_layer;
GFont custom_font;
GFont custom_font2;

void update_layer(AppContextRef ctx) {
    (void)ctx;
    PblTm t;
    get_time(&t);

    static char min_text[] = "             ";
    static char hour_text[] = "             ";

    int my_min = t.tm_min;
    int my_hour = t.tm_hour;
    if ((my_min >= 0) && (my_min < 5)) {
        strcpy(min_text, "");
        strcpy(hour_text, "Um ");
        strcat(hour_text, myHours[my_hour%12]);
    }
    else if ((my_min >= 5) && (my_min < 10)) {
        strcpy(min_text, "Fünf Nach");
        strcpy(hour_text, "Um ");
        strcat(hour_text, myHours[my_hour%12]);
    }
    else if ((my_min >= 10) && (my_min < 15)) {
        strcpy(min_text, "Zehn Nach");
        strcpy(hour_text,  "Um ");
        strcat(hour_text, myHours[my_hour%12]);
    }
    else if ((my_min >= 15) && (my_min < 20)) {
        strcpy(min_text, "Viertel");
        strcpy(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 20) && (my_min < 25)) {
        strcpy(min_text, "Zehn Vor");
        strcpy(hour_text, "Halb ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 25) && (my_min < 30)) {
        strcpy(min_text, "Fünf Vor");
        strcpy(hour_text, "Halb ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 30) && (my_min < 35)) {
        strcpy(min_text, "");
        strcpy(hour_text, "Halb ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 35) && (my_min < 40)) {
        strcpy(min_text, "Fünf Nach");
        strcpy(hour_text, "Halb ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 40) && (my_min < 45)) {
        strcpy(min_text, "Zehn Nach");
        strcpy(hour_text, "Halb ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 45) && (my_min < 50)) {
        strcpy(min_text, "Dreiviertel");
        strcpy(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 50) && (my_min < 55)) {
        strcpy(min_text, "Zehn Vor");
        strcpy(hour_text, "Um ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }
    else if ((my_min >= 55) && (my_min < 60)) {
        strcpy(min_text, "Fünf Vor");
        strcpy(hour_text, "Um ");
        strcat(hour_text, myHours[(my_hour+1)%12]);
    }

    text_layer_set_text(&min_layer, min_text);
    text_layer_set_text(&hour_layer, hour_text);

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *tE) {
    (void)ctx;
    update_layer(ctx);
}

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "Demo");
    window_stack_push(&window, true /* Animated */);

    resource_init_current_app(&FUZZYZEIT_RESOURCES);

    custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MPC_32));
    custom_font2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MPBC_32));

    window_set_background_color(&window, GColorBlack);
    graphics_context_set_text_color(ctx, GColorWhite);

    text_layer_init(&min_layer, GRect(4, 35, 144 /* width */, 50 /* height */));
    text_layer_set_text_color(&min_layer, GColorWhite);
    text_layer_set_text(&min_layer, "");
    text_layer_set_font(&min_layer, custom_font);
    text_layer_set_background_color(&min_layer, GColorClear);

    layer_add_child(&window.layer, &min_layer.layer);
    layer_mark_dirty(&min_layer.layer);

    text_layer_init(&hour_layer, GRect(4, 80, 144 /* width */, 50 /* height */));
    text_layer_set_text_color(&hour_layer, GColorWhite);
    text_layer_set_text(&hour_layer, "");
    text_layer_set_font(&hour_layer, custom_font2);
    text_layer_set_background_color(&hour_layer, GColorClear);

    layer_add_child(&window.layer, &hour_layer.layer);
    layer_mark_dirty(&hour_layer.layer);

    update_layer(ctx);

}


void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
        .tick_info = {
            .tick_handler = &handle_minute_tick,
            .tick_units = MINUTE_UNIT
        }
    };
    app_event_loop(params, &handlers);
}

