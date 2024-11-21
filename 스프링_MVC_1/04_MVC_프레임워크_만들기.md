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

-----

# 프론트 컨트롤러 도입 - v1

### 구조

![image](https://github.com/user-attachments/assets/f925a30c-4969-4003-b96d-174f5228e255)

### ControllerV1

```JAVA
package hello.servlet.web.frontcontroller.v1;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public interface ControllerV1 {

    void process(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException;
}
```

- 컨트롤러 인터페이스를 도입한다.

- 각 컨트롤러는 이 인터페이스를 통해 구현한다.
  
- 프론트 컨트롤러는 이 인터페이스를 호출해 구현과 관계없이 로직의 일관성을 가져갈 수 있다.

### MemberFormControllerV1 - 회원 등록 컨트롤러

```JAVA
package hello.servlet.web.frontcontroller.v1.controller;

import hello.servlet.web.frontcontroller.v1.ControllerV1;
import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public class MemberFormControllerV1 implements ControllerV1 {
    @Override
    public void process(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String viewPath = "/WEB-INF/views/new-form.jsp";
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
//        서블릿에서 JSP를 호출
        dispatcher.forward(request, response);
    }
}
```

### MemberSaveControllerV1 - 회원 저장 컨트롤러

```JAVA
package hello.servlet.web.frontcontroller.v1.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.v1.ControllerV1;
import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public class MemberSaveControllerV1 implements ControllerV1 {

    private MemberRepository memberRepository = MemberRepository.getInstance();
    @Override
    public void process(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
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

### MemberListControllerV1 - 회원 목록 컨트롤러

```JAVA
package hello.servlet.web.frontcontroller.v1.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.v1.ControllerV1;
import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;

public class MemberListControllerV1 implements ControllerV1 {
    private MemberRepository memberRepository = MemberRepository.getInstance();
    @Override
    public void process(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("MvcMemberListServlet.service");
        List<Member> members = memberRepository.findAll();
        request.setAttribute("members", members);
        String viewPath = "/WEB-INF/views/members.jsp";
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }

}

```

### FrontControllerServletV1 - 프론트 컨트롤러

``` JAVA
package hello.servlet.web.frontcontroller.v1;

import hello.servlet.web.frontcontroller.v1.controller.MemberFormControllerV1;
import hello.servlet.web.frontcontroller.v1.controller.MemberListControllerV1;
import hello.servlet.web.frontcontroller.v1.controller.MemberSaveControllerV1;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

@WebServlet(name = "frontControllerServletV1", urlPatterns = "/front-controller/v1/*")
public class FrontControllerServletV1 extends HttpServlet {
    private Map<String, ControllerV1> controllerMap = new HashMap<>();

    public FrontControllerServletV1() {
        controllerMap.put("/front-controller/v1/members/new-form", new MemberFormControllerV1());
        controllerMap.put("/front-controller/v1/members/save", new MemberSaveControllerV1());
        controllerMap.put("/front-controller/v1/members", new MemberListControllerV1());
    }

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("FrontControllerServletV1.service");
        String requestURI = request.getRequestURI();
        ControllerV1 controller = controllerMap.get(requestURI);
        if (controller == null) {
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
            return;
        }
        controller.process(request, response);
    }
}
```

**urlPatterns**

- urlPatterns = "/front-controller/v1/*": 해당 경로를 포함한 하위 모든 요청은 이 서블릿에서 받아들인다.

**controllerMap**

- key: 매핑 URL

- value: 호출될 컨트롤러

**service()**

- requestURI를 조회하여 실제 호출할 컨트롤러를 controllerMap에서 찾으며, 없는 경우 404(SC_NOT_FOUNT) 상태 코드 반환

- 컨트롤러를 찾은 후, controller.precess(request, response);를 실행

**JSP**

- 이전 MVC에서 사용한 그대로 사용

-----

# View 분리 - v2

- 모든 컨트롤러에 뷰로 이동하는 부분에 중복이 존재한다.

```JAVA
String viewPath = "/WEB-INF/views/save-result.jsp";
RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
dispatcher.forward(request, response);
```

### 구조

![image](https://github.com/user-attachments/assets/e43e17cc-2995-45d8-8ef2-45fce7e3ea6a)

### MyView

```JAVA
package hello.servlet.web.frontcontroller;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public class Myview {
    private String viewPath;

    public Myview(String viewPath) {
        this.viewPath = viewPath;
    }

    public void render(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }
}
```

### ControllerV2

```JAVA
package hello.servlet.web.frontcontroller;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public class Myview {
    private String viewPath;

    public Myview(String viewPath) {
        this.viewPath = viewPath;
    }

    public void render(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }
}
```

### MemberFormControllerV2 - 회원 등록 폼

```JAVA
package hello.servlet.web.frontcontroller.v2.controller;

import hello.servlet.web.frontcontroller.Myview;
import hello.servlet.web.frontcontroller.v2.ControllerV2;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public class MemberFormControllerV2 implements ControllerV2 {

    @Override
    public Myview process(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        return new Myview("/WEB-INF/views/new-form.jsp");
    }
}
```

- 이 컨트롤러처럼 중복되는 부분을 모두 MyView 객체를 생성하고 뷰이름을 넣어 반환한다.

### 프론트 컨트롤러 V2

```JAVA
package hello.servlet.web.frontcontroller.v2;

import hello.servlet.web.frontcontroller.Myview;
import hello.servlet.web.frontcontroller.v2.controller.MemberFormControllerV2;
import hello.servlet.web.frontcontroller.v2.controller.MemberListControllerV2;
import hello.servlet.web.frontcontroller.v2.controller.MemberSaveControllerV2;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

@WebServlet(name = "frontControllerServletV2", urlPatterns = "/front-controller/v2/*")
public class FrontControllerServletV2 extends HttpServlet {
    private Map<String, ControllerV2> controllerMap = new HashMap<>();

    public FrontControllerServletV2() {
        controllerMap.put("/front-controller/v2/members/new-form", new MemberFormControllerV2());
        controllerMap.put("/front-controller/v2/members/save", new MemberSaveControllerV2());
        controllerMap.put("/front-controller/v2/members", new MemberListControllerV2());
    }

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String requestURI = request.getRequestURI();

        ControllerV2 controller = controllerMap.get(requestURI);
        if (controller == null) {
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
            return;
        }
        Myview view = controller.process(request, response);
        view.render(request, response);
    }
}
```
-----

# Model 추가 - v3

### 서블릿 종속성 제거

- 컨트롤러 입장에서 HttpServletRequest, HttpServletResponse 이 필요하지 않은 상황

- 요청 파라미터 정보를 Map으로 처리하여 넘겨 컨트롤러가 서블릿 기술을 몰라도 동작 가능하도록 구성

- 이후 request 객체를 Model로 사용하는 대신 별도의 Model 객체를 만들어 반환

- 즉, 컨트롤러가 서블릿 기술을 전혀 사용하지 않도록 변경

- 구현 코드가 매우 단순해지고, 테스트 코드 작성이 쉽다.

> 다만, 테스트 코드의 단순함이 목적이 되어서는 안된다.

### 뷰 이름 중복 제거

- 이전 v2 컨트롤러 코드에서 뷰 이름에 중복을 처리

- 컨트롤러는 뷰의 논리적인 이름만 반환하고, 실제 물리 위치의 이름은 프론트 컨트롤러에서 처리하도록 단순화

- 나중에 뷰의 폴더 위치가 변경해도 프론트 컨트롤러만 수정하면 된다.

> /WEB-INF/views/new-form.jsp new-form
>
> /WEB-INF/views/save-result.jsp save-result
>
> /WEB-INF/views/members.jsp members

### 구조

![image](https://github.com/user-attachments/assets/b3bec588-caa5-4d3d-b16c-1ba542864981)

### ModelView

- v2 까지는 서블릿에 종속적인 HttpServletRequest를 사용했다.

- Model 역시 request.setAttribute()를 통해 데이터를 저장하고 뷰에 전달했다.

- 이러한 종속성을 제거하기 위해 Model을 직접 생성하고, View 이름까지 전달하는 객체를 생성하자.

```JAVA
package hello.servlet.web.frontcontroller;

import java.util.HashMap;
import java.util.Map;

public class ModelView {
    private String viewName;
    private Map<String, Object> model = new HashMap<>();

    public ModelView(String viewName) {
        this.viewName = viewName;
    }

    public String getViewName() {
        return viewName;
    }

    public void setViewName(String viewName) {
        this.viewName = viewName;
    }

    public Map<String, Object> getModel() {
        return model;
    }

    public void setModel(Map<String, Object> model) {
        this.model = model;
    }
}
```

- 단순한 Map으로 구성하며 뷰에 필요한 데이터는 key, value로 넣어준다.

### ControllerV3

```JAVA
package hello.servlet.web.frontcontroller.v3;

import hello.servlet.web.frontcontroller.ModelView;
import java.util.Map;

public interface ControllerV3 {
    ModelView process(Map<String, String> paramMap);
}
```

- 서블릿 기술이 전혀 사용되지 않는다.

- 구현이 단순해지며 테스트 코드 작성이 쉬워졌다.

- HttpServletRequest가 제공하는 파라미터는 프론트 컨트롤러가 paramMap에 담아서 호출해주면 된다.

- 응답 결과로 뷰 이름과 뷰에 전달한 Model 데이터를 포함하는 ModelView 객체를 반환한다.

### MemberFormControllerV3 - 회원 등록 폼

```JAVA
package hello.servlet.web.frontcontroller.v3.controller;

import hello.servlet.web.frontcontroller.ModelView;
import hello.servlet.web.frontcontroller.v3.ControllerV3;
import java.util.Map;

public class MemberFormControllerV3 implements ControllerV3 {
    @Override
    public ModelView process(Map<String, String> paramMap) {
        return new ModelView("new-form");
    }
}
```

- "new-from"이라는 view의 논리적인 이름을 지정한다.

### MemberSaveControllerV3 - 회원 저장

```JAVA
package hello.servlet.web.frontcontroller.v3.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.ModelView;
import hello.servlet.web.frontcontroller.v3.ControllerV3;
import java.util.Map;

public class MemberSaveControllerV3 implements ControllerV3 {

    private MemberRepository memberRepository = MemberRepository.getInstance();

    @Override
    public ModelView process(Map<String, String> paramMap) {
        String username = paramMap.get("username");
        int age = Integer.parseInt(paramMap.get("age"));

        Member member = new Member(username, age);
        memberRepository.save(member);

        ModelView mv = new ModelView("save-result");
        mv.getModel().put("member", member);
        return mv;
    }
}
```

- paramMap에 담겨있는 'username'과 'age'를 꺼내 사용하면 된다.

- ModelView 안에 model은 단순한 map 이므로 member 객체를 담고 반환한다.

### MemberListControllerV3 - 회원 목록

```JAVA
package hello.servlet.web.frontcontroller.v3.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.ModelView;
import hello.servlet.web.frontcontroller.v3.ControllerV3;
import java.util.List;
import java.util.Map;

public class MemberListControllerV3 implements ControllerV3 {

    private MemberRepository memberRepository = MemberRepository.getInstance();
    @Override
    public ModelView process(Map<String, String> paramMap) {
        List<Member> members = memberRepository.findAll();
        ModelView mv = new ModelView("members");
        mv.getModel().put("members", members);
        return mv;
    }
}
```

### FrontControllerServletV3

```JAVA
package hello.servlet.web.frontcontroller.v3;

import hello.servlet.web.frontcontroller.ModelView;
import hello.servlet.web.frontcontroller.Myview;
import hello.servlet.web.frontcontroller.v3.controller.MemberFormControllerV3;
import hello.servlet.web.frontcontroller.v3.controller.MemberListControllerV3;
import hello.servlet.web.frontcontroller.v3.controller.MemberSaveControllerV3;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

@WebServlet(name = "frontControllerServletV3", urlPatterns = "/front-controller/v3/*")
public class FrontControllerServletV3 extends HttpServlet {
    private Map<String, ControllerV3> controllerMap = new HashMap<>();

    public FrontControllerServletV3() {
        controllerMap.put("/front-controller/v3/members/new-form", new MemberFormControllerV3());
        controllerMap.put("/front-controller/v3/members/save", new MemberSaveControllerV3());
        controllerMap.put("/front-controller/v3/members", new MemberListControllerV3());
    }

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String requestURI = request.getRequestURI();

        ControllerV3 controller = controllerMap.get(requestURI);
        if (controller == null) {
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
            return;
        }

        //paramMap
        Map<String, String> paramMap = createParamMap(request);
        ModelView mv = controller.process(paramMap);
        String viewName = mv.getViewName(); // 논리 이름 new-form

//        /WEB-INF/views/new-form.jsp
        Myview view = viewResolver(viewName);

        view.render(mv.getModel(),request,response);

    }

    private static Myview viewResolver(String viewName) {
        return new Myview("/WEB-INF/views/" + viewName + ".jsp");
    }

    private static Map<String, String> createParamMap(HttpServletRequest request) {
        Map<String, String> paramMap = new HashMap<>();
        request.getParameterNames().asIterator()
                .forEachRemaining(paramName -> paramMap.put(paramName, request.getParameter(paramName)));
        return paramMap;
    }
}
```

- 입력이 들어온 requestURI로 controller을 불러온다.

- 입력된 request를 paramMap으로 가공한다.

- 가공된 paramMap을 controller에 넣어 ModelView을 반환받는다. (이 떄 controller 내부의 process 처리)

- 반환받은 ModelView에서 viewName을 추출한다.

- veiwResolver에 viewName을 넣어 MyView을 생성한다.

- MyView의 render를 통해 forward를 진행한다.

### ViewResolver

- 컨트롤러가 반환한 논리 뷰 이름을 실제 물리 뷰 경로로 변경한다.

- 실제 물리 경로가 있는 MyView 객체를 반환한다.

    - 논리 뷰 이름: members

    - 물리 뷰 경로: /WEB-INF/views/members.jsp

**view.render(mv.getModel(), request, response)**

- 뷰 객체를 통해서 HTML 화면을 렌더링 한다.

- 뷰 객체의 render() 는 모델 정보도 함께 받는다.

- JSP는 request.getAttribute() 로 데이터를 조회하기 때문에, 모델의 데이터를 꺼내서 request.setAttribute() 로 담아둔다.

- JSP로 포워드 해서 JSP를 렌더링 한다.

### MyView

```JAVA
package hello.servlet.web.frontcontroller;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Map;

public class Myview {
    private String viewPath;

    public Myview(String viewPath) {
        this.viewPath = viewPath;
    }

    public void render(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }

    public void render(Map<String, Object> model, HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        modelToRequestAttribute(model, request);
        RequestDispatcher dispatcher = request.getRequestDispatcher(viewPath);
        dispatcher.forward(request, response);
    }

    private static void modelToRequestAttribute(Map<String, Object> model, HttpServletRequest request) {
        model.forEach(request::setAttribute);
    }
}

```

-----

# 단순하고 실용적인 컨트롤러 - v4

### 구조

![image](https://github.com/user-attachments/assets/7957edea-93c7-4800-935d-94b3a486c8ae)

- 기본 구조는 이전 v3와 동일하지만 컨트롤러가 ViewName을 반환하도록 수정

- v3는 컨트롤러에서 모델 뷰를 만들어서 반환했지만 v4는 프론트 컨트롤러에서 모델 뷰를 만들어 컨트롤러로 넘어감

### ControllerV4

```JAVA
package hello.servlet.web.frontcontroller.v4;

import java.util.Map;

public interface ControllerV4 {
    /**
     * @param paramMap
     * @param model
     * @return viewName
     */
    String process(Map<String, String> paramMap, Map<String, Object> model);

}
```

- 따라서 Controller의 인터페이스의 process는 model이라는 빈 객체를 입력받는다.

### MemberFormControllerV4

```JAVA
package hello.servlet.web.frontcontroller.v4.controller;

import hello.servlet.web.frontcontroller.v4.ControllerV4;
import java.util.Map;

public class MemberFormControllerV4 implements ControllerV4 {

    @Override
    public String process(Map<String, String> paramMap, Map<String, Object> model) {
            return "new-form";
    }
}
```

### MemberSaveControllerV4

```JAVA
package hello.servlet.web.frontcontroller.v4.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.v4.ControllerV4;
import java.util.Map;

public class MemberSaveControllerV4 implements ControllerV4 {

    private MemberRepository memberRepository = MemberRepository.getInstance();

    @Override
    public String process(Map<String, String> paramMap, Map<String, Object> model) {
        String username = paramMap.get("username");
        int age = Integer.parseInt(paramMap.get("age"));

        Member member = new Member(username, age);
        memberRepository.save(member);

        model.put("member", member);
        return "save-result";
    }
}
```

### MemberListControllerV4

```JAVA
package hello.servlet.web.frontcontroller.v4.controller;

import hello.servlet.domain.member.Member;
import hello.servlet.domain.member.MemberRepository;
import hello.servlet.web.frontcontroller.v4.ControllerV4;
import java.util.List;
import java.util.Map;

public class MemberListControllerV4 implements ControllerV4 {

    private MemberRepository memberRepository = MemberRepository.getInstance();
    @Override
    public String process(Map<String, String> paramMap, Map<String, Object> model) {
        List<Member> members = memberRepository.findAll();
        model.put("members", members);
        return "members";
    }
}
```

- 입력받은 model에 생성한 객체를 넣어주고 논리 주소만 반환해준다.

### FrontControllerServletV4

```JAVA
package hello.servlet.web.frontcontroller.v4;

import hello.servlet.web.frontcontroller.MyView;
import hello.servlet.web.frontcontroller.v4.controller.MemberFormControllerV4;
import hello.servlet.web.frontcontroller.v4.controller.MemberListControllerV4;
import hello.servlet.web.frontcontroller.v4.controller.MemberSaveControllerV4;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

@WebServlet(name = "frontControllerServletV4", urlPatterns = "/front-controller/v4/*")
public class FrontControllerServletV4 extends HttpServlet {
    private Map<String, ControllerV4> controllerMap = new HashMap<>();

    public FrontControllerServletV4() {
        controllerMap.put("/front-controller/v4/members/new-form", new MemberFormControllerV4());
        controllerMap.put("/front-controller/v4/members/save", new MemberSaveControllerV4());
        controllerMap.put("/front-controller/v4/members", new MemberListControllerV4());
    }

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String requestURI = request.getRequestURI();

        ControllerV4 controller = controllerMap.get(requestURI);
        if (controller == null) {
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
            return;
        }

        //paramMap
        Map<String, String> paramMap = createParamMap(request);
        Map<String, Object> model = new HashMap<>(); // 추가
        String viewName = controller.process(paramMap, model);

//        /WEB-INF/views/new-form.jsp
        MyView view = viewResolver(viewName);
        view.render(model,request,response);
    }

    private static MyView viewResolver(String viewName) {
        return new MyView("/WEB-INF/views/" + viewName + ".jsp");
    }

    private static Map<String, String> createParamMap(HttpServletRequest request) {
        Map<String, String> paramMap = new HashMap<>();
        request.getParameterNames().asIterator()
                .forEachRemaining(paramName -> paramMap.put(paramName, request.getParameter(paramName)));
        return paramMap;
    }
}
```

- 모델 객체 전달을 위해 model 객체를 생성해 넘겨준다.

- 뷰의 논리 이름을 직접 반환하여 viewResolver을 통해 실제 물리 뷰를 찾는다.

### 정리

- 이전 v3 버전에 뷰의 논리 이름을 반환한다는 작은 변화를 통해 컨트롤러를 구현하는 개발자 입장에서 더욱 편리해졌다.

> 프레임워크나 공통 기능이 수고로워야 사용하는 개발자가 편리해진다.

-----

# 유연한 컨트롤러 - v5

- 현재까지 프론트 컨트롤러는 한가지의 컨트롤러 인터페이스만을 사용

- 개발자에 따라 적용하고 싶은 컨트롤러가 다를 수 있다. -> 이런 경우를 해결하기위해 어댑터 패턴이 추가

### 어댑터 패턴

- 프론트 컨트롤러가 다양한 방식의 컨트롤러를 처리할 수 있도록 변경

### 구조

![image](https://github.com/user-attachments/assets/51efc0e4-4919-4bd6-823f-6ed9c818851b)

- 핸들러 어댑터: 어댑터하는 역할로 다양한 컨트롤러를 호출 가능

- 핸들러: 기존 컨트롤러의 이름을 더 넓은 범위인 핸들러로 변경

    - 컨트롤러의 개념 뿐만 아니라 다른 종류도 어댑터만 있다면 처리 가능하므로

### MyHandlerAdapter

```JAVA
package hello.servlet.web.frontcontroller.v5;

import hello.servlet.web.frontcontroller.ModelView;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;

public interface MyHandlerAdapter {

    boolean supports(Object handler);
    ModelView handle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws ServletException, IOException;
}
```

- boolean supports(Object handler)

    - 어댑터가 해당 컨트롤러 처리 여부 판단 메서드
 
- ModelView handle(HttpServletRequest request, HttpServletResponse response, Object handler)

    - 어댑터를 통해 실제 컨트롤러를 호출하며 결과로 ModelView를 반환
 
    - ModelView를 반환하지 못할 경우, 어댑터가 직접 생성해서라도 반환해야함 (v4는 viewName을 반환하므로 ModelView로 생성하여 반환)
 
    - 프론트 컨트롤러가 호출했던 실제 컨트롤러를 어댑터를 통해 호출
