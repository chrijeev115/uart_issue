/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_


#define send_event(ev) queue_insert(ev)
#define event_invalid  (event_type) 0
#define event_tick     (event_type) 1
#define event_quit     (event_type) 2
#define event_input    (event_type) 3

#define MAX_EVENT_SIZE 256
#define QUEUE_SIZE 64

typedef int event_type;

typedef struct event {
  int type;
  void *data;
  int size;
} event;

typedef struct queue {
  event buffer[QUEUE_SIZE];
  // the position for inserting
  int pos;
	// the position for reading
  int head;
  int max_size;
} queue;

/**
 * Initialize queue
 */
void queue_init();

/**
 * Insert item `ev` to end of queue
 */
void queue_insert(event ev);

/**
 * Pop the head of the queue
 */
event queue_pop();

/**
 * Peek into the head of the queue
 */
event queue_peek();

#endif /* EVENT_QUEUE_H_ */
