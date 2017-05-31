
#include <stdint.h>
#include <stdbool.h>

#include "timer.h"
#include "interrupts.h"

/** @brief The BCM2835 Interupt controller peripheral at it's base address */
static rpi_irq_controller_t* rpiIRQController =
        (rpi_irq_controller_t*)RPI_INTERRUPT_CONTROLLER_BASE;


/**
    @brief Return the IRQ Controller register set
*/
rpi_irq_controller_t* RPI_GetIrqController( void )
{
    return rpiIRQController;
}

/**
    @brief The IRQ Interrupt handler
    This handler is run every time an interrupt source is triggered. It's
    up to the handler to determine the source of the interrupt and most
    importantly clear the interrupt flag so that the interrupt won't
    immediately put us back into the start of the handler again.
*/
void c_irq_handler ( void )
{
    //Clear the interrupt...
    RPI_GetArmTimer()->IRQClear = 1;

    //Do whatever it is you wanted to do..
    uart_println("\n\nInterrupted");
}
