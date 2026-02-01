#include "global.h"
#include "stdint.h"
#include "game.h"
#include "ssd1306.h"
#include "fonts.h"
#include "stdlib.h"


#define P_INIT_X_POS 59 // 플레이어 기본 x 좌표
#define P_INIT_Y_POS 45 // 플레이어 기본 y 좌표
#define P_WIDTH 10// 플레이어 width값
#define P_HEIGHT 10 // 플레이어 height값
#define MAX_R_COUNT 25 // 최대 산성비 개수 -> 전체 WIDTH를 비의 너비(5)로 나눈값
#define R_WIDTH 5 // 비의 width값
#define R_HEIGHT 10 // 비의 height값
/*
 * 이 파일에서만 쓸 변수와 함수는 다른파일에서 쓰는걸 피하기 위해 static 적용
 */
static I2C_HandleTypeDef* hi2c1Ptr = NULL;
static TIM_HandleTypeDef* htim1Ptr = NULL;

// 플레이어 위치 좌표
typedef struct {
	uint8_t x;
	uint8_t y;
}Pos;
static Pos player_pos = {P_INIT_X_POS,P_INIT_Y_POS};
// 플레이어 이동 스피드
static uint8_t p_speed = 5;

// 산성비 배열
typedef struct {
	Pos pos;
	uint8_t active;
}Rain;
static Rain rains[MAX_R_COUNT];
static uint8_t r_speed = 1;


//게임 에러 핸들러
static void ErrorHanlder(){
	// 에러를 어떻게 표현하지..?
}

// 화면 업데이트
static void updateScreen(){
	if(hi2c1Ptr==NULL){
		ErrorHanlder();
		return;
	}
	ssd1306_UpdateScreen(hi2c1Ptr);
}

// 문구 그리기
static void DrawWord(uint8_t cursor_x, uint8_t cursor_y, const char* word, FontDef font, SSD1306_COLOR color){
	ssd1306_SetCursor(cursor_x, cursor_y);
	ssd1306_WriteString(word, font, color);
}
// 사각형 그리기
static void DrawRectangle(uint8_t cursor_x, uint8_t cursor_y, uint8_t width, uint8_t height,SSD1306_COLOR color){
	ssd1306_SetCursor(cursor_x, cursor_y);
	for (uint8_t i=cursor_x; i<cursor_x+width; i++) {
		for (uint8_t j=cursor_y; j<cursor_y+height; j++) {
			ssd1306_DrawPixel(i, j, color);
		}
	}
}

// 산성비 선택 - 타이머로 특정시간마다 비생성하여 나오는 타이밍 조정
static void SelectRain(){
	// 어느 구역에서 비내릴껀가? 랜덤함수 적용
	while(1){
		uint8_t randN = rand()%MAX_R_COUNT;
		if(rains[randN].active==0){
			rains[randN].active = 1;
			return;
		}
	}
}

// 산성비 내리기
static void RainDown(){
	// 비내리기
	for(uint8_t i =0; i< MAX_R_COUNT; i++){
		if(rains[i].active){
			// 타일 닿으면 초기화
			if(rains[i].pos.y+R_HEIGHT<HEIGHT-10){
				rains[i].pos.y+=1*r_speed;
				DrawRectangle(rains[i].pos.x, rains[i].pos.y, R_WIDTH, R_HEIGHT, White);
			}else{
				DrawRectangle(rains[i].pos.x, rains[i].pos.y, R_WIDTH, R_HEIGHT, Black);
				rains[i].pos.y = 0;
				rains[i].active = 0;
			}
			// 플레이어 닿으면 게임오버
			if((rains[i].pos.x<=player_pos.x && rains[i].pos.x+P_WIDTH > player_pos.x && rains[i].pos.y+R_HEIGHT>=player_pos.y) ||
				(rains[i].pos.x>=player_pos.x && rains[i].pos.x<player_pos.x+P_WIDTH && rains[i].pos.y+R_HEIGHT>=player_pos.y)){
				game.is_start=0;
			}

		}
	}

}

// 플레이어 이동
static void PlayerMove(){
	// 입력에 따른 플레이어 이동
	// uint8_t 변수에 음수 들어가니 예기치 못한 동작나옴... 음수 안들어가도록 방어 코드 철저히...
	if(input.left_btn_down && (player_pos.x-1*p_speed>0) ){
		player_pos.x-=1*p_speed;
	}
	if(input.right_btn_down && player_pos.x<WIDTH-P_WIDTH){
		player_pos.x+=1*p_speed;
	}
}

// 게임 대기 상태 시작
void GameIdle(I2C_HandleTypeDef* _hi2c1Ptr, TIM_HandleTypeDef* _htim1Ptr){
	// i2c 핸들러 세팅
	hi2c1Ptr = _hi2c1Ptr;
	htim1Ptr = _htim1Ptr;
	ssd1306_Fill(Black);
	// 바닥 생성
	DrawRectangle(0,59,128,5,White);
	// 플레이어 생성
	player_pos.x = P_INIT_X_POS;
	player_pos.y = P_INIT_Y_POS;
	DrawRectangle(player_pos.x, player_pos.y, P_WIDTH, P_HEIGHT, White);
	//비 배열 초기화
	for(uint8_t i=0;i<MAX_R_COUNT; i++){
		rains[i].pos.x = i*R_WIDTH;
		rains[i].pos.y = 0;
		rains[i].active = 0;
	}
	// 문구 생성
	DrawWord(5, 10,"Press Any Btn Key", Font_7x10, White);
	// 화면 출력
	updateScreen();
}
// 게임 시작 버튼 인식
void isStartBtnDown(){
	if(!game.is_start && (input.left_btn_down ||input.right_btn_down )){
		game.is_start = 1;
		// 산성비 타이머 시작
		HAL_TIM_Base_Start_IT(htim1Ptr);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
    	SelectRain();
    }
}
void GameStart(){
	if(!game.is_start){
		return;
	}

	// 하늘과 플레이어 부분 초기화
	DrawRectangle(0, 0, 128, 55,Black);
	// 플레이어 이동
	PlayerMove();
	// 플레이어 생성
	DrawRectangle(player_pos.x, player_pos.y, 10, 10,White);
	// 비내리기
	RainDown();
	//화면 출력
	updateScreen();
}
