/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "global.h"

#define BUFFER_SIZE 500
    
static uint8_t rb_delimiter = '\0';
typedef struct {
    uint8_t rb_buffer[BUFFER_SIZE]; 
    uint16_t rb_readIndex;
    uint16_t rb_writeIndex;
    uint16_t rb_fillLevel;
    uint16_t rb_size;
}rb_Ringbuffer_t;

RC_t ringbufferInit(rb_Ringbuffer_t* const me);
RC_t writeToRingbuffer(rb_Ringbuffer_t* const me, uint8_t byteToWrite);
uint8_t readFromRingbuffer(rb_Ringbuffer_t* const me);
uint16_t IndexOfLastDelimiter(rb_Ringbuffer_t* const me);

#endif /* RINGBUFFER_H */

/* [] END OF FILE */
