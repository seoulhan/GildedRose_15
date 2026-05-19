# `updateQuality()` 코드 품질 분석

대상 파일: `cpp/include/GildedRose.h`, `cpp/src/GildedRose.cpp`

## SOLID 및 Code Smell 분석

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---|
| `updateQuality()`가 아이템 순회, 아이템 타입 판별, `sellIn` 감소, `quality` 증감, 경계값 보정, 만료 후 규칙까지 모두 담당한다. | SRP 위반, Long Method | 하나의 요구사항 변경이 메서드 전체 조건 흐름에 영향을 준다. 수정 시 정상 아이템, Aged Brie, Backstage Pass, Sulfuras 규칙이 함께 깨질 위험이 크다. | 아이템별 갱신 책임을 별도 정책/전략 객체로 분리한다. `GildedRose::updateQuality()`는 순회와 전략 호출만 담당하게 축소한다. | 1 |
| `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"` 문자열 비교가 메서드 내부에 반복된다. | OCP 위반, Primitive Obsession, Duplicated Code | 신규 아이템 규칙을 추가할 때 기존 조건문을 계속 수정해야 한다. 예를 들어 Conjured 요구사항 추가 시 `updateQuality()`의 분기 수가 더 증가한다. | 이름 기반 분기를 `ItemUpdater` 전략 선택 로직으로 격리한다. C++17에서는 `std::unique_ptr<ItemRule>`, 함수 테이블, 또는 규칙 레지스트리(`std::unordered_map<std::string, RuleFn>`)로 확장 지점을 만든다. | 1 |
| `0`, `50`, `11`, `6` 같은 의미 있는 숫자가 직접 사용된다. | Magic Number | 품질 하한/상한, Backstage Pass 증가 구간의 의미가 코드만으로 드러나지 않는다. 경계 조건 변경 시 모든 숫자 사용 지점을 찾아야 한다. | `constexpr int MinQuality = 0;`, `MaxQuality = 50;`, `BackstageSecondThreshold = 10;`, `BackstageThirdThreshold = 5;`처럼 도메인 의미가 드러나는 상수로 치환한다. 현재 코드는 `< 11`, `< 6`으로 표현되어 있으므로 이름은 실제 규칙인 `10일 이하`, `5일 이하`를 기준으로 잡는 것이 좋다. | 2 |
| 품질 증감 전후로 `quality > 0`, `quality < 50` 검사가 여러 곳에 흩어져 있다. | Duplicated Code, Shotgun Surgery | 감소/증가 규칙마다 경계 보정이 중복되어 실수 가능성이 높다. 일부 규칙만 clamp가 누락되거나 순서가 바뀔 수 있다. | `increaseQuality(Item&, int amount)`, `decreaseQuality(Item&, int amount)`, `clampQuality()` 같은 작은 헬퍼로 품질 경계 정책을 중앙화한다. Sulfuras의 `quality == 80` 예외는 별도 전략에서 처리한다. | 2 |
| 중첩 `if`와 부정 조건(`!=`) 조합이 많고, 정상 규칙보다 예외 규칙이 먼저 섞여 있다. | 조건문 복잡도, Long Method, Readability Smell | 실제 규칙을 따라 읽기 어렵고, 특정 아이템의 동작을 이해하려면 메서드 전체를 추적해야 한다. 테스트 실패 시 원인 분리가 어렵다. | 조기 분기 또는 전략 패턴으로 아이템별 흐름을 분리한다. 최소 리팩토링으로는 `updateNormal`, `updateAgedBrie`, `updateBackstage`, `updateSulfuras` 함수로 먼저 추출한다. | 2 |
| `Backstage Pass` 규칙이 일반 증가, 10일 이하 추가 증가, 5일 이하 추가 증가, 만료 후 0 처리로 흩어져 있다. | Feature Envy, Temporal Coupling, 조건문 복잡도 | Backstage Pass의 도메인 규칙이 하나의 응집된 단위로 보이지 않는다. `sellIn` 감소 전후 기준을 혼동하기 쉽다. | Backstage 전용 업데이트 함수 또는 전략에 모든 규칙을 모은다. 증가량 계산을 `qualityDeltaForBackstage(sellIn)`처럼 테이블 기반으로 표현할 수 있다. | 3 |
| `items[i]` 인덱스 접근이 반복되고, 루프 내부에서 같은 객체를 여러 번 조회한다. | Readability Smell, Low-level Iteration | 코드가 장황해지고 조건식이 길어진다. 리팩토링 중 잘못된 인덱스 사용이나 복사 실수 가능성이 생긴다. | C++17 범위 기반 for 문을 사용해 `for (Item& item : items)`로 표현한다. 각 규칙 함수도 `Item&`를 인자로 받게 한다. | 4 |
| `items`가 public 참조 멤버로 노출되어 있고, `GildedRose`는 규칙 실행과 외부 컬렉션 변경을 강하게 결합한다. | Encapsulation 약화, SRP 경계 불명확 | 외부에서 컬렉션을 직접 변경할 수 있어 규칙 적용 전후 불변식을 보장하기 어렵다. 테스트나 향후 API 확장 시 캡슐화가 약하다. | 기존 인터페이스 보존이 필요하면 당장 변경하지 않되, 장기적으로는 private 멤버와 접근자 또는 업데이트 서비스 분리를 검토한다. | 5 |
| 타입별 동작이 문자열 이름에 직접 묶여 있어 컴파일 타임 타입 안정성이 없다. | Primitive Obsession, OCP 위반 가능성 | 오타나 이름 변경이 런타임 동작 변경으로 이어진다. 신규 타입 추가 시 테스트 없이는 누락을 발견하기 어렵다. | 단기적으로 문자열 상수를 중앙화하고, 중기적으로 이름을 규칙으로 매핑하는 팩토리를 둔다. 더 큰 설계 변경이 가능하면 `std::variant<Normal, AgedBrie, BackstagePass, Sulfuras, Conjured>` 기반 모델도 고려할 수 있다. | 5 |

## 개선 방향 요약

1. 가장 먼저 `updateQuality()`를 아이템별 함수로 추출해 Long Method와 중첩 조건을 줄인다.
2. `MinQuality`, `MaxQuality`, Backstage Pass 임계값, 아이템 이름을 `constexpr` 상수로 올려 도메인 의미를 드러낸다.
3. 품질 증가/감소 및 하한/상한 보정 로직을 공통 헬퍼로 중앙화한다.
4. 이후 신규 요구사항이 계속 추가될 가능성이 크다면 전략 패턴으로 전환한다. 예시는 `ItemRule` 인터페이스와 `NormalRule`, `AgedBrieRule`, `BackstagePassRule`, `SulfurasRule`, `ConjuredRule` 구현체를 두는 방식이다.
5. 아이템 종류가 고정되어 있고 런타임 확장성이 중요하지 않다면 C++17 함수 테이블이나 `std::variant` 기반 방문자도 가능하다. 다만 현재 레거시 `Item{name, sellIn, quality}` 인터페이스를 유지해야 한다면 전략 패턴 또는 이름 기반 규칙 레지스트리가 가장 점진적이다.

## C++17 리팩토링 방향

- **1단계: 함수 추출**
  - `updateNormal(Item&)`, `updateAgedBrie(Item&)`, `updateBackstagePass(Item&)`, `updateSulfuras(Item&)`로 분리한다.
  - 이 단계는 동작 변경 위험이 낮고 테스트 보강과 병행하기 쉽다.

- **2단계: 상수 및 헬퍼 도입**
  - `constexpr int MinQuality = 0;`
  - `constexpr int MaxQuality = 50;`
  - `constexpr int BackstageDoubleIncreaseSellIn = 10;`
  - `constexpr int BackstageTripleIncreaseSellIn = 5;`
  - `increaseQuality`, `decreaseQuality` 헬퍼에서 `std::clamp`를 사용할 수 있다.

- **3단계: 전략 패턴**
  - `class ItemRule { public: virtual void update(Item&) const = 0; };`
  - `GildedRose::updateQuality()`는 `for (Item& item : items) ruleFor(item).update(item);` 정도로 단순화한다.
  - 신규 아이템 추가 시 기존 규칙 코드를 직접 수정하지 않고 새 전략과 매핑만 추가한다.

- **4단계: 테이블 기반 규칙**
  - Backstage Pass처럼 `sellIn` 구간별 증가량이 명확한 규칙은 조건문 대신 구간 테이블로 표현할 수 있다.
  - 예: `sellIn > 10 => +1`, `6..10 => +2`, `1..5 => +3`, `<=0 => quality = 0`.

- **5단계: `std::variant` 가능성 검토**
  - 도메인 모델을 `NormalItem`, `AgedBrie`, `BackstagePass`, `Sulfuras`처럼 타입으로 분리할 수 있다면 `std::variant`와 `std::visit`로 컴파일 타임 분기를 만들 수 있다.
  - 단, 현재 과제의 레거시 인터페이스가 문자열 기반 `Item`을 요구한다면 variant 전환은 비용이 크므로 후순위가 적절하다.
