# 04. Gilded Rose 테스트 계획서 작성 보고

## 목적

Gilded Rose C++17 레거시 프로젝트의 `updateQuality()` 검증을 위한 테스트 계획서를 작성하였다. 계획서는 Google Test의 `TEST_F` 기반 단위 테스트, 경계값 분석, 예외/특이 케이스, gcov/lcov 커버리지 측정 전략을 포함한다.

## 대상 파일

- `cpp/include/GildedRose.h`
- `cpp/src/GildedRose.cpp`
- `GildedRoseRequirements.txt`
- `docs/requirements_analysis.md`
- `cpp/CMakeLists.txt`
- `cpp/test/GildedRoseTest.cpp`

## 작성 산출물

- `test_plan.md`

## 주요 내용

1. `TEST_F(GildedRoseTest, ...)` 기반 테스트 fixture 구조를 제안하였다.
2. Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 아이템별 단위 테스트 범위를 P0/P1/P2 우선순위로 분류하였다.
3. `quality == 0`, `quality == 1`, `quality == 49`, `quality == 50`, `sellIn == 0`, `sellIn == -1` 경계값 조합을 아이템별로 정리하였다.
4. Sulfuras 불변성, Backstage Pass 만료 후 품질 0, 음수 `sellIn` 유지, 빈 아이템 목록, 알 수 없는 아이템명, Conjured 식별 등 예외/특이 케이스를 정리하였다.
5. 라인 커버리지 90% 이상, 함수 커버리지 100%, 브랜치 커버리지 85% 이상을 목표로 설정하였다.
6. CMake Debug 빌드에서 `--coverage` 플래그를 사용해 `ctest`, `lcov`, `genhtml`로 측정하는 절차를 제시하였다.

## 확인 사항

현재 `cpp/test/GildedRoseTest.cpp`에는 기본 샘플 테스트만 존재하므로, 실제 테스트 구현 단계에서는 해당 테스트를 `TEST_F` 기반으로 교체해야 한다. 또한 현재 `cpp/src/GildedRose.cpp`에는 Conjured 규칙이 아직 구현되어 있지 않으므로, Conjured 테스트는 구현 전 실패하는 신규 요구사항 검증 테스트로 활용하는 것이 적절하다.

## 결과

프로젝트 루트에 `test_plan.md`를 생성하여 테스트 계획을 문서화하였다. 이후 테스트 작성, Conjured 구현, 리팩토링, 커버리지 측정 작업의 기준 문서로 사용할 수 있다.
