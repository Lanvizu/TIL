#define _CRT_SECURE_NO_WARNINGS
#define HEIGHT 512
#define WIDTH 512
#define IMAGE_SIZE WIDTH * HEIGHT
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

const char* filePath = "../Raw_Image/lena.img"; // ���� ���� ��ġ
const char* outputFilePath = "../Raw_Image/lenaOutput.img"; // ��� ���� ��ġ

unsigned char* img_org = NULL; //���� �̹���
unsigned char* img_sav = NULL; //��� �̹���

//����,��� ����
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

//���� ���� �ε�
void loadFile() {
    // ���� �Ҵ�
    img_org = (unsigned char*)malloc(sizeof(unsigned char) * IMAGE_SIZE);
    img_sav = (unsigned char*)malloc(sizeof(unsigned char) * IMAGE_SIZE);
    if (img_sav == NULL || img_org == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    // ���� ����
    inFilePtr = fopen(filePath, "rb");
    if (inFilePtr == NULL) {
        perror("���� ���� ����");
        exit(1);
    }

    //�о�� ���� ����(img_org)�� ����
    fread(img_org, sizeof(unsigned char), IMAGE_SIZE, inFilePtr);
    
    fclose(inFilePtr);
}
// ���� ����
void saveFile() {
    outFilePtr = fopen(outputFilePath, "wb");
    if (outFilePtr == NULL) {
        perror("���� ���� ����");
        exit(1);
    }
    // ���� �Է�
    fwrite(img_sav, sizeof(unsigned char), IMAGE_SIZE, outFilePtr);
    fclose(outFilePtr);

    //���� �Ҵ� ����
    free(img_org);
    free(img_sav);
}

//���� �Է�
void processUserInput() {
    int choice = 0; // ����� �Է��� ������ ����

    printf("���� �� ���ϴ� �۾��� �����ϼ���.\n");
    printf("1: ���������� 90�� ȸ��\n");
    printf("2: ���������� 180�� ȸ��\n");
    printf("3: �������� 90�� ȸ��\n");
    printf("4: �¿� ��Ī\n");
    printf("5: ���� ��Ī\n");

    scanf("%d", &choice);
    while (0 >= choice || 5 < choice) {
        printf("�ٽ� �Է����ּ���\n");
        scanf("%d", &choice);
    }
    startChange(choice);
}

//�Է� ���� ��� ����
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
        printf("1���� 5 ������ ���� �Է����ּ���.\n");
    }
}

//���������� 90�� ȸ��
void rotate_90(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + j * HEIGHT + (HEIGHT - 1 - i)) = *(img_org + i * WIDTH + j);
        }
    }
}

//���������� 180�� ȸ��
void rotate_180(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + (HEIGHT - 1 - i) * WIDTH + (WIDTH - 1 - j));
        }
    }

}

//�������� 90�� ȸ��
void rotate_270(void) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            *(img_sav + i * HEIGHT + j) = *(img_org + j * WIDTH + (WIDTH - 1 - i));
        }
    }
}

//�¿� ����
void mirror(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + i * WIDTH + (WIDTH - 1 - j));
        }
    }
}

//���� ����
void flip(void) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            *(img_sav + i * WIDTH + j) = *(img_org + (HEIGHT - 1 - i) * WIDTH + j);
        }
    }
}
