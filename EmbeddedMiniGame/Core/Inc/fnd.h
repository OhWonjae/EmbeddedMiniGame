#ifndef FND__H
#define FND__H
#include "main.h"
/*
 * 7 Segment 헤더 파일
 */
void send_init(SPI_HandleTypeDef* hspi);
void digit4(int n);
#endif
