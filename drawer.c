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

#include "drawer.h"

RC_t dw_ringbufferInit(dw_Ringbuffer_t* const me){
    me->dw_readIndex = 0;
    me->dw_writeIndex = 0;
    me->dw_fillLevel = 0;
    me->dw_size = DRAWER_SIZE;
    me->dw_type = T_DRAWER;
    return RC_SUCCESS;   
}

RC_t dw_writeToRingbuffer(dw_Ringbuffer_t* const me, Drawer_t byteToWrite){
    GetResource(res_drawer_RB);
    if(me->dw_fillLevel < me->dw_size - 1){
        me->dw_buffer[(me->dw_writeIndex++)%DRAWER_SIZE] = byteToWrite;
        me->dw_fillLevel++;
    }else{
        return RC_ERROR_OVERRUN;
    }
    ReleaseResource(res_drawer_RB);
    return RC_SUCCESS;
}

RC_t dw_readFromRingbuffer(dw_Ringbuffer_t * const me, Drawer_t* buffer_data){
    RC_t ret = RC_SUCCESS;
    GetResource(res_drawer_RB);
    if(me->dw_fillLevel > 0){
       *buffer_data = me->dw_buffer[(me->dw_readIndex++)%DRAWER_SIZE];
        me->dw_fillLevel--;
    }else{
        ret = RC_ERROR_BUFFER_EMTPY;
    }
    ReleaseResource(res_drawer_RB);
    return ret;
}

RC_t dw_readSetFromRingbuffer(dw_Ringbuffer_t* const me, Drawer_t* drawerSet){
    uint16_t set_index = 0;
    GetResource(res_drawer_RB);
    while(me->dw_writeIndex != me->dw_readIndex){
        dw_readFromRingbuffer(me,&drawerSet[set_index++]);   
    }
    ReleaseResource(res_drawer_RB);
    return RC_SUCCESS;   
}

RingBuffer_type dw_getBufferType(dw_Ringbuffer_t* const me){
    return me->dw_type;
}

void dw_setBufferType(dw_Ringbuffer_t * const me, RingBuffer_type type){
    GetResource(res_drawer_RB);
    me->dw_type = type;
    ReleaseResource(res_drawer_RB);
    return;
}

/* [] END OF FILE */
