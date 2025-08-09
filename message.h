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
#ifndef MESSAGE_H
#define MESSAGE_H
#pragma once
 
#define commandsRows 15
#define commandsColumns 20
#define token_nbr 50
#define inputLength 200
#include "jsmn.h" 
#include "drawer.h"
    
    
typedef enum {MB_HMI_DATA_READY, MB_UART_DATA_READY, MB_NOEVENT,}MB_EventMaskType;
typedef enum {MB_HMI,MB_JSON,MB_NOTASK,}MB_TaskType;
typedef enum { BUFFERTYPE_DRAWER, BUFFERTYPE_UINT8,} type_t;
//////////////////////////////////////////////////////
//MessageBox can hold different types of ringbuffers//
//////////////////////////////////////////////////////
//Points to Buffer(in this case "drawer_commands"
typedef struct
{
    uint16_t m_size;
    void* m_pBuffer;
} message_t;
//Messagebox
typedef struct
{
    message_t mb_buffer[POSTBOX_SIZE]; 
    uint16_t mb_readIndex;
    uint16_t mb_writeIndex;
    uint16_t mb_fillLevel;
    uint16_t mb_size;   
    type_t mb_type;
    MB_TaskType mb_taskType;
    MB_EventMaskType mb_ev;
} messagebox_t;
//////////////////////////////////////////////////////
//Methods of MESSAGE BOX (static -> private///////////
//////////////////////////////////////////////////////
RC_t mb_ringbufferInit(messagebox_t* const me);
static RC_t mb_writeToMessageBuffer(messagebox_t* const me,RingBuffer_type type,Drawer_t data, uint8_t uint8Data);
RC_t mb_readFromMessageBuffer(messagebox_t * const me, RingBuffer_type type, Drawer_t * buffer_data);
RC_t mb_setHMIEventAndTaskType(messagebox_t * const me,MB_EventMaskType ev, MB_TaskType tsk);
boolean_t mb_HMIFired(messagebox_t * const me);
RC_t mb_transferTokenIntoDrawer(messagebox_t* const me, int nbrTokens, 
                        char tftCommands[commandsRows][commandsColumns],
                                            jsmntok_t tokens[token_nbr],char* jsonString, Drawer_t* draw);

RC_t MESSAGE_BUFFER_WRITE(messagebox_t* messagebox, void const*const buffer_data, type_t type, uint16_t size);
RC_t MESSAGE_BUFFER_READ(messagebox_t* messagebox, void* output_data, type_t type, uint16_t size);
#endif
/* [] END OF FILE */
