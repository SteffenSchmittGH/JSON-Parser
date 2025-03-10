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
#include "message.h"

RC_t mb_ringbufferInit(messagebox_t* const me){
    me->mb_readIndex = 0;
    me->mb_writeIndex = 0;
    me->mb_fillLevel = 0;
    me->mb_size = POSTBOX_SIZE;
    me->mb_ev = MB_NOEVENT;
    me->mb_taskType = MB_NOTASK;
    for(int i = 0 ; i < POSTBOX_SIZE; i++){
         me->mb_buffer[i].m_pBuffer = NULL;   
    }
    dw_ringbufferInit(&drawer_commands);
    return RC_SUCCESS;   
}
RC_t mb_setHMIEventAndTaskType(messagebox_t * const me,MB_EventMaskType ev, MB_TaskType tsk)
{
    RC_t res = RC_SUCCESS;
    //GetResource(res_messagebox);
    me->mb_taskType = tsk;
    me->mb_ev = ev;
    //ReleaseResource(res_messagebox);
    return res;
}

boolean_t mb_HMIFired(messagebox_t * const me){
    boolean_t ret = 0;
    ret = 0;
    //GetResource(res_messagebox);
    if(me->mb_taskType == MB_HMI && me->mb_ev == MB_HMI_DATA_READY){
        ret = 1;   
    }
    //ReleaseResource(res_messagebox);
    return ret;   
}

RC_t mb_transferTokenIntoDrawer(messagebox_t* const me, 
                                int nbrTokens, 
                                char tftCommands[commandsRows][commandsColumns],
                                jsmntok_t tokens[token_nbr],
                                char* jsonString, 
                                Drawer_t* local_drawer)
{
    //check for Json Strings and put them into an 2D array, for easy access.
    RC_t res = RC_ERROR;
    unsigned int index_tokens = 0;
    unsigned int tftCommandToken = 0; 
    for(int i = 0; i < nbrTokens ; i++)
    {
        if((tokens[index_tokens].type == JSMN_STRING)||
           (tokens[index_tokens].type == JSMN_ARRAY)||
           (tokens[index_tokens].type == JSMN_PRIMITIVE))
           {
                int TokenContent = 0;
                for( int tokenPosition = tokens[index_tokens].start; tokenPosition < tokens[index_tokens].end ; tokenPosition++){
                    tftCommands[tftCommandToken][TokenContent++] = jsonString[tokenPosition];
                }
                tftCommandToken++;
            }
            index_tokens++;
    }
        //Read out information of the json strings and put them into global drawer object
        
    nbrTokens--;
    int color = 0;
    for(int even_commands = 0; even_commands < nbrTokens ; even_commands+=2){
        if(tftCommands[even_commands][0] == 'c'){
            if(!strcmp(tftCommands[even_commands+1],"red"))
            {
                color = 1;
            }
            if(tftCommands[even_commands+1][0] == 'b')
            {   
                color = 2;
            }
        }
        if(tftCommands[even_commands][0] == 'd'){
            if(color == 1){
                     local_drawer->data.color = RED;  
            }
            else if( color == 2)
            {
                    local_drawer->data.color = BLUE;
            }
            else
            {
                local_drawer->data.color = YELLOW;
            }
            local_drawer->data.coordinate[0] = atoi(tftCommands[even_commands+2]);
            local_drawer->data.coordinate[1] = atoi(tftCommands[even_commands+3]);
            local_drawer->data.coordinate[2] = atoi(tftCommands[even_commands+4]);
            local_drawer->data.coordinate[3] = atoi(tftCommands[even_commands+5]);
        
            //GetResource(res_messagebox);  
            MESSAGE_BUFFER_WRITE(me, (void*)local_drawer, BUFFERTYPE_DRAWER, sizeof(local_drawer)+1);
            mb_setHMIEventAndTaskType(me,MB_HMI_DATA_READY, MB_HMI);
            //ReleaseResource(res_messagebox);
        }
    }
    return res;
}

RC_t MESSAGE_BUFFER_WRITE(messagebox_t* messagebox, void const*const buffer_data, type_t type, uint16_t size){
    if(messagebox->mb_fillLevel >= POSTBOX_SIZE){
        return RC_ERROR_BUFFER_FULL;
    }
    void* buffer = malloc(size);
    
    if(buffer == NULL || buffer_data == NULL){
        return RC_ERROR_MEMORY;   
    }
    uint8_t* src = (uint8_t*)buffer_data;
    uint8_t* dst = (uint8_t*)buffer;
    
    for(int i = 0; i <= size; i++){
        dst[i] = src[i];
    }
    
    if(buffer != 0)
    {
        messagebox->mb_buffer[messagebox->mb_writeIndex].m_pBuffer = buffer;
        messagebox->mb_buffer[messagebox->mb_writeIndex].m_size = size;
        messagebox->mb_writeIndex = ( messagebox->mb_writeIndex + 1 )%POSTBOX_SIZE;
        messagebox->mb_fillLevel++;
        messagebox->mb_type = type;
        messagebox->mb_size = size;
    }
    return RC_SUCCESS;   
}

RC_t MESSAGE_BUFFER_READ(messagebox_t* messagebox, void* output_data, type_t type, uint16_t size){
    if(messagebox->mb_fillLevel < 1){
        return RC_ERROR_BUFFER_EMTPY;
    }
    
    
    uint8_t* dst = (uint8_t*)output_data;
    uint8_t* src = (uint8_t*)messagebox->mb_buffer[messagebox->mb_readIndex].m_pBuffer;
    
    for(int i = 0; i <= size; i++)
    {
        dst[i] = src[i];
    }
    
    free(messagebox->mb_buffer[messagebox->mb_readIndex].m_pBuffer);
    messagebox->mb_buffer[messagebox->mb_readIndex].m_pBuffer = NULL;
    
    messagebox->mb_readIndex = (messagebox->mb_readIndex + 1) % POSTBOX_SIZE;
    messagebox->mb_fillLevel--;
    
    return RC_SUCCESS;   
}
/* [] END OF FILE */
