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
#include "tft.h"
#include "bufferTypes.h"
//////////////////////////////////////////////////////
////Drawer object and Drawer Ringbuffer///////////////
//////////////////////////////////////////////////////
typedef enum {DRAWER_CMD_COLOR, DRAWER_CMD_DRAW} Drawer_commandType_t;
struct sCommand {
    Drawer_commandType_t command;
    union
    {
        TFT_colors_t color;
        uint8_t coordinate[4];
    } data;
};
typedef struct sCommand Drawer_t;

//BUFFER
typedef struct {
    Drawer_t dw_buffer[DRAWER_SIZE]; 
    uint16_t dw_readIndex;
    uint16_t dw_writeIndex;
    uint16_t dw_fillLevel;
    uint16_t dw_size;
    
    RingBuffer_type dw_type;
}dw_Ringbuffer_t;
//actual Drawer Ringbuffer
dw_Ringbuffer_t drawer_commands;

//////////////////////////////////////////////////////
//Methods of DRAWER RINGBUFFER (static -> private/////
//////////////////////////////////////////////////////
RC_t dw_ringbufferInit(dw_Ringbuffer_t* const me);
RC_t dw_writeToRingbuffer(dw_Ringbuffer_t* const me, Drawer_t byteToWrite);
RC_t dw_readFromRingbuffer(dw_Ringbuffer_t* const me, Drawer_t * buffer_data);
RC_t dw_readSetFromRingbuffer(dw_Ringbuffer_t* const me, Drawer_t* drawerSet);
RingBuffer_type dw_getBufferType();
void dw_setBufferType(dw_Ringbuffer_t * const me, RingBuffer_type type);


/* [] END OF FILE */
