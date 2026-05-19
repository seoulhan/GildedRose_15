# 05. Gilded Rose 테스트 구현 보고

## 목적

Gilded Rose C++17 프로젝트의 `updateQuality()` 동작을 Google Test 기반 `TEST_F` 테스트로 검증하고, 신규 `Conjured` 규칙까지 포함해 `ctest`가 Green이 되도록 구현을 보완하였다.

## 대상 파일

- `cpp/test/GildedRoseTest.cpp`
- `cpp/src/GildedRose.cpp`
- `docs/requirements_analysis.md`

## 작업 내용

1. 기존 샘플 실패 테스트를 `GildedRoseTest` fixture 기반 테스트로 교체하였다.
2. Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 타입별로 최소 5개 이상의 `TEST_F`를 작성하였다.
3. 모든 테스트에 Given-When-Then 주석 구조를 적용하였다.
4. `EXPECT_EQ`와 `ASSERT_EQ`를 사용해 각 테스트에서 `quality`와 `sellIn`을 검증하였다.
5. `quality == 0`, `quality == 50`, `sellIn == 0`, `sellIn == -1` 경계값을 포함하였다.
6. `Conjured` 아이템의 판매기한 전 2 감소, 판매기한 당일/이후 4 감소, 품질 하한 0 규칙을 구현하였다.
7. `Conjured` 문자열을 포함하는 아이템명에도 Conjured 규칙이 적용되도록 보완하였다.

## 테스트 범위

- Normal: 일반 감소, 판매기한 당일/이후 2배 감소, 품질 하한, 품질 상한 경계 시작값
- Aged Brie: 일반 증가, 판매기한 당일/이후 2배 증가, 품질 상한, 품질 0 시작값
- Backstage Pass: 10일 초과/10일 이하/5일 이하 증가량, 판매기한 당일/이후 품질 0, 품질 상한
- Sulfuras: 일반일/판매기한 당일/판매기한 이후 불변성, 품질 80 예외, 품질 0 입력 불변성
- Conjured: 일반 감소, 판매기한 당일/이후 감소, 품질 하한, 품질 상한 경계 시작값, 이름 식별

## 확인 결과

요청 명령의 `&&` 연산자는 현재 PowerShell 환경에서 구문 오류가 발생하여, 동일한 의미의 PowerShell 조건 실행 구문으로 빌드와 테스트를 확인하였다.

```bash
cmake --build build; if ($LASTEXITCODE -eq 0) { ctest --test-dir build }
```

- 빌드 성공
- `ctest`: 27개 테스트 통과, 실패 0개
