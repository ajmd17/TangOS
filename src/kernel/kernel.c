#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/screen.h>
#include <kernel/terminal.h>
#include <kernel/keyboard.h>
#include <kernel/mouse.h>
#include <kernel/memory.h>
#include <kernel/vgascreen.h>
#include <kernel/sys_time.h>
#include <kernel/power.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isrs.h>
#include <kernel/image.h>
#include <kernel/ata_pio.h>

#include <img/img_error_small.h>
#include <img/img_warn_small.h>
#include <img/img_info_small.h>
#include <img/img_trash_small.h>
#include <img/img_pencil_small.h>
#include <img/img_folder_small.h>
#include <img/img_poop_small.h>
#include <img/img_smile_small.h>
#include <img/img_tangy_logo.h>

#include <img/cursor/img_cursor_p_1.h>
#include <img/cursor/img_cursor_p_2.h>
#include <img/cursor/img_cursor_p_3.h>
#include <img/cursor/img_cursor_p_4.h>
#include <img/cursor/img_cursor_p_5.h>

#include <img/font/img_consolas.h>
#include <img/font/img_dejavu_sans_mono.h>

#include <gui/gui.hpp>
#include <gui/workspace.h>
#include <gui/widget.hpp>
#include <gui/button.h>

#include <app/welcom/welcom.h>
#include <app/tonsole/tonsole.h>

#include <util/logging.h>

vga_screen *screen_ptr = NULL;

// workspace holds all windows and on screen elements
workspace_t *workspace = NULL;
// a welcome app
app_t *welcom_app = NULL;
// a tonsole app
app_t *tonsole_app = NULL;

int background_color = COLOR_DARK_GRAY;

int smiley_x = 135, smiley_y = 15;
gui::cursor cur;

/*static const char *commands[] = {
  "help",
  "quit",
  "start"-
};

#define COMMANDS_SIZE 3*/

typedef struct {
    char *name;
    void (*callback)();
} cl_function_t;

cl_function_t *alloc_cl_function(const char *name, void (*callback)()) {
    cl_function_t *func = (cl_function_t*)malloc(sizeof(cl_function_t));
    // copy string for name
    int len = strlen(name);
    func->name = (char*)malloc(len + 1);
    func->name[len] = '\0';
    strcpy(func->name, name);
    // set callback
    func->callback = callback;
    return func;
}

void free_cl_function(cl_function_t *func) {
    // free the string
    free(func->name);
    // free the object
    free(func);
}

static cl_function_t *cl_functions[10];
static int cl_function_counter = 0;

void left_click() {
    if (workspace != NULL) {
        click_workspace(workspace, cur.x, cur.y);
    }
}

void help_func() {
    printf("help - shows help menu\n");
    printf("start - starts TangOS in VGA mode\n");
}

#if defined(__cplusplus)
extern "C"
#endif

void init() {
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();

    mouse_install();
    keyboard_install();


    terminal_initialize();
    terminal_setcolor(make_color(COLOR_LIGHT_RED, COLOR_BLACK));
    printf("Welcome to TangOS\n");
    terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
    
    putchar('\n');

    ata_pio_install();

    size_t lba = 5000000;

    //write(lba, "NUGGG", 1);

	char *buf;
    buf = read(lba, 1);
    printf("STRING: %s\n", buf);
    free(buf);

    // set up command line functions
    cl_functions[cl_function_counter++] = alloc_cl_function("help", help_func);

    char command[256];
    while (true) {
        terminal_setcolor(make_color(COLOR_LIGHT_GREEN, COLOR_BLACK));
        printf("tangy");
        terminal_setcolor(make_color(COLOR_LIGHT_RED, COLOR_BLACK));
        printf("$");
        terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
        printf(" ");

        terminal_readstring(command, 256);

        bool found = false;

        //if (strcmp(command, "quit") == 0) {
        for (int i = 0; i < cl_function_counter; i++) {
            if (!strcmp(command, cl_functions[i]->name)) {
                // trigger function
                cl_functions[i]->callback();
                found = true;
                break;
            }
        }
        if (!found) {
            if (!strcmp(command, "start")) {
                break;
            } else {
                printf("Invalid command: %s\n", command);
            }
        }
    }
}

#if defined(__cplusplus)
extern "C"
#endif
void main() {
    bool loaded = false;

    mouse_bind_event(mouse_left_release_event, left_click);
    vga_screen screen = vga_init_320_200_256();
    screen_ptr = &screen;

    workspace = alloc_workspace();
    if (workspace == NULL) {
        // not enough memory to alloc workspace!
        puts("Not enough memory to allocate workspace. halting...");
        while(true) PAUSE;
    }

    font_sheet_t dejavu_sans_mono;
    dejavu_sans_mono.char_width = 11;
    dejavu_sans_mono.char_height = 11;
    dejavu_sans_mono.sheet_width = img_dejavu_sans_mono_width;
    dejavu_sans_mono.sheet_height = img_dejavu_sans_mono_height;
    dejavu_sans_mono.sheet_data_ptr = &img_dejavu_sans_mono_data[0];
    set_workspace_font(workspace, &dejavu_sans_mono);

    while (true) { // display boot splash
        vga_clear_screen(screen_ptr, background_color);
        //draw logo
        image_draw(screen_ptr,
        (320/2) - (img_tangos_logo_width/2), (200/2) - (img_tangos_logo_height/2) - 10,
        img_tangos_logo_width, img_tangos_logo_height,
        img_tangos_logo_data);


        font_draw_string(screen_ptr, &dejavu_sans_mono,
        (320/2) - 55, 40, "welcome to TangOS");

        font_draw_string(screen_ptr, &dejavu_sans_mono,
        (320/2) - 45, 40+img_tangos_logo_height+30, "click to login");

        vga_blit(screen_ptr);

        PAUSE;

        if (mouse_left_clicked) {
            break;
        }
    }

    welcom_app = alloc_app(welcom_init, welcom_update, welcom_close);
    welcom_app->init_func(workspace);
    /// todo: update function and close function, managment of app
    welcom_app->close_func();
    // deallocate app
    free_app(welcom_app);
    welcom_app = NULL;

    tonsole_app = alloc_app(tonsole_init, tonsole_update, tonsole_close);
    tonsole_app->init_func(workspace);
    /// todo: update function and close function, managment of app
    tonsole_app->close_func();
    // deallocate app
    free_app(tonsole_app);
    tonsole_app = NULL;

    int time = 0, time_delay = 50;
    while (true) {
        cur.x += mouse_dx;
        cur.y += mouse_dy;
        mouse_dx = 0, mouse_dy = 0;
        // clip in bounds
        if (cur.x < 0) cur.x = 0;
        else if (cur.x > 318) cur.x = 318;

        if (cur.y < 0) cur.y = 0;
        else if (cur.y > 198) cur.y = 198;

        vga_clear_screen(screen_ptr, background_color);

        //drawing random icons
        image_draw(screen_ptr,
            15, 15,
            img_error_small_width, img_error_small_height,
            img_error_small_data);

        image_draw(screen_ptr,
            35, 15,
            img_info_small_width, img_info_small_height,
            img_info_small_data);

        image_draw(screen_ptr,
            55, 15,
            img_trash_small_width, img_trash_small_height,
            img_trash_small_data);

        image_draw(screen_ptr,
            75, 15,
            img_pencil_small_width, img_pencil_small_height,
            img_pencil_small_data);

        image_draw(screen_ptr,
            95, 15,
            img_folder_small_width, img_folder_small_height,
            img_folder_small_data);

        image_draw(screen_ptr,
            115, 15,
            img_warning_small_width, img_warning_small_height,
            img_warning_small_data);

        /*{ // warning messagebox
            gui::widget msg(45, 45, 210, 45);

            button_t btn_take;
            btn_take.x = msg.get_x() + 160;
            btn_take.y = msg.get_y() + 28;
            btn_take.width = 45;
            btn_take.height = 13;
            strcpy(btn_take.text, "Take");

            gui::widget msg_dec(msg.get_x(),
            msg.get_y() - 10,
            msg.get_width(),
            10);

            vga_fill_rect(screen_ptr, msg_dec.get_x() + 2, msg_dec.get_y() + 2, msg_dec.get_width(), msg_dec.get_height(), COLOR_BLACK);
            vga_fill_rect(screen_ptr, msg_dec.get_x(), msg_dec.get_y(), msg_dec.get_width(), msg_dec.get_height(), COLOR_BLUE);

            vga_fill_rect(screen_ptr, msg.get_x() + 2, msg.get_y() + 2, msg.get_width(), msg.get_height(), COLOR_BLACK);
            vga_fill_rect(screen_ptr, msg.get_x(), msg.get_y(), msg.get_width(), msg.get_height(), COLOR_LIGHT_BLUE);


            image_draw(screen_ptr,
            msg.get_x() + 2, msg.get_y() + (msg.get_height() / 2 - img_warning_small_height / 2),
            img_poop_small_width, img_poop_small_height,
            img_poop_small_data);

            font_draw_string(screen_ptr, &dejavu_sans_mono,
            msg.get_x() + 8 + img_warning_small_width, msg.get_y() + 12,
            "It's dangerous to go alone,\ntake this!");


            button_draw(&btn_take, screen_ptr, &dejavu_sans_mono);
        }*/

        image_draw(screen_ptr,
            smiley_x, smiley_y,
            img_smile_small_width, img_smile_small_height,
            img_smile_small_data);


        draw_workspace(workspace, screen_ptr);

        // cursor
        if (mouse_left_clicked) {
            if (time < time_delay) {
            unsigned int cur_width, cur_height;
            char *cur_data;

            if (time < time_delay / 6) {
                cur_width = img_cursor_p_1_width;
                cur_height = img_cursor_p_1_height;
                cur_data = img_cursor_p_1_data;
            } else if (time < time_delay / 5) {
                cur_width = img_cursor_p_2_width;
                cur_height = img_cursor_p_2_height;
                cur_data = img_cursor_p_2_data;
            } else if (time < time_delay / 4) {

                cur_width = img_cursor_p_3_width;
                cur_height = img_cursor_p_3_height;
                cur_data = img_cursor_p_3_data;
            } else if (time < time_delay / 3) {
                cur_width = img_cursor_p_4_width;
                cur_height = img_cursor_p_4_height;
                cur_data = img_cursor_p_4_data;
            } else {
                cur_width = img_cursor_p_5_width;
                cur_height = img_cursor_p_5_height;
                cur_data = img_cursor_p_5_data;
            }

            image_draw(screen_ptr,
                cur.x, cur.y,
                cur_width, cur_height,
                cur_data);

            ++time;
            } else {
                time = 0;
            }
        } else {
            image_draw(screen_ptr,
                cur.x, cur.y,
                img_cursor_p_1_width, img_cursor_p_1_height,
                img_cursor_p_1_data);
            time = 0;
        }

        vga_blit(screen_ptr);

        PAUSE; // wait for an interrupt
    }
  /*} else {
    printf("Cancelled... Press any key to reboot\n");
    keyboard_getchar();
    reboot();
  }*/
exit:
    // free the workspace
    if (workspace != NULL) {
        free_workspace(workspace);
        workspace = NULL;
    }
    // free the command line functions
    for (int i = 0; i < cl_function_counter; i++) {
        free_cl_function(cl_functions[i]);
    }
}
