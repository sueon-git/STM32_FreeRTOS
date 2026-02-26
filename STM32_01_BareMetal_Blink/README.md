# 프로젝트 1: STM32 베어메탈(Bare-Metal) LED 깜빡이기
> STM32 펌웨어 개발, OS 없이 하드웨어를 직접 제어하는 기본 원리 확인

![동작 영상 (GIF)](https://github.com/user-attachments/assets/3b9830d6-99b9-40dc-8286-2f18a3a2ce1a)

---

## 프로젝트 소개
NUCLEO-F401RE 보드를 사용, 가장 기본적인 펌웨어 작성 실행하는 것이 목표
운영체제(OS) 없이 C언어와 HAL 라이브러리만으로 하드웨어를 직접 제어하는 '베어메탈' 실행 목표

---

## 개발 환경 및 사용 기술
* **보드(MCU)**: NUCLEO-F401RE (STM32F401RET6)
* **개발 환경(IDE)**: STM32CubeIDE
* **핵심 기술**:
    * **C Language**: 기본적인 C언어 문법 활용.
    * **Bare-Metal Programming**: OS 없이 `while(1)` 무한 루프 기반으로 순차적 프로그램 제어
    * **HAL (Hardware Abstraction Layer)**: ST사에서 제공하는 라이브러리
    * **GPIO Control**: 범용 입출력 핀을 출력으로 설정하여 LED 제어

---

## 주요 구현 내용
`main` 함수의 `while(1)` 루프 안에서 다음 두 가지 HAL 함수를 반복적으로 호출하여 LED를 깜빡이게 구현

```c
// main.c의 while(1) 루프 내부
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // LD2 LED의 상태를 계속해서 뒤집음
HAL_Delay(1000);                      // 1초(1000ms) 동안 프로그램 실행을 멈춤
```

## 주요 사용 함수 및 특징
* **`HAL_GPIO_TogglePin()`**: 이 함수 하나로 GPIO 핀의 상태를 `HIGH`/`LOW`로 계속 전환하여 코드를 간결하게 유지
* **`HAL_Delay()`**: 간단한 시간 지연을 만들어, LED가 1초 간격으로 깜빡이는 것처럼 보임. 이 함수가 호출되는 동안에는 `MCU`가 다른 일을 하지 않고 멈춤

---

## 실행 결과
NUCLEO-F401RE 보드의 녹색 LED(LD2)가 1초 간격으로 규칙적으로 깜빡이는 것을 확인

---

## 체크포인트 및 다음 학습 목표
> "Bare-Metal과 FreeRTOS의 차이가 뭘까? 만약 LED를 깜빡이면서 동시에 다른 일(예: 버튼 입력 감지)도 해야 한다면 어떻게 구현해야 하지?"

위와 같은 자연스러운 의문이 생기고 이후 `FreeRTOS`와 같은 실시간 운영체제(RTOS)의 필요성을 이해하는 중요한 계기가 됨


