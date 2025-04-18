# Java의 `main` 메서드가 `static`인 이유

Java 프로그램의 시작점인 `main` 메서드는 반드시 `public static void`로 선언되어야 한다.

JVM(Java Virtual Machine)의 동작 방식과 객체지향 프로그래밍의 원칙에 기반

---

## 1. JVM의 프로그램 실행 과정

1. **클래스 로딩**:  
   JVM은 프로그램 실행 시 `.class` 파일을 메모리의 **Method Area(Static Area)**에 로드

2. **`main` 메서드 호출**:  
   클래스 로딩 후, JVM은 `main` 메서드를 찾아 실행

3. **객체 생성**:  
   `main` 메서드 실행 중 필요한 객체를 `new` 키워드로 생성

---

## 2. `main` 메서드가 `static`이어야 하는 이유

### **객체 생성 없이 호출 가능**  
- JVM은 프로그램 시작 시점에 **객체가 생성되지 않은 상태**에서 `main` 메서드를 호출해야 함
  
- `static` 메서드는 클래스에 속하므로, 객체 생성 없이 `클래스명.main()` 형태로 호출 가능
  
- 만약 `static`이 아니라면, `main`을 호출하기 전에 객체를 생성해야 하는 **모순**이 발생

### **프로그램 진입점(Entry Point)의 역할**  
- `main` 메서드는 프로그램의 **시작점**으로, JVM이 항상 동일한 방법으로 접근할 수 있어야 함
  
- `static`으로 고정된 진입점을 제공함으로써, JVM이 일관된 방식으로 프로그램을 실행 가능

### **메모리 효율성**  
- `static` 멤버는 클래스 로딩 시 **Method Area**에 할당되며, 프로그램 종료 시까지 유지
  
- 객체 생성 오버헤드 없이 빠르게 실행 시작 가능

---

## 3. `main` 메서드의 구조

```
public class Main {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
    }
}
```

- **`public`**: JVM이 어디서든 접근 가능
- **`static`**: 객체 생성 없이 호출 가능
- **`void`**: 반환값이 없음을 명시
- **`String[] args`**: 커맨드 라인 인자를 전달

---

## 4. `static`과 JVM 메모리 구조

| 메모리 영역         | 설명                          | 저장 대상                     |
|---------------------|-------------------------------|-------------------------------|
| **Method Area**     | 클래스 메타데이터 저장         | `static` 변수, 메서드, 클래스 정보 |
| **Heap Area**       | 객체 인스턴스 저장             | `new`로 생성된 객체           |
| **Stack Area**      | 메서드 호출 시 임시 데이터 저장 | 지역 변수, 메서드 호출 정보   |

- `main` 메서드는 **Method Area**에 로드되어 JVM이 바로 접근 가능
  
- `static`이 아닌 메서드는 객체 생성 후 **Heap**에 할당된 인스턴스를 통해 호출

---

- **`static`**은 JVM이 객체 생성 없이 `main` 메서드를 호출할 수 있도록 설정
  
- Java의 설계 원칙에 따라 프로그램의 시작점은 항상 **일관된 방식**으로 접근 가능해야 함
