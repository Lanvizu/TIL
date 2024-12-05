# JWT (Json Web Token)

- 토큰 인증 방식의 한 가지 종류로, 클라이언트가 API 요청을 할 때 토큰을 통해 인증 & 인가를 진행하는 방식 

- Json 포맷을 이용해 사용자에 대한 속성을 저장하는 Claim 기반 Web Token

- 토큰 자체를 정보로 사용하는 Self-Contained 방식으로 정보를 전달한다.

## JWT 구조

- Header, Payload, Signature의 3 부분으로 이루어지며 각 부분을 Base64Url로 인코딩되어 전달된다.

- 추가로 각각의 부분을 이어 주기 위해, '.' 구분자를 사용하여 구분한다.

> Base64Url은 암호화된 문자열이 아니며 같은 문자열에 대해 항상 같은 인코딩 문자열을 반환한다.

### Header(헤더)

- typ, alg 두 가지 정보로 구성된다.

- alg: Signature를 해싱하기 위한 알고리즘을 지정

### Payload(페이로드)

- 토큰에서 사용할 정보의 조각들인 Claim(클레임)이 담겨 있다.

- 총 3가지로 나눠지며, Json(Key/Value) 형태로 다수의 정보를 넣어 전달할 수 있다.

1. Registered Claim(등록된 클레임)

    - 이미 정해진 종류의 데이터로 토큰 정보를 표현한다.

2. Public Claim(공개 클레임)

    - 사용자 정의 클레임으로 공개용 정보를 위해 사용된다.
  
    > 충돌 방지를 위해 URI 포맷을 이용한다.

3. Private Claim(비공개 클레임)

    - 사용자 정의 클레임으로 서버와 클라이언트 사이의 임의로 지정한 정보를 저장한다.

### Signature(서명)

- 토큰을 인코딩하거나 유효성 검증을 할 때 사용하는 고유한 암호화 코드

- Signature는 헤더와 페이로드의 값을 각각 Base64Url로 인코딩한 후, 비밀 키를 이용해 헤더에서 정의한 알고리즘으로 해싱을 한다.

- 이후 다시 Base64Url로 인코딩하여 생성한다.

## JWT의 장점

- 세션을 사용하지 않아 서버가 다수 존재하는 환경에서 유용하다.

> 세션을 사용하면 모든 서버에 세션 내용을 공유해야한다.

- 매 요청마다 DB 조회를 하지 않고 토큰 자체만으로 사용자의 정보와 권한을 알 수 있으므로 병목현상을 방지한다.

## JWT의 단점

- 토큰 자체에 정보를 담고 있으므로 양날의 검이 될 수 있다.

- 페이로드에 3종류의 클레임을 저장하므로, 정보가 많아질수록 토큰의 길이가 늘어나 네트워크에 부하를 줄 수 있다.

- 페이로드 자체가 암호화 된 것이 아니므로 중간에 Payload를 탈취하여 디코딩하면 데이터를 볼 수 있다.

> JWE로 암호화를 진행하거나 Payload에 중요 데이터를 넣지 않아야 한다.

- Stateless: JWT는 상태를 저장하지 않으므로 한 번 생성되면 제어가 불가능하다.

> 토큰을 임의로 삭제가 불가능하므로, 토큰 만료 시간을 꼭 넣어줘야한다.

- 클라이언트 측에서 토큰을 관리해야하므로, 토큰을 저장해야 한다.

## Refresh Token

- Access Token이 만료될 경우 Refresh Token을 통해 새로운 Access Token을 발급받는다.

- JWT 토큰 자체에 정보를 담고 있어 보안이 매우 취약하다.

> JWT 토큰을 탈취당할 경우 해당 사용자의 권한과 정보를 모두 빼앗긴다.

- 이를 보완하기 위해 Refresh Token이 사용된다.

    - Access Token의 유효 기간은 매우 짧게 설정
 
    - Refresh Token의 유효 기간은 길게 설정

![image](https://github.com/user-attachments/assets/e2e6e7fa-d115-4db1-9e96-1b759b534993)

- Access Token의 유효 기간이 짦음에도 Refresh Token이 만료될 때 까지 추가적인 로그인을 하지 않아도 된다.

> 세션이 유지되는 것과 같은 효과

### 세션과 JWT 인증 방법의 차이점

1. 확장성

    - 서버 측에서 상태를 유지하지 않기 때문에 분산 시스템에서 더 효율적이다.
  
    - 여러 서버에서 동일한 JWT를 검증할 수 있어 서버간 동기화가 필요하지 않다.
    
2. 리소스 효율성

    - 서버의 메모리를 사용하지 않아 서버 리소르를 절약할 수 있다.
    
3. 클라이언트 다양성

    - 모바일 애플리케이션, 싱글 페이지 애플리케이션(SPA), 크로스 도메인 인증 등 다양한 클라이언트 환경에서 사용하기 적합하다.
  
    - 쿠키를 사용할 수 없는 환겨에서도 JWT 인증이 가능하다.
    
4. 성능

    - 토큰 자체에 인증 정보가 담겨있어 추가적인 데이터베이스 조회를 할 필요가 없다.
  
    - 이는 인증 처리 속도를 향상시킬 수 있다.
    
5. 보안

    - 서명을 통해 데이터 무결성을 보장하므로, 토큰이 변조되면 쉽게 탐지 가능하다.
  
-----

# JWT 구현

> Spring Security 6.1부터 기존에 사용하던 and(), non-Lambda DSL Method가 Deprecated되고, Lambda DSL을 필수로 사용하도록 변경되었다.

### 기술 스택

- Spring Boot 3.3.1
- Spring Security 6.3.1
- JPA
- JWT(Access Token, Refresh Token)
- Spring Security 6.1 이후 lambda 문법을 이용한 코드 적용

## build.gradle

```
plugins {
	id 'java'
	id 'org.springframework.boot' version '3.3.5'
	id 'io.spring.dependency-management' version '1.1.6'
}

group = 'com.study'
version = '0.0.1-SNAPSHOT'

java {
	toolchain {
		languageVersion = JavaLanguageVersion.of(21)
	}
}

configurations {
	compileOnly {
		extendsFrom annotationProcessor
	}
}

repositories {
	mavenCentral()
}

dependencies {
	implementation 'org.springframework.boot:spring-boot-starter-data-jpa'
	implementation 'org.springframework.boot:spring-boot-starter-web'
	testImplementation 'org.springframework.boot:spring-boot-starter-test'
	compileOnly 'org.projectlombok:lombok'
	developmentOnly 'org.springframework.boot:spring-boot-devtools'
	runtimeOnly 'com.mysql:mysql-connector-j'
	annotationProcessor 'org.projectlombok:lombok'
	testRuntimeOnly 'org.junit.platform:junit-platform-launcher'

	// log4jdbc
	implementation 'org.bgee.log4jdbc-log4j2:log4jdbc-log4j2-jdbc4.1:1.16'

	//validation
	implementation 'org.springframework.boot:spring-boot-starter-validation'

	//spring security
	implementation 'org.springframework.boot:spring-boot-starter-security'
	testImplementation 'org.springframework.security:spring-security-test'

	//Jwt
	implementation 'io.jsonwebtoken:jjwt-api:0.11.5'
	implementation 'io.jsonwebtoken:jjwt-impl:0.11.5'
	implementation 'io.jsonwebtoken:jjwt-jackson:0.11.5'

	// gson
	implementation group: 'com.google.code.gson', name: 'gson', version: '2.8.9'

	// UUID
	implementation "com.fasterxml.uuid:java-uuid-generator:4.0.1"
}

tasks.named('test') {
	useJUnitPlatform()
}
```

## SpringBootApplication.java

**NiHoApplication**
```JAVA
package com.study.NIHo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

@EnableJpaAuditing
@SpringBootApplication(exclude = SecurityAutoConfiguration.class)
public class NiHoApplication {

	public static void main(String[] args) {
		SpringApplication.run(NiHoApplication.class, args);
	}

}
```

- 기본 로그인 페이지와 같이 로그인이 필요하지 않은 특정 상황에 대한 기본 보안 구성을 제거하기 위함이다.

## Spring Security Configuration

**SecurityConfig**

```JAVA
package com.study.NIHo.config.login;

import com.study.NIHo.config.login.security.filter.JwtAuthFilter;
import com.study.NIHo.config.login.security.handler.CustomAccessDeniedHandler;
import com.study.NIHo.config.login.security.handler.CustomAuthenticationEntryPointHandler;
import com.study.NIHo.api.user.enums.UserRole;
import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.HttpMethod;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configurers.AbstractHttpConfigurer;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;
import org.springframework.security.web.servlet.util.matcher.MvcRequestMatcher;
import org.springframework.web.servlet.handler.HandlerMappingIntrospector;

@Configuration
@EnableWebSecurity
@RequiredArgsConstructor
public class SecurityConfig {

    private final JwtAuthFilter jwtAuthFilter;

    private final CustomAuthenticationEntryPointHandler customAuthenticationEntryPointHandler;

    private final CustomAccessDeniedHandler customAccessDeniedHandler;

    @Bean
    public BCryptPasswordEncoder bCryptPasswordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    MvcRequestMatcher.Builder mvc(HandlerMappingIntrospector introspector) {
        return new MvcRequestMatcher.Builder(introspector);
    }

    @Bean
    public SecurityFilterChain config(HttpSecurity http, HandlerMappingIntrospector introspector) throws Exception {
        MvcRequestMatcher.Builder mvc = new MvcRequestMatcher.Builder(introspector);

        // white list (Spring Security 체크 제외 목록)
        MvcRequestMatcher[] permitAllWhiteList = {
                mvc.pattern("/login"),
                mvc.pattern("/register"),
                mvc.pattern("/token-refresh"),
                mvc.pattern("/favicon.ico"),
                mvc.pattern("/error")
        };

        // http request 인증 설정
        http.authorizeHttpRequests(authorize -> authorize
                .requestMatchers(permitAllWhiteList).permitAll()
                // 사용자 삭제는 관리자 권한만 가능
                .requestMatchers(HttpMethod.DELETE, "/user").hasRole(UserRole.ROLE_ADMIN.getRole())
                // 그 외 요청 체크
                .anyRequest().authenticated()
        );

        // form login disable
        http.formLogin(AbstractHttpConfigurer::disable);

        // logout disable
        http.logout(AbstractHttpConfigurer::disable);

        // csrf disable
        http.csrf(AbstractHttpConfigurer::disable);

        // session management
        http.sessionManagement(session -> session
                .sessionCreationPolicy(SessionCreationPolicy.STATELESS) // 세션 미사용
        );

        // before filter
        http.addFilterBefore(jwtAuthFilter, UsernamePasswordAuthenticationFilter.class);

        // exception handler
        http.exceptionHandling(conf -> conf
                .authenticationEntryPoint(customAuthenticationEntryPointHandler)
                .accessDeniedHandler(customAccessDeniedHandler)
        );

        // build
        return http.build();
    }
}
```

- LambdaDSL 방식을 사용해 Sercurity Configuration을 구성한다.

- JWT 방식을 사용하므로 form login, logout 등은 사용하지 않도록 설정한다.

- JWT를 사용하는 방식은 세션을 사용하지 않으므로 스프링 시큐리티에서 세션 미사용하도록 설정한다.

- 토큰 검증을 위한 Filter을 설정한다.

## JwtProvider.java

application.properties에 jwt.secret를 설정한다.

**JwtProvider**
```JAVA
package com.study.NIHo.config.login.security.provide;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.ExpiredJwtException;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.MalformedJwtException;
import io.jsonwebtoken.UnsupportedJwtException;
import io.jsonwebtoken.security.Keys;
import jakarta.annotation.PostConstruct;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;
import javax.crypto.SecretKey;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

@Component
@RequiredArgsConstructor
@Slf4j
public class JwtProvider {

    // jwt 만료 시간 1시간
    private static final long JWT_TOKEN_VALID = (long) 1000 * 60 * 30;

    @Value("${jwt.secret}")
    private String secret;

    private SecretKey key;

    @PostConstruct
    public void init() {
        key = Keys.hmacShaKeyFor(secret.getBytes());
    }

    /**
     * token Username 조회
     *
     * @param token JWT
     * @return token Username
     */
    public String getUsernameFromToken(final String token) {
        return getClaimFromToken(token, Claims::getId);
    }

    /**
     * token 사용자 속성 정보 조회
     *
     * @param token          JWT
     * @param claimsResolver Get Function With Target Claim
     * @param <T>            Target Claim
     * @return 사용자 속성 정보
     */
    public <T> T getClaimFromToken(final String token, final Function<Claims, T> claimsResolver) {
        // token 유효성 검증
        if (Boolean.FALSE.equals(validateToken(token)))
            return null;

        final Claims claims = getAllClaimsFromToken(token);

        return claimsResolver.apply(claims);
    }

    /**
     * token 사용자 모든 속성 정보 조회
     *
     * @param token JWT
     * @return All Claims
     */
    private Claims getAllClaimsFromToken(final String token) {
        return Jwts.parserBuilder()
                .setSigningKey(key)
                .build()
                .parseClaimsJws(token)
                .getBody();
    }

    /**
     * 토큰 만료 일자 조회
     *
     * @param token JWT
     * @return 만료 일자
     */
    public Date getExpirationDateFromToken(final String token) {
        return getClaimFromToken(token, Claims::getExpiration);
    }

    /**
     * access token 생성
     *
     * @param id token 생성 id
     * @return access token
     */
    public String generateAccessToken(final String id) {
        return generateAccessToken(id, new HashMap<>());
    }

    /**
     * access token 생성
     *
     * @param id token 생성 id
     * @return access token
     */
    public String generateAccessToken(final long id) {
        return generateAccessToken(String.valueOf(id), new HashMap<>());
    }

    /**
     * access token 생성
     *
     * @param id     token 생성 id
     * @param claims token 생성 claims
     * @return access token
     */
    public String generateAccessToken(final String id, final Map<String, Object> claims) {
        return doGenerateAccessToken(id, claims);
    }

    /**
     * JWT access token 생성
     *
     * @param id     token 생성 id
     * @param claims token 생성 claims
     * @return access token
     */
    private String doGenerateAccessToken(final String id, final Map<String, Object> claims) {
        return Jwts.builder()
                .setClaims(claims)
                .setId(id)
                .setIssuedAt(new Date(System.currentTimeMillis()))
                .setExpiration(new Date(System.currentTimeMillis() + JWT_TOKEN_VALID)) // 30분
                .signWith(key)
                .compact();
    }

    /**
     * refresh token 생성
     *
     * @param id token 생성 id
     * @return refresh token
     */
    public String generateRefreshToken(final String id) {
        return doGenerateRefreshToken(id);
    }

    /**
     * refresh token 생성
     *
     * @param id token 생성 id
     * @return refresh token
     */
    public String generateRefreshToken(final long id) {
        return doGenerateRefreshToken(String.valueOf(id));
    }

    /**
     * refresh token 생성
     *
     * @param id token 생성 id
     * @return refresh token
     */
    private String doGenerateRefreshToken(final String id) {
        return Jwts.builder()
                .setId(id)
                .setExpiration(new Date(System.currentTimeMillis() + (JWT_TOKEN_VALID * 2) * 24)) // 24시간
                .setIssuedAt(new Date(System.currentTimeMillis()))
                .signWith(key)
                .compact();
    }

    /**
     * token 검증
     *
     * @param token JWT
     * @return token 검증 결과
     */
    public Boolean validateToken(final String token) {
        try {
            Jwts.parserBuilder()
                    .setSigningKey(key)
                    .build()
                    .parseClaimsJws(token);
            return true;
        } catch (SecurityException e) {
            log.warn("Invalid JWT signature: {}", e.getMessage());
        } catch (MalformedJwtException e) {
            log.warn("Invalid JWT token: {}", e.getMessage());
        } catch (ExpiredJwtException e) {
            log.warn("JWT token is expired: {}", e.getMessage());
        } catch (UnsupportedJwtException e) {
            log.warn("JWT token is unsupported: {}", e.getMessage());
        } catch (IllegalArgumentException e) {
            log.warn("JWT claims string is empty: {}", e.getMessage());
        }

        return false;
    }
}
```

## JwtAuthFilter.java

```JAVA
package com.study.NIHo.config.login.security.filter;

import com.study.NIHo.api.user.application.UserGetService;
import com.study.NIHo.config.login.security.provide.JwtProvider;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Collections;
import lombok.RequiredArgsConstructor;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.OncePerRequestFilter;

@Component
@RequiredArgsConstructor
public class JwtAuthFilter extends OncePerRequestFilter {

    private final JwtProvider jwtProvider;

    private final UserGetService userGetService;

    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        final String token = request.getHeader("Authorization");

        String username = null;

        // Bearer token 검증 후 user name 조회
        if(token != null && !token.isEmpty()) {
            String jwtToken = token.substring(7);

            username = jwtProvider.getUsernameFromToken(jwtToken);
        }

        // token 검증 완료 후 SecurityContextHolder 내 인증 정보가 없는 경우 저장
        if(username != null && !username.isEmpty() && SecurityContextHolder.getContext().getAuthentication() == null) {
            // Spring Security Context Holder 인증 정보 set
            SecurityContextHolder.getContext().setAuthentication(getUserAuth(username));
        }

        filterChain.doFilter(request,response);
    }

    /**
     * token의 사용자 idx를 이용하여 사용자 정보 조회하고, UsernamePasswordAuthenticationToken 생성
     * @param username 사용자 idx
     * @return 사용자 UsernamePasswordAuthenticationToken
     */
    private UsernamePasswordAuthenticationToken getUserAuth(String username) {
        var userInfo = userGetService.getUserById(Long.parseLong(username));

        return new UsernamePasswordAuthenticationToken(userInfo.id(), userInfo.password(),
                Collections.singleton(new SimpleGrantedAuthority(userInfo.userRole().name())));
    }

}
```

- 요청을 필터링하고 JWT를 사용해 인증 및 권한 부여를 처리한다.

## Handler 구현

**CustomAccessDeniedHandler**

```JAVA
package com.study.NIHo.config.login.security.handler;

import com.google.gson.JsonObject;
import com.study.NIHo.config.exception.common.enums.ApiExceptionEnum;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import lombok.extern.slf4j.Slf4j;
import org.springframework.security.access.AccessDeniedException;
import org.springframework.security.web.access.AccessDeniedHandler;
import org.springframework.stereotype.Component;

/**
 * Custom Access Denied Handler Handler
 */
@Slf4j
@Component
public class CustomAccessDeniedHandler implements AccessDeniedHandler {

    @Override
    public void handle(HttpServletRequest request, HttpServletResponse response, AccessDeniedException accessDeniedException)
            throws IOException {
        log.info("[CustomAccessDeniedHandler] :: {}", accessDeniedException.getMessage());
        log.info("[CustomAccessDeniedHandler] :: {}", request.getRequestURL());
        log.info("[CustomAccessDeniedHandler] :: 토근 정보가 만료되었거나 존재하지 않음");

        response.setStatus(ApiExceptionEnum.FORBIDDEN.getStatus().value());
        response.setCharacterEncoding("UTF-8");
        response.setContentType("application/json; charset=UTF-8");

        JsonObject returnJson = new JsonObject();
        returnJson.addProperty("errorCode", ApiExceptionEnum.FORBIDDEN.getCode());
        returnJson.addProperty("errorMsg", ApiExceptionEnum.FORBIDDEN.getMessage());

        PrintWriter out = response.getWriter();
        out.print(returnJson);
    }
}
```

**CustomAuthenticationEntryPointHandler**

```JAVA
package com.study.NIHo.config.login.security.handler;

import com.google.gson.JsonObject;
import com.study.NIHo.config.exception.common.enums.ApiExceptionEnum;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import lombok.extern.slf4j.Slf4j;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.web.AuthenticationEntryPoint;
import org.springframework.stereotype.Component;

/**
 * Custom Authentication Entry Point Handler
 */
@Slf4j
@Component
public class CustomAuthenticationEntryPointHandler implements AuthenticationEntryPoint {

    @Override
    public void commence(HttpServletRequest request, HttpServletResponse response, AuthenticationException authException)
            throws IOException {
        log.info("[CustomAuthenticationEntryPointHandler] :: {}", authException.getMessage());
        log.info("[CustomAuthenticationEntryPointHandler] :: {}", request.getRequestURL());
        log.info("[CustomAuthenticationEntryPointHandler] :: 토근 정보가 만료되었거나 존재하지 않음");

        response.setStatus(ApiExceptionEnum.ACCESS_DENIED.getStatus().value());
        response.setCharacterEncoding("UTF-8");
        response.setContentType("application/json; charset=UTF-8");

        JsonObject returnJson = new JsonObject();
        returnJson.addProperty("errorCode", ApiExceptionEnum.ACCESS_DENIED.getCode());
        returnJson.addProperty("errorMsg", ApiExceptionEnum.ACCESS_DENIED.getMessage());

        PrintWriter out = response.getWriter();
        out.print(returnJson);
    }

}
```

- AccessDeniedHandler의 구현체인 CustomAccessDeniedHandler를 구현한다.

## RTR (Refresh Token Rotation)

- Refresh Token을 단 한 번만 사용할 수 있도록 설정하는 방법

- RefreshToken은 Stateless로 클라이언트가 관리하므로 서버 입장에서는 탈취 여부를 알 수 없다.

- RefreshToken의 만료시간을 길어 탈취 당할 경우 장시간 악용될 수 있다.

![image](https://github.com/user-attachments/assets/387f1406-11cf-496d-909f-5bb3b89fb782)

- 이전 단계의 Refresh Token을 통해 다음 단계의 Refresh Token과 Access Token을 얻는 방식

- 클러스터링 환경에서 빠르고 편하게 Token을 처리하려면 **Redis**가 적절하다.

- Redis에 Refresh Token을 저장하고 처리하는 방식으로 진행된다.

> 현재는 기능 구현 목적으로 참고 블로그를 따라가는 방식으로 공부한 상황이다.
>
> 이 후에 HashMap 대신 Redis를 사용해 다시 수정할 예정이다.

- 2가지 경우에 Refresh Toekn을 폐기

  1. 로그일할 때, 사용자의 기존 Refresh Token을 폐기하고 새로운 Refresh Token을 발급
 
  2. 기존 Refresh Token을 사용해 Access Token과 Refresh Token을 갱신할 때.

- 공격자가 Refresh Token을 탈취하여 사용 시 기존 사용자는 Refresh Token을 사용하지 못 함.

- 이때 기존 사용자가 Token을 갱신하기 위해 다시 로그인을 하여, 공격자가 탈취한 Refresh Token을 폐기시킨다.

**RefreshToken**

```JAVA
package com.study.NIHo.api.token.vo;

import com.study.NIHo.api.token.exception.RefreshTokenException;
import com.study.NIHo.api.token.exception.RefreshTokenExceptionResult;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import lombok.AccessLevel;
import lombok.NoArgsConstructor;

/**
 * RefreshToken 저장 객체
 *
 * <p>
 * 해당 프로젝트는 스프링 시큐리티 위주의 프로젝트이기 때문에 간단하게 구현
 * 운영환경에서는 해당 방식이 아닌 Redis 사용을 추천
 * Redis 에서 만료시간을 설정하여 관리
 * </p>
 */
@NoArgsConstructor(access = AccessLevel.PRIVATE)
public class RefreshToken {

    protected static final Map<String, Long> refreshTokens = new HashMap<>();

    /**
     * refresh token get
     *
     * @param refreshToken refresh token
     * @return id
     */
    public static Long getRefreshToken(final String refreshToken) {
        return Optional.ofNullable(refreshTokens.get(refreshToken))
                .orElseThrow(() -> new RefreshTokenException(RefreshTokenExceptionResult.NOT_EXIST));
    }

    /**
     * refresh token put
     *
     * @param refreshToken refresh token
     * @param id id
     */
    public static void putRefreshToken(final String refreshToken, Long id) {
        refreshTokens.put(refreshToken, id);
    }

    /**
     * refresh token remove
     *
     * @param refreshToken refresh token
     */
    private static void removeRefreshToken(final String refreshToken) {
        refreshTokens.remove(refreshToken);
    }

    // user refresh token remove
    public static void removeUserRefreshToken(final long refreshToken) {
        for(Map.Entry<String, Long> entry : refreshTokens.entrySet()) {
            if(entry.getValue() == refreshToken) {
                removeRefreshToken(entry.getKey());
            }
        }
    }

}
```

- RefreshToken을 static하게 관리한다.

**RefreshTokenServiceImpl**

```JAVA
package com.study.NIHo.api.token.application.impl;

import com.study.NIHo.api.token.dto.response.RefreshTokenResponseDTO;
import com.study.NIHo.api.token.exception.RefreshTokenException;
import com.study.NIHo.api.token.exception.RefreshTokenExceptionResult;
import com.study.NIHo.config.login.security.provide.JwtProvider;
import com.study.NIHo.api.token.application.RefreshTokenService;
import com.study.NIHo.api.token.vo.RefreshToken;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class RefreshTokenServiceImpl implements RefreshTokenService {

    private final JwtProvider jwtProvider;

    /**
     * refresh token을 이용하여 access token, refresh token 재발급
     *
     * @param refreshToken refresh token
     * @return RefreshTokenResponseDTO
     */
    @Override
    public RefreshTokenResponseDTO refreshToken(final String refreshToken) {
        // refresh token 유효성 검증
        checkRefreshToken(refreshToken);

        // refresh token id 조회
        var id = RefreshToken.getRefreshToken(refreshToken);

        // 새로운 access token 생성
        String newAccessToken = jwtProvider.generateAccessToken(id);

        // 기존에 가지고 있는 사용자의 refresh token 제거
        RefreshToken.removeUserRefreshToken(id);

        // 새로운 refresh token 생성 후 저장
        String newRefreshToken = jwtProvider.generateRefreshToken(id);
        RefreshToken.putRefreshToken(newRefreshToken, id);

        return RefreshTokenResponseDTO.builder()
                .accessToken(newAccessToken)
                .refreshToken(newRefreshToken)
                .build();
    }

    /**
     * refresh token 검증
     *
     * @param refreshToken refresh token
     */
    private void checkRefreshToken(final String refreshToken) {
        if(Boolean.FALSE.equals(jwtProvider.validateToken(refreshToken)))
            throw new RefreshTokenException(RefreshTokenExceptionResult.INVALID);
    }

}
```
- Token을 갱신 시 동작하는 Service.

- Refresh Token이 검증 시 기존 사용자의 Refresh Token을 폐기하고 새로운 Token을 생성 후 저장한다.

- Refresh Token이 검증되지 않은 경우 Custom Exception을 발생시킨다.

**LoginServiceImpl**

```JAVA
package com.study.NIHo.api.login.application.impl;

import com.study.NIHo.api.login.application.LoginService;
import com.study.NIHo.api.login.dto.request.LoginRequestDTO;
import com.study.NIHo.api.login.dto.response.LoginResponseDTO;
import com.study.NIHo.api.login.exception.LoginException;
import com.study.NIHo.api.login.exception.LoginExceptionResult;
import com.study.NIHo.api.token.vo.RefreshToken;
import com.study.NIHo.api.user.application.UserGetService;
import com.study.NIHo.api.user.dto.response.UserGetResponseDTO;
import com.study.NIHo.config.login.security.provide.JwtProvider;
import jakarta.transaction.Transactional;
import lombok.RequiredArgsConstructor;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class LoginServiceImpl implements LoginService {

    private final UserGetService userGetService;

    private final BCryptPasswordEncoder bCryptPasswordEncoder;

    private final JwtProvider jwtProvider;

    @Override
    @Transactional
    public LoginResponseDTO login(final LoginRequestDTO loginRequestDTO) {
        // 사용자 정보 조회
        UserGetResponseDTO userInfo = userGetService.getUserByEmail(loginRequestDTO.getEmail());

        // password 일치 여부 체크
        if(!bCryptPasswordEncoder.matches(loginRequestDTO.getPassword(), userInfo.password()))
            throw new LoginException(LoginExceptionResult.NOT_CORRECT);

        // jwt 토큰 생성
        String accessToken = jwtProvider.generateAccessToken(userInfo.id());

        // 기존에 가지고 있는 사용자의 refresh token 제거
        RefreshToken.removeUserRefreshToken(userInfo.id());

        // refresh token 생성 후 저장
        String refreshToken = jwtProvider.generateRefreshToken(userInfo.id());
        RefreshToken.putRefreshToken(refreshToken, userInfo.id());

        return LoginResponseDTO.builder()
                .accessToken(accessToken)
                .refreshToken(refreshToken)
                .build();
    }

}
```

- 사용자가 로그인 하면 기존에 저장되어 있던 사용자의 Refresh Token을 모두 제거한다.

- 그리 새로운 토큰을 생성하고 저장한 후 사용자에게 반환한다.
