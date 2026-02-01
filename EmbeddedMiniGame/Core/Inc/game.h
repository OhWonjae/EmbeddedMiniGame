#ifndef GAME__H
#define GAME__H
#include "main.h"
/*
 * 게임 로직
 * 1. 게임 시작 전 대기화면 - 버튼 클릭시 시작(발판위에 플레이어 존재, 그위에 버튼 누르라는 글씨)
 * 2. 시작 시 버튼 누르라는 글씨 사라지고 위에서 비내리기 시작
 * 3. 비맞으면 점수판 초기화, 대기화면으로 돌아감
 * 4. 비피하면 점수판 증가
 *
 */

// OLED 해당도 128 x 64
#define WIDTH 128
#define HEIGHT 64



void GameIdle(I2C_HandleTypeDef* hi2c1, TIM_HandleTypeDef* _htim1Ptr);
void isStartBtnDown();
void GameStart();


#endif
