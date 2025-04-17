# Java의 final 키워드

`final` 키워드는 Java에서 **변경을 제한**하기 위해 사용되는 제어자

변수, 메서드, 클래스 등 다양한 요소에 적용할 수 있으며, 적용 대상에 따라 의미가 다름

---

## 1. 변수에 final 사용

### **기본(Primitive) 타입 변수**

- 한 번 값이 할당되면 **변경이 불가능**
  
- 상수처럼 동작하며, 재할당 시 컴파일 에러가 발생

``` JAVA
final int MAX = 100;
MAX = 200; // 컴파일 에러
```

### **참조(Reference) 타입 변수**
- **참조값(주소)** 만 변경이 불가능
  
- 즉, 한 번 객체를 참조하면 다른 객체로 변경 불가
  
- 단, 참조하는 객체의 **내부 상태(필드 값)** 는 변경 가능
  
```JAVA
final List names = new ArrayList<>();
names.add("Tom"); // 가능: 내부 값 변경
names = new ArrayList<>(); // 불가능: 참조값 변경
```

### **지역 변수/파라미터**
- 메서드 내 지역 변수나 파라미터에 `final`을 붙이면, 해당 변수는 **한 번만 할당** 가능

```JAVA
public void print(final String msg) {
    msg = "Hello"; // 컴파일 에러
}
```

---

## 2. 메서드에 final 사용

- `final`로 선언된 메서드는 **오버라이딩(재정의)**이 불가능
  
- 상속받은 자식 클래스에서 해당 메서드의 동작을 변경 불가
  
- 주로 **핵심 로직 보호**나 **보안상 변경 금지**가 필요한 경우 사용
  
```JAVA
public class Parent {
    public final void show() { ... }
}
public class Child extends Parent {
    @Override
    public void show() { ... } // 컴파일 에러
}
```

---

## 3. 클래스에 final 사용

- `final`로 선언된 클래스는 **상속이 불가능**
  
- 즉, 자식 클래스를 생성 불가
  
- 대표적으로 `String`, `Integer` 등 Java의 핵심 클래스들이 `final`로 선언
  
```Java
public final class MyClass { ... }
public class SubClass extends MyClass { ... } // 컴파일 에러
```

---

## 4. final의 특징과 주의점

- **불변성(immutability)**:  
  - `final` 변수는 재할당이 불가능하므로 값의 불변성을 보장
    
  - 단, 참조 타입의 경우 객체 내부 상태까지 불변임을 보장하지는 않음
    
- **스레드 안전성**:  
  - 멀티스레드 환경에서 `final` 필드는 한 번만 초기화되므로, 안전하게 공유 가능
    
- **코드 명확성**:  
  - 변수나 파라미터에 `final`을 사용하면, 해당 값이 변경되지 않음을 명확하게 표현 가능
    
- **불변 객체와의 차이**:  
  - `final`은 변수의 재할당만 막을 뿐, 객체 자체의 불변성을 보장하지 않음
    
  - **불변 객체(Immutable Object)** 를 만들려면 내부 필드도 변경할 수 없도록 별도의 설계가 필요

---

- `final` 키워드는 변수, 메서드, 클래스에 적용되어 **변경을 제한**
