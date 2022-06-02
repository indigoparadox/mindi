
#ifndef MINDIINT_H
#define MINDIINT_H

#ifdef USE_STDINT

#include <stdint.h>

#else

typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef short int16_t;
typedef char int8_t;

#endif /* USE_STDINT */

#endif /* !MINDIINT_H */

