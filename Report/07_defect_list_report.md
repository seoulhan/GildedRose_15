# 07. Gilded Rose 결함 목록 작성 보고

## 목적

현재까지 테스트 구현 및 디버깅 과정에서 발견된 테스트 실패/결함을 QA 리드 관점에서 추적 가능한 Markdown 결함 목록으로 정리하였다.

## 대상 파일

- `docs/defect_list.md`
- `Report/05_test_implementation_report.md`
- `Report/06_debugging_defect_analysis_report.md`
- `cpp/src/GildedRose.cpp`
- `cpp/test/GildedRoseTest.cpp`

## 작업 내용

1. 기존 테스트 구현 보고서와 디버깅/결함 분석 보고서를 확인해 결함 근거를 수집하였다.
2. 현재 로컬 테스트 결과가 `27/27` 통과 상태임을 전제로, 활성 실패가 아닌 발견/분석된 결함 이력을 분리해 작성하였다.
3. Conjured 규칙 누락으로 발생 가능한 기대/실제 차이를 `DEF-001`, `DEF-002`로 정리하였다.
4. Windows PowerShell의 `&&` 조건 실행 문제를 제품 결함과 구분되는 테스트 실행 이슈 `DEF-003`으로 정리하였다.
5. 각 항목을 요청 형식인 `[ID] [Severity] [ItemType] [Steps] [Expected] [Actual] [Root Cause] [Fix Summary]` 필드로 작성하였다.

## 작성 산출물

- `docs/defect_list.md`

## 결함 요약

- `DEF-001` Major: Conjured 아이템이 판매기한 전 일반 아이템처럼 `quality` 1만 감소할 수 있던 결함
- `DEF-002` Major: Conjured 아이템이 판매기한 당일/이후 일반 만료 규칙처럼 `quality` 2만 감소할 수 있던 결함
- `DEF-003` Minor: 현재 PowerShell 환경에서 `cmake --build build && ctest` 명령이 구문 호환성 문제로 중단될 수 있는 테스트 실행 이슈

## 확인 사항

현재 `cpp/src/GildedRose.cpp`에는 Conjured 판별 및 전용 감소 규칙이 반영되어 있으며, 마지막으로 확인된 `ctest` 결과는 27개 테스트 통과, 실패 0개다.

## 결과

요청된 형식의 결함 목록을 `docs/defect_list.md`에 저장하였다. 대응 프롬프트 transcript는 `Prompting/07_defect_list.md`에 저장한다.
