#include "spi.h"

/*****************************************************************************************
*函数功能：SPI1初始化
*函数形参:无
*函数返回值：无
*函数备注:FLASH_CS->PB14		SPI1_MOSI->PB5		SPI1_MISO->PB4  SPI1_SCK->PB3
*
*****************************************************************************************/

void Spi1_init(void)
{
	RCC->AHB1ENR |=1<<1;//打开PB组时钟
	
	//FLASH_CS->PB14
	GPIOB->MODER &=~(3<<(2*14));//清零
	GPIOB->MODER |=1<<(2*14);//输出模式
	GPIOB->OTYPER &=~(1*14);//推挽输出
	GPIOB->OSPEEDR &=~(3<<(2*14));//清零
	GPIOB->OSPEEDR |=2<<(2*14);//50M输出速度
	GPIOB->ODR |=1<<14;//初始化拉高CS
	
	//SPI1_MOSI->PB5
	GPIOB->MODER &=~(3<<(2*5));//清零
	GPIOB->MODER |=2<<(2*5);//复用功能模式
	GPIOB->AFR[0] &=~(0XF<<(4*5));//清零
	GPIOB->AFR[0] |=5<<(4*5);//复用成SPI1模式

	//SPI1_MISO->PB4
	GPIOB->MODER &=~(3<<(2*4));//清零
	GPIOB->MODER |=2<<(2*4);//复用功能模式
	GPIOB->AFR[0] &=~(0XF<<(4*4));//清零
	GPIOB->AFR[0] |=5<<(4*4);//复用成SPI1模式

	//SPI1_SCK->PB3
	GPIOB->MODER &=~(3<<(2*3));//清零
	GPIOB->MODER |=2<<(2*3);//复用功能模式
	GPIOB->AFR[0] &=~(0XF<<(4*3));//清零
	GPIOB->AFR[0] |=5<<(4*3);//复用成SPI1模式
	

	
	/* 配置SPI工作方式 */
	RCC->APB2ENR |= 1 << 12;   //使能SPI1时钟
	
	SPI1->CR1 &=~(1<<15);//选择双线单向通信数据模式
	SPI1->CR1 &=~(1<<11);//8位数据帧格式
	SPI1->CR1 &=~(1<<10);//全双工
	SPI1->CR1 |=1<<9;//使能软件从器件管理
	SPI1->CR1 |= 1 << 8;       //NSS软件控制
	SPI1->CR1 &=~(1<<7);//先发高位
	SPI1->CR1 &=~(7<<3);//清零
	SPI1->CR1 |=0x0018;//16分频
	SPI1->CR1 |=1<<2;//主配置
	SPI1->CR1 &=~(1<<1);
	SPI1->CR1 &=~(1<<0);//模式0
	
	SPI1->CR1 |=1<<6;//SPI使能
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  SPI_InitTypeDef  SPI_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
// 
//  //初始化设置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
//	
//  //PB14初始化设置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14复用功能输出	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出功能
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1

// 
//	//这里只针对SPI口初始化
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
////	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
//	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
// 
//	SPI_Cmd(SPI1, ENABLE); //使能SPI外设	
//	SPI1_Send_Read(0XFF);
}

/*****************************************************************************************
*函数功能：SPI发送接收1个字节
*函数形参:无
*函数返回值：无
*函数备注:FLASH_CS->PB14		SPI1_MOSI->PB5		SPI1_MISO->PB4  SPI1_SCK->PB3
*
*****************************************************************************************/
u8 SPI1_Send_Read(u8 data)
{
//	SPI1->DR = data;
//	while(!(SPI1->SR & (1 << 1)))
//	{
//		/* 等待发送完成 */
//		lite_debug("");
//	}
//	while(!(SPI1->SR & (1 << 0)))
//	{lite_debug("");
//		/* 等待接收数据 */
//	}
//	return SPI1->DR;
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
		}//等待发送区空  
	SPI_I2S_SendData(SPI1, data); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
	} //等待接收完一个byte  
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}
