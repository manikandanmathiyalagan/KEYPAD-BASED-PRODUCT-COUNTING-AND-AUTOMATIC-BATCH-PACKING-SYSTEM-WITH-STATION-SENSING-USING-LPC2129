#include <lpc21xx.h>


#define lcd_d 0x0f<<8
#define RS 1<<6
#define E 1<<7

void delay_ms(unsigned int );
void LCD_INIT(void);
void LCD_DATA(unsigned char);
void LCD_COMMAND(unsigned char);
void LCD_STRING(unsigned char *);
void LCD_INTEGER(int);

void LCD_INIT(void)
{
	IODIR0=lcd_d|RS|E;
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X02);
	LCD_COMMAND(0X0C);
	LCD_COMMAND(0x28);
}

void LCD_COMMAND(unsigned char ch)
{
	//UPPER NIBBLE
	IOCLR0=lcd_d;
	IOSET0=(ch&0XF0)<<4;
	IOCLR0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
	// LOWER NIBBLE
	IOCLR0=lcd_d;
	IOSET0=(ch&0X0F)<<8;
	IOCLR0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}

void LCD_DATA(unsigned char d)
{
	//UPPER NIBBLE
	IOCLR0=lcd_d;
	IOSET0=(d&0XF0)<<4;
	IOSET0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
	// LOWER NIBBLE
	IOCLR0=lcd_d;
	IOSET0=(d&0X0F)<<8;
	IOSET0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}

void LCD_STRING(unsigned char * s)
{
	unsigned char pos=0;
	while(*s)
	{
		LCD_DATA(*s++);
		pos++;
		if(pos==16)
			LCD_COMMAND(0XC0);
	}
}

void delay_ms(unsigned int ms)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
}

void LCD_INTEGER(int n)
{
	signed char i=0;
	unsigned char arr[5];
	if(n==0)
		LCD_DATA('0');
	else
	{
		if(n<0)
		{
			LCD_DATA('-');
			n=-n;
		}
		while(n>0)
		{
			arr[i++]=n%10;
			n=n/10;
		}
		for(i=i-1;i>=0;i--)
			LCD_DATA(arr[i]+48);
	}
}
