출처: https://tech.kakaoenterprise.com/127 [카카오엔터프라이즈 기술블로그 Tech&(테크앤):티스토리]

해당 블로그를 보고 공부한 API 문서 작성 가이드 라인을 정리

-----

# API 문서 구성

서버와 클라이언트 간 특정 기술을 사용하기 위한 **약속**

  - 기능들만 나열한 API 명세서 X

## 개요

기술 문서의 서론 -> 독자들에게 본문의 요약, 작성 배경, 관련된 개념 등을 설명하는 역할

즉, 글을 읽을 때 도움을 주는 가이드 역할

API 문서에서의 서론의 역할 - **개요(Overwiew)**

전반적인 API 소개와 동작 설명

- API 소개, 개발 배경, 비즈니스 목적 등 + 공통 요청 Request 및 응당 Response 형식, 공통 에러 타입 등

### API 소개

API - 특정 목적을 가지고 탄생한 기능

외부 독자들이 읽는다는 생각으로 단순 API의 기능 설명 보다 API의 개발 배경, 비즈니스 목적, 장점 등을 포함

-> 외부 개발자들도 이해하기 편함

### 공통 요청 / 응답 형식

보통 API는 통일된 방식으로 API를 호출 -> 개발자에 따라 문서의 구성이 달라짐

ex) API 요청에 사용된 데이터 형식이 ‘applicatoin/json’으로 제한, 또는 ‘application/x-www-form-urlencode’로 표현 등

응답도 마찬가지로 표현하는 방식이 다른 경우 존재 -> 성공 실패 여부를 success 필드 설정 또는 상태 코드를 통해 제공

-> 이런 부분이 API 문서에 정확하게 반영되어야 함

공통된 요청과 응답 형식을 정리해 독자가 API를 접근 방법 파악할 수 있도록 도와줌

-> 각각의 API 상단에 반복하여 정리하지 않아도 되므로 가독성 또한 상승.

### 공통 에러

API 간 공통 에러가 존재 시, 문서의 한 섹션에 에러 코드를 모아 관리하는 것이 효율적임

API에 에러 코드를 각각 추가하지 않아도 되며, 변경도 한곳에서만 해도 되니 문서 정합성 유지에도 도움이 됨

## 시작하기(Getting Started)

특정 API를 호출하기 전 인증 API 등의 선제적 API 호출 조건이나 관리자 사이트 등에서 인증키 정보가 필요한 경우 존재

이를 시작 가이드에 추가해 정보를 제공하여 사용자가 이용할 때 누락이 없도록 명시해야 함

### 사전 작업

계정 등록이나 API KEY 등록 처럼 인증하는 사전 작업이 필요할 수 있음.

시작 가이드에는 사전에 인증키(APP KEY)를 어떻게 발급할 수 있고 어떤 용도로 사용되는지 상세한 설명 필요

![image](https://github.com/user-attachments/assets/c543dd8f-26d2-4731-8a70-9357a86caa8c)

### API 사용 시퀀스

사용 시퀀스가 존재하는 경우 외부 개발자들은 스스로 탐구하는 자세로 여러 API를 순서에 맞지 않게 호출해 원하지 않는 결과 도출 가능

넘버링 형식으로 시퀀스를 정리하는 것이 좋음

## API 레퍼런스

API 별 요청과 응답을 정리해 놓은 문서

주로 **요청(Request)**와 **응답(Response)**로 구성

### 요청 (Request)

API 요청을 제대로 하려면 -> 특정 항목들을 일정 포맷에 따라 호출

카카오 엔터프라이즈 팀 - Request Syntax, Request Header, Request Element로 구분하고, 모든 서비스에 이 구성을 적용

![image](https://github.com/user-attachments/assets/e9d22134-8473-4ebb-a146-6b74066f0fc1)

1. Request Syntax

API의 형태, 구조에 대한 정의

어떤 메서드를 사용하고, 요청 URL의 형태, 코드 예제가 함께 제공

![image](https://github.com/user-attachments/assets/8f5b7c7d-37b8-4832-94eb-6666b5bd9eaf)

메서드(POST)와 요청 URL, 코드 예제 예시

2. Request Header

요청에 대한 추가 정보를 포함

인증을 위한 정보나 메시지의 총 길이(Content-Length), 형식(Content-Type)을 Header에 작성하기도 함

![image](https://github.com/user-attachments/assets/c37bc1b1-e742-4426-8789-050ff083a47e)

3. Request Element

해당 요청의 실제 메시지/내용이 해당

API를 요청하기 위한 파라미터와 파라미터의 유형, 필수 여부와 설명, 제약 사항 등이 제공되어야 함.

정보를 불러올 때 사용하는 GET 메서드에서 Element가 없는 요청이 존재

### 응답 (Response)

API 요청에 대한 결과값을 의미

ex) 메세지를 전송했을 때 메세지가 정상적으로 전송되었는지 전송 결과를 확인 가능

1. Response Element

API 요청에 대한 결과값을 확인 가능

요청한 API의 메서드에 따라 응답 형태가 달라질 수 있음

POST - Body에 값을 실어보낼 때 해당 값이 잘 저장되거나, 전달되었는지 성공여부

GET - 특정 정보를 조회하거나 받아올 때 값들을 코드로 확인하거나 자동적으로 다운로드 되기도 함

필드, 필드 유형, 필수 여부와 설명이 제공되어야 함

# 좋은 API 문서 작성 Tips

## API 리스트업

하나의 API 레퍼런스 안에 다양한 API 제공

API를 종류에 따라 구분하고 한눈에 리스트업한 후 링크를 활용해 바로가기를 하여 관리

![image](https://github.com/user-attachments/assets/69edc307-c11d-48d7-9aed-bf8eb3a13c48)

## 시작적 UI 활용

API는 기술의 변화와 사용자의 피드백을 반영하여 지속적으로 업데이트

업데이트된 API에 따라 가지 못한 문서는 뒤떨어진(out of date)문서가 됨

지속적인 업데이트가 필요

