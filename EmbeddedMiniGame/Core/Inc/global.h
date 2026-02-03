#ifndef GLOBAL_H
#define GLOBAL_H
#include "stdint.h"
/*
 * .h파일에서 선언된 구조체를 extern, .c에서 초기화 (변수의 multiple definition 방지)
 */
// 버튼입력 정보
typedef struct _INPUT{
	uint8_t left_btn_down;
	uint8_t right_btn_down;
}Input;

// 게임 정보
typedef struct _GAME{
	uint8_t is_start;
	uint8_t score;
}Game;

// 점수판 자리수
extern uint8_t fndOrder;

extern Input input;
extern Game game;
#endif
