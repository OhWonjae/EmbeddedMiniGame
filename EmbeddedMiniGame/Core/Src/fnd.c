#include "main.h"
#include "global.h"
/*
 * 7 segment 파일
 */
static const uint8_t _LED_0F[29];
static SPI_HandleTypeDef* hspi;
void send_init(SPI_HandleTypeDef* _hspi){
	hspi = _hspi;
	_LED_0F[0] = 0xC0; //0
	  _LED_0F[1] = 0xF9; //1
	  _LED_0F[2] = 0xA4; //2
	  _LED_0F[3] = 0xB0; //3
	  _LED_0F[4] = 0x99; //4
	  _LED_0F[5] = 0x92; //5
	  _LED_0F[6] = 0x82; //6
	  _LED_0F[7] = 0xF8; //7
	  _LED_0F[8] = 0x80; //8
	  _LED_0F[9] = 0x90; //9
	  _LED_0F[10] = 0x88; //A
	  _LED_0F[11] = 0x83; //b
	  _LED_0F[12] = 0xC6; //C
	  _LED_0F[13] = 0xA1; //d
	  _LED_0F[14] = 0x86; //E
	  _LED_0F[15] = 0x8E; //F
	  _LED_0F[16] = 0xC2; //G
	  _LED_0F[17] = 0x89; //H
	  _LED_0F[18] = 0xF9; //I
	  _LED_0F[19] = 0xF1; //J
	  _LED_0F[20] = 0xC3; //L
	  _LED_0F[21] = 0xA9; //n
	  _LED_0F[22] = 0xC0; //O
	  _LED_0F[23] = 0x8C; //P
	  _LED_0F[24] = 0x98; //q
	  _LED_0F[25] = 0x92; //S
	  _LED_0F[26] = 0xC1; //U
	  _LED_0F[27] = 0x91; //Y
	  _LED_0F[28] = 0xFE; //hight -

}
void send(uint8_t x){
	  HAL_SPI_Transmit(hspi, &x, sizeof(x), 100);
}
void send_port(uint8_t x, uint8_t port){
	  send(x);
	  send(port);
	  HAL_GPIO_WritePin(SPI1_RCK_GPIO_Port ,SPI1_RCK_Pin, 0);
	  HAL_GPIO_WritePin(SPI1_RCK_GPIO_Port ,SPI1_RCK_Pin, 1);
}
void digit4(int n)
{
  int n1, n2, n3, n4;
  n1 = (int)  n % 10;
  n2 = (int) ((n % 100)-n1)/10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;

  if(fndOrder==0){
	  send_port(_LED_0F[n1], 0b0001);
	  fndOrder+=1;
  }else if(fndOrder==1){
	  send_port(_LED_0F[n2], 0b0010);
	  fndOrder+=1;
  }else if(fndOrder==2){
	  send_port(_LED_0F[n3], 0b0100);
	  fndOrder+=1;
  }else if(fndOrder==3){
	  send_port(_LED_0F[n4], 0b1000);
	  fndOrder=0;
  }
}
