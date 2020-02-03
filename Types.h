#ifndef __Types_H__
#define __Types_H__


typedef unsigned char         UINT8;  //           0..255
typedef signed char           SINT8;  //        -128..127
typedef unsigned short int    UINT16; //           0..65535
typedef signed short int      SINT16; //      -32768..32767
typedef unsigned long int     UINT32; //           0..4294967295
typedef signed long int       SINT32; // -2147483648..2147483647


#ifndef FALSE
#define FALSE  0
#endif

#ifndef TRUE
#define TRUE   (!FALSE)
#endif

#ifndef NULL
#define NULL   0L
#endif

#endif
