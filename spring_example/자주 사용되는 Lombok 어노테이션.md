## 접근자/설정자 자동 생성
### @Getter, @Setter

- Lombok에서 가장 많이 사용되는 어노테이션으로 자동으로 생성된 접근자와 설정자 메소드를 사용할 수 있어서 매우 편리하다.
- 또한, 필드 레벨이 아닌 클래스 레벨에 @Getter 또는 @Setter를 선언해줄 경우, 모든 필드에 접근자와 설정자가 자동으로 생성된다.

### @Setter 사용을 지양하는 이유

- 사용한 의도를 쉽게 파악하기 어렵다.
  
  ```JAVA
  Member member = new Member();
  member.setUsername("홍길동");
  member.setAge(40);
  ```
  해당 코드가 값을 생성하는지, 값을 변경하는지 의도를 파악하기 어렵다.

이는 엔티티 필드가 많아질수록 파악하기 어려워진다.

- 일관성을 파괴한다.
  
  ```JAVA
  public Member updateMember(Long id) {
       Member member = findById(id);
       member.setUsername("박해수"); // 이름 변경
       member.setAge(70); // 나이 변경
       return member;
  }
  ```
  setter 메서드는 항상 public으로 어디든 접근 가능하기 때문에 의도치 않게 다른곳에서 Member의 값이 변경될 수 있다.

### 해결 방법

**setter 대신 Lombok의 빌더를 사용해서 값을 생성한다.**

```JAVA
@Builder
    public SignupRequest(String email, String password,
                          String passwordCheck, String nickname, List<TagOption> tags) {
        this.email = email;
        this.password = password;
        this.passwordCheck = passwordCheck;
        this.nickname = nickname;
        this.tags = tags;
    }

    public User ofEntity() {
        return User.builder()
                .email(this.getEmail())
                .password(this.getPassword())
                .nickname(this.getNickname())
                .build();
    }
```
이런 방식으로 @Builder 어노테이션을 설정해주고 builder()을 사용하여 값을 생성해주는 방식을 사용한다.

다음과 같이 메서드 이름을 통해서 어떤 기능을 수행하는지 알 수 있도록 해야한다.

-------

## 생성자 자동 생성
### @NoArgsConstructor

- 파라미터가 없는 기본 생성자를 만들어준다.

  ```JAVA
  @NoArgsConstructor
  public class Person {
      private String name;
      private int age;
  // getters and setters
  }
  ```

  ```JAVA
  public class Person {
      private String name;
      private int age;
      
  	public Person(){}
  }
  ```

### @AllArgsConstructor

- 모든 필드 값을 파라미터로 받는 생성자를 만들어준다.

  ```JAVA
  @AllArgsConstructor
  public class Person {
      private String name;
      private int age;
      // getters and setters
  }
  ```
  
  ```JAVA
  public class Person {
      private String name;
      private int age;
  	
      public Person(String name, int age) {
      	this.name = name;
          this.age = age;
      }
  }
  ```

### @RequiredArgsConstructor

- final이나 @NonNull 인 필드 값만 파라미터로 받는 생성자를 만들어준다.

```JAVA
@RequiredArgsConstructor
public class Person {
    private final String name;
    private final int age;
    private String address;
    // getters and setters
}
```
```JAVA
public class Person {
    private final String name;
    private final int age;
    private String address;

	public Person(final String name, final int age) {
    	this.name = name;
        this.age = age;
    }
}
```

---------

## @Data

- @Getter, @Setter, @RequiredArgsConstructor, @ToString, @EqualsAndHashCode 을 모두 설정해주는 어노테이션이다.

- 클래스 레벨에서 @Data 어노테이션을 붙여주면, 모든 필드를 대상으로 접근자와 설정자가 자동으로 생성되고,

- final 또는 @NonNull 필드 값을 파라미터로 받는 생성자가 만들어지며, toString, equals, hashCode 메서드가 자동으로 생성된다.

------
참고 자료

https://www.daleseo.com/lombok-popular-annotations/

https://hstory0208.tistory.com/entry/JPA-Entitiy%EC%97%90%EC%84%9C-%EC%99%9C-Setter%EB%A5%BC-%EC%82%AC%EC%9A%A9%ED%95%98%EC%A7%80-%EB%A7%90%EB%9D%BC%EA%B3%A0-%ED%95%A0%EA%B9%8C
