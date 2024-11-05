# 단일 책임 원칙 (SRP: Single Responsibility Principle)

단일 책임 원칙은 객체 지향 프로그래밍의 SOLID 원칙 중 하나로, 소프트웨어 설계의 핵심 개념이다.

## 정의

> "하나의 클래스는 하나의 책임만 가져야 한다."

여기서 '책임'은 '기능 담당'으로 해석할 수 있다. 즉, 각 클래스는 단 하나의 기능만을 담당해야 한다.

## 원칙의 중요성

1. **유지보수성 향상**: 각 클래스가 단일 책임을 가짐으로써, 변경 사항이 발생했을 때 영향을 받는 부분을 최소화 가능하다.
2. **코드 가독성 개선**: 클래스의 책임이 명확해져 코드를 이해하기 쉬워진다.
3. **재사용성 증가**: 단일 책임을 가진 클래스는 다른 상황에서도 쉽게 재사용될 수 있다.

## SRP 적용의 장점

1. **변경의 영향 최소화**: 한 책임의 변경이 다른 책임에 영향을 미치지 않는다.
2. **코드 구조 개선**: 각 클래스의 목적이 명확해져 전체적인 코드 구조가 개선된다.
3. **테스트 용이성**: 단일 책임을 가진 클래스는 테스트하기 더 쉽다.

## SRP 적용 방법

1. **기능 분석**: 클래스의 모든 메서드와 속성을 검토하여 서로 다른 책임을 식별한다.
2. **책임 분리**: 식별된 책임들을 별도의 클래스로 분리한다.
3. **인터페이스 활용**: 필요한 경우 인터페이스를 사용하여 책임을 추상화한다.

## 주의사항

- 과도한 분리는 오히려 복잡성을 증가시킬 수 있다. 적절한 균형을 찾는 것이 중요하다.
- 클래스의 책임을 정의할 때는 비즈니스 로직과 애플리케이션의 요구사항을 고려해야 한다.

## 결론

단일 책임 원칙을 적절히 적용하면 코드의 유지보수성, 가독성, 그리고 재사용성을 크게 향상시킬 수 있다.

이는 장기적으로 프로젝트의 성공과 개발자의 생산성 향상에 기여한다.

-----

# SRP 원칙 적용 주의점

## 1. 명확한 클래스명 작성

- 클래스명은 해당 클래스의 책임을 명확히 나타내야 함
- 각 클래스는 하나의 개념을 대표해야 함

## 2. 결합도와 응집도 고려

- 책임 분리 시 결합도와 응집도를 신중히 고려해야 함
- **응집도**: 프로그램 요소의 집중도 (높을수록 효율적)
- **결합도**: 프로그램 구성 요소 간의 의존성 (낮을수록 효율적)

### 주의사항
- 책임 분리 시 결합도를 최소화해야 함
- 과도한 책임 분할로 인한 파편화 주의
- 필요 시 '산탄총 수술'을 통해 응집력 향상 필요

> 균형 잡힌 책임 분리가 중요: 너무 세분화하거나 너무 뭉치지 않도록 주의

[참고 블로그](https://inpa.tistory.com/entry/OOP-%F0%9F%92%A0-%EC%95%84%EC%A3%BC-%EC%89%BD%EA%B2%8C-%EC%9D%B4%ED%95%B4%ED%95%98%EB%8A%94-SRP-%EB%8B%A8%EC%9D%BC-%EC%B1%85%EC%9E%84-%EC%9B%90%EC%B9%99)

-----

# 적용 - 우테코 프리코스_1주차

![image](https://github.com/user-attachments/assets/7d6155c7-e8ae-44d4-965d-6d702e5ef439)

목적 - 구분자와 문자열을 함께 입력받아 구분자로 문자열을 구분해 덧셈을 진행

> 입력 : "//x\n1x2x3"

> 출력 : 6

기능을 총 4가지로 나눠 SRP를 적용시켜봤다.
1. 초기 입력값을 \n을 기준으로 구분하여 구분자 입력과 문자열 입력으로 나눈다.
2. 구분자 입력에서 구분자를 추출한다.
3. 구분자를 통해 문자열 입력을 구분하여 문자열을 추출한다.
4. 추출된 문자열을 통해 덧셈을 진행한다.

<details>
  
<summary> 1. InputParser </summary>

```JAVA
package calculator.domain;

import static calculator.global.exception.ErrorMessage.EMPTY_INPUT;

import calculator.global.exception.CalculatorException;

public class InputParser {
    private String delimiterString;
    private String inputString;

    /*"\\n"을 기준으로 구분자 설정 입력과 문자열 입력을 나눠주는 메서드
     * 빈 값이 들어오는 경우 예외 처리
     * "\\n"이 포함되지 않는 경우 구분자 빈 값으로 저장*/
    public void parse(String input) {
        validateInput(input);
        String[] parts = input.split("\\\\n", 2);
        if (parts.length == 2) {
            delimiterString = parts[0];
            inputString = parts[1];
        } else {
            delimiterString = "";
            inputString = input;
        }
    }

    private void validateInput(String input) {
        if (input == null || input.isEmpty()) {
            throw CalculatorException.of(EMPTY_INPUT);
        }
    }

    public String getDelimiterString() {
        return delimiterString;
    }

    public String getInputString() {
        return inputString;
    }
}
```

</details>

<details>
  
<summary> 2. DelimiterParser </summary>

```JAVA
package calculator.domain;

import static calculator.global.exception.ErrorMessage.EMPTY_CUSTOM_DELIMITER;
import static calculator.global.exception.ErrorMessage.INVALID_DELIMITER_START;

import calculator.global.exception.CalculatorException;

public class DelimiterParser {
    private String delimiter;

    public DelimiterParser() {
        this.delimiter = ",|:";
    }

    //   새로운 구분자 설정
    /*결과적으로 "//"로 시작하지 않는 경우 , "//"로 시작하더라도 구분자가 없는 경우*/
    /*빈 값이 들어올 경우 디폴트 구분자로 유지*/
    public void parse(String input) {
        if (input == null || input.isEmpty()) {
            return;  // 빈 입력일 경우 기존 구분자
        }
        validateCustomDelimiterInput(input);
        this.delimiter = extractCustomDelimiter(input);
    }

    //    입력된 구분자 형식 검사 ("//"로 시작 + "//" 제외 문자가 있는지 확인)
    private void validateCustomDelimiterInput(String input) {
        if (!input.startsWith("//")) {
            throw CalculatorException.of(INVALID_DELIMITER_START);
        }
        if (input.length() == 2) {
            throw CalculatorException.of(EMPTY_CUSTOM_DELIMITER);
        }
    }

    //    입력에서 구분자 추출
    private String extractCustomDelimiter(String input) {
        return input.substring(2);
    }

    public String getDelimiter() {
        return delimiter;
    }
}
```
  
</details>

<details>
  
<summary> 3. StringSplitter </summary>

```JAVA
package calculator.domain;

import static calculator.global.exception.ErrorMessage.EMPTY_NUMBER_INPUT;
import static calculator.global.exception.ErrorMessage.INVALID_DELIMITER_INPUT;
import static calculator.global.exception.ErrorMessage.INVALID_STRING_END;
import static calculator.global.exception.ErrorMessage.INVALID_STRING_START;

import calculator.global.exception.CalculatorException;
import java.util.ArrayList;
import java.util.List;

public class StringSplitter {

    /*구분자는 에러 없이 들어온다고 가정
     * input 즉 /n 이후의 문자열에 대한 예외 처리 진행
     * 이후 문자열을 정해진 구분자로 나눠주는 과정 진행*/
    /*예외 처리
     * 숫자로 시작하지 않는 경우
     * 숫자로 끝나지 않는 경우
     * 정해진 구분자가 아닌 문자가 들어간 경우*/

    public String[] split(String input, String delimiter) {
        validateInput(input, delimiter);
        return input.split(delimiter);
    }

    //    빈 값, 숫자로 시작, 숫자로 끝나는 지 검증
    private void validateInput(String input, String delimiter) {
        if (input == null || input.isEmpty()) {
            throw CalculatorException.of(EMPTY_NUMBER_INPUT);
        }

        List<String> parts = splitIntoParts(input);
        if (!isValidStart(parts)) {
            throw CalculatorException.of(INVALID_STRING_START);
        }

        if (!isValidEnd(parts)) {
            throw CalculatorException.of(INVALID_STRING_END);
        }

        if (!containsOnlyValidCharacters(parts, delimiter)) {
            throw CalculatorException.of(INVALID_DELIMITER_INPUT);
        }
    }

    //    입력을 모든 문자로 쪼개는 메서드
    private List<String> splitIntoParts(String input) {
        List<String> parts = new ArrayList<>();
        for (char c : input.toCharArray()) {
            parts.add(String.valueOf(c));
        }
        return parts;
    }

    //    숫자로 시작하는지 체크
    private boolean isValidStart(List<String> parts) {
        return Character.isDigit(parts.getFirst().charAt(0));
    }

    //    숫자로 끝나는지 체크
    private boolean isValidEnd(List<String> parts) {
        return Character.isDigit(parts.getLast().charAt(0));
    }

    //    유효한 구분자만 포함되는지 체크
    private boolean containsOnlyValidCharacters(List<String> parts, String delimiter) {
        for (String part : parts) {
            char ch = part.charAt(0);
            if (!Character.isDigit(ch) && !delimiter.contains(part)) {
                return false;
            }
        }
        return true;
    }
}
```

</details>

<details>
  
<summary> 4. SumCalculator </summary>

```JAVA
package calculator.domain;

import static calculator.global.exception.ErrorMessage.EMPTY_NUMBERS_INPUT;

import calculator.global.exception.CalculatorException;

public class SumCalculator {
    public int calculateSum(String[] numbers) {
        int sum = 0;
        for (String number : numbers) {
            validateNumbers(number);
            sum += Integer.parseInt(number);
        }
        return sum;
    }

    //    빈 값 처리
    private void validateNumbers(String number) {
        if (number == null || number.isEmpty()) {
            throw CalculatorException.of(EMPTY_NUMBERS_INPUT);
        }
    }

}

```

</details>
