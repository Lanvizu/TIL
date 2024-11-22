인프런 강의 '스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술'을 공부하며 정리한 내용입니다.

[스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81-mvc-1)

-----

# 스프링 MVC - 기본 기능

> 스프링 부트에 JAR을 사용 시 /resources/static/ 위치에 index.html 파일을 두면 Welcome 페이지로 처리

## 로깅

- 운영 시스템에서는 System.out.println() 같은 시스템 콘솔을 사용해서 필요한 정보를 출력하지 않는다.

- 별도의 라이브러리를 통해 로그를 출력한다.

### 로깅 라이브러리

> 스프링 부트 라이브러리를 사용하면 스프링 부트 로깅 라이브러리가 포함

- SLF4J

- Logback

- SLF4J는 인터페이스고, 그 구현체로 Logback 같은 로그 라이브러리를 선택

> 실무에서 주로 스프링부트의 Logback를 사용

### 로그 선언

- private Logger log = LoggerFactory.getLogger(getClass());

- private static final Logger log = LoggerFactory.getLogger(Xxx.class)

- @Slf4j : 롬복 사용 가능

### 로그 호출

- 예) log.info("hello")

```JAVA
package hello.springmvc.basic;

import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@Slf4j
@RestController
public class LogTestController {

    @RequestMapping("/log-test")
    public String logTest() {

        log.trace("trace");
        log.debug("debug");
        log.info("info");
        log.warn("warn");
        log.error("error");

        return "ok";
    }
}

```

- 로그 레벨 설정에 따라 출력 결과가 변함

    - LEVEL : TRACE > DEBUG > INFO > WARN > ERROR

    - 개발 서버는 debug 출력
 
    - 운영 서버는 info 출력

- @Slf4j 로 변경

### 로그 레벨 설정

application.properties

```
#전체 로그 레벨 설정(기본 info)
logging.level.root=info
#hello.springmvc 패키지와 그 하위 로그 레벨 설정
logging.level.hello.springmvc=debug
```

### 올바른 로그 사용법

- log.debug("data={}", data)

> log.debug("data="+data)
>
> - '+'가 실제 실행이 되어 결과적으로 문자 더하기 연산이 발생한다.

### 로그 사용시 장점

- 쓰레드 정보, 클래스 이름 같은 부가 정보를 함께 볼 수 있고, 출력 모양을 조정 가능

- 개발 상황에 맞춰 로그의 출력 여부를 설정 가능

- 시스템 아웃 콘솔뿐 아니라 파일이나 네트워크 등, 로그를 별도의 위치로 남길 수 있음

- System.out보다 성능도 뛰어나 실무에서 꼭 로그를 사용해야 함

> 내부 버퍼링이나, 멀티 쓰레드 등 부분에서 뛰어남

-----

# 요청 매핑
