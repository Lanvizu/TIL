인프런 강의 '스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술'을 공부하며 정리한 내용입니다.

[스프링 MVC 1편 - 백엔드 웹 개발 핵심 기술](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81-mvc-1)

-----

### 서비스 제공 흐름

![image](https://github.com/user-attachments/assets/ce495425-c3b6-4664-abd3-717c1d253c00)

- 디자이너 -> 웹 퍼블리셔 -> 백엔드 개발자

> React, Vue.js 같은 웹 클라이언트 기술을 사용하고, 웹 프론트엔드 개발자가 별도로 있으면, 웹 프론트 엔드 개발자가 웹 퍼블리셔 역할까지 포함하는 경우 있음

### 상품 서비스 HTML

> 부트스트랩 - 웹 사이트를 쉽게 만들 수 있게 도와주는 HTML, CSS, JS 프레임워크이다.
>
> 하나의 CSS로 다양한 기기에서 작동하며 쉽게 웹사이트를 제작, 유지, 보수 가능

> - 주의
> 
> 정적 리소스를 공개되는 /resources/static 폴더에 HTML을 넣으면 실제 서비스에서도 공개된다.

### @RequiredArgsConstructor

- final이 붙은 멤버 변수만 사용해 생성자를 자동으로 생성

```JAVA
public BasicItemController(ItemRepository itemRepository) {
 this.itemRepository = itemRepository;
}
```

- 생성자가 하나만 있는 경우 스프링이 해당 생성자에 @Autowired로 의존관계 주입

- 꼭 final 키워드를 붙여야 의존관계 주입이 된다.

### 속성 변경 - th:href

th:href="@{/css/bootstrap.min.css}"

- href="value1" 을 th:href="value2" 의 값으로 변경한다.

- 타임리프 뷰 템플릿을 거치게 되면 원래 값을 th:xxx 값으로 변경한다. 만약 값이 없다면 새로 생성한다.

- HTML을 그대로 볼 때는 href 속성이 사용되고, 뷰 템플릿을 거치면 th:href 의 값이 href 로 대체되면서 동적으로 변경할 수 있다.

- 대부분의 HTML 속성을 th:xxx 로 변경할 수 있다.

### 타임리프 핵심

- 핵심은 th:xxx 가 붙은 부분은 서버사이드에서 렌더링 되고, 기존 것을 대체한다. th:xxx 이 없으면 기존 html의 xxx 속성이 그대로 사용된다.

- HTML을 파일로 직접 열었을 때, th:xxx 가 있어도 웹 브라우저는 th: 속성을 알지 못하므로 무시한다.

- 따라서 HTML을 파일 보기를 유지하면서 템플릿 기능도 할 수 있다.

### URL 링크 표현식 - @{...},

th:href="@{/css/bootstrap.min.css}"

- @{...} : 타임리프는 URL 링크를 사용하는 경우 @{...} 를 사용한다. 이것을 URL 링크 표현식이라 한다.

- URL 링크 표현식을 사용하면 서블릿 컨텍스트를 자동으로 포함한다.

# 상품 등록 처리 - @ModelAttribute

```JAVA
package hello.itemservice.web.basic;

import hello.itemservice.domain.item.Item;
import hello.itemservice.domain.item.ItemRepository;
import jakarta.annotation.PostConstruct;
import java.util.List;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

@Controller
@RequestMapping("/basic/items")
@RequiredArgsConstructor
public class BasicItemController {
    private final ItemRepository itemRepository;

    @GetMapping
    public String items(Model model) {
        List<Item> items = itemRepository.findAll();
        model.addAttribute("items", items);
        return "basic/items";
    }

    @GetMapping("/{itemId}")
    public String item(@PathVariable Long itemId, Model model) {
        Item item = itemRepository.findById(itemId);
        model.addAttribute("item", item);
        return "basic/item";
    }

    @GetMapping("/add")
    public String addForm() {
        return "basic/addForm";
    }

//    @PostMapping("/add")
//    public String addItemV1(@RequestParam String itemName,
//                            @RequestParam int price,
//                            @RequestParam Integer quantity,
//                            Model model) {
//        Item item = new Item();
//        item.setItemName(itemName);
//        item.setPrice(price);
//        item.setQuantity(quantity);
//        itemRepository.save(item);
//        model.addAttribute("item", item);
//        return "basic/item";
//    }

    /**
     * @ModelAttribute("item") Item item
     * model.addAttribute("item", item); 자동 추가
     */
//    @PostMapping("/add")
//    public String addItemV2(@ModelAttribute("item") Item item, Model model) {
//        itemRepository.save(item);
//        //model.addAttribute("item", item); //자동 추가, 생략 가능
//        return "basic/item";
//    }

    /**
     * @ModelAttribute name 생략 가능
     * model.addAttribute(item); 자동 추가, 생략 가능
     * 생략시 model에 저장되는 name은 클래스명 첫글자만 소문자로 등록 Item -> item
     */
//    @PostMapping("/add")
//    public String addItemV3(@ModelAttribute Item item) {
//        itemRepository.save(item);
//        return "basic/item";
//    }

    /**
     * @ModelAttribute 자체 생략 가능
     * model.addAttribute(item) 자동 추가
     */
//    @PostMapping("/add")
//    public String addItemV4(Item item) {
//        itemRepository.save(item);
//        return "basic/item";
//    }
    /**
     * PRG - Post/Redirect/Get
     */
//    @PostMapping("/add")
//    public String addItemV5(Item item) {
//        itemRepository.save(item);
//        return "redirect:/basic/items/" + item.getId();
//    }

    /**
     * RedirectAttributes
     */
    @PostMapping("/add")
    public String addItemV6(Item item, RedirectAttributes redirectAttributes) {
        Item savedItem = itemRepository.save(item);
        redirectAttributes.addAttribute("itemId", savedItem.getId());
        redirectAttributes.addAttribute("status", true);
        return "redirect:/basic/items/{itemId}";
    }

    @GetMapping("/{itemId}/edit")
    public String editForm(@PathVariable Long itemId, Model model) {
        Item item = itemRepository.findById(itemId);
        model.addAttribute("item", item);
        return "basic/editForm";
    }

    @PostMapping("/{itemId}/edit")
    public String edit(@PathVariable Long itemId, @ModelAttribute Item item) {
        itemRepository.update(itemId, item);
        return "redirect:/basic/items/{itemId}";
    }
    
    /**
     * 테스트용 데이터 추가
     */
    @PostConstruct
    public void init() {
        itemRepository.save(new Item("testA", 10000, 10));
        itemRepository.save(new Item("testB", 20000, 20));
    }
}
```

## V2

### 요청 파라미터 처리

- @ModelAttribute는 Item 객체를 생성하고, 요청 파라미터의 값을 프로퍼티 접근법(setXxx)으로 입력

### Model 추가

- 모델에 @ModelAttribute로 지정한 객체를 자동으로 넣어준다.

> model.Attribute("item", item)이 주석처리 되어 있어도 작동한다.

## V3

- @ModelAttribute 이름을 생략 시 모델에 저장될 때 클래스명을 사용

- 이때 클래스의 첫글자만 소문자로 변경되어 등록

## V4

- @ModelAttribute 자체도 생략 가능하며 대상 객체는 모델에 자동 등록된다.

# PRG Post/Redirect/Get

### 전체 흐름

![image](https://github.com/user-attachments/assets/20969c15-5c41-4c91-b218-979d103c462b)

### POST 등록 후 새로 고침

![image](https://github.com/user-attachments/assets/db56b134-fb61-46b4-85c3-f580233b6de6)

- 웹 브라우저의 새로 고침은 마지막에 서버에 전송한 데이터를 다시 전송

- 상품 등록 폼에서 데이터를 입력하고 저장을 선택 시 -> Post /add + 상품 데이터를 서버로 전송

- 이 상태에서 새로 고침을 다시하면 마지막에 전송한 Post /add + 상품 데이터를 서버로 다시 전송

### POST, Redirect GET

![image](https://github.com/user-attachments/assets/42581cc1-06f0-426f-9819-38dff3306960)

- 이를 해결하기 위해 상품 저장 후 뷰 템플릿으로 이동하는 것이 아니라, 상품 상세 화면으로 리다이렉트 호출

- 상품 저장 후 -> 상품 상세 화면으로 이동: GET /items/{id}

- 이후 새로고침을 해도 상품 상세 화면으로 이동하게 되므로 새로 고침 문제 해결

> 주의
>
> "redirect:/basic/items/" + item.getId()
>
> 이와 같이 URL에 변수를 더해 사용하면 URL 인코딩이 안되므로 위험하다.
>
> 대신 RedirectAttributes를 사용하자.

# RedirectAttributes

```JAVA
/**
 * RedirectAttributes
 */
@PostMapping("/add")
public String addItemV6(Item item, RedirectAttributes redirectAttributes) {
 Item savedItem = itemRepository.save(item);
 redirectAttributes.addAttribute("itemId", savedItem.getId());
 redirectAttributes.addAttribute("status", true);
 return "redirect:/basic/items/{itemId}";
}
```

- 리다이렉트할 때 간단하게 status=true를 추가해 값의 유무에 따라 메시지 출력 가능

- RedirectAttributes을 사용 시 URL 인코딩도 해주며, pathVariable, 쿼리 파라미터까지 처리
