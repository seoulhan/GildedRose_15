# Test plan for C++ project
_Exported on 5/19/2026 at 10:14:30 GMT+9 from Cursor (3.4.17)_

---

**User**

@cpp/include/GildedRose.h @cpp/src/GildedRose.cpp @GildedRoseRequirements.txt @docs/requirements_analysis.md 

[Persona] 시니어 QA 리드입니다.
[Context] C++17, Google Test, CMake, (가능하면 gcov/lcov)
[Task] 테스트 계획서를 작성해줘.
    - TEST_F 기반 단위 테스트 범위/우선순위
    - 경계값 케이스 목록 (quality 0,1,49,50 / sellIn 0,-1)
    - 예외/특이 케이스 목록
    - 커버리지 목표 (예: 90%+)와 gcov/lcov 측정·개선 전략
[Format] Markdown 문서. test_plan.md로 저장
[Wrap-up] 
 - Report/ 디렉토리 아래에 작업 내용 보고서를 번호를 붙여 생성하고 저장


---

**Cursor**

요청하신 C++ Gilded Rose 테스트 계획서를 작성하기 위해 먼저 요구사항과 현재 C++ 구현, 기존 보고서 번호 흐름을 확인하겠습니다.

핵심 요구사항과 현재 구현을 확인했습니다. 이제 테스트 구조와 빌드 설정을 살펴본 뒤, 계획서와 `Report`용 요약 문서를 같은 기준으로 작성하겠습니다.

현재 테스트는 기본 `TEST` 샘플만 있고, CMake는 Google Test를 이미 FetchContent로 연결하고 있습니다. 이제 루트 `test_plan.md`와 `Report/04_test_plan_report.md`를 생성하겠습니다.

문서 두 개를 생성했습니다. 마지막으로 새 Markdown 파일의 진단 상태를 확인하고, 변경 파일 목록을 점검하겠습니다.

완료했습니다.

생성한 파일:
- `test_plan.md`
- `Report/04_test_plan_report.md`

포함 내용:
- `TEST_F` 기반 Google Test 단위 테스트 범위와 P0/P1/P2 우선순위
- `quality 0, 1, 49, 50`, `sellIn 0, -1` 중심 경계값 목록
- Sulfuras, Backstage Pass 만료, Conjured, 빈 목록 등 예외/특이 케이스
- gcov/lcov 기준 커버리지 목표와 측정/개선 전략

확인 결과 새 Markdown 파일에 linter 진단은 없습니다. 참고로 기존 작업 상태에는 제가 만든 파일 외에 `Prompting/02_requirements_analysis.md`, `Prompting/03_code_quality.md` 변경/추가가 이미 남아 있습니다.

