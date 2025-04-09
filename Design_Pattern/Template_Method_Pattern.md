# Template Method Pattern

- **행위(Behavioral) 디자인 패턴** 중 하나로, 알고리즘의 구조를 상위 클래스에서 정의하고, 세부적인 동작은 하위 클래스에서 구현하도록 하는 패턴
- 알고리즘의 **불변적인 부분**은 상위 클래스에서 구현하고, **가변적인 부분**은 하위 클래스에서 오버라이딩하여 확장할 수 있도록 설계

    > 전체적인 작업 흐름(템플릿)은 고정하면서, 세부 동작은 다양화할 수 있는 경우에 적합

### 구성 요소

1. **Abstract Class (추상 클래스)**  
   - 템플릿 메서드와 함께 알고리즘의 공통된 흐름을 정의  
   - 하위 클래스에서 구현해야 할 추상 메서드나 선택적으로 오버라이딩 가능한 훅(Hook) 메서드를 포함  

2. **Template Method**  
   - 알고리즘의 전체적인 구조를 정의하는 메서드  
   - 불변적인 부분은 구현하고, 가변적인 부분은 추상 메서드나 훅 메서드로 위임  

3. **Concrete Class (구체적인 클래스)**  
   - 추상 클래스를 상속받아, 세부 동작을 구현  

### 사용 이유

- 코드 중복을 줄이고, 공통된 알고리즘 구조를 재사용하기 위해
- 알고리즘의 전체 흐름은 유지하면서, 특정 단계만 변경하거나 확장해야 하는 경우
- 상속을 활용하여 코드의 유연성과 확장성을 높이기 위해

### 주로 사용하는 경우

- 여러 클래스가 동일한 알고리즘 구조를 가지지만, 일부 세부 동작이 다른 경우
    > 예: 다양한 종류의 문서 포맷팅, 게임 캐릭터 행동 패턴 등
- 공통된 작업 흐름을 유지하면서 특정 단계만 커스터마이징해야 하는 경우

### 장점

- **코드 재사용성 증가**: 공통된 알고리즘 구조를 상위 클래스에 정의하여 중복 제거
- **유지보수 용이**: 알고리즘 변경 시 상위 클래스만 수정하면 됨
- **확장성**: 하위 클래스에서 필요한 부분만 재정의 가능

### 단점

- 상속에 의존하기 때문에, 자식 클래스와 부모 클래스 간의 강한 결합 발생
- 새로운 기능 추가 시 하위 클래스를 계속 생성해야 할 수 있음

### 예제 코드

```Java
// Abstract Class
public abstract class AbstractTemplate {
    // Template Method
    public final void templateMethod() {
        stepOne();
        stepTwo();
        hook();
    }

    protected abstract void stepOne(); // 추상 메서드 (하위 클래스에서 구현)
    protected abstract void stepTwo(); // 추상 메서드 (하위 클래스에서 구현)

    protected void hook() { // 선택적으로 오버라이딩 가능한 훅 메서드
        System.out.println("Default Hook");
    }
}

// Concrete Class 1
public class ConcreteClassA extends AbstractTemplate {
    @Override
    protected void stepOne() {
        System.out.println("ConcreteClassA: Step One");
    }

    @Override
    protected void stepTwo() {
        System.out.println("ConcreteClassA: Step Two");
    }
}

// Concrete Class 2
public class ConcreteClassB extends AbstractTemplate {
    @Override
    protected void stepOne() {
        System.out.println("ConcreteClassB: Step One");
    }

    @Override
    protected void stepTwo() {
        System.out.println("ConcreteClassB: Step Two");
    }

    @Override
    protected void hook() {
        System.out.println("ConcreteClassB: Custom Hook");
    }
}

// Main Class
public class Main {
    public static void main(String[] args) {
        AbstractTemplate templateA = new ConcreteClassA();
        templateA.templateMethod();

        AbstractTemplate templateB = new ConcreteClassB();
        templateB.templateMethod();
    }
}
```

### 실행 결과

```
ConcreteClassA: Step One  
ConcreteClassA: Step Two  
Default Hook  

ConcreteClassB: Step One  
ConcreteClassB: Step Two  
ConcreteClassB: Custom Hook  
```

### 활용 사례

- **프레임워크 개발**: 프레임워크에서 공통 로직을 제공하고, 사용자 정의 로직만 구현하도록 설계
- **게임 개발**: 캐릭터의 기본 행동 패턴은 동일하지만, 특정 행동(공격 방식 등)은 다르게 구현 가능
- **데이터 처리 파이프라인**: 데이터 전처리 단계는 동일하지만, 특정 변환 로직만 다르게 적용

---

**Template Method 패턴은 공통된 작업 흐름을 유지하면서도 유연하게 확장할 수 있는 강력한 도구지만, 상속으로 인한 결합도 증가를 주의해야 한다.**
