# Factory Pattern

- **생성(Creational) 디자인 패턴** 중 하나로, 객체 생성 로직을 캡슐화하여 클라이언트 코드에서 객체 생성 과정을 분리하는 패턴
  
- 객체를 직접 생성하지 않고, **팩토리(Factory)** 라는 특별한 클래스나 메서드를 통해 생성합니다.

    > 객체 생성 방식을 캡슐화하여, 클라이언트가 구체적인 클래스 이름을 알지 않아도 객체를 생성할 수 있도록 설계

---

### 구성 요소

1. **Product (제품)**  
   - 팩토리가 생성하는 객체의 공통 인터페이스 또는 추상 클래스  
   - 구체적인 제품 클래스들이 이 인터페이스를 구현하거나 상속받음  

2. **Concrete Product (구체적인 제품)**  
   - `Product` 인터페이스를 구현한 클래스  
   - 팩토리에서 생성되는 실제 객체  

3. **Creator (팩토리)**  
   - 객체 생성 로직을 캡슐화한 클래스 또는 메서드  
   - 클라이언트가 요청한 타입에 따라 적절한 `Concrete Product`를 반환  

4. **Client (클라이언트)**  
   - 팩토리를 통해 객체를 생성하며, 구체적인 클래스 이름에 의존하지 않음  

---

### 사용 이유

- 객체 생성 로직을 클라이언트 코드와 분리하여 **유지보수성**과 **확장성**을 높이기 위해
- 새로운 제품 클래스가 추가되더라도 클라이언트 코드를 수정하지 않도록 하기 위해
- 객체 생성 과정이 복잡하거나 조건에 따라 달라지는 경우 이를 간소화하기 위해

---

### 주로 사용하는 경우

- 구체적인 클래스 이름을 클라이언트 코드에서 숨기고 싶을 때
    > 예: 데이터베이스 연결 객체 생성, 다양한 형식의 문서 처리 등
- 객체 생성 로직이 복잡하거나 조건에 따라 달라지는 경우
    > 예: 특정 설정 파일에 따라 다른 객체를 생성해야 하는 경우

---

### 장점

- **유연성 증가**: 클라이언트는 구체적인 클래스 이름을 알 필요 없이 팩토리를 통해 객체를 생성할 수 있음
- **코드 재사용성 증가**: 객체 생성 로직을 중앙 집중화하여 중복 제거
- **확장성**: 새로운 제품 클래스를 추가하더라도 기존 코드를 수정할 필요 없음

---

### 단점

- 팩토리 클래스를 추가하거나 유지보수해야 하므로 코드 복잡도가 증가할 수 있음
- 너무 많은 팩토리 클래스를 만들면 관리가 어려워질 수 있음

---

### 예제 코드

```Java
// Product Interface
public interface Shape {
    void draw();
}

// Concrete Products
public class Circle implements Shape {
    @Override
    public void draw() {
        System.out.println("Drawing a Circle");
    }
}

public class Rectangle implements Shape {
    @Override
    public void draw() {
        System.out.println("Drawing a Rectangle");
    }
}

public class Square implements Shape {
    @Override
    public void draw() {
        System.out.println("Drawing a Square");
    }
}

// Factory Class
public class ShapeFactory {
    public Shape getShape(String shapeType) {
        if (shapeType == null) {
            return null;
        }
        if (shapeType.equalsIgnoreCase("CIRCLE")) {
            return new Circle();
        } else if (shapeType.equalsIgnoreCase("RECTANGLE")) {
            return new Rectangle();
        } else if (shapeType.equalsIgnoreCase("SQUARE")) {
            return new Square();
        }
        return null;
    }
}

// Client Code
public class FactoryPatternDemo {
    public static void main(String[] args) {
        ShapeFactory shapeFactory = new ShapeFactory();

        // Get Circle object and call its draw method
        Shape shape1 = shapeFactory.getShape("CIRCLE");
        shape1.draw();

        // Get Rectangle object and call its draw method
        Shape shape2 = shapeFactory.getShape("RECTANGLE");
        shape2.draw();

        // Get Square object and call its draw method
        Shape shape3 = shapeFactory.getShape("SQUARE");
        shape3.draw();
    }
}
```

---

### 실행 결과

```
Drawing a Circle  
Drawing a Rectangle  
Drawing a Square  
```

---

### 활용 사례

- **GUI 컴포넌트 생성**: 버튼, 체크박스 등 다양한 UI 요소를 동적으로 생성하는 경우
- **데이터베이스 연결**: 특정 DBMS(MySQL, Oracle 등)에 맞는 연결 객체를 동적으로 제공
- **문서 처리 시스템**: 다양한 문서 형식(XML, JSON 등)을 처리하기 위한 파서(Parser) 생성

---

**Factory Pattern은 객체 생성 로직을 캡슐화하여 코드의 유연성과 재사용성을 높이는 강력한 도구지만, 불필요한 복잡성을 초래하지 않도록 필요한 경우에만 사용하는 것이 좋다.**
