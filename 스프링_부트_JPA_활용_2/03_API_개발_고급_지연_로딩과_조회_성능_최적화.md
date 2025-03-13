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
