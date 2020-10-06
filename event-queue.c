/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 *
 * Naive FIFO queue
 *
 * Provides a global queue of QUEUE_SIZE that overwrites previous values on
 * insertion. The code is only to be used in simple examples as it has no
 * safety mechanisms what so ever.
 *
 * Example:
 *
 *   queue_init();
 *   queue_insert({(event_type) 1, NULL);
 *   queue_insert({(event_type) 1, NULL);
 *   queue_insert({(event_type) 1, NULL);
 *   queue_insert({(event_type) 1, NULL);
 *
 *   printf("event type: %i\n", queue_pop().type);
 *   printf("event type: %i\n", queue_pop().type);
 *   printf("event type: %i\n", queue_pop().type);
 *   printf("event type: %i\n", queue_pop().type);
 *   printf("event type: %i\n", queue_pop().type);
 */

#include "event-queue.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

static queue q;

void queue_init() {
	PRINT("queue: init at %p\n", q);
	q.pos = 0;
	q.head = 0;
	q.max_size = QUEUE_SIZE;
}

int queue_size() {
	return q.pos - q.head;
}

void queue_insert(event ev) {
	/* Assume old values are stale and overwrite existing data once queue is full */
	q.buffer[q.pos] = ev;
	q.pos = (q.pos + 1) % QUEUE_SIZE;
}

event queue_peek() {
		event ev = q.buffer[q.head];

		return ev;
}

event queue_pop() {
	event ev = queue_peek();

	if (0 != ev.type) {
		q.head = (q.head + 1) % QUEUE_SIZE;
	}

	return ev;
}
