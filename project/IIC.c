#include "stm8l10x.h"
#include "IIC.h"

//****************************Micro*******************************/
#define	READ_CW2015				0xc5
#define	WRITE_CW2015			0xc4

#define I2C_DELAY		2

//***************************function*******************************/
void delay_10us()
{
  unsigned char a,b;
  for(b=12;b>0;b--)
    for(a=7;a>0;a--);
}


void start(void)			
{
	SDA_SET_OUT();
	SDA_Pin_OUT(1);				
	SCL_Pin_OUT(1);			
	delay_10us();
	SDA_Pin_OUT(0);				
	delay_10us();
	SCL_Pin_OUT(0);			
}
//-------------------------------------------------------------------
void stop(void)				
{
	SDA_SET_OUT();
	SDA_Pin_OUT(0);			
	delay_10us();
	SCL_Pin_OUT(1);			
	delay_10us();
	SDA_Pin_OUT(1);			
	delay_10us();
	//	SCL_Pin_OUT(0);
	delay_10us();
}
//-------------------------------------------------------------------
unsigned char r_ack(void)				//����Ӧ���źź���
{
  unsigned char ack;				//����һ��λ���������ݴ�Ӧ��״̬��
  SDA_SET_IN();
	SCL_Pin_OUT(1);				//����ʱ���ߡ�
	delay_10us();
	ack = SDA_Pin_IN();				//��ȡӦ���źŵ�״̬��
	delay_10us();
	SCL_Pin_OUT(0);				//����ʱ���ߡ�
	delay_10us();
	return ack;				//����Ӧ���źŵ�״̬��0��ʾӦ��1��ʾ��Ӧ��
}

unsigned char wait_r_ack(void)
{
	unsigned char ack;
	unsigned int i;	
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	return 0;
}
//---------------------------------------`----------------------------
void s_ack(void)				//����Ӧ���źź���
{
	SDA_SET_OUT();
	SDA_Pin_OUT(1);				//�ͷ��������ߣ�׼������Ӧ���źš�
	delay_10us();
	SCL_Pin_OUT(1);				//����ʱ���ߡ�
	delay_10us();
	SCL_Pin_OUT(0);				//����ʱ���ߡ�
	delay_10us();

}

void sn_ack(void)				//Master����׼������ָ��
{
	SDA_SET_OUT();
	SDA_Pin_OUT(0);
	delay_10us();
	SCL_Pin_OUT(1);
	delay_10us();
	SCL_Pin_OUT(0);
	SDA_Pin_OUT(1);
	delay_10us();
}

//-------------------------------------------------------------------
void write_byte(unsigned char w_data)		//��IIC����д��һ���ֽڵ����ݺ��� 
{
	unsigned char i;
	SDA_SET_OUT();
	for(i = 0; i < 8; i++)				//��8λ����
	{
		if(w_data & 0x80)
			SDA_Pin_OUT(1);
		else
			SDA_Pin_OUT(0);
		delay_10us();
		SCL_Pin_OUT(1);						//����ʱ���ߣ�����д�뵽�豸�С�
		delay_10us();
		SCL_Pin_OUT(0);						//����ʱ���ߣ�����ı������ߵ�״̬
		delay_10us();
		w_data = w_data << 1;				//��������һλ���Ѵθ�λ�������λ,Ϊд��θ�λ��׼��
  }
}
//-------------------------------------------------------------------
unsigned char read_byte(void)			//��IIC���߶�ȡһ���ֽڵ����ݺ���
{
	unsigned char i;
	unsigned char r_data = 0;					//����һ������Ĵ�����
	SDA_SET_IN();
	for(i = 0; i < 8; i++)				//��8λ����
	{
		SCL_Pin_OUT(1);						//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
		delay_10us();
		r_data = r_data << 1;				//�������ֽڵ���������һλ��׼����ȡ���ݡ�
		delay_10us();
		if(SDA_Pin_IN())							//���������Ϊ��ƽ��ƽ��
		{
			r_data = r_data | 0x1;			//��������ֽڵ����λд1��
		}
		SCL_Pin_OUT(0);						//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
		delay_10us();
	}	
	return r_data;						//���ض�ȡ��һ���ֽ����ݡ�
}

//------------------------IIC��д����--------------------------------
unsigned char cw_read(unsigned char point_reg,unsigned char *r_pdata)
{
	start();
	write_byte(WRITE_CW2015);
	if(wait_r_ack())
	{
		return 1;
	}
	write_byte(point_reg);
	if(wait_r_ack())
	{
		return 1;
	}
	start();
	write_byte(READ_CW2015);
	if(wait_r_ack())
	{
		return 1;
	}
	*r_pdata = read_byte();
	s_ack();
	stop();
	return 0;
}

unsigned char cw_read_word(unsigned char point_reg,unsigned char *r_pdata, unsigned int length)
{
	unsigned char count;

	start();
	write_byte(WRITE_CW2015);
	if(wait_r_ack())
	{
		return 1;
	}
	write_byte(point_reg);
	if(wait_r_ack())
	{
		return 1;
	}
	start();
	write_byte(READ_CW2015);
	if(wait_r_ack())
	{
		return 1;
	}
	for(count = 0; count < length; count++ ){
		*r_pdata = read_byte();
		if(count + 1 < length){
			r_pdata++;
			sn_ack();
		}
	}
	s_ack();
	stop();
	return 0;
}

unsigned char cw_write(unsigned char point_reg,unsigned char *w_pdata)
{
	start();
	write_byte(WRITE_CW2015);
	if(wait_r_ack())
	{
		return 1;
	}
	write_byte(point_reg);
	if(wait_r_ack())
	{
		return 1;
	}
	write_byte(*w_pdata);
	if(wait_r_ack())
	{
		return 1;
	}
	stop();
	return 0;
}
