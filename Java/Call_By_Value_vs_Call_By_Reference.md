# Call by Value vs Call by Reference

Java에서 메서드에 인자를 전달하는 방식은 **Call by Value**와 **Call by Reference**로 구분된다.

두 방식의 핵심 차이는 **전달되는 대상**과 **원본 데이터의 변경 여부**

---

## 1. Call by Value (값에 의한 호출)

- 메서드 호출 시 **변수의 값**을 복사하여 전달
- 전달받은 값은 새로운 지역 변수로 생성되며, 원본 변수와 독립적

### **특징**  
- 기본 자료형(`int`, `double`, `boolean` 등)에 적용  
- 메서드 내에서 값 변경 시 **원본 변수에 영향 없음**  
- 메모리 효율적 (작은 크기의 값 복사)

```Java
void updateValue(int num) {
    num = 20; // 원본 변수 값 변경 X
}

public static void main(String[] args) {
    int a = 10;
    updateValue(a);
    System.out.println(a); // 10 출력
}
```

---

## 2. Call by Reference (참조에 의한 호출)
 
- 메서드 호출 시 **변수의 참조(주소)**를 전달 
- 전달받은 참조로 원본 객체의 상태를 직접 수정 가능

### **특징**  
- 참조 자료형(`객체`, `배열` 등)에 적용  
- 메서드 내에서 객체 상태 변경 시 **원본 객체에도 반영**  
- Java에서는 **공식적으로 지원하지 않음** (Call by Value로 동작)

```Java
class Person {
    String name;
    Person(String name) { this.name = name; }
}

void changeName(Person p) {
    p.name = "hee"; // 원본 객체 상태 변경
}

public static void main(String[] args) {
    Person person = new Person("doy");
    changeName(person);
    System.out.println(person.name); // "hee" 출력
}
```

---

## 3. Java는 항상 Call by Value

Java는 **참조 타입을 전달할 때도 참조값(주소)을 복사하여 전달**

```Java
void reassignObject(Person p) {
    p = new Person("hee"); // 새로운 객체 생성 (원본 참조 변경 X)
}

public static void main(String[] args) {
    Person person = new Person("doy");
    reassignObject(person);
    System.out.println(person.name); // "doy" 출력
}
```
- **참조값 복사**: `person`의 참조값이 복사되어 전달 
- **재할당 영향 없음**: 새로운 객체를 할당해도 원본 참조는 변하지 않음

---

## 4. Call by Value vs Call by Reference 비교

| 구분               | Call by Value                          | Call by Reference                     |
|--------------------|----------------------------------------|---------------------------------------|
| **전달 대상**      | 값 복사                                | 참조(주소) 전달                       |
| **원본 변경 가능** | 불가능                                 | 가능                                  |
| **Java 지원 여부** | O (모든 타입)                          | X (참조값 복사로 인해 유사하게 동작)  |
| **사용 예시**      | 기본 타입(int, double 등)              | 객체, 배열                            |

---

## 5. Java에서의 참조 타입 동작 원리

**객체 전달 시**  
   - 객체의 참조값(주소)이 복사되어 전달
   - 복사된 참조값으로 **동일한 객체를 가리키므로 상태 변경이 가능** 
   - 하지만 **참조 자체를 변경**하면 원본에 영향이 없음

```Java
void modifyArray(int[] arr) {
    arr = 100;  // 원본 배열 변경
    arr = new int[3]; // 새로운 배열 할당 (원본 영향 X)
}

public static void main(String[] args) {
    int[] array = {1, 2, 3};
    modifyArray(array);
    System.out.println(array); // 100 출력
}
```

---


- **Java는 Call by Value만 지원**
- **참조 타입**은 참조값(주소)을 복사하여 전달하므로,  
  객체의 상태 변경은 가능하지만 참조 재할당은 원본에 영향을 주지 않음
- 이로 인해 참조 타입이 마치 Call by Reference처럼 보이지만, 엄밀히 다른 동작 방식

