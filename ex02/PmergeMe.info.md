## PmergeMe 의존도 트리

### 파일 구성
- **main.cpp**: 프로그램 엔트리. `PmergeMe.hpp` 포함, `PmergeMe` 인스턴스 생성.
- **PmergeMe.hpp**: 클래스 선언 및 예외 클래스 선언. 표준 라이브러리 헤더 포함.
- **PmergeMe.cpp**: 클래스 정의 및 정적 헬퍼 함수 정의. `PmergeMe.hpp` 포함.
- **Makefile**: 빌드 규칙. 표준 `c++` 컴파일러, `-std=c++98` 사용.

### 빌드/링크 의존성
- 실행 파일 `PmergeMe`
  - 링크 대상: `main.o`, `PmergeMe.o`
- 오브젝트 생성
  - `main.cpp` → `main.o`
  - `PmergeMe.cpp` → `PmergeMe.o`

### 포함(Include) 의존성
- `main.cpp`
  - 포함: `PmergeMe.hpp`
- `PmergeMe.cpp`
  - 포함: `PmergeMe.hpp`
- `PmergeMe.hpp`
  - 포함: `<vector>`, `<list>`, `<iostream>`, `<iomanip>`, `<exception>`, `<ctime>`, `<cstdlib>`, `<cmath>`

### 실행 흐름(상위 수준)
- `main(int, char**)`
  - 인자 검증 후 `PmergeMe pm(++av)` 생성
  - 모든 예외는 `std::exception` 기준으로 캐치하여 `what()` 출력

### 클래스/함수 의존도 트리
- `class PmergeMe`
  - 멤버 데이터
    - `_jacobstal: std::vector<size_t>`
    - `_cnt: size_t`
    - `_vec: std::vector<int>*`
    - `_lst: std::list<int>*`
  - 중첩 예외 클래스: `InvalidInputException`, `DuplicateNumberException`, `OrderedInputExcetion`
  - 파일 내부 정적 헬퍼
    - `binaryInsertVec(std::vector<int>&, int, size_t)`
    - `binaryInsertList(std::list<int>&, int, size_t)`
  - 생성/파괴
    - `PmergeMe(char** av)` → 입력 파싱, 초기 출력, 정렬 트리거
    - `~PmergeMe()` → `_vec`, `_lst` 해제
  - 입력/검증
    - `toInt(char* c)` → 숫자성 검증(문자 검사·`strtod`), 중복 검사(`_vec` 선행 요소와 비교)
      - 예외 발생 가능: `InvalidInputException`, `DuplicateNumberException`
    - `checkOrder()` → 초기 입력이 이미 오름차순인지 검사
      - 생성자에서 `true`면 `OrderedInputExcetion` 던짐
  - 정렬 파이프라인
    - `sort()`
      - `getJacobstalSequence(size_t)`로 야코브스탈 수열 준비
      - `fordJohnsonByList(std::list<int>*)` 수행 및 시간 측정
        - 내부에서 `binaryInsertList` 사용
      - `fordJohnsonByVec(std::vector<int>*)` 수행 및 시간 측정
        - 내부에서 `binaryInsertVec` 사용
      - 결과 및 소요 시간 출력
    - `fordJohnsonByVec(std::vector<int>*)`
      - 쌍 구성(큰 값, 작은 값), 큰 값 부분 재귀 정렬 → 메인 체인 구성 → 작은 값들을 야코브스탈 순서로 이진 삽입
      - 의존: `binaryInsertVec`, `_jacobstal`
    - `fordJohnsonByList(std::list<int>*)`
      - 벡터 버전과 동일 로직을 리스트에 맞춰 구현
      - 의존: `binaryInsertList`, `_jacobstal`
    - `binaryInsertByVec(std::vector<int>*, int)`/`binaryInsertByList(std::list<int>*, int)`
      - 각각 `binaryInsertVec`/`binaryInsertList`에 얇은 래핑 (현재 직접 사용 빈도 낮음)
    - `listIdx(std::list<int>&, size_t)`
      - 인덱스 위치 반복자 반환 유틸리티 (현재 직접 사용 없음)

### 예외 흐름 의존성
- `toInt` → `InvalidInputException`, `DuplicateNumberException`
- `PmergeMe(char** av)` → `checkOrder()`가 `true`면 `OrderedInputExcetion` 던짐
- `main`에서 모든 `std::exception` 파생 예외를 캐치하고 메시지 출력 후 종료

### 표준 라이브러리 의존성 요약
- 컨테이너: `std::vector`, `std::list`
- 스트림/포맷: `std::cout`, `std::cerr`, `std::fixed`, `std::setprecision`
- 시간: `std::clock`, `CLOCKS_PER_SEC`
- 변환: `strtod`
- 예외: `std::exception`

### 출력/사이드 이펙트
- 생성자에서 "Before :" 라인과 입력 값을 출력
- `sort()`에서 리스트/벡터 처리 시간과 "After:" 정렬 결과 출력

### 요약 트리(한눈에 보기)
```
PmergeMe (실행파일)
└─ 링크: main.o, PmergeMe.o
   ├─ main.cpp
   │  └─ include → PmergeMe.hpp
   │     └─ 호출 → PmergeMe::PmergeMe(char**)
   └─ PmergeMe.cpp
      └─ include → PmergeMe.hpp
         ├─ 정적 헬퍼 → binaryInsertVec, binaryInsertList
         └─ class PmergeMe
            ├─ 입력 검증 → toInt → {InvalidInputException, DuplicateNumberException}
            ├─ 정렬 여부 검사 → checkOrder → {OrderedInputExcetion}
            └─ sort
               ├─ getJacobstalSequence
               ├─ fordJohnsonByList → binaryInsertList
               └─ fordJohnsonByVec  → binaryInsertVec
```
