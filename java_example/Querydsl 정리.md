### QueryDSL이란

QueryDSL은 정적 타입을 이용해서 SQL과 같은 쿼리를 생성할 수 있도록 해 주는 오픈소스 프레임워크이다.

Querydsl을 사용하는 이유는 컴파일 시점에서 알 수 있는 Type-Safe 쿼리를 날리기 위해서이다.

여기서 Type-Safe란 말 그대로 타입에 안정적인 것을 의미한다.

예를 들어, '타입에 불안정적이다' 라고 하는 것은 타입을 판별(Type Check) 하지 못해 Runtime 시점에서 타입으로 인한 문제가 발생하지만

Type-Safe는 반대로 타입을 판별(Type Check)할 수 있어 Runtime이 아닌 컴파일 시점에서 문제를 잡을 수 있다는 것이다.

> **Query** 란 데이터베이스 또는 기타 데이터 소스의 정보 또는 데이터에 대한 요청을 의미한다.
  쿼리는 일반적으로 데이터가 저장되고 관리되는 다양한 유형의 애플리케이션 및 시스템에서 사용된다.

> **DSL** 이란 특정 도메인에서 발생하는 문제를 효과적으로 해결하기 위해 설계된 언어를 말한다.
  예를 들어 데이터베이스 내의 정보를 검색, 삽입, 수정, 삭제하기 위한 SQL,
  웹의 디자인, 레이아웃 및 시각적 스타일링을 정의하기 위한 CSS,
  문자열 내에서 특정 패턴을 검색, 추출, 교체하기 위한 Regex 등이 있다.

----------------

Gradle 설정 - QueryDSL은 JPA 표준이 아니기 때문에 별도로 라이브러리를 추가해줘야한다.

```JAVA
// QueryDSL 5.0이상 부터는 아래의 옵션을 추가해주시면 됩니다.
 plugins {
 	...
 	id "com.ewerk.gradle.plugins.querydsl" version "1.0.10"
 	...
 }
 ...
 dependencies {
 	// querydsl 추가
 	implementation "com.querydsl:querydsl-jpa:5.0.0"
 	implementation "com.querydsl:querydsl-apt:5.0.0"
     ...
 }
 // Qtype 생성 경로
 def querydslDir = "$buildDir/generated/querydsl"
 querydsl {
 	jpa = true
 	querydslSourcesDir = querydslDir
 }
 sourceSets {
 	main.java.srcDir querydslDir
 }
 compileQuerydsl{
 	options.annotationProcessorPath = configurations.querydsl
 }
 configurations {
 	compileOnly {
 		extendsFrom annotationProcessor
 	}
 	querydsl.extendsFrom compileClasspath
 }
```

---------

QueryDSL로 쿼리를 작성할 때, QType을 이용해 쿼리를 Type-Safe 하게 작성할 수 있다.

Qtype 파일을 만들기 위해 Gradle > Tasks > other > complieQuerydsl을 클릭해준다.

#### 장점
  1. 컴파일 시점에 타입 체크가 가능하다.
  2. 동적 쿼리를 직관적으로 확인하기 쉽다.
  3. 메서드 분리를 통한 재사용성 향상된다.
  4. 가독성이 향상되며 메서드 네이밍을 통해 쿼리 조건, 정렬 방식을 유추가 가능하다.

#### 단점
  1. 1차 캐시 장점을 누릴 수 없다(JPQL의 단점이라고 볼 수 있다)
  2. 최신 버전이 존재하지 않는다.

> QueryDSL을 사용하면서 Q타입 에러를 정말 많이 만났는데 버전마다 gradle 설정이 달라서 꽤나 고생을 했다.


----------

참고 블로그

https://velog.io/@uiseok/CS-QueryDSL-with-JPA

https://dev.gmarket.com/33
    
