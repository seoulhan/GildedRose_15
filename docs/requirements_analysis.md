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
