# 01. Gilded Rose Cursor AI Rules Report

## 목적

Gilded Rose C++17 레거시 프로젝트에서 Cursor AI가 일관된 QA 및 리팩토링 기준을 따르도록 프로젝트 루트에 `.cursorrules`를 작성하였다.

## 적용 기술 스택

- C++17
- CMake
- Google Test
- gcov/lcov

## 핵심 규칙

- `Item` 구조체는 절대 수정하지 않는다.
- `quality`는 `Sulfuras`를 제외하고 항상 `0` 이상 `50` 이하로 유지한다.
- `sellIn`이 음수인 상태는 유효한 도메인 상태로 간주하며, 0으로 보정하지 않는다.
- 기존 레거시 동작은 명시적인 요구가 없는 한 유지한다.

## 테스트 기준

- Google Test 기반 테스트를 작성한다.
- 아이템 갱신 동작은 `TEST_F` 사용을 우선한다.
- 테스트는 Given-When-Then 구조를 따르도록 작성한다.
- `quality == 0`, `quality == 50`, `sellIn == 0`, `sellIn == -1` 경계값을 포함한다.
- 갱신 후 `sellIn`과 `quality`를 모두 검증한다.

## 리팩토링 기준

- 테스트가 Green 상태일 때만 리팩토링을 진행한다.
- 리팩토링은 동작 보존을 전제로 작게 나누어 수행한다.
- 매직 넘버는 품질 범위와 변화율을 중심으로 상수화한다.
- CMake 및 Google Test의 기존 프로젝트 관례를 유지한다.

## 결과

프로젝트 루트의 `.cursorrules`에 Gilded Rose 도메인 제약, 테스트 작성 기준, 리팩토링 원칙, C++17 코딩 스타일을 명시하였다. 이후 Cursor AI는 해당 규칙을 기준으로 테스트 추가, 리팩토링, 코드 변경을 수행해야 한다.
