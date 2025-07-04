#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Scheduler Related */
#define configUSE_PREEMPTION                    1
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     1 // Empfohlen, um bspw. Statistiken zu sammeln oder den WDT zurückzusetzen
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 ) // 1000 Hz Tick-Rate = 1 ms Periodendauer
#define configMAX_PRIORITIES                    32 // Maximale Task-Prioritäten
#define configMINIMAL_STACK_SIZE                ( configSTACK_DEPTH_TYPE ) 256 // Minimale Stack-Größe für eine Idle-Task
#define configUSE_16_BIT_TICKS                  0 // Verwende 32-Bit Ticks, da RP2040 32-Bit-Architektur ist

#define configIDLE_SHOULD_YIELD                 1 // Erlaubt der Idle-Task das Yielding, wenn eine andere Task gleicher Priorität bereit ist

// **NEU**: CPU-Taktrate für den Raspberry Pi Pico RP2040
#define configCPU_CLOCK_HZ                      ( 125000000UL ) // Standard Pico RP2040 Taktfrequenz

/* Synchronization Related */
#define configUSE_MUTEXES                       1 // Aktiviere Mutexes für den Schutz von Ressourcen
#define configUSE_RECURSIVE_MUTEXES             1 // Aktiviere rekursive Mutexes
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1 // Aktiviere Zähl-Semaphore
#define configQUEUE_REGISTRY_SIZE               8 // Größe des Queue Registry, nützlich für Debugging
#define configUSE_QUEUE_SETS                    1 // Aktiviere Queue Sets
#define configUSE_TIME_SLICING                  1 // Aktiviere Time Slicing (Rundlauf-Scheduling für gleiche Priorität)
#define configUSE_NEWLIB_REENTRANT              0 // Setze auf 1, wenn Newlib reentrant ist (für C Standard Library Funktionen)
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

/* System */
#define configSTACK_DEPTH_TYPE                  uint32_t // Typ für Stack-Tiefe
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t   // Typ für Nachrichtenpuffer-Länge

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1 // Aktiviere statische Speicherallokation (z.B. für Tasks)
#define configSUPPORT_DYNAMIC_ALLOCATION        1 // Aktiviere dynamische Speicherallokation (z.B. xTaskCreate)
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 128 * 1024 ) ) // 128 KB Heap-Größe für FreeRTOS
#define configAPPLICATION_ALLOCATED_HEAP        0 // Setze auf 1, wenn der Heap manuell definiert wird

/* Hook function related definitions. */
#define configCHECK_FOR_STACK_OVERFLOW          2 // Überprüfung auf Stack-Überlauf (2 für vollen Check)
#define configUSE_MALLOC_FAILED_HOOK            1 // Aktiviere Hook bei fehlgeschlagener malloc-Operation
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0 // Aktiviere Laufzeitstatistiken (benötigt Hardware-Timer)
#define configUSE_TRACE_FACILITY                1 // Aktiviere Trace-Funktionen (z.B. für Tracealyzer)
#define configUSE_STATS_FORMATTING_FUNCTIONS    0 // Formatiert Statistiken (wenn configGENERATE_RUN_TIME_STATS 1 ist)

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0 // Deaktiviere Co-Routinen (veraltet, Tasks bevorzugt)
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1 // Aktiviere Software-Timer
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 ) // Priorität der Software-Timer-Task
#define configTIMER_QUEUE_LENGTH                10 // Länge der Software-Timer-Befehlswarteschlange
#define configTIMER_TASK_STACK_DEPTH            1024 // Stack-Tiefe der Software-Timer-Task

/* Interrupt nesting behaviour configuration. */
// **NEU**: MCU-spezifische Definitionen für Interrupt-Prioritäten auf ARM Cortex-M0+
// __NVIC_PRIO_BITS ist ein CMSIS-Define, das die Anzahl der implementierten Prioritätsbits im NVIC angibt.
// Für den RP2040 sind dies 4 Bits.
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS                     __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS                     4 // Fallback-Wert für RP2040, falls nicht definiert
#endif

// Die niedrigste (numerisch höchste) Interrupt-Priorität, die der Kernel intern verwendet.
// ISRs, die FreeRTOS-API-Funktionen aufrufen, müssen eine HÖHERE Priorität haben als diese.
// (1U << configPRIO_BITS) - 1U setzt alle 4 Bits auf 1, was 15 (die niedrigste Priorität) ergibt.
#define configKERNEL_INTERRUPT_PRIORITY         ( ( 1U << configPRIO_BITS ) - 1U )

// Die maximale Interrupt-Priorität (numerisch niedrigste), von der aus FreeRTOS-API-Funktionen
// aufgerufen werden dürfen. Interrupts mit höherer Priorität (numerisch niedriger, näher an 0)
// als diese DÜRFEN KEINE FreeRTOS-API-Funktionen aufrufen, um die Integrität des Schedulers
// zu gewährleisten. Ein Wert von 5 ist hier ein gängiger Ausgangspunkt.
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( 5 << ( 8 - configPRIO_BITS ) ) // Passt an die Anzahl der Prioritätsbits an.

/* SMP port only */
#define configNUM_CORES                         2 // Setze dies auf 2 für den Dual-Core RP2040
#define configTICK_CORE                         0 // Welcher Kern den FreeRTOS-Tick generiert (Core 0 empfohlen)
#define configRUN_MULTIPLE_PRIORITIES           0 // Setze auf 1, um gleiche Prioritäten auf verschiedenen Kernen laufen zu lassen

/* RP2040 specific */
// **NEU**: Diese sind entscheidend für die reibungslose Zusammenarbeit mit dem Pico SDK
#define configSUPPORT_PICO_SYNC_INTEROP         1 // Ermöglicht FreeRTOS und Pico SDK, dieselben Synchronisationsmechanismen zu nutzen
#define configSUPPORT_PICO_TIME_INTEROP         1 // Ermöglicht FreeRTOS und Pico SDK, dass die Zeitmessung kompatibel ist

#include <assert.h>
/* Define to trap errors during development. */
#define configASSERT(x)                         assert(x) // Assertion Makro für Debugging

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1 // Nützlich für die Überwachung des Stack-Verbrauchs
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xTaskResumeFromISR              1
#define INCLUDE_xQueueGetMutexHolder            1

// **NEU**: FreeRTOS Interrupt-Handler für Cortex-M Prozessoren
// Diese leiten die FreeRTOS-internen Handler-Namen auf die tatsächlichen
// Interrupt-Vektor-Tabelle-Namen um.
#define vPortSVCHandler                         SVC_Handler
#define xPortPendSVHandler                      PendSV_Handler
#define xPortSysTickHandler                     SysTick_Handler

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */