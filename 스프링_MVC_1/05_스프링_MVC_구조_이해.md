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

    2. HttpRequestHandlerAdapter 는 핸들러인 MyHttpRequestHandler 를 내부에서 실행하고, 그 결과를 반환한다.

### 정리

- HandlerMapping = BeanNameUrlHandlerMapping

- HandlerAdapter = HttpRequestHandlerAdapter

-----

# 뷰 리졸버

1. 핸들러 어댑터 호출

- 핸들러 어댑터를 통해 new-form 이라는 논리 뷰 이름을 획득한다.

2. ViewResolver 호출

- new-form 이라는 뷰 이름으로 viewResolver를 순서대로 호출한다.

- BeanNameViewResolver 는 new-form 이라는 이름의 스프링 빈으로 등록된 뷰를 찾아야 하는데 없다.

- InternalResourceViewResolver 가 호출된다.

3. InternalResourceViewResolver

- 이 뷰 리졸버는 InternalResourceView 를 반환한다.

4. 뷰 - InternalResourceView

- InternalResourceView 는 JSP처럼 포워드 forward() 를 호출해서 처리할 수 있는 경우에 사용한다.

5. view.render()

- view.render() 가 호출되고 InternalResourceView 는 forward() 를 사용해서 JSP를 실행한다.

> Thymeleaf 뷰 템플릿을 사용하면 ThymeleafViewResolver를 등록해야함. -> 스프링부트가 작업 자동화

-----

# 스프링 MVC

### @RequestMapping

- RequestMappingHandlerMapping

- RequestMappingHandlerAdapter

- 가장 우선순위가 높은 핸들러 매핑으로 애노테이션 기반의 컨트롤러를 지원하는 핸들러 매핑과 어댑터

**@Controller**

- 스프링이 자동으로 스프링 빈으로 등록한다. (내부에 @Component 애노테이션이 존재해 컴포넌트 스캔의 대상)

**@RequestMapping**

- 요청 정보를 매핑하며 해당 URL이 호출되면 이 메서드가 호출된다.

- 애노테이션을 기반으로 동작하므로, 메서드의 이름을 임의로 지으면 안된다.

**ModelAndView**

모델과 뷰 정보를 담아 반환한다.

# 스프링 MVC - 실용적인 방식

```JAVA
package hello.servlet.web.springmvc.v3;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import java.util.List;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
@RequestMapping("/springmvc/v3/members")
public class SpringMemberControllerV3 {

    private MemberRepository memberRepository = MemberRepository.getInstance();

    @GetMapping("/new-form")
    public String newForm() {
        return ("new-form");
    }

    @PostMapping("/save")
    public String save(@RequestParam("username") String username,
                             @RequestParam("age") int age,
                             Model model) {

        Member member = new Member(username, age);
        memberRepository.save(member);

        model.addAttribute("member", member);
        return "save-result";
    }

    @GetMapping
    public String members(Model model) {
        List<Member> members = memberRepository.findAll();

        model.addAttribute("members", members);
        return "members";
    }
}

```

### Model 파라미터

- save(), members() 에 Model을 파라미터로 받음

### ViewName 직접 반환

- 뷰의 논리 이름을 반환

### @RequestParam

- HTTP 요청 파라미터를 직접 @RequestParam으로 받음

- @RequestParam("username") 과 request.getParameter("username") 은 거의 같은 코드

- GET 쿼리 파라미터. POST Form 방식 모두 지원

### @RequestMapping -> @GetMapping, @PostMapping

- URL만 매칭하는 것이 아닌 HTTP Method도 함께 구분 가능

- Get, Post, Put, Delete, Patch 모두 애노테이션 준비되어있음
