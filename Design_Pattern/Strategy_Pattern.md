# Strategy Pattern

- **행위(Behavioral) 디자인 패턴** 중 하나로, 객체의 행위를 동적으로 변경할 수 있도록 설계된 패턴
- 동일한 작업을 수행하는 다양한 알고리즘을 정의하고, 이를 캡슐화하여 실행 시점에 교체할 수 있도록 하는 패턴

    > 객체의 행위를 직접 수정하지 않고, 전략 객체를 교체하는 방식으로 유연하게 동작을 변경할 수 있음

### 구성 요소

1. **Context (컨텍스트)**  
   - 전략 객체를 사용하는 주체  
   - 전략 객체를 참조하며, 이를 통해 동작을 위임받아 실행  

2. **Strategy Interface (전략 인터페이스)**  
   - 모든 전략 클래스가 구현해야 할 공통 인터페이스  
   - 다양한 알고리즘이나 동작을 정의  

3. **Concrete Strategy (구체적인 전략)**  
   - `Strategy Interface`를 구현한 클래스  
   - 특정 알고리즘이나 동작을 캡슐화  

### 사용 이유

- 코드의 **유연성**과 **확장성**을 높이기 위해
- 알고리즘을 클라이언트 코드와 분리하여 **결합도를 낮추고**, 유지보수를 용이하게 하기 위해
- 실행 시점에 적절한 알고리즘을 선택하거나 변경할 필요가 있는 경우

### 주로 사용하는 경우

- 여러 알고리즘 중 하나를 선택해야 하는 상황
    > 예: 결제 방식(카드, 현금, 포인트) 선택, 경로 탐색 알고리즘 등
- 특정 기능의 확장 가능성을 고려해야 하는 경우

### 장점

- **알고리즘 교체 용이**: 실행 중에도 전략(알고리즘)을 변경 가능
- **코드 재사용성 증가**: 공통 인터페이스를 통해 다양한 구현체를 활용 가능
- **결합도 감소**: 알고리즘과 클라이언트 코드가 분리되어 독립적인 개발 및 테스트 가능

### 단점

- 클래스 수 증가: 각 알고리즘마다 별도의 클래스 필요
- 클라이언트가 적절한 전략을 선택하고 관리해야 하는 부담 증가

### 예제 코드

``` Java
// Strategy Interface
public interface Strategy {
    int execute(int a, int b);
}

// Concrete Strategies
public class AddOperation implements Strategy {
    @Override
    public int execute(int a, int b) {
        return a + b;
    }
}

public class SubtractOperation implements Strategy {
    @Override
    public int execute(int a, int b) {
        return a - b;
    }
}

// Context Class
public class Context {
    private Strategy strategy;

    public Context(Strategy strategy) {
        this.strategy = strategy;
    }

    public void setStrategy(Strategy strategy) {
        this.strategy = strategy;
    }

    public int executeStrategy(int a, int b) {
        return strategy.execute(a, b);
    }
}

// Main Class
public class Main {
    public static void main(String[] args) {
        Context context = new Context(new AddOperation());
        System.out.println("Addition: " + context.executeStrategy(10, 5)); // 15

        context.setStrategy(new SubtractOperation());
        System.out.println("Subtraction: " + context.executeStrategy(10, 5)); // 5
    }
}
```

### 활용 사례

- **결제 시스템**: 신용카드, 페이팔 등 다양한 결제 방식을 런타임에 변경 가능
- **게임 개발**: 캐릭터의 공격 방식(근접 공격, 원거리 공격 등)을 동적으로 변경 가능
- **데이터 정렬**: 정렬 알고리즘(버블 정렬, 퀵 정렬 등)을 필요에 따라 교체 가능

---

**Strategy 패턴은 상황에 맞는 유연한 설계를 가능하게 하지만, 불필요한 복잡성을 초래하지 않도록 필요한 경우에만 사용하자.**
