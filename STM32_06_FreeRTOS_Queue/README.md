# 프로젝트 4: FreeRTOS 큐(Queue)를 이용한 작업 간 통신
> 독립적으로 움직이는 작업(Task)들이 서로 데이터를 주고받는 방법 

<img width="320" height="567" alt="STM32_06_FreeRTOS_Queue_TimeBased" src="https://github.com/user-attachments/assets/35f4ee64-46d8-446a-aaa9-8f5f775a6c6d" />


---

## 프로젝트 개요 (Project Overview)
이전 프로젝트들을 통해 여러 작업을 동시에 실행(Multi-tasking)하고 우선순위를 제어하였지만, 각 작업은 독립적으로 움직일 뿐, 서로 협력하지는 못함.

이 프로젝트는 **"한 작업이 다른 작업에게 어떻게 안전하게 정보를 전달할 수 있는가?"** 라는 문제를 해결하기 위해, FreeRTOS의 핵심적인 통신 메커니즘인 **큐(Queue)**를 사용하는 것이 목표.

**주요 목표**: `DataSenderTask`가 시간 기반으로 주기적인 명령을 보내면, `LedControlTask`가 그 명령을 받아서 LED의 동작을 변경하는, 기본적인 작업 간 통신(Inter-Task Communication) 모델을 구현.

---

## 개발 환경 및 핵심 개념 (Tech Stack & Core Concepts)
* **개발 환경**: STM32CubeIDE, NUCLEO-F401RE
* **핵심 기술**:
    * **큐 (Queue)**: 작업 간에 데이터를 안전하게 전달하는 통로(메시지 우체통) 역할을 하는 RTOS 객체.
    * **작업 간 통신 (Inter-Task Communication, ITC)**: 큐를 사용하여 두 개의 다른 작업이 데이터를 주고받으며 협력하는 구조 설계.
    * **데이터 생산자-소비자 모델 (Producer-Consumer Model)**: `DataSenderTask`(생산자)가 데이터를 생성하여 큐에 넣고, `LedControlTask`(소비자)가 큐에서 데이터를 가져와 사용하는 고전적인 프로그래밍 모델 구현.
    * **API**: `osMessageQueueNew()`, `osMessageQueuePut()`, `osMessageQueueGet()`

---

## 주요 구현 내용
* **`DataSenderTask` (데이터 생산자)**: 2초마다(`osDelay(2000)`) 숫자 `0` 또는 `1`을 번갈아가며 생성하여 큐에 집어넣음.
* **`LedControlTask` (데이터 소비자)**: 큐에 새로운 데이터가 들어올 때까지 대기하다가, 받은 데이터 값에 따라 LED의 깜빡임 속도를 '빠르게(100ms)' 또는 '느리게(1000ms)'로 변경.
* **`main()` 함수**: 두 개의 작업과 함께, 이들이 사용할 하나의 큐(`ledCmdQueueHandle`)를 생성.

---

## 실행 결과 및 분석
NUCLEO 보드의 녹색 LED(LD2)가 2초마다 깜빡이는 속도가 주기적으로 변경되는 것을 확인. (2초는 빠르게, 다음 2초는 느리게 반복)

이것은 `DataSenderTask`가 큐를 통해 보낸 명령에 따라 `LedControlTask`가 자신의 동작을 성공적으로 변경했음을 의미함. 두 작업은 서로의 존재를 직접 알지 못하지만, 큐라는 중간 매개체를 통해 서로 협력하고 있음을 확인.

---

## 체크포인트 및 결론 (Key Takeaways)
이번 큐(Queue) 프로젝트를 통해 **디커플링(Decoupling, 역할 분리)**의 중요성을 알게됨. `DataSenderTask`는 LED 제어 방법을 전혀 몰라도 되고, `LedControlTask`는 명령이 어디서 오는지 신경 쓸 필요가 없습니다. 이러한 구조는 나중에 한쪽의 기능을 변경하더라도 다른 쪽에 미치는 영향을 최소화하여, 시스템의 유지보수성과 확장성을 크게 향상시킴.

큐는 단순한 데이터 전달을 넘어, 복잡한 시스템의 여러 기능들을 독립적인 모듈로 나누고 이들을 유기적으로 연결하는 핵심적인 설계 도구임.
