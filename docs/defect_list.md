# Gilded Rose 결함 목록

작성일: 2026-05-19

현재 로컬 기준 `ctest`는 27개 테스트 모두 통과 상태다. 아래 항목은 현재까지의 테스트 구현 및 디버깅 과정에서 발견되었거나 결함 분석 대상으로 식별된 이슈를 정리한 것이다.

## 결함 항목

### [DEF-001] [Major] [Product Defect]

- [ID] DEF-001
- [Severity] Major
- [ItemType] Product Defect / Conjured
- [Steps]
  1. `Conjured Mana Cake` 아이템을 `sellIn = 10`, `quality = 20`으로 생성한다.
  2. `GildedRose::updateQuality()`를 1회 호출한다.
  3. `sellIn`과 `quality`를 확인한다.
- [Expected] `sellIn == 9`, `quality == 18`
- [Actual] Conjured 전용 규칙이 없던 구현에서는 일반 아이템처럼 처리되어 `sellIn == 9`, `quality == 19`가 된다.
- [Root Cause] `updateQuality()` 내부에 `"Conjured"` 아이템명을 판별하는 분기가 없어 신규 요구사항인 "일반 아이템의 2배 속도로 품질 감소" 규칙이 적용되지 않았다.
- [Fix Summary] `isConjured(const Item&)` 헬퍼를 추가하고, 일반 아이템 처리 전에 Conjured 분기를 배치해 판매기한 전에는 `quality`를 2 감소, 판매기한 당일/이후에는 4 감소하도록 수정하였다. 품질 하한은 기존 `decreaseQuality()` 헬퍼로 `0` 미만이 되지 않게 보정한다.

### [DEF-002] [Major] [Product Defect]

- [ID] DEF-002
- [Severity] Major
- [ItemType] Product Defect / Conjured Boundary
- [Steps]
  1. `Conjured Mana Cake` 아이템을 `sellIn = 0`, `quality = 20`으로 생성한다.
  2. `GildedRose::updateQuality()`를 1회 호출한다.
  3. 판매기한 당일의 품질 감소량을 확인한다.
- [Expected] `sellIn == -1`, `quality == 16`
- [Actual] Conjured 전용 규칙이 없던 구현에서는 일반 아이템처럼 판매기한 경과 감소량만 적용되어 `sellIn == -1`, `quality == 18`이 된다.
- [Root Cause] Conjured의 판매기한 당일/경과 후 감소량이 일반 아이템 감소량 `2`의 2배인 `4`여야 하지만, 별도 분기가 없어 일반 아이템 만료 규칙으로 대체되었다.
- [Fix Summary] Conjured 분기에서 `item.sellIn <= 0 ? 4 : 2` 감소량을 사용하도록 수정하였다. `sellIn` 감소는 기존 공통 흐름을 유지해 모든 비-Sulfuras 아이템이 하루마다 `1` 감소하도록 했다.

### [DEF-003] [Minor] [Test Execution Issue]

- [ID] DEF-003
- [Severity] Minor
- [ItemType] Test Execution Issue / Shell Compatibility
- [Steps]
  1. 현재 Windows PowerShell 환경에서 `cmake --build build && ctest`를 실행한다.
  2. 빌드 성공 시 테스트가 이어서 실행되는지 확인한다.
- [Expected] 빌드가 성공하면 `ctest`가 이어서 실행되고, 테스트 결과가 출력된다.
- [Actual] 현재 PowerShell 환경에서는 `&&` 연산자 구문 오류로 명령이 중단될 수 있다.
- [Root Cause] 사용 중인 PowerShell 버전/호환성에서 `&&` 조건 실행 연산자가 지원되지 않아 Unix shell 또는 최신 PowerShell과 동일하게 해석되지 않는다.
- [Fix Summary] 동일 의미의 조건 실행을 위해 `cmake --build build; if ($LASTEXITCODE -eq 0) { ctest --test-dir build }` 또는 `cmd /c "cmake --build build && ctest --test-dir build --output-on-failure"` 명령을 사용해 검증하였다.

## 현재 상태

- `cpp/src/GildedRose.cpp`에는 Conjured 판별 및 감소 규칙이 반영되어 있다.
- `cpp/test/GildedRoseTest.cpp`에는 Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 타입별 테스트가 작성되어 있다.
- 마지막 확인된 테스트 결과는 `27/27` 통과, 실패 `0`개다.
