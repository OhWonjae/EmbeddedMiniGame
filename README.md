
# Bare-Metal 기반 STM32 임베디드 제어 시스템
# Bare-Metal 환경에서 STM32F103 MCU를 기반으로 입력·출력·통신 제어 시스템을 설계 및 구현
인터럽트 기반 이벤트 처리 구조를 적용하여 ISR에서는 최소한의 Flag 처리만 수행하고, 실제 로직은 Main Loop에서 처리하는 구조로 설계하여 시스템 안정성과 확장성을 고려했습니다.



https://github.com/user-attachments/assets/a9f17bbb-454a-48a4-886c-c38dc7128dea

# 코드 구조

### 1. 이벤트 기반 설계
### ISR에선 Flag만 처리 후 실제 태스크는 메인에서 처리하여 타이밍 이슈, 부작용 방지

<img width="1223" height="548" alt="image" src="https://github.com/user-attachments/assets/3770f8c5-5a7c-4516-a3ad-9f8899069dc6"> event callback 함수</img><br/>
<img width="848" height="222" alt="image" src="https://github.com/user-attachments/assets/1ba1db10-3a81-42b7-8b07-f322bc31a7d9"/>
<img width="693" height="334" alt="image" src="https://github.com/user-attachments/assets/4d8a551b-e71e-4765-a6de-01c86cbe2d87" >메인루틴</img>

                                                                                                                                  
### 2. 작성된 코드 파일 설명
- main.c, main.h : 메인 파일로 각 핸들러에 대한 초기화와 메인 while문 진행 
- global.c, global.h : 전역 변수 파일 적용
- exti.c : 외부 인터럽트 콜백 함수 파일
- game.c, game.h : 인게임 로직 및 실제 이벤트 처리와 동작이 포함된 파일
- fnd.c,fnd.h : 7-Segment의 led를 켜기위해 SPI 통신함수와 래치클럭을 호출하는 파일
  <br/>

# 입력 처리 및 신호 안정화 설계
### 1. 외부 Pull-Up 저항을 적용하여 GPIO 입력의 플로팅 현상 방지 및 신호 안정성 확보
### 2. Rising/Falling Edge 기반 EXTI 인터럽트 구성
### 3. 소프트웨어 Debouncing 로직 구현
#### - 인터럽트 발생 이후 50ms 동안 재트리거 방지
#### - 기계적 채터링으로 인한 오동작 제거
#### - 입력 신호 안정화 로직을 타이머 기반으로 설계


# 7-Segment 제어(SPI 프로토콜 사용)
### 1. SPI 프로토콜을 사용하여 SHIFT REGISTER 칩 제어
<img width="764" height="265" alt="image" src="https://github.com/user-attachments/assets/ba1cd280-0e1a-4e05-a47e-3918b87a2f04" />


### 2. TRANSMIT-ONLY로 마스터에서 송신만 하도록 설정, 8bit 데이터 전송, 클럭이 2edge일때 데이터 신호 감지
### 3. 2ms 주기의 타이머(내부 클럭이 8MHZ 이므로 prescaler 8000-1, ARR 2-1 설정)를 통해 한자리수 씩 타이머 인터럽트 완료시 출력

# OLED 제어(I2C 프로토콜 사용)
### 1. SSD1306칩 내부에서 I2C 프로토콜을 사용하여 통신
### 2. 좌표 기반 화면 렌더링 로직 설계

# 산성비 내리는 로직
### 1. 화면의 구역을 세로로 나눈 후 랜덤 함수를 통해 나온 번호의 구역에 산성비를 생성
### 2. 3초 주기의 타이머를 통해 산성비를 생성하는 로직과 생성된 산성비가 내리는 로직(내려가고, 플레이어나 바닥에 닿으면 사라지는것 포함) 구현
<img width="1333" height="702" alt="image" src="https://github.com/user-attachments/assets/7bf153ab-ae92-4214-87c9-b8ac415f271b" />

# 로직 분석기를 활용한 신호 검증 및 디버깅
### 1. 각 자리수를 보낼떄 어떤 몇번째 자리인지에 어떤 숫자가 들어갈지에 대한 데이터에 대한 파형
<img width="1219" height="834" alt="image" src="https://github.com/user-attachments/assets/6e6ba85d-b419-4948-b7f2-f5b1106dfeda" />

### 2. 전체 4자리의 데이터가 출력될때의 파형과 각 자리수 데이터 사이에 타이머로 설정해놓은 2ms 주기가 실제 파형으로 보여지는 모습
<img width="1422" height="713" alt="2ms" src="https://github.com/user-attachments/assets/f19e8e80-590e-49c5-adc6-12fd00fa422d" />

# 회로도 
[회로도.pdf](https://github.com/user-attachments/files/25739535/default.pdf)

