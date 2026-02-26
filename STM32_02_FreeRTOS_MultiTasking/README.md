# 프로젝트 2: FreeRTOS 멀티태스킹 입문
> 첫 베어메탈 프로젝트에서 가졌던 궁금증, "LED를 깜빡이면서 동시에 다른 일을 하려면 어떻게 해야 할까?" 에 대한 해답

![Image](https://github.com/user-attachments/assets/1234dc4a-4608-4e22-89e3-38da81fcc144)

---

## 프로젝트 개요 (Project Overview)
**FreeRTOS**라는 실시간 운영체제를 처음으로 도입하여 **멀티태스킹(Multitasking)**의 기본 원리를 학습하고 구현하는 것이 목표.

**목표**: 두 개의 독립적인 작업(Task)을 만들어, NUCLEO 보드가 LED를 깜빡이는 동시에 PC로 주기적인 메시지를 보내는 동작 구현

---

## 개발 환경 및 핵심 개념 (Tech Stack & Core Concepts)
* **개발 환경**: STM32CubeIDE, NUCLEO-F401RE
* **핵심 기술**:
    * **RTOS (Real-Time Operating System)**: FreeRTOS를 처음 도입하여 시스템을 관리.
    * **멀티태스킹 (Multitasking)**: 두 개의 독립적인 작업(`LedBlinkTask`, `SerialPrintTask`)을 생성하여 동시에 실행.
    * **작업(Task) 생성**: `osThreadNew()` API를 사용하여 각 기능별로 작업을 생성.
    * **`osDelay()`**: 특정 작업만 대기 상태로 만들어, 다른 작업이 실행될 수 있도록 CPU 시간을 양보하는 RTOS의 핵심적인 시간 지연 함수.
    * **UART 통신**: `HAL_UART_Transmit()` 함수를 사용하여 보드에서 PC로 데이터를 전송.

---

## 주요 구현 내용 (Implementation Details)
* **`LedBlinkTask`**: 2초 간격(`osDelay(2000)`)으로 LED를 토글하는 단순한 작업을 구현.
* **`SerialPrintTask`**: 1초 간격(`osDelay(1000)`)으로 카운터 값을 증가시키며 시리얼 포트로 메시지를 출력하는 작업을 구현.
* **`main()` 함수**: 두 개의 작업을 각각 동일한 우선순위(`osPriorityNormal`)로 생성하고, `osKernelStart()`를 호출하여 FreeRTOS 스케줄러에게 모든 제어권을 부여.

---

## 실행 결과 (Result)
NUCLEO 보드의 녹색 사용자 LED(LD2)는 2초 간격으로 느리게 깜빡이는 동시에, PC의 시리얼 터미널 창에는 1초 간격으로 카운터 메시지가 끊김 없이 출력되는 것을 확인.

---

## 체크포인트 및 배운 점 (Key Takeaways)
이전 베어메탈 프로젝트의 `HAL_Delay()`는 시스템 전체를 멈췄지만, FreeRTOS의 `osDelay()`는 해당 작업만 "휴식" 상태로 만들고 다른 작업은 계속 실행될 수 있도록 구현. (멀티태스킹)

`LedBlinkTask`가 2초라는 긴 시간 동안 쉬고 있음에도 불구하고, 그 사이에 `SerialPrintTask`가 두 번이나 메시지를 보내는 것을 보며 RTOS 스케줄러가 어떻게 CPU 시간을 효율적으로 분배하는지 확인. 이를 통해 "여러 기능을 독립적으로, 그리고 동시에" 실행할 수 있는 RTOS의 기능 체감.