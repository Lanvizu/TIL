인프런 강의 '스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술'을 공부하며 정리한 내용입니다.

[스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81-mvc-1)

-----

# 스프링 MVC 전체 구조

### 이전에 만든 MVC 프레임워크 구조

![image](https://github.com/user-attachments/assets/5f2f04fd-80d3-4fe8-ab08-caa2b53c874d)

### SpringMVC 구조

![image](https://github.com/user-attachments/assets/ea6c2b2e-c779-4cdf-af0d-ef66b38899b3)

### 만든 MVC 프레임워크 -> 스프링 MVC 비교

- FrontController -> DispatcherServlet

- handlerMappingMap -> HandlerMapping

- MyHandlerAdapter -> HandlerAdapter

- ModelView -> ModelAndView

- viewResolver -> ViewResolver

- MyView -> View

## DispatcherServlet 구조

- 스프링 MVC도 프론트 컨트롤러 패턴으로 구현 : DispatcherServlet

- 스프링 MVC의 핵심

### DispatcherServlet 서블릿 등록

- 부모 클래스에서 HttpServlet를 상속 받아 사용하며 서블릿으로 동작한다.

    - DispatcherServlet FrameworkServlet HttpServletBean HttpServlet

- 스프링 부트는 DispatcherServlet 을 서블릿으로 자동으로 등록하면서 모든 경로( urlPatterns="/" )에
대해 매핑한다.

    > 더 자세한 경로가 우선순위가 높으므로 기존에 등록한 서블릿도 함께 동작한다.

### 요청 흐름

- 서블릿이 호출되면 HttpServlet이 제공하는 service()가 호출

- 스프링 MVC는 부모인 FrameServlet의 service()를 오버라이드

- FrameworkServlet.service() -> DispatcherServlet.doDispatch()

### 스프링 MVC 동작 순서

1. **핸들러 조회**: 핸들러 매핑을 통해 요청 URL에 매칭된 핸들러(컨트롤러)를 조회

2. **핸들러 어댑터 조회**: 핸들러를 실행할 수 있는 핸들러 어댑터를 조회

3. **핸들러 어댑터 실행**: 해당 핸들러 어댑터를 실행

4. **핸들러 실행**: 핸들러 어댑터가 실제 핸들러를 실행

5. **ModelAndView 반환**: 핸들러가 반환한 정보를 핸들러 어댑터가 ModelAndView로 변환해 반환

6. **viewResolver 호출**: 뷰 리졸버를 찾아 실행

7. **View 반환**: 뷰 리졸버는 뷰의 논리 이름을 물리 이름으로 수정 후, 렌더링 역할을 담당하는 뷰 객체로 반환

8. **뷰 렌더링**: 뷰를 통해 뷰를 렌더링

> 렌더링: 웹 페이지를 화면에 표현하는 단계

 ### 인터페이스 살펴보기

- DispatcherServlet 코드의 변경 없이 원하는 기능을 변경, 확장이 가능한 점은 스프링 MVC의 큰 강점

### 정리

- 코드 분량도 많고 복잡해 내부 구조를 전부 파악하는 것은 어려움

- 하지만 핵심 동작방식을 알아두는 것이 향후 문제가 발생 시 보다 쉽게 파악하고 해결 가능

-----

# 핸들러 매핑과 핸들러 어댑터

### 동작 방식

1. 핸들러 매핑으로 핸들러 조회

    1. HandlerMapping을 순서대로 실행해 핸들러를 찾는다.
    
    2. 이 경우 빈 이름으로 핸들러를 찾아야 하기 때문에 이름 그대로 빈 이름으로 핸들러를 찾아주는 BeanNameUrlHandlerMapping 가 실행에 성공하고 핸들러인 MyHttpRequestHandler 를 반환한다.

2. 핸들러 어댑터 조회

    1. HandlerAdapter 의 supports() 를 순서대로 호출한다.

    2. HttpRequestHandlerAdapter 가 HttpRequestHandler 인터페이스를 지원하므로 대상이 된다.

3. 핸들러 어댑터 실행

    1. 디스패처 서블릿이 조회한 HttpRequestHandlerAdapter 를 실행하면서 핸들러 정보도 함께 넘겨준다.

    2. HttpRequestHandlerAdapter 는 핸들러인 MyHttpRequestHandler 를 내부에서 실행하고, 그 결과를
반환한다.
