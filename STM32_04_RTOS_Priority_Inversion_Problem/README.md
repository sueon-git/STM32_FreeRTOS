# 프로젝트 4: FreeRTOS 우선순위 역전(Priority Inversion) 문제 재현
> 우선순위 스케줄링과 공유 자원이 만났을 때 발생하는 문제(역전) 상황 분석

<img width="406" height="720" alt="STM32_04_FreeRTOS_Priority_Inversion_Problem" src="https://github.com/user-attachments/assets/6c17eb5f-2724-404f-a0d7-820df4f01e7b" />


---

## 프로젝트 개요 (Project Overview)
전 프로젝트에서 작업 우선순위에 따라 실행 순서가 결정됨을 확인함. 이 프로젝트에서는 우선순위 스케줄링이 **공유 자원(Shared Resource)**과 결합되었을 때 발생할 수 있는 문제인 **우선순위 역전(Priority Inversion)** 현상을 의도적으로 재현하고 그 원인과 대응방안을 분석하는 것이 목표.

**구현 내용**: 우선순위가 '낮음'인 작업이 가진 자원을 '높음'인 작업이 기다릴 때, 상관없는 '중간' 우선순위 작업이 끼어들어 시스템 전체의 반응성을 떨어뜨리는 상황을 연출.

---

## 개발 환경 및 핵심 개념 (Tech Stack & Core Concepts)
* **개발 환경**: STM32CubeIDE, NUCLEO-F401RE
* **핵심 기술**:
    * **우선순위 역전 (Priority Inversion)**: 낮은 우선순위의 작업이 점유한 자원을 높은 우선순위의 작업이 기다리게 되어, 중간 우선순위의 작업이 대신 실행되는 현상.
    * **공유 자원 (Shared Resource)**: 세 개의 작업이 접근하려는 UART 통신 채널.
    * **임계 영역 (Critical Section)**: 공유 자원에 접근하는 코드 구간. 이 예제에서는 뮤텍스 없이 구현하여 문제 상황을 연출.
    * **선점형 스케줄링 (Preemptive Scheduling)**: 우선순위가 높은 작업이 낮은 작업을 언제든지 중단시킬 수 있는 RTOS의 특성.

---

## 주요 구현 내용
* **세 개의 작업 생성**:
    * `HighPrioTask` (우선순위: 높음): 공유 자원을 사용하기 위해 대기.
    * `MediumPrioTask` (우선순위: 중간): 공유 자원과 상관없이 주기적으로 CPU를 사용.
    * `LowPrioTask` (우선순위: 낮음): 공유 자원을 획득하고 오랫동안 점유.
* **문제 상황 유도**: `LowPrioTask`가 공유 자원을 사용하는 동안 `HighPrioTask`가 대기하게 만들고, 그 사이에 `MediumPrioTask`가 실행되도록 `osDelay`를 조절.

---

## 실행 결과 및 분석
프로그램 실행 결과, 시리얼 터미널에는 다음과 같은 패턴이 나타남.

1.  `LowPrioTask`가 먼저 공유 자원(UART) 사용을 시작.
2.  `HighPrioTask`가 실행될 차례가 되었지만, `LowPrioTask`가 자원을 점유하고 있어 대기 상태에 들어감.
3.  **핵심 문제**: `LowPrioTask`가 자원을 반납해야 `HighPrioTask`가 실행될 수 있음에도 불구하고, 우선순위가 중간인 `MediumPrioTask`가 계속 끼어들어 CPU를 차지함.
4.  결과적으로 가장 중요한 `HighPrioTask`의 실행이, 전혀 상관없는 `MediumPrioTask` 때문에 지연되는 **우선순위 역전** 현상 관찰.

---

## 체크포인트 및 결론 (Key note)
단순히 작업에 우선순위를 부여하는 것만으로는 안정적인 멀티태스킹 시스템을 만들 수 없다는 것을 알게됨.

여러 작업이 공유 자원을 사용하는 환경에서는, 우선순위가 높은 작업이 낮은 작업 때문에 지연되는 '우선순위 역전'이라는 심각한 문제가 발생할 수 있음을 직접 체감함. 다음 프로젝트에서 이 문제를 해결하기 위한 동기화 기법인 **뮤텍스(Mutex)**, 특히 **우선순위 상속(Priority Inheritance)** 기능의 필요성을 이해하는 결정적인 계기가 되었음. 

<참고> 우선순위 상속 : 서열이 다른 직원 ABC 3명이 있을때, 일시적 순간동안 2명의 서열을 같게 만들어주는 기능 (중간에 끼어들지 못함)
