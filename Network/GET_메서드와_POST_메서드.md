# HTTP GET과 POST 메서드

GET과 POST는 HTTP 프로토콜에서 가장 많이 사용되는 메서드로, 클라이언트가 서버에 요청을 보낼 때 사용하는 방식

두 메서드는 **사용 목적**과 **데이터 전송 방식**에서 차이

---

## GET 메서드

### **개념**
- 서버로부터 데이터를 **조회**하기 위한 메서드
- 주로 리소스를 가져오고, 데이터를 조회하는 데 사용

### **특징**
1. **데이터 전송 방식**:  
   - 요청 데이터는 URL의 쿼리스트링(Query String)에 포함되어 전송
   - 예: `http://example.com/resource?name=value&age=25`

2. **보안**:  
   - URL에 데이터가 노출되므로 보안에 취약
   - 민감한 정보를 전송하기에 적합하지 않음

3. **데이터 크기 제한**:  
   - URL 길이에 제한이 있어 대용량 데이터를 전송하기 어려움

4. **캐싱 가능**:  
   - 브라우저가 GET 요청과 응답을 캐싱할 수 있어, 동일한 요청 시 빠르게 처리
     > 캐싱: 파일이나 데이터의 복사본을 임시 저장소에 저장해두었다가 필요할 때 빠르게 사용하는 기술

5. **멱등성(Idempotent)**:  
   - 동일한 요청을 여러 번 보내더라도 서버의 상태가 변하지 않음
     > 멱등성: 연산을 여러 번 수행해도 결과가 달라지지 않는 성질

### **사용 사례**
- 게시판 글 목록 조회
- 검색 결과 페이지
- 특정 상품 정보 조회

---

## POST 메서드

### **개념**
- 서버에 데이터를 **전송**하거나, 서버의 값을 변경하기 위한 메서드
- 주로 리소스를 생성하거나 업데이트할 때 사용

### **특징**
1. **데이터 전송 방식**:  
   - 요청 데이터는 HTTP 메시지의 Body에 포함되어 전송
   - URL에는 데이터가 포함되지 않음

2. **보안**:  
   - 요청 데이터가 Body에 숨겨져 전송되므로 GET보다 상대적으로 안전 
   - 하지만 암호화되지 않은 상태에서는 여전히 보안 위협 존재

3. **데이터 크기 제한 없음**:  
   - 대용량 데이터를 전송 가능

4. **캐싱 불가능**:  
   - POST 요청은 기본적으로 캐싱되지 않음

5. **멱등성 없음**:  
   - 동일한 요청을 여러 번 보내면 서버 상태 변경 가능성 존재

### **사용 사례**
- 회원가입/로그인
- 게시판 글 작성
- 파일 업로드

---

## GET과 POST 비교

| 특징                  | GET                                      | POST                                     |
|-----------------------|------------------------------------------|------------------------------------------|
| **사용 목적**         | 데이터 조회                              | 데이터 생성/업데이트                     |
| **데이터 위치**       | URL (쿼리스트링)                         | HTTP 메시지 Body                         |
| **보안성**            | 낮음 (URL에 데이터 노출)                 | 높음 (Body에 데이터 숨김)                |
| **데이터 크기 제한**  | 있음 (URL 길이 제한)                     | 없음                                     |
| **캐싱 가능 여부**    | 가능                                     | 불가능                                   |
| **멱등성(Idempotent)**| 있음                                     | 없음                                     |
| **사용 사례**         | 게시글 조회, 검색                       | 회원가입, 파일 업로드                   |

---

- GET은 데이터를 가져오는 데 사용되며, URL에 데이터가 노출되어 보안에 취약하지만 빠르고 캐싱이 가능
  
- POST는 데이터를 서버로 전송하거나 업데이트하는 데 사용되며, 대용량 데이터와 민감한 정보를 다룰 때 적합
