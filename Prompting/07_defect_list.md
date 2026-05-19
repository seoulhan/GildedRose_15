# Gilded Rose 결함 목록 작성
_Exported on 5/19/2026 at 11:02 GMT+9 from Cursor_

---

**User**

[Persona] QA 리드입니다.

[Context] 현재까지 발견된 테스트 실패/결함들을 문서화합니다.

[Task] 발견된 결함을 다음 형식으로 docs\ 디렉토리 아래 defect_list.md에 정리해줘.

항목: [ID] [Severity] [ItemType] [Steps] [Expected] [Actual] [Root Cause] [Fix Summary]

[Format] Markdown. docs\ 디렉토리 아래 defect_list.md로 저장

[Wrap-up]

- Report\ 디렉토리 아래에 작업 내용 보고서를 번호를 붙여 생성하고 저장
- 현재까지의 프롬프트 대화 전체를 Prompting\ 디렉토리 아래에 Export Transcript 해줘. Transcript 파일명은 보고서의 이름에서 _report 접미사를 뺀 것으로 해줘.

---

**Cursor**

요청하신 결함 목록, 보고서, 프롬프트 transcript 파일을 생성하겠습니다. 먼저 기존 문서와 프로젝트 테스트 상태를 확인해서 "현재까지 발견된 결함"의 근거를 맞추겠습니다.

기존 산출물에서 `06_debugging_defect_analysis`가 이미 만들어져 있어, 그 내용을 중심으로 실제 실패/결함 항목을 추적하겠습니다. 다음으로 해당 보고서와 테스트 로그, 구현/테스트 파일을 확인합니다.

확인 결과 현재 워크스페이스의 `ctest`는 27개 모두 통과 상태이고, 기존 분석에서 실제 결함으로 남길 수 있는 핵심 항목은 `Conjured` 규칙 누락 이력입니다. 테스트 실행 과정에서 확인된 PowerShell `&&` 문제도 제품 결함이 아닌 테스트 실행 이슈로 분리해 기록하겠습니다.

이제 `docs/defect_list.md`에는 요청 형식의 필드를 그대로 유지해 결함 항목을 작성하고, 이어서 `Report/07...` 및 `Prompting/07...` 파일을 같은 번호 체계로 추가하겠습니다.
