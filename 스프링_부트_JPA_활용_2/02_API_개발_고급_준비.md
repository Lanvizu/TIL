인프런 강의 ['실전! 스프링 부트와 JPA 활용2 - API 개발과 성능 최적화'](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81%EB%B6%80%ED%8A%B8-JPA-API%EA%B0%9C%EB%B0%9C-%EC%84%B1%EB%8A%A5%EC%B5%9C%EC%A0%81%ED%99%94)
을 공부하며 정리한 내용입니다.

-----

> 등록과 수정은 성능문제가 거의 발생하지 않음

> 데이터 한 건을 단순하게 넣는 과정이기 때문
>
> 시스템 문제의 90%는 조회 부분에서 발생

> 이를 해결하기 위한 학습 과정

# 조회용 샘플 데이터 입력

### InitDb

```Java
package jpashop.jpashop;

import jakarta.annotation.PostConstruct;
import jakarta.persistence.EntityManager;
import jpashop.jpashop.controller.BookForm;
import jpashop.jpashop.controller.MemberForm;
import jpashop.jpashop.domain.*;
import jpashop.jpashop.domain.item.Book;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

@Component
@RequiredArgsConstructor
public class InitDb {

    private final InitService initService;

    @PostConstruct
    public void init() {
        initService.dbInit1();
        initService.dbInit2();
    }

    @Component
    @Transactional
    @RequiredArgsConstructor
    static class InitService {
        private final EntityManager em;

        public void dbInit1() {
            Member member = Member.createMember(new MemberForm("userA", "서울", "1", "1111"));
            em.persist(member);

            BookForm bookForm1 = BookForm.createBookForm("JPA1 BOOK", 10000, 100);
            Book book1 = Book.createBook(bookForm1);
            em.persist(book1);

            BookForm bookForm2 = BookForm.createBookForm("SPRING2 BOOK", 40000, 300);
            Book book2 = Book.createBook(bookForm2);
            em.persist(book2);

            Delivery delivery = Delivery.createDelivery(member.getAddress());

            OrderItem orderItem1 = OrderItem.createOrderItem(book1, 20000, 3);
            OrderItem orderItem2 = OrderItem.createOrderItem(book2, 40000, 4);

            Order order = Order.createOrder(member, delivery, orderItem1, orderItem2);
            em.persist(order);
        }

        public void dbInit2() {
            Member member = Member.createMember(new MemberForm("userB", "진주", "2", "2222"));
            em.persist(member);

            BookForm bookForm1 = BookForm.createBookForm("SPRING1 BOOK", 20000, 200);
            Book book1 = Book.createBook(bookForm1);
            em.persist(book1);

            BookForm bookForm2 = BookForm.createBookForm("SPRING2 BOOK", 40000, 300);
            Book book2 = Book.createBook(bookForm2);
            em.persist(book2);

            Delivery delivery = Delivery.createDelivery(member.getAddress());
            
            OrderItem orderItem1 = OrderItem.createOrderItem(book1, 20000, 3);
            OrderItem orderItem2 = OrderItem.createOrderItem(book2, 40000, 4);
            
            Order order = Order.createOrder(member, delivery, orderItem1, orderItem2);
            em.persist(order);
        }
    }
}
```

- 강의와 다르게 코드를 리팩토링한 상황

- 생성자를 각각의 엔티티에서 관리하도록 추가

- createBook 같은 경우는 DTO를 매개변수로 생성하도록 변경

- MemberForm은 기존 MemberController에서 model.addAttribute에서 매개변수 없이 적용하는 코드가 있는 상황

```Java
@GetMapping("/members/new")
    public String createForm(Model model) {
        model.addAttribute("memberForm", new MemberForm());
        return "members/createMemberForm";
    }
```

- 따라서 @NoArgsConstructor 어노테이션을 추가하여 해결.

### MemberForm
```Java
package jpashop.jpashop.controller;

import jakarta.validation.constraints.NotEmpty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class MemberForm {

    @NotEmpty(message = "회원 이름은 필수 입니다")
    private String name;
    private String city;
    private String street;
    private String zipcode;

}
```

- @NoArgsConstructor과 @AllArgsConstructor 두 어노테이션을 함께 사용 시, 매개변수가 없는 기본 생성자와 모든 필드를 초기화하는 생성자가 동시에 생성

- 주로 DTO나 엔티티 객체에서 사용

