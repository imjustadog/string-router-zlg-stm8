#ifndef __I2C_H__
#define __I2C_H__

#define GPIO_SDA GPIOC
#define GPIO_PIN_SDA GPIO_Pin_0

#define GPIO_SCL GPIOC
#define GPIO_PIN_SCL GPIO_Pin_1

#define IIC_SDA0    GPIOC->ODR &= (uint8_t)(~GPIO_Pin_0)
#define IIC_SDA1    GPIOC->ODR |= GPIO_Pin_0
#define IIC_SCL0    GPIOC->ODR &= (uint8_t)(~GPIO_Pin_1)
#define IIC_SCL1    GPIOC->ODR |= GPIO_Pin_1

#define SDA_SET_IN()	GPIOC->DDR &= (uint8_t)(~(GPIO_Pin_0))
#define SDA_SET_OUT()   GPIOC->DDR |= GPIO_Pin_0

#define noACK 0            
#define ACK   1            

#define SCL_Pin_OUT(x) do{if(x){IIC_SCL1;}else{IIC_SCL0;}}while(0)
#define SDA_Pin_OUT(x) do{if(x){IIC_SDA1;}else{IIC_SDA0;}}while(0)
#define SDA_Pin_IN() ((uint8_t)(GPIOC->IDR & (uint8_t)GPIO_Pin_0))

unsigned char cw_write(unsigned char point_reg,unsigned char *w_pdata);
unsigned char cw_read(unsigned char point_reg,unsigned char *r_pdata);
unsigned char cw_read_word(unsigned char point_reg,unsigned char *r_pdata, unsigned int length);

#endif
