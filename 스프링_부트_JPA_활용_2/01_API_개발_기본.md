인프런 강의 ['실전! 스프링 부트와 JPA 활용2 - API 개발과 성능 최적화'](https://www.inflearn.com/course/%EC%8A%A4%ED%94%84%EB%A7%81%EB%B6%80%ED%8A%B8-JPA-API%EA%B0%9C%EB%B0%9C-%EC%84%B1%EB%8A%A5%EC%B5%9C%EC%A0%81%ED%99%94)
을 공부하며 정리한 내용입니다.

-----

# 회원 등록 API

### MemnerAPIController

```Java
package jpashop.jpashop.api;

import jakarta.validation.Valid;
import jpashop.jpashop.domain.Member;
import jpashop.jpashop.service.MemberService;
import lombok.Data;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController // Controller + ResponseBody
@RequiredArgsConstructor
public class MemberApiController {

    private final MemberService memberService;

    /**
     * 등록 V1: 요청 값으로 Member 엔티티를 직접 받는다.
     * 문제점
     * - 엔티티에 프레젠테이션 계층을 위한 로직이 추가된다.
     * - 엔티티에 API 검증을 위한 로직이 들어간다. (@NotEmpty 등등)
     * - 실무에서는 회원 엔티티를 위한 API가 다양하게 만들어지는데, 한 엔티티에 각각의 API를 위한
     모든 요청 요구사항을 담기는 어렵다.
     * - 엔티티가 변경되면 API 스펙이 변한다.
     * 결론
     * - API 요청 스펙에 맞추어 별도의 DTO를 파라미터로 받는다.
     */
    @PostMapping("/api/v1/members")
    public CreateMemberResponse saveMemberV1(@RequestBody @Valid Member member) {
        Long id = memberService.join(member);
        return new CreateMemberResponse(id);
    }

    @Data
    static class CreateMemberResponse {
        private Long id;
        public CreateMemberResponse(Long id) {
            this.id = id;
        }
    }
}
```

## V1 엔티티를 Request Body에 직접 매핑

- 문제점
  - 엔티티에 프레젠테이션 계층을 위한 로직이 추가된다.
  - 엔티티에 API 검증을 위한 로직이 들어간다. (@NotEmpty 등)
  - 실무에서 회원 엔티티를 위한 다양한 API가 생성되는데, 하나의 엔티티에서 각각의 API 요청 요구사항을 담기 어렵다.
  - 엔티티가 변경되면 API 스펙이 변한다.
- 결론
  - API 요청 스펙에 맞춰 별도의 DTO를 파라미터로 받는다.
 
## V2 엔티티 대신에 DTO를 RequestBody에 매핑

### MemberAPIController

```Java
/**
     * 등록 V2: 요청 값으로 Member 엔티티 대신에 별도의 DTO를 받는다.
     */
    @PostMapping("/api/v2/members")
    public CreateMemberResponse saveMemberV2(@RequestBody @Valid CreateMemberRequest request) {
        Member member = new Member();
        member.setName(request.name);

        Long id = memberService.join(member);
        return new CreateMemberResponse(id);
    }

    @Data
    static class CreateMemberRequest {
        private String name;
    }
    @Data
    static class CreateMemberResponse {
        private Long id;
        public CreateMemberResponse(Long id) {
            this.id = id;
        }
    }
```

- 기존 Member 엔티티 댇신 RequestBody를 매핑한다.
- 엔티티와 프레젠테이션 계층을 위한 로직을 분리 가능하다.
- 엔티티와 API 스펙을 독집적으로 관리할 수 있다.
- 엔티티가 변경되어도 API 스펙이 변하지 않는다.

  > 실무에서는 엔티티를 API 스펙에 노출하면 안된다.

-----

# 회원 수정 API

### MemberAPIController

```Java
@PatchMapping("/api/v2/members/{id}")
    public UpdateMemberResponse updateMemberV2(
            @PathVariable("id") Long id,
            @RequestBody @Valid UpdateMemberRequest request) {

        memberService.updateName(id, request.name);
        // 커맨드와 쿼리를 철저히 분리하여 코드 개발
        // 데이터를 업데이트 하는 코드와 조회하는 코드를 분리하여 진행하자.
        Member findMember = memberService.findOne(id);
        return new UpdateMemberResponse(findMember.getId(), findMember.getName());
    }

    @Data
    static class UpdateMemberRequest {
        private String name;
    }

    @Data
    @AllArgsConstructor
    static class UpdateMemberResponse {
        private Long id;
        private String name;
    }
```

- 회원 수정도 DTO를 요청 파라미터에 매핑

### MemberService

```Java
// 회원 이름 변경
    @Transactional
    public void updateName(Long id, String name) {
        Member member = memberRepository.findOne(id);
        member.setName(name);
    }
```

> 강의에선 PutMapping으로 회원 정보를 업데이트하는데, PUT 방식은 전체 업데이트를 진행할 때 사용
>
> PATCH나 POST 방식을 통해 부분 업데이트를 진행

# Command와 Query를 분리 (Command Query Separation)

- 하나의 함수나 메서드가 오직 상태를 변경하는 작업 또는 데이터를 변경하는 작업 중 하나만 수행하도록 설계

### Command

- 데이터 생성, 수정, 삭제처럼 시스템의 상태를 변경하는 작업을 수행

- 일반적으로 트랜잭션을 통해 안정적인 상태 변경을 보장하며 실행 결과로는 성공 여부나 생성된 식별자만 반환

- 반환 값이 없거나 최소한의 정보만 전달

### Query

- 데이터베이스나 캐시 등에서 데이터를 조회하고 그 결과를 반환하는 역할에 집중

- 상태 변경 없이 순수하게 읽기 전용 작업을 수행하며, 별도의 최적화나 캐싱 전략 적용 가능

```Java
memberService.updateName(id, request.name); // Command
Member findMember = memberService.findOne(id); // Query
```

-----

# 회원 조회 API

## 회원 조회 V1: 응답 값으로 엔티티를 직접 외부에 노출

### MemberAPIController

```Java
/**
     * 조회 V1: 응답 값으로 엔티티를 직접 외부에 노출한다.
     * 문제점
     * - 엔티티에 프레젠테이션 계층을 위한 로직이 추가된다.
     * - 기본적으로 엔티티의 모든 값이 노출된다.
     * - 응답 스펙을 맞추기 위해 로직이 추가된다. (@JsonIgnore, 별도의 뷰 로직 등등)
     * - 실무에서는 같은 엔티티에 대해 API가 용도에 따라 다양하게 만들어지는데, 한 엔티티에 각각의 API를 위한 프레젠테이션 응답 로직을 담기는 어렵다.
     * - 엔티티가 변경되면 API 스펙이 변한다.
     * - 추가로 컬렉션을 직접 반환하면 항후 API 스펙을 변경하기 어렵다.(별도의 Result 클래스 생성으로 해결)
     * 결론
     * - API 응답 스펙에 맞추어 별도의 DTO를 반환한다.
     */
    //조회 V1: 안 좋은 버전, 모든 엔티티가 노출, @JsonIgnore -> 이건 정말 최악, api가 이거 하나인가! 화면에 종속적이지 마라!
    @GetMapping("/api/v1/members")
    public List<Member> membersV1() {
        return memberService.findMembers();
    }
```

> 엔티티를 외부에 노출하지 말자
>
> 실무에서는 member엔티티의 데이터가 필요한 API가 계속 증가한다.
>
> API 마다 필요한 필드가 다르므로 각 API 스펙에 맞는 별도의 DTO를 노출해야한다.

## 회원조회 V2: 응답 값으로 엔티티가 아닌 별도의 DTO 사용

### MemberAPIController

```Java
/**
     * 조회 V2: 응답 값으로 엔티티가 아닌 별도의 DTO를 반환한다.
     */
    @GetMapping("/api/v2/members")
    public Result membersV2() {
        List<Member> findMembers = memberService.findMembers();
        List<MemberDto> collect = findMembers.stream()
                .map(m -> new MemberDto(m.getName()))
                .collect(Collectors.toList());
        return new Result(collect);
    }

    @Data
    @AllArgsConstructor
    static class Result<T> {
        private T data;
    }

    @Data
    @AllArgsConstructor
    static class MemberDto {
        private String name;
    }
```

- 엔티티를 DTO에 변환해서 반환
- 엔티티가 변해도 API 스펙이 변경되지 않는다.
- 추가로 Result 클래스로 컬렉션을 감싸서 향후 필요한 필드를 추가할 수 있다.
- Result<T> 클래스는 제네릭 클래스로, 내부에 T 타입의 data 필드를 갖고 있다.
> 즉, Result 객체는 "data"라는 필드에 실제 반환할 데이터를 담아주는 래퍼(wrapper) 역할

## Json 구조 설계

- 단순한 배열 형태를 사용하는 대신 객체 내부에 특정 키(data)에 배열을 포함시켜 반환하는 방식

- 나중에 추가적인 정보를 쉽게 확장시켜 포함시킬 수 있다. + 유연성
  
``` Json
[
  { "id": 1, "name": "Alice" },
  { "id": 2, "name": "Bob" }
]
```

```Json
{
  "data": [
    { "id": 1, "name": "Alice" },
    { "id": 2, "name": "Bob" }
  ],
  "status": "success",
  "message": "OK"
}
```
