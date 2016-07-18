#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <kernel/irq.h>

typedef void(*mouse_event_callback)();

enum mouse_event_type {
  mouse_unknown_event,
  mouse_left_click_event,
  mouse_left_released_event,
  mouse_left_drag_event,
  mouse_right_click_event,
  mouse_right_released_event,
  mouse_right_drag_event,
  mouse_middle_click_event,
  mouse_middle_released_event,
  mouse_middle_drag_event,
};

typedef struct {
  mouse_event_type type;
  mouse_event_callback callback;
} mouse_event_t;

extern int mouse_dx;
extern int mouse_dy;
extern bool mouse_left_clicked;
extern bool mouse_right_clicked;
extern bool mouse_middle_clicked;

bool mouse_bind_event(mouse_event_type, mouse_event_callback);

void mouse_handler(regs_t *regs);
void mouse_install(void);

#endif
