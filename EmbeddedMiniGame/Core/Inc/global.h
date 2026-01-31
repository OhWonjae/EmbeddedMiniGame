#ifndef _GLOBAL_H
#define _GLOBAL_H
#include "stdint.h"
/*
 * .h파일에서 선언된 구조체를 extern, .c에서 초기화 (변수의 multiple definition 방지)
 */
typedef struct _INPUT{
	uint8_t left_btn_down;
	uint8_t right_btn_down;
}Input;

extern Input input;
#endif _GLOBAL_H
