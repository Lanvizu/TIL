#define _CRT_SECURE_NO_WARNINGS
#define HEIGHT 512
#define WIDTH 512
#define IMAGE_SIZE WIDTH * HEIGHT
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

const char* filePath = "../Raw_Image/lena.img"; // 원본 파일 위치
const char* outputFilePath = "../Raw_Image/lenaOutput.img"; // 출력 파일 위치

unsigned char* img_org = NULL; //원본 이미지
unsigned char* img_sav = NULL; //출력 이미지

//원본,출력 파일
FILE* inFilePtr;
FILE* outFilePtr;

void rotate_90(void);
void rotate_180(void);
void rotate_270(void);
void mirror(void);
void flip(void);

void loadFile(void);
void saveFile(void);
void startChange(int);
void processUserInput(void);

int main(void) {
    loadFile();
    processUserInput();
    saveFile();
    return 0;
}

//원본 파일 로드
void loadFile() {
    // 동적 할당
    img_org = (unsigned char*)malloc(sizeof(unsigned char) * IMAGE_SIZE);
    img_sav = (unsigned char*)malloc(sizeof(unsigned char) * IMAGE_SIZE);
    if (img_sav == NULL || img_org == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    // 파일 열기
    inFilePtr = fopen(filePath, "rb");
    if (inFilePtr == NULL) {
        perror("파일 열기 실패");
        exit(1);
    }

    //읽어온 파일 원본(img_org)에 저장
    fread(img_org, sizeof(unsigned char), IMAGE_SIZE, inFilePtr);
    
    fclose(inFilePtr);
}
// 파일 저장
void saveFile() {
    outFilePtr = fopen(outputFilePath, "wb");
    if (outFilePtr == NULL) {
        perror("파일 열기 실패");
        exit(1);
    }
    // 파일 입력
    fwrite(img_sav, sizeof(unsigned char), IMAGE_SIZE, outFilePtr);
    fclose(outFilePtr);

    //동적 할당 해제
    free(img_org);
    free(img_sav);
}

//유저 입력
void processUserInput() {
    int choice = 0; // 사용자 입력을 저장할 변수

    printf("다음 중 원하는 작업을 선택하세요.\n");
    printf("1: 오른쪽으로 90도 회전\n");
    printf("2: 오른쪽으로 180도 회전\n");
    printf("3: 왼쪽으로 90도 회전\n");
    printf("4: 좌우 대칭\n");
    printf("5: 상하 대칭\n");

    scanf("%d", &choice);
    while (0 >= choice || 5 < choice) {
        printf("다시 입력해주세요\n");
        scanf("%d", &choice);
    }
    startChange(choice);
}

//입력 받은 대로 실행
void startChange(int choice) {
    switch (choice) {
    case 1:
        rotate_90();
        break;
    case 2:
        rotate_180();
        break;
    case 3:
        rotate_270();
        break;
    case 4:
        mirror();
        break;
    case 5:
        flip();
        break;
    default:
        printf("1에서 5 사이의 값을 입력해주세요.\n");
    }
}

//오른쪽으로 90도 회전
void rotate_90(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + j * HEIGHT + (HEIGHT - 1 - i)) = *(img_org + i * WIDTH + j);
        }
    }
}

//오른쪽으로 180도 회전
void rotate_180(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + (HEIGHT - 1 - i) * WIDTH + (WIDTH - 1 - j));
        }
    }

}

//왼쪽으로 90도 회전
void rotate_270(void) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            *(img_sav + i * HEIGHT + j) = *(img_org + j * WIDTH + (WIDTH - 1 - i));
        }
    }
}

//좌우 반전
void mirror(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + i * WIDTH + (WIDTH - 1 - j));
        }
    }
}

//상하 반전
void flip(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + (HEIGHT - 1 - i) * WIDTH + j);
        }
    }
}
