#include "bsp_debug_usart.h"
#include "stdio.h"
#include "stdlib.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

uint16_t  t;                                 //���ڼ����ı���

//����1
/*******************************************************/
uint8_t   USART1_RX_BUF[USART1_REC_LEN];     //����1���ջ���,���USART_REC_LEN���ֽ�.
uint8_t   USART1_aRxBuffer[RXBUFFERSIZE];    //����1HAL��ʹ�õĴ��ڽ��ջ���
uint16_t  USART1_RX_STA=0;                   //����1����״̬���	
/*******************************************************/

//����2
/*******************************************************/
uint8_t   USART2_RX_BUF[USART2_REC_LEN];     //����2���ջ���,���USART_REC_LEN���ֽ�.
uint8_t   USART2_aRxBuffer[RXBUFFERSIZE];    //����2HAL��ʹ�õĴ��ڽ��ջ���
uint16_t  USART2_RX_STA=0;                   //����2����״̬���	
/*******************************************************/

//����3
/*******************************************************/
uint8_t   USART3_RX_BUF[USART3_REC_LEN];     //����3���ջ���,���USART_REC_LEN���ֽ�.
uint8_t   USART3_aRxBuffer[RXBUFFERSIZE];    //����3HAL��ʹ�õĴ��ڽ��ջ���
uint16_t  USART3_RX_STA=0;                   //����3����״̬���	
/*******************************************************/

//��ݮ�ɴ���1ͨ��
/*******************************************************/
uint8_t   raspberry_len;                     //��ݮ�ɴ�������ݳ���
uint8_t   *rapberry_BUF;                     //��ݮ�����ݴ������
/*******************************************************/

//�����മ��2ͨ�ź���
/*******************************************************/
uint8_t   tof_len;                           //�����ഫ������ݳ���
uint8_t   *tof_BUF;                          //���������ݴ������
/*******************************************************/

//���ھ��
/*******************************************************/
UART_HandleTypeDef  UART1_Handler;           //UART1���
UART_HandleTypeDef  UART2_Handler;           //UART2���
UART_HandleTypeDef  UART3_Handler;           //UART3���
/*******************************************************/

  
//����1��ʼ��������������������
void uart1_init(uint32_t bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance          =  USART1;					      //USART1
	UART1_Handler.Init.BaudRate     =  bound;				          //������
	UART1_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //һ��ֹͣλ
	UART1_Handler.Init.Parity       =  UART_PARITY_NONE;		  //����żУ��λ
	UART1_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					                  //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)USART1_aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

//����2��ʼ��������������������
void uart2_init(uint32_t bound)
{	
	//UART ��ʼ������
	UART2_Handler.Instance          =  USART2;					      //USART2
	UART2_Handler.Init.BaudRate     =  bound;				          //������
	UART2_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //һ��ֹͣλ
	UART2_Handler.Init.Parity       =  UART_PARITY_NONE;		  //����żУ��λ
	UART2_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					                  //HAL_UART_Init()��ʹ��UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)USART2_aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

//����3��ʼ��������������������
void uart3_init(uint32_t bound)
{	
	//UART ��ʼ������
	UART3_Handler.Instance          =  USART3;					      //USART3
	UART3_Handler.Init.BaudRate     =  bound;				          //������
	UART3_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //һ��ֹͣλ
	UART3_Handler.Init.Parity       =  UART_PARITY_NONE;		  //����żУ��λ
	UART3_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					                  //HAL_UART_Init()��ʹ��UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)USART3_aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)                 //����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			        //ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			      //ʹ��USART1ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			        //PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			      //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			        //PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //��ʼ��PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				  //ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);		//��ռ���ȼ�3�������ȼ�3
	}
	
	else if(huart->Instance==USART2)            //����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			        //ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			      //ʹ��USART1ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_2;			        //PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			      //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //��ʼ��PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			        //PA3
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //��ʼ��PA10
		
		HAL_NVIC_EnableIRQ(USART2_IRQn);				  //ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,3);		//��ռ���ȼ�3�������ȼ�3
	}
	
	else if(huart->Instance==USART3)            //����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			        //ʹ��GPIOBʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			      //ʹ��USART3ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_10;			        //PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			      //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //����
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	  //��ʼ��PB10

		GPIO_Initure.Pin=GPIO_PIN_11;			        //PB11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	  //��ʼ��PB11
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);				  //ʹ��USART3�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);		//��ռ���ȼ�3�������ȼ�3
	}
}


//�����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(USART1_aRxBuffer[0]!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(USART1_aRxBuffer[0]==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_aRxBuffer[0] ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}
	
	else if(huart->Instance==USART2)//����Ǵ���2
	{
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(USART2_aRxBuffer[0]!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(USART2_aRxBuffer[0]==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=USART2_aRxBuffer[0] ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}
	
	else if(huart->Instance==USART3)//����Ǵ���3
	{
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(USART3_aRxBuffer[0]!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(USART3_aRxBuffer[0]==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=USART3_aRxBuffer[0] ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}
}
 
//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)USART1_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}	

//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)USART2_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

//����3�жϷ������
void USART3_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)USART3_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

//��ݮ��������ȡ
void raspberry_extracts(uint8_t *str) //a[0]Ϊx���꣬a[1]Ϊy����
{
	int i,j=0,flag=0,t=0,a[20];
   for(i=0;str[i]!='\0';i++){
      if(str[i]>='0'&&str[i]<='9'&&flag==0){//�ж��ǲ��ǵ�һ������ 
          flag=1;
          t=(int)str[i]-'0';//�ַ���ǿ��ת��Ϊ���Ͷ���-'0'������ 
   }
      else if(str[i]>='0'&&str[i]<='9'&&flag!=0)//�ж��ǲ���ǰһ�����ֺ���������� 
          t=t*10+(int)str[i]-'0';
    else if(flag!=0){
     a[j]=t;
     j++;
     flag=0;
    }
   }
   if(str[i-1]>='0'&&str[i-1]<='9'){//������һ���ַ�������Ҳ���Խ������ 
               a[j]=t;
            j++;
          }
   printf("����%d�������ֱ��ǣ�\n",j);
   for(i=0;i<j;i++)
   printf("%d\t",a[i]);
}

//������������ȡ
void tof_extracts(uint8_t *str) //a[0]Ϊ ����
{
 int i,j=0,flag=0,t=0;
 float a[20],distance;
   for(i=0;str[i]!='\0';i++){
      if(str[i]>='0'&&str[i]<='9'&&flag==0){//�ж��ǲ��ǵ�һ������ 
          flag=1;
          t=(float)str[i]-'0';//�ַ���ǿ��ת��Ϊ���Ͷ���-'0'������ 
   }
      else if(str[i]>='0'&&str[i]<='9'&&flag!=0)//�ж��ǲ���ǰһ�����ֺ���������� 
          t=t*10+(float)str[i]-'0';
    else if(flag!=0){
     a[j]=t;
     j++;
     flag=0;
    }
   }
   if(str[i-1]>='0'&&str[i-1]<='9'){//������һ���ַ�������Ҳ���Խ������ 
               a[j]=t;
            j++;
          }
   printf("����%d�������ֱ��ǣ�\n",j);
   for(i=0;i<j;i++)
       printf("%f\t",a[i]);
//   HAL_UART_Transmit(&UART1_Handler,a,20,1000);
		//return(distance = a[0]);
}

//��ݮ�ɴ���1ͨ�ź���
void raspberry_communication(void)
{
	if(USART1_RX_STA&0x8000)
		{					   
			raspberry_len=USART1_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			rapberry_BUF=(uint8_t *) malloc ( sizeof(uint16_t) * raspberry_len);
			for(t=0;t<raspberry_len;t++)
			{
				rapberry_BUF[t] = USART1_RX_BUF[t];
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			}
			USART1_RX_STA=0;
			//���ݻ���
			for(t=0;t<raspberry_len;t++)
			{
				printf("%c",rapberry_BUF[t]);
			}
			//������ȡ
			raspberry_extracts(rapberry_BUF);
//			HAL_UART_Transmit(&UART1_Handler,rapberry_BUF,raspberry_len,1000);	//���ͽ��յ�������
		}
}

//�����മ��2ͨ�ź���
void tof_communication(void)   
{
	//Usart_SendString("hi\r\n");
//	uint8_t TXBUFF[5] = {"h","\","r","\","n"};
//  HAL_UART_Transmit(&UART2_Handler,TXBUFF,5,1000);
	//HAL_UART_Transmit(&UART2_Handler, "hi\r\n", sizeof("hi\r\n"), 1000);
  //printf("hi\r\n");
//	if(USART2_RX_STA&0x8000)
//		{					   
			tof_len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART2_RX_BUF,tof_len,1000);	//���ͽ��յ������ݣ�ͨ������1����
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
//			USART2_RX_STA=0;
			
			tof_BUF=(uint8_t *) malloc ( sizeof(uint16_t) * tof_len);
			for(t=0;t<tof_len;t++)
			{
				tof_BUF[t] = USART2_RX_BUF[t];
				while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			}
			//���ݻ���
			HAL_UART_Transmit(&UART1_Handler,tof_BUF,tof_len,1000);
			free(tof_BUF);
			USART2_RX_STA=0;
			//������ȡ
			tof_extracts(tof_BUF);
//		}

//   printf("hi\r\n");
//	 if(USART2_RX_STA&0x8000)
//		{					   
//			tof_len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
////			printf("\r\n�����͵���ϢΪ:\r\n");
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART2_RX_BUF,tof_len,1000);	//���ͽ��յ������ݣ�ͨ������1����
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
////			printf("\r\n\r\n");//���뻻��
//			USART2_RX_STA=0;
//		 }


}

