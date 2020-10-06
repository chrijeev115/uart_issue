/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdint.h>

enum attr_tables {
  TABLE_0,
  TABLE_1,
  TABLE_2,
  TABLE_3,
  LAST_TABLE_ELEMENT
};

enum attr_table_1 {
  TABLE_1_ACCESS = 0,
};

enum attr_table_2 {
  TABLE_2_COUNTER = 0,
  TABLE_2_LAST    = 1
};

enum attr_table_3 {
  TABLE_3_DEFAULT = 0
};


/**
 * Set attribute `attr` in `table`
 */
int attr_set(enum attr_tables table, int attr, uint8_t value);

/**
 * Get attribute `attr` in `table`
 */
int attr_get(enum attr_tables table, int attr);

/**
 * Reset all attributes to `0` in `table`
 */
int attr_reset(enum attr_tables table);

#endif /* ATTRIBUTES_H_ */
