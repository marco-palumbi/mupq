#ifndef HAL_TRACETX_H
#define HAL_H

#include <stdint.h>

void tracetx_dma_setup(void);
void tracetx_set_lenght(uint16_t number);
void tracetx_start(void *txbuffer);
void tracetx_wait(void);

#endif