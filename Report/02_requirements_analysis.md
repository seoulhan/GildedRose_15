# Gilded Rose 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | 식별 기준 예시 | sellIn 변경 | quality 변경 | 경계/예외 |
|---|---|---:|---:|---|
| Normal | 특수 아이템명이 아닌 일반 아이템 | 매일 `-1` | 판매기한 전 `-1`, 판매기한 경과 후 `-2` | `quality`는 `0` 미만으로 내려가지 않음 |
| Aged Brie | 이름이 정확히 `"Aged Brie"` | 매일 `-1` | 판매기한 전 `+1`, 판매기한 경과 후 `+2` | `quality`는 `50` 초과 불가 |
| Backstage Pass | `"Backstage passes"` 계열 이름 | 매일 `-1` | `sellIn > 10`: `+1`, `6 <= sellIn <= 10`: `+2`, `1 <= sellIn <= 5`: `+3`, 판매기한 경과 후 `0` | 증가 시 `quality`는 `50` 초과 불가, 콘서트 이후에는 즉시 `0` |
| Sulfuras | 이름이 정확히 `"Sulfuras, Hand of Ragnaros"` | 변경 없음 | 변경 없음 | 전설 아이템 예외. `quality`는 보통 `80`이며 `0~50` 제한을 적용하지 않음 |
| Conjured | `"Conjured"` 아이템 | 매일 `-1` | 판매기한 전 `-2`, 판매기한 경과 후 `-4` | Normal 아이템 감소율의 2배. `quality`는 `0` 미만으로 내려가지 않음 |

> `updateQuality()`는 하루가 끝날 때 한 번 호출되는 갱신 로직으로 본다. 대부분의 아이템은 `sellIn` 감소 후 판매기한 경과 여부에 따라 추가 품질 변화가 발생할 수 있으므로, 구현에서는 기존 코드의 평가 순서를 명확히 유지해야 한다.

## 2. 문자열 비교/분기 시 주의점

1. `std::string::find()`는 위치 인덱스 또는 `std::string::npos`를 반환한다. 조건문에서 `if (name.find("Conjured"))`처럼 직접 사용하면, 문자열이 맨 앞에 있을 때 반환값 `0`이 `false`로 평가되어 버그가 된다.
2. 부분 문자열 판별은 `name.find("Conjured") != std::string::npos` 형태로 작성한다.
3. 정확한 아이템 식별이 필요한 경우에는 `==`를 사용한다. 예: `name == "Aged Brie"`, `name == "Sulfuras, Hand of Ragnaros"`.
4. Backstage Pass는 전체 공식 이름이 길기 때문에 기존 구현이 부분 문자열로 판별할 수 있다. 단, `find()` 사용 시 반드시 `npos` 비교를 해야 한다.
5. 분기 순서는 중요하다. Sulfuras는 `sellIn`과 `quality`가 모두 변하지 않아야 하므로, 가장 먼저 예외 처리하거나 공통 감소 로직에서 확실히 제외해야 한다.
6. Conjured가 일반 아이템 규칙의 확장이라면 Normal 처리보다 먼저 판별하거나, 아이템 타입을 먼저 결정한 뒤 감소율을 적용해야 한다.
7. 문자열 리터럴 오타는 테스트로 잡기 어렵지 않지만 영향이 크다. 가능하면 상수화해서 `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"`, `"Conjured"`를 한 곳에서 관리한다.

## 3. 예외/경계값 조건

1. `quality == 0`
   - Normal/Conjured는 더 이상 감소하지 않아야 한다.
   - Backstage Pass가 콘서트 이후라면 결과는 여전히 `0`이다.
2. `quality == 50`
   - Aged Brie와 Backstage Pass는 증가 규칙이 적용되어도 `50`을 초과하면 안 된다.
   - Normal/Conjured는 감소 가능하다.
3. `sellIn == 0`
   - 해당 날짜의 갱신 후 판매기한이 지난 상태가 된다.
   - Normal은 하루 갱신 결과 `quality`가 총 `2` 감소하는 케이스로 검증해야 한다.
   - Aged Brie는 `quality`가 총 `2` 증가하는 케이스로 검증해야 한다.
   - Backstage Pass는 콘서트 이후가 되어 `quality`가 `0`이 되어야 한다.
4. `sellIn == -1`
   - 이미 판매기한이 지난 아이템이다.
   - Normal은 `quality`가 `2` 감소한다.
   - Aged Brie는 `quality`가 `2` 증가한다.
   - Conjured는 `quality`가 `4` 감소한다.
5. `sellIn < 0`
   - 음수 `sellIn`은 유효한 상태다. `0`으로 보정하거나 clamp하지 않는다.
   - 매일 계속 `-1`씩 감소해야 한다. 단, Sulfuras는 예외다.
6. Sulfuras
   - `sellIn`은 감소하지 않는다.
   - `quality`는 변경하지 않는다.
   - `quality == 80`은 `0~50` 품질 상한 예외로 다룬다.

## 4. Conjured 신규 요구사항 명세

1. Conjured 아이템은 Normal 아이템과 동일하게 매일 `sellIn`이 `1` 감소한다.
2. Conjured 아이템의 `quality` 감소량은 Normal 아이템의 2배다.
3. 판매기한 전(`sellIn > 0`)에는 하루에 `quality`가 `2` 감소한다.
4. 판매기한 당일 또는 경과 후에는 Normal의 판매기한 경과 감소량 `2`의 2배인 `4`가 감소한다.
5. 감소 후에도 `quality`는 `0` 미만이 될 수 없다.
6. Conjured는 Sulfuras가 아니므로 `quality` 상한 예외를 갖지 않는다.
7. 이름 판별은 프로젝트 요구에 맞춰 `"Conjured"` 접두/부분 문자열을 사용할 수 있으나, C++에서는 `find("Conjured") != std::string::npos`처럼 명확히 작성해야 한다.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 하루 후 `sellIn`이 `1` 감소하고 `quality`가 `1` 감소한다.
2. Normal 아이템의 `sellIn == 0`에서는 하루 후 `sellIn == -1`, `quality`는 `2` 감소한다.
3. Normal 아이템의 `sellIn == -1`에서는 하루 후 `quality`가 `2` 감소한다.
4. Normal 아이템의 `quality == 0`은 감소 후에도 `0`을 유지한다.
5. Aged Brie는 하루 후 `sellIn`이 `1` 감소하고 `quality`가 `1` 증가한다.
6. Aged Brie의 `sellIn == 0`에서는 하루 후 `quality`가 `2` 증가한다.
7. Aged Brie의 `sellIn < 0`에서는 하루 후 `quality`가 `2` 증가한다.
8. Aged Brie의 `quality == 50`은 증가 후에도 `50`을 초과하지 않는다.
9. Backstage Pass의 `sellIn > 10`에서는 하루 후 `quality`가 `1` 증가한다.
10. Backstage Pass의 `sellIn == 10`에서는 하루 후 `quality`가 `2` 증가한다.
11. Backstage Pass의 `sellIn == 5`에서는 하루 후 `quality`가 `3` 증가한다.
12. Backstage Pass의 `sellIn == 1`에서는 하루 후 콘서트가 지나 `quality`가 `0`이 된다.
13. Backstage Pass의 `sellIn == 0`에서는 하루 후 `quality`가 `0`이 된다.
14. Backstage Pass의 `quality == 50`은 증가 구간에서도 `50`을 초과하지 않는다.
15. Sulfuras는 하루 후 `sellIn`과 `quality`가 모두 변경되지 않는다.
16. Sulfuras의 `quality == 80`은 일반 품질 상한 `50`으로 보정되지 않는다.
17. Conjured 아이템은 판매기한 전 하루 후 `sellIn`이 `1` 감소하고 `quality`가 `2` 감소한다.
18. Conjured 아이템의 `sellIn == 0`에서는 하루 후 `quality`가 `4` 감소한다.
19. Conjured 아이템의 `sellIn == -1`에서는 하루 후 `quality`가 `4` 감소한다.
20. Conjured 아이템의 `quality`가 감소량보다 작을 때 결과는 `0`으로 고정된다.
21. Conjured 아이템명이 `"Conjured Mana Cake"`처럼 `"Conjured"`를 포함할 때 Conjured 규칙이 적용된다.
22. 여러 아이템이 한 번에 들어 있는 경우, `updateQuality()` 호출 후 각 아이템이 자기 타입 규칙에 따라 독립적으로 갱신된다.
