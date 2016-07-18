#include <kernel/mouse.h>
#include <kernel/sys_asm.h>
#include <kernel/irq.h>
#include <kernel/types.h>

#include <stdio.h>

#define MOUSE_IRQ 12
#define MOUSE_MAX_CALLBACKS 32
#define PROCESS_MOUSE_EVENTS(EVENT_TYPE) \
  do { \
    for (int i = 0; i < num_callbacks; i++) { \
      if (callbacks[i].type == EVENT_TYPE) { \
        callbacks[i].callback(); \
      } \
    } \
  } while (0)

byte mouse_cycle = 0;
int mouse_byte[3];

// definition of mouse positions
int mouse_dx = 0;
int mouse_dy = 0;

static mouse_event_t callbacks[MOUSE_MAX_CALLBACKS];
size_t num_callbacks = 0;
bool mouse_left_clicked = false;
bool mouse_right_clicked = false;
bool mouse_middle_clicked = false;

bool mouse_bind_event(mouse_event_type type, mouse_event_callback callback) {
  if (num_callbacks < MOUSE_MAX_CALLBACKS) {
    mouse_event_t evt;
    evt.type = type;
    evt.callback = callback;
    callbacks[num_callbacks++] = evt;
    return true;
  }
  return false;
}

void outport60(byte data) {
  volatile byte good = 0x02;
  while (good & 0x02) {
    good = inb(0x64);
  }
  outb(0x60, data);
}

dword inport60() {
  volatile byte good = 0x00;
  while (!(good & 0x01)) {
    good = inb(0x64);
  }
  return (dword)inb(0x60);
}

void outport64(byte data) {
  volatile byte good = 0x02;
  while (good & 0x02) {
    good = inb(0x64);
  }
  outb(0x64, data);
}

dword inport64() {
  return (dword)inb(0x64);
}

void mouse_handler(regs_t *regs) {
  switch (mouse_cycle) {
  case 0:
    mouse_byte[0] = (int)inb(0x60);
    mouse_cycle++;
    break;
  case 1:
    mouse_byte[1] = (int)inb(0x60);
    mouse_cycle++;
    break;
  case 2:
    mouse_byte[2] = (int)inb(0x60);
    if (mouse_byte[0] & 0x10) {
      mouse_dx = (int)((256 - mouse_byte[1]) * -1);
    } else {
      mouse_dx = (int)mouse_byte[1];
    }
    if (mouse_byte[0] & 0x20) {
      mouse_dy = (int)(256 - mouse_byte[2]);
    } else {
      mouse_dy = (int)(mouse_byte[2] * -1);
    }

    if (mouse_byte[0] & 0x1) {
      if (!mouse_left_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_left_click_event);
      } else {
        PROCESS_MOUSE_EVENTS(mouse_left_drag_event);
      }
      mouse_left_clicked = true;
    } else {
      if (mouse_left_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_left_release_event);
      }
      mouse_left_clicked = false;
    }

    if (mouse_byte[0] & 0x2) {
      if (!mouse_right_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_right_click_event);
      } else {
        PROCESS_MOUSE_EVENTS(mouse_right_drag_event);
      }
      mouse_right_clicked = true;
    } else {
      if (mouse_right_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_right_release_event);
      }
      mouse_right_clicked = false;
    }

    if (mouse_byte[0] & 0x4) {
      if (!mouse_middle_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_middle_click_event);
      } else {
        PROCESS_MOUSE_EVENTS(mouse_middle_drag_event);
      }
      mouse_middle_clicked = true;
    } else {
      if (mouse_middle_clicked) {
        PROCESS_MOUSE_EVENTS(mouse_middle_release_event);
      }
      mouse_middle_clicked = false;
    }

    mouse_cycle = 0;
    break;
  }
  // acknowledge
  irq_ack(MOUSE_IRQ);
}

void mouse_install(void) {
  CLI; // disable interrupts

  int mouse_type;
  byte data;

  for (int i = 0; i < 5; i++) {
    outport64(0xA7);
    outport64(0xA8);
    outport64(0xD4);
    outport60(0xF5);

    data = inport60();
    if (data != 0xFA) {
      continue;
    }

    outport64(0xD4);
    outport60(0xFF);

    data = inport60();
    if (data != 0xFA) {
      continue;
    }

    data = inport60();
    if (data != 0xAA) {
      continue;
    }

    mouse_type = inport60();

    outport64(0xD4);
    outport60(0xE6);

    data = inport60();
    if (data != 0xFA) {
      continue;
    }

    outport64(0x20);

    data = inport60();
    data |= 0x02;

    outport64(0x60);
    outport60(data);
    outport64(0xD4);
    outport60(0xF4);

    data = inport60();
    if (data != 0xFA) {
      continue;
    }

    break;
  }

  STI; // enable interrupts
  irq_install_handler(MOUSE_IRQ, mouse_handler); // install the handler
}