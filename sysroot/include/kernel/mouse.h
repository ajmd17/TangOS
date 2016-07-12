#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>

#include <kernel/sys_asm.h>

typedef unsigned char byte;
typedef char sbyte;
typedef unsigned int dword;

byte mouse_cycle=0;     //unsigned char
int mouse_byte[3];    //signed char
int mouse_x=0;         //signed char
int mouse_y=0;         //signed char
unsigned char data[3];
static volatile int button[3] = {0, 0, 0};

void left_button_down(int x, int y)
{
}

void left_button_up(int x, int y)
{
}

void right_button_down(int x, int y)
{
}

void right_button_up(int x, int y)
{
}

void middle_button_down(int x, int y)
{
}

void middle_button_up(int x, int y)
{
}


void outport60(unsigned char data)
{
    volatile unsigned char good = 0x02;
    while ((good & 0x02) != 0)
        good = inb(0x64);
    outb(0x60, data);
}

unsigned inport60()
{
    volatile unsigned char good = 0x00;
    while ((good & 0x01) == 0)
        good = inb(0x64);
    return inb(0x60);
}

void outport64(unsigned char data)
{
    volatile unsigned char good = 0x02;
    while ((good & 0x02) != 0)
        good = inb(0x64);
    outb(0x64, data);
}

unsigned inport64()
{
    return inb(0x64);
}

//Mouse functions
void mouse_handler(void)
{
  /*switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0] = inb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1] = inb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inb(0x60);
      mouse_x = mouse_byte[1];
      mouse_y = mouse_byte[2];
      mouse_cycle=0;
      break;
  }*/
  
  int x, y;
  
  
  data[0] = inport60();
    data[1] = inport60();
    data[2] = inport60();
    if ((data[0] & 0x01) != button[0])
    {
        button[0] ^= 1;
        if (button[0]) left_button_down(x, y);
        else left_button_up(x, y);
    }
    if ((data[0] & 0x04) != button[1])
    {
        button[1] ^= 1;
        if (button[1]) middle_button_down(x, y);
        else middle_button_up(x, y);
    }
    if ((data[0] & 0x02) != button[2])
    {
        button[2] ^= 1;
        if (button[2]) right_button_down(x, y);
        else right_button_up(x, y);
    }
    if (data[0] & 0x10)
        x += (int)((256 - data[1]) * -1);
    else
        x += (int)data[1];
    if (data[0] & 0x20)
        y += (int) (256 - data[2]);
    else
        y += (int)(data[2] * -1);
    if (y > 184) y = 184;
    else if (y < 0) y = 0;
    if (x > 311) x = 311;
    else if (x < 0) x = 0;
    
    mouse_x = x;
    mouse_y = y;
}

inline void mouse_wait(byte a_type) //unsigned char
{
  dword _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(byte a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

byte mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void mouse_install()
{
 /* byte _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);
 
  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  _status=(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, _status);
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge*/
  
  int mouse_type;
  int x;
    unsigned char data_read;
    for(x = 0; x < 5; x++)
    {
        outport64(0xA7);                    //reset every thing
        outport64(0xA8);
        outport64(0xD4);
        outport60(0xF5);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
        outport64(0xD4);
        outport60(0xFF);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
        data_read = inport60();             //    did the self-
        if (data_read != 0xAA) continue;    //    test work ?
        mouse_type = inport60();
        outport64(0xD4);
        outport60(0xE6);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
/*
        outport64(0xD4);
        outport60(0xF3);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
        outport60(210);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
*/
        outport64(0x20);                   
        data_read = inport60();             //
        data_read |= 0x02;                  //
        outport64(0x60);                    //    get it to report data
        outport60(data_read);               //
        outport64(0xD4);                    //
        outport60(0xF4);
        data_read = inport60();             //    did it
        if (data_read != 0xFA) continue;    //    work ?
        break;
    }
}

#endif
