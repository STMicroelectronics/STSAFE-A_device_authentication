/**
 ******************************************************************************
 * \brief  ST1Wire Platform Hardware abstraction layer
 * \author STMicroelectronics CS Application Team
 *****************************************************************************/

#include "Drivers/delay_ms/delay_ms.h"
#include "Drivers/delay_us/delay_us.h"
#include "stm32l4xx.h"

extern uint32_t SystemCoreClock;

/* ---------- Static Platform Abstraction layer Declarations ---------- */

/********* Communication protocol debug log ****************/
//#define ST1WIRE_USE_ST1WIRE_DEBUG_LOG
#ifdef ST1WIRE_USE_ST1WIRE_DEBUG_LOG
#define ST1WIRE_ST1WIRE_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#endif

#ifdef USE_FREERTOS
#define ST1WIRE_START_CRITICAL_SECTION \
    vTaskSuspendAll();                 \
    __disable_irq();
#define ST1WIRE_END_CRITICAL_SECTION \
    xTaskResumeAll();                \
    __enable_irq();
#else
#define ST1WIRE_START_CRITICAL_SECTION __disable_irq();
#define ST1WIRE_END_CRITICAL_SECTION __enable_irq();
#endif /* USE_FREERTOS */

void st1wire_platform_init(void);
void st1wire_platform_deinit(void);
void st1wire_platform_io_set(uint8_t bus_addr);
void st1wire_platform_io_clear(uint8_t bus_addr);
uint8_t st1wire_platform_io_get(uint8_t bus_addr);
void st1wire_platform_io_in(uint8_t bus_addr);
void st1wire_platform_io_out(uint8_t bus_addr);
void st1wire_platform_wake(uint8_t bus_addr);
void st1wire_platform_delay(uint32_t delay);
void st1wire_platform_start_timeout(uint32_t timeout);
int8_t st1wire_platform_is_timeout_exceeded(void);
