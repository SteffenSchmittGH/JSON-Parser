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
#include "myRingbuffer.h"
#include "UART_LOG.h"
#include "project.h"

RC_t ringbufferInit(rb_Ringbuffer_t* const me){
    me->rb_readIndex = 0;
    me->rb_writeIndex = 0;
    me->rb_fillLevel = 0;
    me->rb_size = BUFFER_SIZE;
    return RC_SUCCESS;   
}
RC_t writeToRingbuffer(rb_Ringbuffer_t* const me, uint8_t byteToWrite){
    RC_t res = RC_ERROR;
    GetResource(res_UARTLOG_ringbuffer);
    if(me->rb_fillLevel < me->rb_size - 1){
        me->rb_buffer[(me->rb_writeIndex++)%BUFFER_SIZE] = byteToWrite;
        me->rb_fillLevel++;
        res = RC_SUCCESS;
    }else{
        res = RC_ERROR_OVERRUN;
    }
    ReleaseResource(res_UARTLOG_ringbuffer);
    return res;
}

uint8_t readFromRingbuffer(rb_Ringbuffer_t * const me){
    uint8_t buffer_data = '-';
    GetResource(res_UARTLOG_ringbuffer);
    if(me->rb_fillLevel > 0){
        buffer_data = me->rb_buffer[(me->rb_readIndex++)%BUFFER_SIZE];
        me->rb_fillLevel--;
    }
    ReleaseResource(res_UARTLOG_ringbuffer);
    return buffer_data;
}

uint16_t IndexOfLastDelimiter(rb_Ringbuffer_t* const me){
        uint8_t lastDelimiter = 'G';
        GetResource(res_UARTLOG_ringbuffer);
        uint16_t lastDelIndex = (me->rb_fillLevel + me->rb_size)%me->rb_size;
        for(int i = lastDelIndex; i > 0; i--){
            lastDelimiter = me->rb_buffer[i];
            if(lastDelimiter == rb_delimiter){
                break;
            }
        }
        ReleaseResource(res_UARTLOG_ringbuffer);
    return lastDelIndex;   
}
/* [] END OF FILE */
