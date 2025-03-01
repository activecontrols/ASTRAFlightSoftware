#include "Scheduler.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * Construct a scheduler with a list of modules.
 * Must pass correct size.
 */
Scheduler::Scheduler(FlightModule** schedule, int scheduleSize) {
    this->schedule = schedule;
    this->size = scheduleSize;
}
/**
 * Call inits for each module.
 */
int Scheduler::init()
{
    for (unsigned int i = 0; i < size; ++i) {
        this->schedule[i]->init();  // Calls init() on each module in the schedule
    }
    return NO_ERROR_CODE; // All modules have been initialized
}
/**
 * Task function for FreeRTOS
 */
void Scheduler::taskFunction(void* pvParameters) { // pvParameters is a pointer to the FlightModule object
    FlightModule* module = static_cast<FlightModule*>(pvParameters);
    while (true) { // Runs as long as task is running
        module->update(xTaskGetTickCount()); // updates the module with the current tick count
        vTaskDelay(pdMS_TO_TICKS(100));  // Delay for 0.1 seconds ()
    }
}
/**
 * Create FreeRTOS tasks for each module.
 */
 void Scheduler::createTasks() {
    for (unsigned int i = 0; i < size; ++i) {
        xTaskCreate( // creates new task
            Scheduler::taskFunction,       // Task function
            "ModuleTask",                  // Task name
            configMINIMAL_STACK_SIZE,      // Stack size
            this->schedule[i],             // Task parameter
            tskIDLE_PRIORITY,              // Task priority
            NULL                           // Task handle
        );
    }
}
/**
 * Call updates for each module.
 */
void Scheduler::update(unsigned long time) {
    for (unsigned int i = 0; i < size; ++i) {
        this->schedule[i]->update(time);  // Calls update() on each module in the schedule
    }
}