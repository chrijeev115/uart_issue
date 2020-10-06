/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
  #define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
  #define PRINT(...) do {} while(0)
#endif /* DEBUG */

#endif /* DEBUG_H_ */
