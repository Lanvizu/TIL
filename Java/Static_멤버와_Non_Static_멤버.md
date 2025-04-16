# Java의 Static 멤버 vs Non-Static 멤버

Java에서 **Static 멤버**와 **Non-Static 멤버**는 메모리 관리 방식과 사용 목적에서 근본적인 차이 존재

---

### **Static 멤버**  
- 클래스에 속하는 멤버 (클래스 레벨에서 관리)  
- `static` 키워드로 선언  
- **예시**: `static int count;`, `static void print() { ... }`

### **Non-Static 멤버**  
- 객체(인스턴스)에 속하는 멤버 (객체 레벨에서 관리)  
- 별도의 키워드 없이 선언  
- **예시**: `String name;`, `void setName() { ... }`

---

## 주요 차이점 비교

| 특성                | Static 멤버                          | Non-Static 멤버                     |
|---------------------|--------------------------------------|-------------------------------------|
| **소속**            | 클래스                               | 객체(인스턴스)                      |
| **메모리 할당 시점** | 클래스 로딩 시                       | 객체 생성 시 (`new` 키워드 사용 시) |
| **메모리 영역**      | Method Area (클래스 영역)            | Heap 영역 (객체 내부)               |
| **생명 주기**        | 프로그램 종료 시까지 유지            | 객체가 GC에 의해 제거될 때까지      |
| **공유 여부**        | 모든 객체가 공유                     | 객체마다 독립적                     |
| **접근 방법**        | `클래스명.멤버명` (객체 없이 접근 가능) | `객체명.멤버명` (객체 필요)          |

---

## 사용 예시

```
public class Car {
    // Static 멤버 (클래스 소속)
    static int totalCars = 0;          // 생성된 자동차 총 대수
    static void printTotal() {         // 총 대수 출력 메서드
        System.out.println("Total: " + totalCars);
    }
    
    // Non-Static 멤버 (객체 소속)
    String model;                      // 자동차 모델명
    void setModel(String model) {      // 모델명 설정 메서드
        this.model = model;
    }
}

// 사용 예시
public class Main {
    public static void main(String[] args) {
        // Static 멤버 접근 (객체 생성 필요 X)
        Car.totalCars = 5;
        Car.printTotal();              // 출력: Total: 5
        
        // Non-Static 멤버 접근 (객체 생성 필요)
        Car myCar = new Car();
        myCar.setModel("Model S");
        System.out.println(myCar.model); // 출력: Model S
    }
}
```

---

## 주의사항

1. **Static 메서드에서 Non-Static 멤버 접근 불가**  
   - Static 메서드는 객체 생성 전에 호출될 수 있으므로, Non-Static 멤버에 직접 접근할 수 없음
   - **에러 예시**:  
     ```
     static void printModel() {
         System.out.println(model); // ❌ 컴파일 에러
     }
     ```

2. **Static 멤버의 남용 금지**  
   - Static 멤버는 모든 객체가 공유하므로, 상태 저장용으로 사용 시 예기치 않은 버그 발생 가능
   - **적절한 사용 예**: 상수(`static final`), 유틸리티 함수, 카운터 등

---

## 사용하는 곳

### **Static 멤버**  
- 모든 객체가 공유해야 하는 데이터 (예: 환경 설정 값)  
- 객체 생성 없이 사용해야 하는 유틸리티 함수 (예: `Math.sqrt()`)  
- 인스턴스 생성 횟수 카운팅

### **Non-Static 멤버**  
- 객체마다 고유한 상태를 저장해야 할 때 (예: 사용자 이름, 계좌 잔액)  
- 객체의 행위를 정의할 때 (예: `user.login()`, `account.deposit()`)

