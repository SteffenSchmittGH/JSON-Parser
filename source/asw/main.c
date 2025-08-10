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
#include "myRingbuffer.h"
rb_Ringbuffer_t theRingbuffer;
messagebox_t messages;

ISR(systick_handler){
    CounterTick(cnt_systick);
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
    EE_systick_enable_int();
   
    UART_LOG_Start();
    TFT_init();
    TFT_setBacklight(100);
    //Uhrzeit
    TFT_setColors(WHITE, BLACK);
    TFT_fillCircle(10, 10, 10, RED);
    ringbufferInit(&theRingbuffer);
    for(;;)
    {
        /* Place your application code here. */
        StartOS(OSDEFAULTAPPMODE);
    }
}

TASK(tsk_init){
    //MCAL
    UART_LOG_PutString("UART JSON PARSER! \r\n");
    //SysTick
    EE_system_init();
    EE_systick_start(); 
    mb_ringbufferInit(&messages);
    //Task and Alarm activation
    SetRelAlarm(alm_tft,100,250);
    ActivateTask(tsk_json);
    ActivateTask(tsk_tft);
    ActivateTask(tsk_backround);
    TerminateTask();
}

TASK(tsk_json){
    EventMaskType eventMask;
    while(1){
        //Wait, Get and Clear Command
        WaitEvent(ev_endOfString);
        GetEvent(tsk_json, &eventMask);
        ClearEvent(eventMask);
        //Local variables to store Rindbuffer Data
        char *jsonString;
        //while(){//!!!!!!!!
            jsonString = malloc(sizeof(char)*inputLength);
            if(jsonString == NULL){
                UART_LOG_PutString("Memory allocation failed!");   
            }
            //set Delimiter
            for(int i = 0; i < inputLength; i++){
                jsonString[i] = '\0';   
            }
            //Semaphore for Ringbuffer and Ringbuffer read in myRingbuffer.c
            uint16_t lastDelimiter=IndexOfLastDelimiter(&theRingbuffer); //if this finishes, and now event is fired to store new data is in the UINT8 Ringbuffer
            int indexJsStr = 0;
            for(int rb_index = 0; rb_index < lastDelimiter; rb_index++)
            {
                jsonString[indexJsStr++] += readFromRingbuffer(&theRingbuffer);
            }
        //}
        //Parse the json data which are stored in the global variable **jsonString
        //global_token holds the json tokens now(e.g. string, array, ...)
        jsmn_parser jsmn_parser;
        jsmn_init(&jsmn_parser);
        jsmntok_t tokens[token_nbr];
        int nbrTokens = 0;
        nbrTokens = jsmn_parse(&jsmn_parser, jsonString, strlen(jsonString),
                        tokens, token_nbr);
        // reads out the tokens and puts them into global tftCommand array (Ringbuffer ?)
        unsigned int index_tokens = 0;
        unsigned int tftCommandToken = 0;
        char tftCommands[commandsRows][commandsColumns];
        Drawer_t drawerObj;
        RC_t res = mb_transferTokenIntoDrawer(&messages, nbrTokens,tftCommands,tokens, jsonString, &drawerObj);
    }
        
    TerminateTask;
}

TASK(tsk_tft){
    if(mb_HMIFired(&messages))
    {
        Drawer_t tftCommando;
        RC_t res = RC_ERROR;
        //res = mb_readFromMessageBuffer(&messages,T_DRAWER,&tftCommando);
        //GetResource(res_messagebox);
        res = MESSAGE_BUFFER_READ(&messages, &tftCommando,BUFFERTYPE_DRAWER, messages.mb_size);
        //ReleaseResource(res_messagebox);
       
        TFT_fillCircle(tftCommando.data.coordinate[0],tftCommando.data.coordinate[1],10,BLACK);
        TFT_fillCircle(tftCommando.data.coordinate[2],tftCommando.data.coordinate[3],10,tftCommando.data.color);
        if(res == RC_ERROR_BUFFER_EMTPY){
            UART_LOG_PutString("RC_ERROR_BUFFER_EMPTY");
            mb_setHMIEventAndTaskType(&messages, MB_NOEVENT, MB_NOTASK);
        }
    }
    TerminateTask();
}

TASK(tsk_backround)
{
    while(1){
        __asm("nop");   
    }
    TerminateTask();   
}

ISR2(isr_log_rx){
    int8_t data = UART_LOG_GetByte();
    writeToRingbuffer(&theRingbuffer, data);
    if(data == '\0'){
       SetEvent(tsk_json, ev_endOfString);
    }
}
/* [] END OF FILE */
