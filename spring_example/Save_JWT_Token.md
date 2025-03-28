# JWT Token 저장 방법

[JWT](https://github.com/Lanvizu/TIL/blob/main/spring_example/JWT.md) 관련 정리 후 프로젝트에 적용하면서 Access Token과 Refresh Token을 어디에 저장할 지 분석

-----

## 사용자 인증

- 시스템이나 애플리케이션에 접근하려는 사용자가 실제로 그 사용자가 맞는지 확인하는 과정

## Session 인증 방식

- 상태유지(stateful): 서버에 상태를 저장하는 방식

- 로그인 -> 서버로 요청 -> 요청 정보와 DB 일치 확인 -> 사용자의 세션 생성 -> 해당 세션 ID를 서버나 메모리에 저장

- 이후 사용자의 후속 요청마다 세션 ID 담긴 쿠키를 서버로 전송 -> 해당 세션 ID가 저장소에 존재하는지 확인 후 인증

### 고려할 부분

- 장점: 서버에서 인증을 관리하므로 클라이언트 측에서 정보 노출 위험이 적음

- 단점: 사용자 요청마다 DB에 접근 -> 응답 속도 저하 -> 서버 부하 원인

## 토큰 인증 방식

- 상태 비저장(Stateless): 서버에 상태를 저장하지 않는 방식

- DB에 거치지 않고 오로지 토큰을 이용한 방법

> 서버를 안거치는 방식이 아님

### 고려할 부분

- 장점

    - DB를 거치지 않아 응답속도 저하 문제 해결

    - 확장성 면에서도 우수
 
    > 단순한 문자열 형식이며 서버에 저장하지 않는 방식으로 다양한 플랫폼과 환경에서 쉽게 사용 가능

- 단점

    - 토큰에 정보를 내재한 Self-Contained 방식인데 탈취를 당할 우려가 있음
 
    - XSS(악성 코드) 공격이나 클라이언트 -> 서버로 전송 중 탈취 가능성 있음
 
    - 탈취를 당했을 때 서버 쪽에서 감지하기 어렵고 강제로 해제도 불가능 (클라이언트가 관리하므로)
 
    > 블랙리스트로 설정 가능하지만 탈취 감지가 어렵다.

> XSS (Cross-Site Scripting)
>
> 공격자가 악성 스크립트를 웹 페이지에 삽입하여 사용자의 브라우저에서 실행되도록 하는 공격

> CSRF (Cross-Site Request Forgery)
>
> 공격자가 인증된 사용자의 권한을 이용하여 원치 않는 동작을 수행하도록 하는 공격

| 특성 | XSS | CSRF |
|------|-----|------|
| 공격 대상 | 사용자 브라우저 | 서버 |
| 주요 목적 | 사용자 데이터 탈취 | 원치 않는 작업 수행 |
| 주요 방어법 | 입력 검증, 출력 인코딩 | 토큰 검증, Referrer 확인 |

-----

Access Token과 Refresh Token을 나눠 분석

# Access Token

- 사용자의 인증과 권한이 담긴 문자열 유효 토큰

- 동작 방식: 클라이언트 -> 서버 API 요청 (토큰을 헤더에 포함) -> 헤더에 담긴 Access Token 유효한지 검증

- JWT의 페이로드 내부 사용자의 정보를 통해 DB 접근 없이 확인 가능 + 서명 부분을 통해 변조 감지 가능

## Access Token 저장 위치

### 로컬 스토리지 (Local Storage)

- 클라이언트 측에 데이터를 영구적으로 저장하는 방법

- 브라우저를 닫아도 유지되고 클라이언트가 명시적으로 삭제하지 않는 한 유지

- 악성 코드가 로컬 스토리지의 저장된 액세스 토큰을 탈취 가능 -> XSS에 취약

> 권장 X

### 세션 스토리지 (Session Storage)

- 브라우저가 닫히면 데이터가 삭제되며, 같은 탭에서만 유효

- 로컬 스토리지보단 안전하지만, 여전히 XSS 공격에 취약

> 권장 X

### 쿠키 (Cookies)

- HTTP의 무상태(Stateless) 특성을 보완하는 데이터

- 모든 클라이언트 요청에 자동적으로 포함 -> 클라이언트에서 쿠키를 별도로 포함시키지 않아도 됨

- 만료 날짜를 지정하는 영구 쿠키와 브라우저가 닫힐 때 사라지는 세션 쿠키가 있으며 주로 후자를 권장

- HttpOnly 플래그를 사용해 JavaScript에서 쿠키에 접근할 수 없도록 설정 -> XSS 공격에 대한 보안을 높임

> 하지만 다양한 XSS 공격에 대해서 완벽하게는 방어 불가능

- Secure 플래그를 사용해 HTTPS 연결에서만 쿠키가 전송되도록 설정 -> 중간자 공격(MITM)으로부터 쿠키 보호 가능

- SameSite 쿠키 설정을 통해 CSRF 공격 방어 가능

> 쿠키 전송 제한: SameSite 속성이 설정된 쿠키는 제3자 웹사이트에서 시작된 요청에 포함되지 않는다.
>
> 이로 인해 공격자가 사용자의 인증 정보를 이용한 요청을 만들 수 없다.
>
> 세션 쿠키 보호: 세션 쿠키에 SameSite 속성을 설정하면, 제3자 웹사이트에서 시작된 요청에 세션 정보가 포함되지 않아 CSRF 공격을 효과적으로 방지 가능
>
> 브라우저 지원: 대부분의 최신 브라우저가 SameSite 속성을 지원하므로, 추가적인 서버 측 구현 없이도 CSRF 보호 제공

### 메모리 (Memory)

- 서버의 메모리에 저장하는 방법

> 예를 들어 JavaScript의 private 변수에 저장 -> 매 요청 Api 호출 시 Access Token 접근이 쉬워짐

- 브라우저의 메모리는 세션 단위로 관리되기 때문에 페이지 이동 시 Access Token 사라짐

- 하지만 SPA(Single Page Application)를 사용해 페이지를 이동하는 것처럼 보여도 실제로는 이동하지 않아 private 변수가 유지

> 라우팅 변경 시 JavaScript 변수 초기화 될 수 있음

- 새로고침을 하게되면 private 변수가 소멸되어 다시 로그인 해야함

- 스크립트를 통해서 메모리 공간을 직접적으로 제어할 수 없으므로 XSS 공격에 의해 쉽게 탈취 불가

> 하지만 이 또한 완벽하게 방어 어려움
>
> 웹 애플리케이션 자체에 XSS 취약점이 있다면 악의적인 스크립트를 통해 탈취 가능성 있음


# Refresh Token

- Access Token을 재발급하기 위한 토큰

- Access Token이 만료되면 클라이언트는 RefreshToken을 통해 서버로 부터 AccessToken을 재발급

- 연속적인 사용자 인증이 가능하며 사용자가 다시 로그인 불필요

- 하지만 만료기간이 Access Token에 비해 길어 탈취당할 경우 해커가 Access Token을 재발급 가능

- **Refresh Token의 보안성이 더 중요**

- Refresh Token 역시 JWT로 구현하며 페이로드 부분의 최소한의 데이터를 가지고 DB에 접근하여 Access Token 재발급

## Refresh Token 저장 위치

- Access Token과 동일한 위치도 가능하며 추가로 DB에서 관리하는 것도 고려

> 로컬 스토리지, 세션 스토리지, HttpOnly 쿠키, 메모리

### 서버 DB (Redis, RDBMS)

- 세션 인증 방식처럼 (쿠키보다) 보안적으로 더 우수한 방식

- 하지만 토큰 방식의 Stateless 특성이 사라짐

## RTR (Refresh Token Rotation)

- Refresh Token이 단 한 번의 재발급만 허용하는 방식

- 한 번의 재발급을 진행할 때 Refresh Token 또한 재발급

- 이때 사용자와 연결된 모든 Refresh Token은 무효화

- 해커가 먼저 재발급 요청을 한 경우를 방지하기 위함

- 즉, 해커든 사용자든 Refresh Token에 대한 Replay Attack이 감지되면 토큰을 모두 무효화시키는 방법으로 보안을 강화하는 방식

![image](https://github.com/user-attachments/assets/9da40f1e-d651-4d59-8d95-b477efce3598)

### 고려할 부분

- AccessToken과 마찬가지로 Self-cotained(자체 데이터 포함)특징으로 DB에 접근하지 않도록 설정 가능: Stateless 유지

- 하지만 AccessToken의 유효성 연장 이외의 정보들을 포함하여 보안적으로 취약점이 생길 수 있음

- 완전한 Stateless 시스템은 아닐지라도 보안을 생각해 DB에 접근하도록 설정

- 클라이언트 측에서 저장할 위치와 서버 측에 저장할 위치를 고민 (HttpOnly 쿠키 & Redis)

-----

# 적용한 방법

토이 프로젝트인 NIHo에 적용하기 위해 적합한 조합을 생각했다.

일단 로컬 스토리지와 세션 스토리지에 저장하는 방법은 XSS 공격에 위험해 사용하지 않을 생각이다.

메모리에 저장하는 방식은 새로고침 시 저장된 정보가 초기화되어 매 번 다시 로그인해야하므로 사용에 불편할 것이라 판단했다.

그래서 사용자가 받은 Access Token과 Refresh Token은 Http-Only 쿠키에 저장하는 방식으로 선택했다.

추가로 Secure 플래그와 SameSite 추가 설정을 통해 보안을 강화하는 방식으로 결정했다.

그리고 서버 측에서 저장하는 Refresh Token은 보안적으로 더 유리한 Redis 서버에 저장을 하는 방식으로 선택했다.

이 방법은 토큰의 특징인 Stateless를 완벽하게 유지하는 방법은 아니지만 In-MemoryDB로 DB에 빠르게 접근이 가능하다.

> Key-Value 방식으로 DB에 빠르게 접근 가능

### 참고

https://olrlobt.tistory.com/98
