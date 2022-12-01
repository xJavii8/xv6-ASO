#ifndef XV6_TYPES_H
#define XV6_TYPES_H

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

enum proc_prio { NORM_PRIO, HI_PRIO };

#ifndef NULL
#define NULL 0
#endif

#endif
