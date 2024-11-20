인프런 강의 '스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술'을 공부하며 정리한 내용입니다.

[스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81-mvc-1)

-----

# 프론트 컨트롤러 패턴

### 프론트 컨트롤러 도입 전

![image](https://github.com/user-attachments/assets/433857e1-05a4-4fe0-a33f-12ac647c744e)

### 프론트 컨트롤러 도입 후

![image](https://github.com/user-attachments/assets/531bfafa-36ce-45dc-aaa9-65e916a8d730)

### FrontController 패턴 특징

- 프론트 컨트롤러 서블릿 하나로 클라이언트의 요청을 받음

- 요청에 맞는 컨트롤러를 찾아 호출

- 프론트 컨트롤러를 제외한 나머지 컨트롤러는 서블릿을 사용하지 않아도 됨

- 하나의 입구로 공통 처리 가능

### 스프링 웹 MVC와 프론트 컨트롤러

- 스프링 웹 MVC 핵심도 FrontController

- 스프링 웹 MVC의 DispatcherServlet 또한 동일한 FrontController로 구현

# 프론트 컨트롤러 도입 - v1

### 구조

![image](https://github.com/user-attachments/assets/f925a30c-4969-4003-b96d-174f5228e255)

