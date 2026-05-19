# Cursor AI project rules
_Exported on 5/19/2026 at 09:42:01 GMT+9 from Cursor (3.4.17)_

---

**User**

[P] 당신은 레거시 코드 QA/리팩토링을 돕는 시니어 C++ 엔지니어입니다.
[C] Gilded Rose C++17 프로젝트에서 Cursor AI가 항상 따라야 하는 규칙을
    프로젝트 루트의 .cursorrules로 작성하려고 합니다.
[T] 아래 요구를 만족하는 .cursorrules 내용을 작성해줘.
    - 기술 스택: C++17 + CMake + Google Test (+ gcov/lcov)
    - 절대 규칙: Item 구조체 수정 금지, quality 0~50 (Sulfuras 제외), sellIn 음수 허용
    - 테스트 규칙: Given-When-Then, TEST_F, 경계값(quality 0/50, sellIn 0/-1) 포함
    - 리팩토링 규칙: 테스트 Green 상태에서만 진행, 매직 넘버 상수화 권장
[F] .cursorrules 파일에 그대로 붙여넣을 수 있는 텍스트

---

**Cursor**

요청하신 `.cursorrules` 작성에 맞춰 Cursor 규칙 작성용 지침을 먼저 확인한 뒤, 바로 붙여넣을 수 있는 형태로 정리하겠습니다.

아래 내용을 `.cursorrules`에 그대로 붙여넣으면 됩니다.

```text
# Gilded Rose C++17 Project Rules

You are a senior C++ engineer helping with QA and safe refactoring of legacy code.

## Tech Stack

- Use C++17.
- Use CMake for builds.
- Use Google Test for automated tests.
- Use gcov/lcov when checking coverage.

## Absolute Domain Rules

- Never modify the `Item` struct.
- Preserve the public behavior of the legacy Gilded Rose kata unless a task explicitly says otherwise.
- `quality` must always stay between `0` and `50`, except for `Sulfuras`.
- `Sulfuras` is legendary and must not decrease in `sellIn` or `quality`.
- Negative `sellIn` values are valid and must be supported. Do not clamp `sellIn` to zero.
- Do not introduce behavior changes without tests that describe the intended rule.

## Testing Rules

- Write tests with Google Test.
- Prefer `TEST_F` for item update behavior tests.
- Structure test cases using Given-When-Then comments or naming.
- Include boundary cases:
  - `quality == 0`
  - `quality == 50`
  - `sellIn == 0`
  - `sellIn == -1`
- Cover normal items, Aged Brie, Backstage passes, Sulfuras, and Conjured items when relevant.
- Tests should verify both `sellIn` and `quality` after update.
- Add or update tests before changing production behavior.

## Refactoring Rules

- Refactor only when the test suite is green.
- Keep refactors small, behavior-preserving, and easy to review.
- Do not mix large refactoring with new feature behavior in the same change.
- Prefer replacing magic numbers with named constants, especially quality bounds and rate changes.
- Improve readability incrementally without over-engineering.
- Preserve existing CMake and Google Test project conventions.
- After refactoring, run the test suite again and ensure it remains green.

## Coding Style

- Prefer clear, simple C++17 over clever abstractions.
- Use descriptive names for domain concepts and constants.
- Keep legacy compatibility in mind.
- Avoid modifying unrelated files or formatting unrelated code.
```

---

**User**

Report 폴더에 번호를 붙여 보고서를 작성해줘.

---

**Cursor**

`Report` 폴더의 기존 파일 번호 체계를 확인한 다음, 방금 작성한 `.cursorrules` 내용을 기준으로 새 보고서를 추가하겠습니다.

기존 규칙 파일은 확인했습니다. `Report` 폴더 검색이 경로 형식 문제로 실패해서, Windows 경로를 조정해 폴더와 기존 보고서명을 다시 확인하겠습니다.

현재 루트에는 `Report` 폴더가 없어 새로 만들고, 첫 번째 보고서로 `01_...md` 형식의 파일을 추가하겠습니다. 내용은 `.cursorrules` 작성 결과와 적용 기준을 간단히 정리한 보고서로 구성하겠습니다.

`Report` 폴더를 새로 만들고 번호를 붙인 보고서를 작성했습니다.

생성한 파일: `Report/01_cursorrules_report.md`

보고서에는 `.cursorrules`의 목적, 기술 스택, 핵심 도메인 규칙, 테스트 기준, 리팩토링 기준을 정리했습니다.

