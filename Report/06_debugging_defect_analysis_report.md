# 06. Gilded Rose 디버깅 및 결함 분석 보고

## 목적

`cpp/test/GildedRoseTest.cpp`와 `cpp/src/GildedRose.cpp` 기준으로 `ctest` 실패를 분석하고, `updateQuality()`의 결함 위치와 최소 수정 방안을 정리한다.

## 입력 및 재현 결과

사용자 프롬프트의 `[T]` 영역에는 실제 `ctest` 실패 로그가 첨부되지 않았다. 따라서 현재 워크스페이스에서 빌드와 테스트를 직접 실행해 실패 여부를 확인하였다.

```bash
cmd /c "cmake --build build && ctest --test-dir build --output-on-failure"
```

확인 결과:

- 빌드 성공
- `ctest`: 27개 테스트 통과, 실패 0개
- 현재 상태에서는 `EXPECT_EQ` 실패가 재현되지 않음

## EXPECT_EQ 기대/실제 차이 요약

현재 로컬 실행 결과는 Green이므로 실제 실패 로그 기반 기대/실제 차이는 없다.

다만 `Conjured` 규칙이 누락된 구현이었다면 다음 테스트들이 대표적으로 실패한다.

- `ConjuredItemDecreasesQualityTwiceAsFastBeforeSellDate`: 기대 `quality == 18`, 일반 아이템 처리 시 실제 `quality == 19`
- `ConjuredItemDecreasesQualityFourPointsOnSellDate`: 기대 `quality == 16`, 일반 아이템 처리 시 실제 `quality == 18`
- `ConjuredItemDecreasesQualityFourPointsAfterSellDate`: 기대 `quality == 16`, 일반 아이템 처리 시 실제 `quality == 18`
- `ConjuredItemQualityCanStartAtUpperBoundaryAndDecrease`: 기대 `quality == 48`, 일반 아이템 처리 시 실제 `quality == 49`
- `ConjuredItemNameCanContainConjuredKeyword`: 기대 `quality == 18`, 일반 아이템 처리 시 실제 `quality == 19`

## 버그 위치 특정

현재 `cpp/src/GildedRose.cpp`에는 `Conjured` 판별과 전용 감소 규칙이 이미 반영되어 있어 결함은 재현되지 않는다.

결함이 있었던 경우의 위치는 `updateQuality()`의 일반 아이템 감소 분기 직전이다.

- 파일: `cpp/src/GildedRose.cpp`
- 위치: `updateQuality()` 내부, `Backstage passes` 처리 이후와 일반 아이템 `else` 처리 이전
- 현재 수정 완료 지점: `isConjured(item)` 분기와 `decreaseQuality(item, item.sellIn <= 0 ? 4 : 2)` 호출

## 심각도

분류: Major

근거:

- `Conjured` 아이템의 핵심 비즈니스 규칙이 누락되면 정상 아이템보다 두 배 빠르게 품질이 감소해야 한다는 요구사항을 지속적으로 위반한다.
- 품질 하한 `0` 보정은 공통 헬퍼로 유지될 수 있어 데이터 손상이나 크래시는 발생하지 않는다.
- `Sulfuras`, `Aged Brie`, `Backstage passes`, 일반 아이템에는 영향이 제한적이므로 Critical은 아니다.

## 최소 변경 수정 방안

`Item` 구조체는 수정하지 않고, 이름에 `"Conjured"`가 포함되는지 판별하는 헬퍼와 `updateQuality()`의 전용 분기만 추가한다. C++17 기준으로 기존 `std::string::find()` 사용만으로 충분하다.

수정 diff 제안:

```diff
 namespace {
 const char* kAgedBrie = "Aged Brie";
 const char* kBackstagePass = "Backstage passes to a TAFKAL80ETC concert";
 const char* kSulfuras = "Sulfuras, Hand of Ragnaros";
+
+bool isConjured(const Item& item) {
+    return item.name.find("Conjured") != std::string::npos;
+}
 
 void increaseQuality(Item& item, int amount = 1) {
     item.quality += amount;
@@
-        } else {
+        } else if (isConjured(item)) {
+            decreaseQuality(item, item.sellIn <= 0 ? 4 : 2);
+        } else {
             decreaseQuality(item, item.sellIn <= 0 ? 2 : 1);
         }
```

현재 `cpp/src/GildedRose.cpp`는 위 수정 방안이 이미 반영된 상태다.

## Green 확인 절차

Windows PowerShell 환경에서는 `&&` 지원 여부가 셸 버전에 따라 다를 수 있으므로, `cmd /c`로 요청 명령의 조건 실행 의미를 그대로 확인하였다.

```bash
cmd /c "cmake --build build && ctest --test-dir build --output-on-failure"
```

결과:

- `100% tests passed`
- `0 tests failed out of 27`

## 작업 결과

- 소스 파일은 이미 최소 수정 상태였으므로 추가 코드 변경은 수행하지 않았다.
- 디버깅/결함 분석 보고서를 본 파일로 생성하였다.
- 대응 프롬프트 transcript는 `Prompting/06_debugging_defect_analysis.md`에 저장한다.
