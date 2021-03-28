#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#define Flash_CS_H ( GPIOB->BSRRL |= 1 << 14 )//��λ����
#define Flash_CS_L ( GPIOB->BSRRH |= 1 << 14 )//��λ����

void Spi1_init(void);
u8 SPI1_Send_Read(u8 data);




#endif
