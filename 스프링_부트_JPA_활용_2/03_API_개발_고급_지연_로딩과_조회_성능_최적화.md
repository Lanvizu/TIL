인프런 강의 ['실전! 스프링 부트와 JPA 활용2 - API 개발과 성능 최적화'](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81%EB%B6%80%ED%8A%B8-JPA-API%EA%B0%9C%EB%B0%9C-%EC%84%B1%EB%8A%A5%EC%B5%9C%EC%A0%81%ED%99%94)
을 공부하며 정리한 내용입니다.

-----

# 간단한 주문 조회 V1: 엔티티를 직접 노출

- Lazy Fetch 시 연관 객체는 실제 사용 전까지 프록시 객체로 유지되며, 객체를 "건드리지 않으면" 데이터는 로드되지 않는다.
  
- 프록시 객체(예: ByteBuddyInterceptor)는 객체에 접근하는 순간 초기화되어 실제 데이터를 가져온다.

### OrderSimpleApiController

```Java
@RestController
@RequiredArgsConstructor
public class OrderSimpleApiController {

    private final OrderRepository orderRepository;
    /**
     * V1. 엔티티 직접 노출
     * - Hibernate5Module 모듈 등록, LAZY=null 처리
     * - 양방향 관계 문제 발생 -> @JsonIgnore
     */
    @GetMapping("/api/v1/simple-orders")
    public List<Order> ordersV1() {
        List<Order> all = orderRepository.findAllByString(new OrderSearch());
        for (Order order : all) {
            //getMember 까지는 프록시 객체, getName에서 jpa가 쿼리를 날려서 실제 데이터를 가져옴
            order.getMember().getName(); //Lazy 강제 초기화
            order.getDelivery().getAddress(); //Lazy 강제 초기환
        }
        return all;
    }
}
```

- 엔티티를 직접 노출
- order -> member, order -> delivery는 지연 로딩
  - 실제 엔티티 대신 프록시 존재
- jackson 라이브러리는 기본적으로 프록시 객체를 json으로 어떻게 생성하는 지 모름 -> 예외 발생

### JpashopApplication

```Java
@SpringBootApplication
public class JpashopApplication {

	public static void main(String[] args) {
		SpringApplication.run(JpashopApplication.class, args);
	}

	@Bean
	Hibernate5JakartaModule hibernate5Module() {
		Hibernate5JakartaModule hibernate5JakartaModule = new Hibernate5JakartaModule();
		//강제 지연 로딩 설정
//		hibernate5JakartaModule.configure(Hibernate5JakartaModule.Feature.FORCE_LAZY_LOADING, true);
		return hibernate5JakartaModule;
	}
}
```

- Hibernate5JakartaModule을 스프링 빈으로 등록하여 해결
  
- hibernate5Module.configure(Hibernate5Module.Feature.FORCE_LAZY_LOADING,true); 을 통해서 강제 지연 로딩 가능

![image](https://github.com/user-attachments/assets/1d87f116-6d95-4857-af3c-8ed1a224fab4)

- order -> member, member -> orders 양방향 연관관계를 계속 로딩
  
- 한곳을 @JsonIgnore 설정하여 무한루프를 해결 가능

> 가능한 API 응답으로는 엔티티를 외부로 노출하지 말자
>
> Hibernate5Module 보다는 DTO로 변환하여 반환하자

> 지연 로딩(LAZY)을 피하기 위해 즉시 로딩(EAGER)을 설정하면 안된다.
>
> 연관 관계가 필요 없는 경우에도 데이터를 항상 조회하여 성능 문제 발생 가능
>
> 즉시 로딩 시 성능 튜닝이 매우 어려워짐
>
> 항상 지연 로딩을 기본으로 설정, 최적화가 필요한 경우 페치 조인(fetch join) 사용

-----

# 간단한 주문 조회 V2: 엔티티를 DTO로 변환

### OrderSimpleApiController

```Java
   /**
 * V2. 엔티티를 조회해서 DTO로 변환(fetch join 사용X)
 * - 단점: 지연로딩으로 쿼리 N번 호출
 */
    @GetMapping("/api/v2/simple-orders")
    public Result ordersV2() {
        List<Order> orders = orderRepository.findAllByString(new OrderSearch());
        List<SimpleOrderDto> collect = orders.stream()
                .map(o -> new SimpleOrderDto(o))
                .collect(Collectors.toList());
        return new Result(collect);
    }

    @Data
    @AllArgsConstructor
    static class Result<T> {
        private T data;
    }

    @Data
    static class SimpleOrderDto {
        private Long orderId;
        private String name;
        private LocalDateTime orderDate;
        private OrderStatus orderStatus;
        private Address address;

        public SimpleOrderDto(Order order) {
            orderId = order.getId();
            name = order.getMember().getName(); //LAZY 초기화
            orderDate = order.getOrderDate();
            orderStatus = order.getStatus();
            address = order.getDelivery().getAddress(); //LAZY 초기화
        }
    }
```

- 엔티티를 DTO로 변환하는 일반적인 방법

- 쿼리가 총 1 + N + N 번 실행 (v1과 쿼리수 결과는 동일)

   - order 조회 1번 (order 조회 결과 수: N)
 
   - order -> member 지연 로딩 조회 N 번
 
   - order -> delivery 지연 로딩 조회 N 번
 
   - 지연 로딩은 영속성 컨텍스트에서 조회하므로, 이미 조회된 경우 쿼리를 생략한다.

> 그렇다고 EAGER로 변환 시 예측할 수 없는 쿼리가 발생

-----

# 간단한 주문 조회 V3: 엔티티를 DTO로 변환 - 페치 조인 최적화

- 성능 문제의 거의 90% 는 N + 1 에서 발생

- 이를 해결하기 위해 페치 조인 적용

### OrderSimpleApiController - 추가

```Java
    @GetMapping("api/v3/simple-orders")
    public Result orderV3() {
        List<Order> orders = orderRepository.findAllWithMemberDelivery();
        List<SimpleOrderDto> collect = orders.stream()
                .map(o -> new SimpleOrderDto(o))
                .collect(Collectors.toList());
        return new Result(collect);
    }
```

### OrderRepository

```Java
public List<Order> findAllWithMemberDelivery() {
        return em.createQuery(
                "select o from Order o" +
                        " join fetch o.member m" +
                        " join fetch o.delivery d", Order.class
        ).getResultList();
    }
```

- 엔티티를 페치 조인(fetch join)을 사용해서 쿼리 1번에 조회

- order -> member, order -> delivery는 이미 조회 된 상태이므로 지연 로딩(Lazy Loading) X

> 실무에서 많이 사용하는 방식

-----

# 간단한 주문 조회 V4: JPA에서 DTO로 바로 조회

### OrderSimpleApiController - 추가

```Java
@GetMapping("api/v4/simple-orders")
    public Result ordersV4() {
        List<OrderSimpleQueryDto> orderDtos = orderRepository.findOrderDtos();
        return new Result(orderDtos);
    }
```

### OrderRepository

```Java
public List<OrderSimpleQueryDto> findOrderDtos() {
        return em.createQuery(
                "select new jpashop.jpashop.repository.OrderSimpleQueryDto(o.id, m.name, o.orderDate, o.status, d.address)" +
                        " from Order o" +
                        " join o.member m" +
                        " join o.delivery d", OrderSimpleQueryDto.class
                ).getResultList();
    }
```

### OrderSimpleQueryDto

```Java
package jpashop.jpashop.repository;

import jpashop.jpashop.domain.Address;
import jpashop.jpashop.domain.OrderStatus;
import lombok.Data;

import java.time.LocalDateTime;

@Data
public class OrderSimpleQueryDto {
    private Long orderId;
    private String name;
    private LocalDateTime orderDate;
    private OrderStatus orderStatus;
    private Address address;

    public OrderSimpleQueryDto(Long orderId, String name, LocalDateTime orderDate, OrderStatus orderStatus, Address address) {
        this.orderId = orderId;
        this.name = name;
        this.orderDate = orderDate;
        this.orderStatus = orderStatus;
        this.address = address;
    }
}
```

- 일반적인 SQL을 사용할 때 처럼 원하는 값을 선택해서 조회
- new 명령어를 통해 JPQL의 결과를 DTO로 즉시 변환
- SELECT 절에서 원하는 데이터를 직접 선택하므로 DB -> 애플리케이션 네트웍 용량 최적화(생각보다 미비)
- 리포지토리 재사용성 떨어짐, API 스펙에 맞춘 코드가 리포지토리에 들어가는 단점

> 물리적으로는 계층이 나눠져있지만, 논리적으로는 계층이 깨져있는 상태
>
> 대부분의 경우 성능차이가 거의 나지 않는다 (네트워크가 좋아서 + join에서 대부분의 성능 문제가 생김)

-----

# 정리

## 쿼리 방식 선택 권장 순서

1. 우선 엔티티를 DTO로 변환하는 방법을 선택한다.

2. 필요하면 페치 조인으로 성능을 최적화한다. -> 대부분의 성능 이슈가 해결

3. DTO로 직접 조회하는 방법을 사용한다.

4. 최후의 방법은 JPA가 제공하는 네이티브 SQL이나 스프링 JDBC Template로 직접 SQL을 사용

### V2

![image](https://github.com/user-attachments/assets/06d0f854-74a4-439a-8535-e84a653fd028)

### V3

![image](https://github.com/user-attachments/assets/c7f62340-5379-4fc9-b62b-b39d357313ec)

### V4

![image](https://github.com/user-attachments/assets/498d2239-1f76-46ef-bb79-330fe32dacc7)

> V2: 67ms
> 
> V3: 42ms
> 
> V4: 1.39s
> 
> 미미하지만 Response Time 차이가 존재했다.
> 
> 데이터가 많아진다면 차이는 더욱 커질 것이라고 생각한다.
