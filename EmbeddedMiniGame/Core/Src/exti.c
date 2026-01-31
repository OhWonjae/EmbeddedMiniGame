#include "global.h"
#include "main.h"
/*
 * exti 버튼 인터럽트 콜백(FALLING, RASING 트리거) - 버튼누르는 동안 계속 감지 필요!!
 * HAL과 어플리케이션 레벨 분리 적용
 */
// static 씀으로써 외부노출 피함
static void Btn_Trigger(uint16_t GPIO_Pin){
	static uint32_t last = 0;
	uint32_t now = HAL_GetTick();
	uint8_t isHigh = 0;
	if(now - last>50){
		if (GPIO_Pin == left_btn_Pin){
			isHigh = HAL_GPIO_ReadPin(left_btn_GPIO_Port, left_btn_Pin);
			input.left_btn_down = !isHigh;
		}else if(GPIO_Pin == right_btn_Pin){
			isHigh = HAL_GPIO_ReadPin(right_btn_GPIO_Port, right_btn_Pin);
			input.right_btn_down = !isHigh;
		}
	}

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  Btn_Trigger(GPIO_Pin);
}
