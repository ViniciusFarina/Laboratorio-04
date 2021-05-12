#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

typedef struct param_thread {
	uint8_t LED;
	uint32_t delay;
  }param_thread;

osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

/*void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100);
  } // while
} // thread1
*/
  void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  struct param_thread* args = (struct param_thread*) arg;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= args -> LED;
    LEDWrite(args -> LED, state);
    
    osDelayUntil(tick + args -> delay);
  } // while
} // thread2

void main(void){
  LEDInit(LED1 | LED2 | LED3 | LED4);

  osKernelInitialize();
  
  param_thread S1 = {LED1, 200};
  param_thread S2 = {LED2, 300};
  param_thread S3 = {LED3, 500};
  param_thread S4 = {LED4, 700};
  
  thread1_id = osThreadNew(thread2, &S1, NULL);  // Não há real necessidade de ter uma id pra cada thread neste cenário em questão
  thread2_id = osThreadNew(thread2, &S2, NULL);
  thread3_id = osThreadNew(thread2, &S3, NULL);
  thread4_id = osThreadNew(thread2, &S4, NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
