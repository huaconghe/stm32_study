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

extern vu8 bDeviceState;		//USB���� ���
void usb_vcp_test(void)//������
{
	u16 t;
	u16 len;	
	u16 times=0;    
	u8 usbstatus=0;	
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
	 while(1)
	{
		if(usbstatus!=bDeviceState)//USB����״̬�����˸ı�.
		{
			usbstatus=bDeviceState;//��¼�µ�״̬
			if(usbstatus==1)
			{
				//printf("USB Connected   \r\n");
				LED1_ON;//DS1��
			}
			else
			{
				//printf("USB DisConnected   \r\n");
				LED1_OFF;//DS1��
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
			usb_printf("\r\n�����͵���ϢΪ:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				VCP_DataTx(USB_USART_RX_BUF[t]);//���ֽڷ�ʽ,���͸�USB 
			}
			usb_printf("\r\n\r\n");//���뻻��
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				usb_printf("\r\n������STM32F4/F7������USB���⴮��ʵ��\r\n");
				usb_printf("����ԭ��@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)usb_printf("����������,�Իس�������\r\n");  
			if(times%30==0)
				LED2_XOR;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
	}  
	

}












	FATFS fsobject;
	BYTE work[FF_MAX_SS]; 

	FIL fp ;
	const char write_buf[] = "12345��ӭʹ��67890";
	char  read_buf[4096] = {0};
	UINT bw;
	UINT br;
//fatfs test
void fatfs_test(void)
{
	FRESULT  res ;
 
		
		res = f_mount(&fsobject,  "1:",  1);   //�����ļ�ϵͳ
		printf("f_mount res = %d\r\n",res);
		printf("����һ��FATFS�ļ�ϵͳ��ֲʵ��\r\n");
		if(res == FR_NO_FILESYSTEM)
		{
			res = f_mkfs("1:",0,work,sizeof(work));
			printf("f_mkfs  is  over\r\n");
			printf("res = %d\r\n",res);
			res = f_mount(NULL,  "1:",  1);   //ȡ���ļ�ϵͳ
				res = f_mount(&fsobject,  "1:",  1);   //�����ļ�ϵͳ
			printf("res = %d\r\n",res);
		}
			
		printf("hello word......\r\n");
		
		res = f_open(&fp , "1:abcd" , FA_OPEN_ALWAYS|FA_READ |FA_WRITE );//????&fp/fp
		//printf("f_open res = %d fp = %d\r\n",res,fp);
		if(res == RES_OK)
		{
			f_lseek(&fp,0);
			 res = f_write(&fp,write_buf,sizeof(write_buf),&bw); //bwд��ɹ���
			 printf("bw=%d\r\n",bw);
			 if(res == RES_OK)
			 {
					f_lseek(&fp,0);
					res = f_read(&fp ,read_buf,f_size(&fp),&br);
					if(res == RES_OK)
					{
						 printf("�ļ�����Ϊ��%s ,br=%d\r\n",read_buf,br);
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
			 res = f_write(&fp,"abcdefghs1234567890d",sizeof(write_buf),&bw); //bwд��ɹ���
			 printf("bw=%d\r\n",bw);
			 if(res == RES_OK)
			 {
					f_lseek(&fp,0);
					res = f_read(&fp ,read_buf,f_size(&fp),&br);
					if(res == RES_OK)
					{
						 printf("�ļ�����Ϊ��%s ,br=%d\r\n",read_buf,br);
					}
			 }
		}
		res = f_close(&fp);
		printf("f_close res = %d\r\n",res);
		while(1)
		{
		}

}

