/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#include "main.h"
#include "attributes.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * Send a tick event, signaling to all processes that a processing cycle have
 * completed.
 */
PROCESS(tick_handler, ev) {
  static event outgoing = {event_tick, NULL};

	usleep(1000);

	send_event(outgoing);
}

/**
 * Check for IO on standard input
 */
PROCESS(io_handler, ev) {
  static unsigned char buf[128];
  static event outgoing = {event_input, &buf[0]};

  PROCESS_YIELD(event_tick != ev.type);

  fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

  // we don't accept multi byte charaters. This means only ASCII values will be
  // sent and code drops any multi-key (ie Alt+S) or special keys (ie arrow keys)
  if (1 == read(0, buf, 4)) {
    recv_byte(buf[0]);
  }
}


/**
 * Packet interface
 *
 *  - Accept to get / set attribute values
 *  - Accept packet to reset attribute table
 *  - Accept packets to shutdown
 *
 *  Packets:
 *    // get attribute 1 from table 2
 *    GET 2 1
 *
 *    // Set attribute 0 in table 3
 *    GET 3 0 99
 *
 *    // Reset table 3
 *    RESET 3
 *
 *    // Shutdown the process
 *    QUIT
 */
static void write_bytes(unsigned char *data, int size) {
  for (int i = 0; i < size; i++) {
    write_byte(data[i]);
  }
  write_byte('\n');
}

#define ERR_INPUT "ERROR INPUT"
#define ERR_NO_ATTR "ERROR NO-ATTR"
#define ERR_INVALID "ERROR INVALID"

#define RESPOND(var, str) {          \
  memcpy(var, str, sizeof(str)); \
  write_bytes((unsigned char *) var, sizeof(str)); \
}

PROCESS(attributes_handler, ev) {
  unsigned char res[128] = "";

  PROCESS_YIELD(event_input != ev.type);

  if (0 == memcmp(ev.data, "GET ", 4)) {
    if (ev.size != 8) {
      RESPOND(&res, ERR_INPUT);
      reset_input();
      return;
    } else {

      int table = ((unsigned char *) ev.data)[4] - 48;
      int attr = ((unsigned char *) ev.data)[6] - 48;

      int value = attr_get(table, attr);

      if (-1 == value || value == -2) {
        RESPOND(&res, ERR_NO_ATTR);
        reset_input();
        return;
      } else {
        memcpy(&res, "ATTR ", 5);
        res[5] = (unsigned char) value + 48;
        res[6] = 0;
        write_bytes((unsigned char *) &res, 7);
        reset_input();
        return;
      }
    }
  }
  else if (0 == memcmp(ev.data, "SET ", 4)) {
	  /**
	   * TODO Add data conversion, error correction etc
	   */
	  if (ev.size != 10) {
		  RESPOND(&res, ERR_INPUT);
		  reset_input();
		  return;
	  }
	  else {
		  int table = ((unsigned char*)ev.data)[4] - 48;
		  int attr = ((unsigned char*)ev.data)[6] - 48;
		  int value = ((unsigned char*)ev.data)[8] - 48;

		  /**
		   * TODO - Add return value to notify success
		   */
		  attr_set(table, attr, value);

		  if (-1 == value || -2 == value) {
			  RESPOND(&res, ERR_NO_ATTR);
			  reset_input();
			  return;
		  }
		  else {

			  memcpy(&res, "SUCCESS ", 8);
			  res[8] = 0;
			  write_bytes((unsigned char*)& res, 9);
			  reset_input();
			  return;
		  }
	  }
  }

  /**
   * TODO - Add `RESET <table>`
   * TODO - add `QUIT` command
   */

  else if (0 == memcmp(ev.data, "RESET ", 6)) {

	  if (ev.size != 8) {
		  RESPOND(&res, ERR_INPUT);
		  reset_input();
		  return;
	  }
	  else {
		  int tbl = ((unsigned char*)ev.data)[6] - 48;
		  int val = attr_reset(tbl);
		  if (val == -1)// || val == -2)
		  {
			  RESPOND(&res, ERR_NO_ATTR);
		  }
		  else {
			  memcpy(&res, "SUCCESS ", 8);
			  res[8] = 0;
			  write_bytes((unsigned char*)& res, 9);
		  }
		  reset_input();
		  return;
	  }
  }
  else if (0 == memcmp(ev.data, "QUIT ", 5)) {
	  exit(0);
  }
  RESPOND(&res, ERR_INVALID);
  reset_input();
}

void main() {
  /* register all the tasks to be scheduled */
  void (*tasklist[])() = {tick_handler, io_handler, attributes_handler};
  int taskcount;
  int buf;

	PRINT("start\n");
	queue_init();
  set_term_raw();

  // set the first tick
  queue_insert((event) {.type = event_tick});

  while (1) {
    event ev = queue_pop();

    if (event_quit == ev.type) {
      break;
    }

    for (taskcount = 0; taskcount < NUMTASKS(tasklist); taskcount++) {
      (*tasklist[taskcount])(ev);
    }
  }

	PRINT("end\n");
}
