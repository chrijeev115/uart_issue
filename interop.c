/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#include "event-queue.h"
#include "debug.h"

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int index = 0;
static char buf[128];
event insert_rcv;


int recv_byte(unsigned char c)
{

	if (index < 128)
			buf[index++] = c;

	else if (c == '\n' || c == '\r')
	{
		insert_rcv.type = event_input;
		insert_rcv.data = &buf[0];
		insert_rcv.size = index;
		queue_insert(insert_rcv);
	}

	return 0;
}

void reset_input(void) {

	memset(buf, 0, 128);
	memset(insert_rcv, 0, sizeof(event));
	index = 0;
}

int write_byte(char c) {
  return putchar(c);
}



void set_term_raw(void) {
  struct termios raw;

  tcgetattr(STDIN_FILENO, &raw);

  raw.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
