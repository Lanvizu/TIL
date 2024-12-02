# Builder Pattern

- 복잡한 생성 과정과 표현 방법을 분리하여 다양한 구성의 인스턴스를 만드는 생성 패턴

- 생성자에 들어갈 매개 변수를 메서드로 하나하나 받아들인 후 마지막에 통합으로 빌드해 객체를 생성

```JAVA

public static void main(String[] args) {

    // 생성자 방식
    Hamburger hamburger = new Hamburger(2, 3, 0, 3, 0, 0);

    // 빌더 방식
    Hamburger hamburger = new Hamburger.Builder(10)
        .bun(2)
        .patty(3)
        .lettuce(3)
        .build();
}
```

## 빌더 패턴의 장점

1. 객체 생성 과정을 일관된 프로세스로 표현

- 생성자 방식을 통해 객체를 생성하게될 경우 매개변수가 많아질수록 가독성이 급격하게 떨어진다.

- 각 인자의 순서에 따라 해당 값이 어떤 값인지 분류해야되기 때문이다.

- 반면 빌더 패턴을 적용하면 직관적으로 어떤 데이터에 어떠한 값이 설정되는지 확인할 수 있다.

- 특히 연속된 동일 타입의 매개 변수를 많이 설정할 경우에 발생할 수 있는 설정 오류와 같은 실수 방지도 가능하다.

2. 디폴트 매개변수 생략을 간접적으로 지원

- 파이썬, 자바스크립트와 달리 자바에서 기본적으로 메서드에 대한 디폴트 매개변수를 지원하지 않는다.

- 빌더패턴에서는 디폴트 매개변수가 설정된 필드를 설정하는 메서드를 호출하지 않는 방식을 통해 매개변수를 생략할 수 있다.

3. 필수 멤버와 선택적 멤버 분리 가능

- 초기화가 필수인 멤버 변수와 선택적인 멤버 변수가 존재한다.

- 초기화가 필수인 멤버 변수는 빌더의 생성자로 받게 하여 필수 멤버를 설정해야 빌더 객체가 생성되도록 유도한다.

4. 객체 생성 단계 지연 가능

- 단계별로 구성하거나 구성 단계를 지연해 재귀적으로 생성을 처리 가능하다.

- 즉, 객체 생성을 주도적으로 지연 가능하다.

5. 멤버에 대한 변경 가능성 최소화를 추구

- 멤버에 값을 할당할 때 Setter 메서드를 사용하면 매우 좋지 않은 방법

    - 캡슐화 위반 / 불명확한 의도 / 객체 일관성 유지 어려움 / 책임 분산

- 즉, 가능한 변경 가능성을 최소화하여 가능한 불변 객체로 구성해야한다.

- 빌더 패턴은 생성자 없이 어느 객체에 대한 변경 가능성을 최소화를 추구해 불변성을 갖게 해준다.

## 빌더 패턴의 단점

1. 코드 복잡성 증가

- N개의 클래스에 대해 N개의 새로운 빌더 클래스를 생성해야 해 관리해야할 클래스가 많아지고 구조가 복잡해진다.

2. 생성자 보다 성능이 떨어진다.

- 매번 메서드를 호출하여 빌더를 거쳐 인스턴스화를 진행하므로 성능이 떨어진다.

3. 지나친 빌더 남용은 금지

- 클래스의 필드의 개수가 4개보다 적고, 필드의 변경 가능성이 없는 경우라면 생성자나 정적 팩토리 메소드를 이용하는 것이 더 적합할 수 있다.

-----

## Lombok의 @Builder

- Lombok에서 별도의 어노테이션을 지원

- 클래스에 @Builder 어노테이션을 붙여주면 클래스 컴파일 시 자동으로 클래스 내부에 빌더 API가 생성

### SignUpDto

```JAVA
package com.study.NIHo.service.dto.user.req;

import com.study.NIHo.domain.user.User;
import lombok.Builder;
import lombok.Getter;

@Getter
public class SignUpDto {

    private final String username;
    private final String email;
    private final String password;
    private final String confirmPassword;

    @Builder
    public SignUpDto(String username, String email, String password, String confirmPassword) {
        this.username = username;
        this.email = email;
        this.password = password;
        this.confirmPassword = confirmPassword;
    }

    public User ofEntity() {
        return User.builder()
                .username(this.getUsername())
                .email(this.getEmail())
                .password(this.getPassword())
                .build();
    }
}
```


