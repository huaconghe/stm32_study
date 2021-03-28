#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "system.h"
#include "system_config.h"
#include "ff.h"
/*usb*/  
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h" 

/*
	get rtos tick
	input: none
	output: pxTimeOut
	return: none
*/
void sys_get_tick(TimeOut_t * const pxTimeOut)
{
	vTaskSetTimeOutState(pxTimeOut);
}

/*
	check if timeout from tick pxTimeOut
	input: 	pxTimeOut-tick before
			ms-interval to check
	output: none
	return: 0-not timeout, 1-timeout
*/
int sys_is_timeout(TimeOut_t * const pxTimeOut, TickType_t *pxTicksToWait)
{
	//TickType_t pxTicksToWait=pdMS_TO_TICKS(ms);
	
	return xTaskCheckForTimeOut(pxTimeOut, pxTicksToWait);
}

USB_OTG_CORE_HANDLE  USB_OTG_dev;
void AppTask(void* parameter)
{

	//wifi_init();
	while(1)
	{
		//LED1_XOR;
		printf("w25q64_read_ID = %x\r\n",w25q64_read_ID());
		//wifi_test();
		//sdk_wifi_test();
		//fatfs_test();
		usb_vcp_test();
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

extern vu8 bDeviceState;		//USB连接 情况
void usb_vcp_test(void)//开发板
{
	u16 t;
	u16 len;	
	u16 times=0;    
	u8 usbstatus=0;	
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
	 while(1)
	{
		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
		{
			usbstatus=bDeviceState;//记录新的状态
			if(usbstatus==1)
			{
				//printf("USB Connected   \r\n");
				LED1_ON;//DS1亮
			}
			else
			{
				//printf("USB DisConnected   \r\n");
				LED1_OFF;//DS1灭
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
			usb_printf("\r\n您发送的消息为:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				VCP_DataTx(USB_USART_RX_BUF[t]);//以字节方式,发送给USB 
			}
			usb_printf("\r\n\r\n");//插入换行
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				usb_printf("\r\n阿波罗STM32F4/F7开发板USB虚拟串口实验\r\n");
				usb_printf("正点原子@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)usb_printf("请输入数据,以回车键结束\r\n");  
			if(times%30==0)
				LED2_XOR;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
	}  
	

}












	FATFS fsobject;
	BYTE work[FF_MAX_SS]; 

	FIL fp ;
	const char write_buf[] = "12345欢迎使用67890";
	char  read_buf[4096] = {0};
	UINT bw;
	UINT br;
//fatfs test
void fatfs_test(void)
{
	FRESULT  res ;
 
		
		res = f_mount(&fsobject,  "1:",  1);   //挂载文件系统
		printf("f_mount res = %d\r\n",res);
		printf("这是一个FATFS文件系统移植实验\r\n");
		if(res == FR_NO_FILESYSTEM)
		{
			res = f_mkfs("1:",0,work,sizeof(work));
			printf("f_mkfs  is  over\r\n");
			printf("res = %d\r\n",res);
			res = f_mount(NULL,  "1:",  1);   //取消文件系统
				res = f_mount(&fsobject,  "1:",  1);   //挂载文件系统
			printf("res = %d\r\n",res);
		}
			
		printf("hello word......\r\n");
		
		res = f_open(&fp , "1:abcd" , FA_OPEN_ALWAYS|FA_READ |FA_WRITE );//????&fp/fp
		//printf("f_open res = %d fp = %d\r\n",res,fp);
		if(res == RES_OK)
		{
			f_lseek(&fp,0);
			 res = f_write(&fp,write_buf,sizeof(write_buf),&bw); //bw写入成功的
			 printf("bw=%d\r\n",bw);
			 if(res == RES_OK)
			 {
					f_lseek(&fp,0);
					res = f_read(&fp ,read_buf,f_size(&fp),&br);
					if(res == RES_OK)
					{
						 printf("文件内容为：%s ,br=%d\r\n",read_buf,br);
					}
			 }
		}
		res = f_close(&fp);
		printf("f_close res = %d\r\n",res);
		
		res = f_open(&fp , "1:hello" , FA_OPEN_ALWAYS|FA_READ |FA_WRITE );//????&fp/fp
		//printf("f_open res = %d fp = %d\r\n",res,fp);
		if(res == RES_OK)
		{
			f_lseek(&fp,0);
			 res = f_write(&fp,"abcdefghs1234567890d",sizeof(write_buf),&bw); //bw写入成功的
			 printf("bw=%d\r\n",bw);
			 if(res == RES_OK)
			 {
					f_lseek(&fp,0);
					res = f_read(&fp ,read_buf,f_size(&fp),&br);
					if(res == RES_OK)
					{
						 printf("文件内容为：%s ,br=%d\r\n",read_buf,br);
					}
			 }
		}
		res = f_close(&fp);
		printf("f_close res = %d\r\n",res);
		while(1)
		{
		}

}

