# CORS (Cross-Origin Resource Sharing)

CORS는 웹 브라우저에서 **교차 출처 요청(Cross-Origin Request)** 을 허용하기 위한 HTTP 헤더 기반의 메커니즘

기본적으로 브라우저는 보안상의 이유로 **동일 출처 정책(Same-Origin Policy, SOP)** 을 따르며, 다른 출처의 리소스 요청을 차단

CORS는 이러한 제약을 완화하여, 서버가 명시적으로 허용한 경우에만 교차 출처 요청을 허용

---

## CORS의 필요성

1. **동일 출처 정책(SOP)**:  
   - 브라우저는 기본적으로 동일한 프로토콜, 도메인, 포트를 가진 출처에서만 리소스를 요청할 수 있다.
   - 예를 들어, `http://example.com`에서 로드된 웹 페이지는 `http://another.com`의 리소스 요청 불가

2. **문제점**:  
   - SOP는 보안을 강화하지만, API 호출 등 다양한 웹 애플리케이션 요구사항을 충족시키기 어렵다.
   - 예를 들어, 프론트엔드와 백엔드가 서로 다른 도메인을 사용하는 경우 데이터 요청이 차단

3. **해결책**:  
   - CORS를 통해 특정 출처에서의 요청을 허용하도록 서버가 **응답 헤더에 정보를 추가**하면 교차 출처 요청이 가능

---

## CORS 동작 과정

### 1. 기본 동작 (Simple Request)
- 클라이언트가 HTTP 요청을 보낼 때, 브라우저는 **Origin 헤더**에 현재 출처 정보를 포함하여 서버로 전송
- 서버는 응답 헤더에 **Access-Control-Allow-Origin** 필드를 추가하여 허용된 출처를 명시
- 브라우저는 서버의 응답 헤더를 확인하고, 요청이 허용된 출처인지 판단

#### 기본 동작 흐름:
1. 클라이언트 → 서버: HTTP 요청 전송 (Origin 헤더 포함)
2. 서버 → 클라이언트: HTTP 응답 전송 (Access-Control-Allow-Origin 헤더 포함)
3. 브라우저:
   - Origin과 Access-Control-Allow-Origin 비교
   - 일치하면 리소스 접근 허용, 일치하지 않으면 CORS 에러 발생

---

### 2. Preflight Request (예비 요청)
- 복잡한 요청(예: `PUT`, `DELETE` 메서드 사용 또는 커스텀 헤더 포함)의 경우, 브라우저는 실제 요청 전에 **OPTIONS 메서드**로 서버에 사전 요청(Preflight)을 전송
- 서버가 해당 요청을 허용하면 실제 요청 진행

#### Preflight 동작 흐름:
1. 클라이언트 → 서버: OPTIONS 메서드로 사전 요청 전송
   - **Access-Control-Request-Method**: 실제로 사용할 HTTP 메서드
   - **Access-Control-Request-Headers**: 실제로 사용할 커스텀 헤더
2. 서버 → 클라이언트: OPTIONS 응답 전송
   - **Access-Control-Allow-Methods**: 허용된 메서드 목록
   - **Access-Control-Allow-Headers**: 허용된 헤더 목록
3. 클라이언트:
   - 응답 내용을 확인하고 조건이 충족되면 실제 요청 전송

---

## 주요 CORS 관련 HTTP 헤더

### 1. 클라이언트에서 전송하는 헤더
- **Origin**:
  - 현재 클라이언트의 출처 정보
  - 예: `Origin: http://example.com`

### 2. 서버에서 반환하는 헤더
- **Access-Control-Allow-Origin**:
  - 허용된 출처
  - 예: `Access-Control-Allow-Origin: http://example.com`
  - 모든 출처를 허용하려면 `*` 사용 가능 (권장되지 않음)

- **Access-Control-Allow-Methods**:
  - 허용된 HTTP 메서드
  - 예: `Access-Control-Allow-Methods: GET, POST`

- **Access-Control-Allow-Headers**:
  - 허용된 커스텀 헤더
  - 예: `Access-Control-Allow-Headers: Content-Type`

- **Access-Control-Max-Age**:
  - Preflight 요청 결과를 캐싱할 시간(초)을 설정
  - 예: `Access-Control-Max-Age: 3600`

---

## CORS 동작 예시

### Simple Request 예시
#### 클라이언트 → 서버 (HTTP Request):
```
GET /data.json HTTP/1.1
Host: api.example.com
Origin: http://frontend.example.com
```

#### 서버 → 클라이언트 (HTTP Response):
```
HTTP/1.1 200 OK
Content-Type: application/json
Access-Control-Allow-Origin: http://frontend.example.com
```

### Preflight Request 예시
#### 클라이언트 → 서버 (OPTIONS Request):
```
OPTIONS /data.json HTTP/1.1
Host: api.example.com
Origin: http://frontend.example.com
Access-Control-Request-Method: PUT
Access-Control-Request-Headers: Content-Type
```

#### 서버 → 클라이언트 (OPTIONS Response):
```
HTTP/1.1 204 No Content
Access-Control-Allow-Origin: http://frontend.example.com
Access-Control-Allow-Methods: GET, POST, PUT, DELETE
Access-Control-Allow-Headers: Content-Type
```

---

CORS는 보안상의 이유로 교차 출처 HTTP 요청을 제한하는 브라우저 정책(SOP)을 완화하기 위한 메커니즘

서버가 적절한 CORS 헤더를 제공하면 특정 출처 간의 데이터 공유 가능
