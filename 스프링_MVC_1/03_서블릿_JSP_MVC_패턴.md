인프런 강의 '스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술'을 공부하며 정리한 내용입니다.

[스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81-mvc-1)

-----

# 서블릿 웹 애플리케이션

> 생성 과정은 생략.

- 서블릿과 자바 만으로 동적인 HTML을 만드는 것은 매우 복잡하고 비효율

- HTML 문서에 동적으로 변경해야 하는 부분만 자바 코드를 넣는 방식을 생각: 템플릿 엔진이 나온 이유

- 예) JSP, Thymeleaf, Freemarker, Velocity 등

> 참고
>
> JSP는 다른 템플릿과의 경쟁에서 밀리는 상황 -> 스프링과 잘 통합되는 Thymeleaf 사용

> 관리자 페이지 200 이 갈색이면 캐시되었다는 의미 -> 업데이트 원할 시 새로고침

# JSP 웹 애플리케이션

> 생성 과정은 생략

- JSP를 사용하여 뷰를 생성하는 HTML 작업을 깔끔하게 처리하며 중간중간 동적으로 변경이 필요한 부분에만 자바를 적용

- JSP가 데이터를 조회하거나 저장하는 다양한 역할을 맡음

-----

# MVC 패턴 - 개요

### 너무 많은 역할

- 하나의 서블릿이나 JSP만을 사용해 비즈니스 로직과 뷰 로직을 구성하게되면 너무 많은 역할을 한다.

- 유지보수가 어려워진다.

### 변경의 라이프 사이클

- 비즈니스 로직과 뷰 로직 사이의 라이프 사이클이 다르다는 점이 큰 문제이다.

- 예를 들어 UI의 일부 수정과 비즈니스 로직을 수정하는 일은 각각 다르게 발생할 가능성이 높고 서로 영향을 주지 않는다.

- 이렇게 변경의 라이프 사이클이 다른 부분을 하나의코드로 관리하는 것은 유지보수하기 좋지 않다.  

### 기능 특화

- JSP 같은 뷰 템플릿은 화면을 렌더링 하는데 최적화 되어 있으므로 해당 업무만 담당하는 것이 가장 효과적이다.

### Model View Controller

- 서블릿이나 JSP로 처리하던 것을 Controller와 View라는 영역으로 서로 나눈 것을 의미

> 웹 애플리케이션은 보통 이 MVC 패턴을 사용

- **컨트롤러**: HTTP 요청을 받아 파라미터를 검증하고, 비즈니스 로직을 실행한다. 이후 뷰에 전달한 결과 데이터를 조회해 모델에 담는다.

- **모델**: 뷰에 출력할 데이터를 담아둔다.

- 뷰에 필요한 데이터를 모두 모델에 담아둠으로써 뷰는 비즈니스 로직이나 데이터 접근을 몰라도 되며, 화면을 렌더링 하는 일에 집중할 수 있다.

- **뷰**: 모델에 담겨있는 데이터를 사용해 화면을 그리는 일에 집중한다. (HTML을 생성하는 부분)

> 참고
>
> 컨트롤러에 비즈니스 로직을 둘 수 있지만, 이렇게 되면 컨트롤러가 너무 많은 역할을 담당한다.
>
> 비즈니스 로직에 서비스(Service) 계층을 별도로 추가해 처리한다.
>
> 컨트롤러는 비즈니스 로직이 있는 서비스를 호출하는 역할을 담당한다.
>
> 추가로 비즈니스 로직을 변경하면 호출하는 컨트롤러의 코드도 변경될 수 있다.

### MVC 패턴 이전

![image](https://github.com/user-attachments/assets/32f12c0c-b039-4760-b535-94f3ffe6a017)

### MVC 패턴 1

![image](https://github.com/user-attachments/assets/3689748b-ee7c-473f-bbc4-407e047df3ca)

### MVC 패턴 2

![image](https://github.com/user-attachments/assets/84a0a5e1-e513-4118-a644-c79533b65e3f)

# MVC 패턴 - 적용

- 서블릿을 컨트롤러로사용하고, JSP를 뷰로 사용해 MVC 패턴 적용

- Model은 HttpServletRequest 객체를 사용

- request 내부에 데이터 저장소를 가지며, request.setAttribute() , request.getAttribute()을 통해 데이터를 보관하고, 조회 가능

### 회원 등록 폼 - 컨트롤러

```JAVA
package hello.servlet.web.servletmvc;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

@WebServlet(name = "mvcMemberSaveServlet", urlPatterns = "/servlet-mvc/members/save")
public class MvcMemberSaveServlet extends HttpServlet {
    private MemberRepository memberRepository = MemberRepository.getInstance();

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String username = request.getParameter("username");
        int age = Integer.parseInt(request.getParameter("age"));
        Member member = new Member(username, age);
        System.out.println("member = " + member);
        memberRepository.save(member);
        //Model에 데이터를 보관한다.
        request.setAttribute("member", member);
        String viewPath = "/WEB-INF/views/save-result.jsp";
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }
}
```

> **/WEB-INF**
>
> 해당 경로안의 JSP는 외부에서 호출이 불가 -> 컨트롤러를 통해 JSP를 호출하도록 유도

> **redirect vs forward**
>
> redirect는 실제 클라이언트(웹 브라우저)에 응답이 나갔다가, 다시 redirect 경로로 요청한다.
>
> 실제로 URL 경로도 변경된다.
>
> forward는 서버 내부에서 일어나는 호출이다.

### 회원 등록 폼 - 뷰

```JSP
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<!-- 상대경로 사용, [현재 URL이 속한 계층 경로 + /save] -->
<form action="save" method="post">
    username: <input type="text" name="username" />
    age: <input type="text" name="age" />
    <button type="submit">전송</button>
</form>
</body>
</html>
```

# MVC 패턴 - 한계

- MVC 패턴으로 컨트롤로와 뷰의 역할이 명확하게 구분된다.

- 하지만 컨트롤러에 중복이 많으며 필요하지 않은 코드들도 많이 보인다.

## MVC 컨트롤러의 단점

- 포워드 중복

- ViewPath에 중복

- 사용하지 않는 코드 (HttpServletResponse response)

> HttpServletRequest, HttpServletResponse을 사용하는 코드는 테스트 케이스 작성하기 어려움

- 공통 처리의 어려움

따라서 컨트롤러의 호출 전 공통 기능을 먼저 처리해야한다. -> **수문장 역할** 기능이 필요하다.

**프론트 컨트롤러(Front Controller) 패턴**을 도입해 문제 해결이 가능하다.
