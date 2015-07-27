/*
* lpreserve.c
* A Lua library for serializing and deserializing Lua State
* Jason Burmark <mr.burmark@gmail.com>
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
#include "lua.hpp"
#else
#include "lua.h"
#endif
#include "lualib.h"
#include "lauxlib.h"

#define BIGENDIAN 0
#define LITTLEENDIAN 1

/* endianess checking and variables */
static int p_helper = 1;
static const unsigned char p_endianness = ((unsigned char *)&p_helper)[0];
static unsigned char p_read_endianness = -1;

#define P_MAGIC 0xa7

#define P_NIL         0
#define P_BOOLEAN     1
#define P_NUMBER      2
#define P_STRING      3
#define P_TABLE       4
#define P_FUNCTION    5
#define P_USERDATA    6

#define SEEN_STRING_INDEX       1
#define SEEN_TABLE_INDEX        2
#define SEEN_LUAFUNCTION_INDEX  3
#define SEEN_CFUNCTION_INDEX    4
#define SEEN_USERDATA_INDEX     5

/* control byte 
 * 
 * composed of 4 bit-fields, type, option1, option2, option3, and size
 * With 3 bits, 1 bit, 1 bit, 1 bit, and 2 bits respectively
 * 
 * | . . . | . | . | . | . . |
 *   type    1   2   3   size
 *            options
 * 
 * where type is one of the types above P_NIL...
 * size represents one of 1, 2, 4, 8 for the stand sizes of numbers
 */

#define P_CONTROL_GET_TYPE(p_c) ( ((p_c) >> 5) & 0x7 )
#define P_CONTROL_GET_OPT1(p_c) ( ((p_c) >> 4) & 0x1 )
#define P_CONTROL_GET_OPT2(p_c) ( ((p_c) >> 3) & 0x1 )
#define P_CONTROL_GET_OPT3(p_c) ( ((p_c) >> 2) & 0x1 )
#define P_CONTROL_GET_SIZE(p_c) ( 1 << ((p_c) & 0x3) )

#define P_CONTROL_SET_TYPE(p_c, data) ( ((p_c) & 0x1f) | (((data) & 0x7) << 5) )
#define P_CONTROL_SET_OPT1(p_c, data) ( ((p_c) & 0xef) | (((data) & 0x1) << 4) )
#define P_CONTROL_SET_OPT2(p_c, data) ( ((p_c) & 0xf7) | (((data) & 0x1) << 3) )
#define P_CONTROL_SET_OPT3(p_c, data) ( ((p_c) & 0xfb) | (((data) & 0x1) << 2) )
#define P_CONTROL_SET_SIZE(p_c, data) ( ((p_c) & 0xfc) | (((data) == 8 ? 3 : (data) == 4 ? 2 : (data) == 2 ? 1 : 0) & 0x3) )

typedef struct {
    unsigned char h[8];
} char8;

typedef struct {
    unsigned char h[4];
} char4;

#define P_STREAM_GET_NUMBER(p_s, number) \
    sizeof(number) == 8 ?\
        (number)\
    : sizeof(number) == 4 ?\
        \
    : sizeof(number) == 2 ?\
        \
    : \
        \








/* default output error function just prints the error message to stdout */
static void p_default_output_error(const char * error) 
{
    printf("%s", error);
}

static void (p_output_error*) (const char * error) = p_default_output_error;

/* function to set error message output routine */
void luap_aterror(void (output_error*) (const char *))
{
    p_output_error = output_error;
}



/* buffer definition and functions */
typedef struct {
    size_t         size;
    size_t         head;
    unsigned char* data;
} p_Buffer;

static void pbuf_init(p_Buffer *buf)
{
    buf->size = 128;
    buf->head = 0;
    buf->data = malloc(buf->size * sizeof(unsigned char))
    if ( buf->data == NULL )
    {
        buf->size = 0;
    }
}

static void pbuf_dealloc(p_Buffer *buf)
{
    free(buf->data);
}

static void pbuf_addlen(p_Buffer *buf, size_t len)
{
    size_t new_size = buf->size;
    size_t cur_head = buf->head;
    while (new_size <= cur_head + len) {
        new_size <<= 1;
    }
    if (new_size > buf->size) {
        buf->data = realloc(buf->data, new_size);
        if (buf->data == NULL) {
            p_output_error("Out of memory!");
        }
        buf->size = new_size;
    }
}

