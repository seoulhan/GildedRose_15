# Gilded Rose 테스트 계획서

대상: `cpp/include/GildedRose.h`, `cpp/src/GildedRose.cpp`  
기술 스택: C++17, Google Test, CMake, gcov/lcov

## 1. 테스트 목표

`GildedRose::updateQuality()`가 요구사항에 따라 모든 아이템의 `sellIn`과 `quality`를 정확히 갱신하는지 검증한다. 특히 레거시 코드의 중첩 조건문과 문자열 기반 아이템 판별로 인해 발생하기 쉬운 회귀를 방지하고, 신규 `Conjured` 규칙을 안전하게 추가할 수 있는 테스트 기반을 마련한다.

핵심 품질 기준은 다음과 같다.

- `Sulfuras`를 제외한 모든 아이템의 `quality`는 `0` 이상 `50` 이하를 유지한다.
- `sellIn == 0`은 갱신 후 판매기한이 지난 상태로 간주한다.
- `sellIn < 0`은 유효한 도메인 상태이며 `0`으로 보정하지 않는다.
- `Sulfuras`는 `sellIn`, `quality` 모두 변경하지 않으며 `quality == 80` 예외를 유지한다.
- `Conjured`는 Normal 아이템보다 두 배 빠르게 품질이 감소한다.

## 2. 테스트 구조

기존 `cpp/test/GildedRoseTest.cpp`의 샘플 테스트는 `TEST_F` 기반 fixture로 교체한다. 테스트 fixture는 아이템 생성, 하루 갱신, 결과 검증을 반복적으로 작성하지 않도록 최소한의 헬퍼만 제공한다.

권장 fixture 구조:

```cpp
class GildedRoseTest : public ::testing::Test {
protected:
    std::vector<Item> items;

    Item& updateOne(const std::string& name, int sellIn, int quality) {
        items = { Item(name, sellIn, quality) };
        GildedRose app(items);
        app.updateQuality();
        return items.front();
    }
};
```

테스트 작성 원칙:

- 모든 테스트는 `TEST_F(GildedRoseTest, ...)` 형식으로 작성한다.
- Given-When-Then 흐름이 보이도록 준비, 실행, 검증을 분리한다.
- 갱신 후 `quality`뿐 아니라 `sellIn`도 함께 검증한다.
- 아이템명 문자열 오타를 줄이기 위해 테스트 파일 안에 상수로 둔다.
- 여러 아이템 동시 처리 테스트는 독립 갱신 여부를 검증하는 회귀 테스트로 별도 작성한다.

## 3. 단위 테스트 범위와 우선순위

### P0: 핵심 도메인 규칙

가장 먼저 작성해야 하는 회귀 방지 테스트다. 실패 시 제품 동작이 요구사항과 직접 어긋난다.

| 영역 | 테스트 케이스 | 기대 결과 |
|---|---|---|
| Normal | 판매기한 전 하루 갱신 | `sellIn -1`, `quality -1` |
| Normal | `sellIn == 0` 하루 갱신 | `sellIn == -1`, `quality -2` |
| Normal | `quality == 0` | `quality == 0` 유지 |
| Aged Brie | 판매기한 전 하루 갱신 | `sellIn -1`, `quality +1` |
| Aged Brie | `sellIn == 0` 하루 갱신 | `quality +2` |
| Aged Brie | `quality == 50` | `quality == 50` 유지 |
| Backstage Pass | `sellIn > 10` | `quality +1` |
| Backstage Pass | `sellIn == 10` | `quality +2` |
| Backstage Pass | `sellIn == 5` | `quality +3` |
| Backstage Pass | `sellIn == 0` | `quality == 0` |
| Sulfuras | 하루 갱신 | `sellIn`, `quality` 모두 불변 |
| Conjured | 판매기한 전 하루 갱신 | `sellIn -1`, `quality -2` |
| Conjured | `sellIn == 0` 하루 갱신 | `quality -4` |

### P1: 경계값과 조합 규칙

경계값 보정, 구간 전환, 음수 `sellIn` 유지 여부를 검증한다.

| 영역 | 테스트 케이스 | 기대 결과 |
|---|---|---|
| Normal | `quality == 1`, `sellIn > 0` | `quality == 0` |
| Normal | `quality == 1`, `sellIn == 0` | `quality == 0` |
| Aged Brie | `quality == 49`, `sellIn > 0` | `quality == 50` |
| Aged Brie | `quality == 49`, `sellIn == 0` | `quality == 50` |
| Backstage Pass | `quality == 49`, `sellIn == 10` | `quality == 50` |
| Backstage Pass | `quality == 49`, `sellIn == 5` | `quality == 50` |
| Backstage Pass | `sellIn == 1` | 갱신 후 콘서트 경과로 `quality == 0` |
| Conjured | `quality == 1`, `sellIn > 0` | `quality == 0` |
| Conjured | `quality == 1`, `sellIn == 0` | `quality == 0` |
| Sulfuras | `quality == 80` | `80` 유지, `50`으로 보정하지 않음 |

### P2: 통합성 및 회귀 테스트

한 번의 `updateQuality()` 호출이 여러 아이템을 독립적으로 갱신하는지 검증한다.

- Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured가 같은 벡터에 있을 때 각 규칙이 독립 적용된다.
- 비어 있는 `items` 벡터에서 `updateQuality()` 호출 시 예외 없이 종료된다.
- 알 수 없는 이름의 아이템은 Normal 규칙을 따른다.
- `"Conjured Mana Cake"`처럼 `"Conjured"`를 포함한 이름은 Conjured 규칙을 따른다.

## 4. 경계값 케이스 목록

아래 경계값은 아이템 타입별 최소 1회 이상 포함한다.

| 경계값 | 검증 의도 | 우선순위 |
|---|---|---|
| `quality == 0` | 감소 규칙이 음수 품질을 만들지 않는지 검증 | P0 |
| `quality == 1` | 감소량이 품질보다 클 때 `0`으로 고정되는지 검증 | P1 |
| `quality == 49` | 증가 규칙이 `50`까지만 올리는지 검증 | P1 |
| `quality == 50` | 증가 규칙이 상한을 넘지 않는지 검증 | P0 |
| `quality == 80` | Sulfuras 전용 예외 유지 검증 | P0 |
| `sellIn == 0` | 갱신 후 만료 상태가 되어 추가 규칙이 적용되는지 검증 | P0 |
| `sellIn == -1` | 이미 만료된 상태에서 감소/증가율이 적용되는지 검증 | P1 |

아이템별 필수 경계 조합:

| 아이템 | 필수 조합 |
|---|---|
| Normal | `(sellIn, quality) = (0, 1)`, `(0, 50)`, `(-1, 1)`, `(1, 0)` |
| Aged Brie | `(0, 49)`, `(0, 50)`, `(-1, 49)`, `(1, 50)` |
| Backstage Pass | `(10, 49)`, `(5, 49)`, `(1, 50)`, `(0, 50)`, `(-1, 50)` |
| Sulfuras | `(0, 80)`, `(-1, 80)` |
| Conjured | `(0, 1)`, `(0, 50)`, `(-1, 1)`, `(1, 0)` |

## 5. 예외 및 특이 케이스 목록

| 케이스 | 기대 동작 | 테스트 목적 |
|---|---|---|
| `Sulfuras, Hand of Ragnaros` | `sellIn`, `quality` 모두 불변 | 전설 아이템 예외 보장 |
| Sulfuras `quality == 80` | `50`으로 clamp하지 않음 | 일반 품질 상한 예외 보장 |
| Backstage Pass 만료 | `quality == 0` | 콘서트 이후 가치 소멸 보장 |
| Backstage Pass 증가 중 상한 도달 | `quality == 50` | 구간 증가량이 상한을 넘지 않음 |
| `sellIn < 0` | 매일 계속 `-1` 감소 | 음수 `sellIn` 보정 금지 |
| 빈 아이템 목록 | 예외 없이 종료 | 루프 안정성 확인 |
| 알 수 없는 아이템명 | Normal 규칙 적용 | 기본 분기 보장 |
| `"Conjured"` 포함 이름 | Conjured 규칙 적용 | 신규 아이템 식별 보장 |
| 감소량이 현재 `quality`보다 큼 | `quality == 0` | 음수 품질 방지 |

## 6. 커버리지 목표

목표 커버리지:

- 라인 커버리지: 90% 이상
- 함수 커버리지: 100%
- 브랜치 커버리지: 85% 이상
- 핵심 도메인 분기 커버리지: Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 각각 100%

현재 `updateQuality()`는 조건문 중첩이 많으므로 단순 라인 커버리지보다 브랜치 커버리지를 함께 본다. 특히 다음 분기는 반드시 커버한다.

- Normal 감소와 만료 후 추가 감소
- `quality > 0` / `quality == 0`
- Aged Brie 증가와 만료 후 추가 증가
- `quality < 50` / `quality == 50`
- Backstage Pass의 `> 10`, `6..10`, `1..5`, `<= 0`
- Sulfuras 불변 분기
- Conjured 판매기한 전/후 감소 분기

## 7. gcov/lcov 측정 전략

CMake Debug 빌드에서 GCC/Clang 계열 컴파일러를 사용할 때 커버리지 플래그를 활성화한다.

권장 실행 절차:

```bash
cmake -S cpp -B build/coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build build/coverage
ctest --test-dir build/coverage --output-on-failure
lcov --capture --directory build/coverage --output-file build/coverage/coverage.info
lcov --remove build/coverage/coverage.info "*/_deps/*" "*/test/*" "/usr/*" --output-file build/coverage/coverage.filtered.info
genhtml build/coverage/coverage.filtered.info --output-directory build/coverage/html
```

Windows 환경에서 MinGW 또는 MSYS2를 사용하는 경우에도 같은 흐름을 적용하되, `lcov`, `genhtml` 설치 여부를 먼저 확인한다. Visual Studio/MSVC 컴파일러를 사용할 경우 gcov/lcov 대신 OpenCppCoverage 같은 도구를 별도 검토한다.

## 8. 커버리지 개선 전략

1. 먼저 P0 테스트를 모두 작성해 핵심 요구사항을 고정한다.
2. 커버리지 리포트에서 `GildedRose.cpp`의 미커버 라인과 미커버 브랜치를 확인한다.
3. 미커버 브랜치가 도메인적으로 의미 있는 조건이면 P1/P2 테스트로 추가한다.
4. 단순 방어 코드나 도달 불가능한 분기가 발견되면 테스트 추가보다 리팩토링 대상으로 분류한다.
5. Conjured 구현 후에는 기존 Normal 테스트와 별개로 전용 경계값 테스트를 추가해 신규 요구사항 회귀를 방지한다.
6. 커버리지 수치가 목표를 넘더라도 `sellIn == 0`, `sellIn == -1`, `quality == 0`, `quality == 50` 조합이 빠져 있으면 테스트 완료로 보지 않는다.

## 9. 완료 기준

- `cpp/test/GildedRoseTest.cpp`가 `TEST_F` 기반 테스트로 구성되어 있다.
- P0 테스트가 모두 통과한다.
- P1 경계값 테스트가 모두 통과한다.
- Conjured 관련 테스트가 실패 상태에서 시작해 구현 후 통과한다.
- `ctest --test-dir build/coverage --output-on-failure`가 성공한다.
- lcov 기준 라인 커버리지 90% 이상, 브랜치 커버리지 85% 이상을 달성한다.
