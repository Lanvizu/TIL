#define _CRT_SECURE_NO_WARNINGS

#define INPUT_NODES 784 //입력 계층
#define OUTPUT_NODES 10 // 출력 계층
#define HIDDEN_LAYERS 2 // 히든 레이어 수 1,2,3
#define BATCH_SIZE 16 // 배치 크기
#define LEARNING_RATE 0.01 // 학습률
#define EPOCH 20 // 학습 반복 수
#define IMG_SIZE 784 // 28*28 이미지 크기
#define SIGMOID(x) 1.0 / (1.0 + exp(-x))
#define MOMENTUM 0.9
#define DROPOUT_RATE 0.2

#define IMAGES_0 5923 //0 이미지
#define IMAGES_1 6742 //1 이미지
#define IMAGES_2 5958 //2 이미지
#define IMAGES_3 6131 //3 이미지
#define IMAGES_4 5842 //4 이미지
#define IMAGES_5 5421 //5 이미지
#define IMAGES_6 5918 //6 이미지
#define IMAGES_7 6265 //7 이미지
#define IMAGES_8 5851 //8 이미지
#define IMAGES_9 5949 //9 이미지

#define TEST_IMAGES_0 980 //0 테스트 이미지
#define TEST_IMAGES_1 1135 //1 테스트 이미지
#define TEST_IMAGES_2 1032 //2 테스트 이미지
#define TEST_IMAGES_3 1010 //3 테스트 이미지
#define TEST_IMAGES_4 982 //4 테스트 이미지
#define TEST_IMAGES_5 892 //5 테스트 이미지
#define TEST_IMAGES_6 958 //6 테스트 이미지
#define TEST_IMAGES_7 1028 //7 테스트 이미지
#define TEST_IMAGES_8 974 //8 테스트 이미지
#define TEST_IMAGES_9 1009 //9 테스트 이미지

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

const char* basePath = "../mnist_raw/training/";
const char* baseTestPath = "../mnist_raw/testing/";

double** imageBuffer = NULL; //이미지 픽셀 저장
int** targets = NULL; // 목표값 저장
char* filePath;
const int FILE_PATH_SIZE = 256;

int totalImages = IMAGES_0 + IMAGES_1 + IMAGES_2 + IMAGES_3 + IMAGES_4
+ IMAGES_5 + IMAGES_6 + IMAGES_7 + IMAGES_8 + IMAGES_9; // 전체 이미지 수
int imageCounts[] = {
    IMAGES_0, IMAGES_1, IMAGES_2, IMAGES_3, IMAGES_4,
    IMAGES_5, IMAGES_6, IMAGES_7, IMAGES_8, IMAGES_9
}; // 이미지마다 개수 배열

int totalTestImages = TEST_IMAGES_0 + TEST_IMAGES_1 + TEST_IMAGES_2 + TEST_IMAGES_3 + TEST_IMAGES_4
+ TEST_IMAGES_5 + TEST_IMAGES_6 + TEST_IMAGES_7 + TEST_IMAGES_8 + TEST_IMAGES_9; // 전체 테스트 이미지 수
int testImageCounts[] = {
    TEST_IMAGES_0, TEST_IMAGES_1, TEST_IMAGES_2, TEST_IMAGES_3, TEST_IMAGES_4,
    TEST_IMAGES_5, TEST_IMAGES_6, TEST_IMAGES_7, TEST_IMAGES_8, TEST_IMAGES_9
}; // 이미지마다 개수 배열

int* layers; //레이어마다 노드 수 저장
double*** weights; //가중치
double*** prevWeightUpdates; //이전 가중치 업데이트 값
double** biases; // 편향
double** prevBiasUpdates; // 이전 편향 업데이트 값
double** predictions; // 예측값
double*** saveWeights; //가중치 누적
double** saveBiases; //편향 누적
double** gradients; // 업데이트 기울기 저장
double** accumulatedGradientsWeights; // 그라디언트 제곱합
double** accumulatedGradientsBiases; // 바이어스 제곱합
double avgLoss; // 평균 손실

void inputNodes(void);
void weightsBiasesMalloc(void);
void loadFile(void);
double sigmoid_derivative(double x);
void freeWeightsBiases(void);
void forwardPass(double* inputs);
void printLayers(void);
void printPredictions(void);
void shuffle(int n);
void saveWeightsBiasesToZero(void);
void prevWeightsBiasesToZero(void);
void saveWeightsBiases(int count, double* inputs);
void updateWeights(void);
void train(void);
void printAvgLoss(int count);
void freeImageBufferAndTargets(int imagesCount);
bool isCorrect(int i);
void predictTestImages(void);
void loadTestFile(void);
void trainEpoch(void);
int getTrueLabel(int i);
double sigmoid(double x);
double relu(double x);
void softmax(double* z, int size);
double relu_derivative(double x);
//void forwardPassDropOut(double* inputs);

int main(void) {

    clock_t start, end;
    double cpu_time_used;
    start = clock(); // 함수 실행 전 시간 측정 시작

    inputNodes();
    weightsBiasesMalloc();
    loadFile();
    trainEpoch();
    freeImageBufferAndTargets(totalImages);

    //accuracy 측정
    loadTestFile();
    predictTestImages();
    freeImageBufferAndTargets(totalTestImages);

    freeWeightsBiases();

    end = clock(); // 함수 실행 후 시간 측정 종료

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // 측정된 시간을 초 단위로 변환

    printLayers();

    printf("걸린 시간 = %f \n", cpu_time_used);
}

void trainEpoch() {
    for (int i = 0; i < EPOCH; i++) {
        printf("\n");
        printf("EPOCH %d", i + 1);
        printf("\n");
        train();
    }
}

// 시그모이드 함수
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// 시그모이드 미분
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// 렐루 함수
double relu(double x) {
    return (x > 0) ? x : 0;
}

// 렐루 미분
double relu_derivative(double x) {
    return x > 0 ? 1.0 : 0.0;
}


// 소프트맥스 함수
void softmax(double* z, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += exp(z[i]);
    }
    for (int i = 0; i < size; i++) {
        z[i] = exp(z[i]) / sum;
    }
}

// 이미지 순서 섞는 함수
void shuffle(int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        double* t = *(imageBuffer + j);
        *(imageBuffer + j) = *(imageBuffer + i);
        *(imageBuffer + i) = t;

        int* s = *(targets + j);
        *(targets + j) = *(targets + i);
        *(targets + i) = s;
    }
}

// 이미지 파일 불러오는 함수
void loadFile() {
    imageBuffer = (double**)malloc(totalImages * sizeof(double*));
    filePath = (char*)malloc(FILE_PATH_SIZE * sizeof(char));
    targets = (int**)malloc(totalImages * sizeof(int*));
    unsigned char* tempBuffer = (unsigned char*)malloc(IMG_SIZE * sizeof(unsigned char));
    if (filePath == NULL || imageBuffer == NULL || targets == NULL || tempBuffer == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    int totalCount = 0;
    for (int i = 0; i < 10; i++) {
        int count = imageCounts[i];
        for (int j = 0; j < count; j++) {
            snprintf(filePath, FILE_PATH_SIZE, "%s%d/%d-%d.raw", basePath, i, i, j);
            FILE* inFilePtr = fopen(filePath, "rb");
            if (inFilePtr == NULL) {
                fprintf(stderr, "파일 '%s' 열기 실패\n", filePath);
                continue;
            }
            *(imageBuffer + totalCount + j) = (double*)malloc(IMG_SIZE * sizeof(double));
            *(targets + totalCount + j) = (int*)malloc(OUTPUT_NODES * sizeof(int));
            if (*(imageBuffer + totalCount + j) == NULL || *(targets + totalCount + j) == NULL) {
                printf("메모리 할당 실패\n");
                exit(1);
            }
            size_t bytesRead = fread(tempBuffer, sizeof(unsigned char), IMG_SIZE, inFilePtr);
            if (bytesRead != IMG_SIZE) {
                fprintf(stderr, "파일 '%s' 열기 실패\n", filePath);
            }
            else {
                for (int k = 0; k < IMG_SIZE; k++) {
                    *(*(imageBuffer + totalCount + j) + k) = *(tempBuffer + k) / 255.0f;
                }
            }
            for (int k = 0; k < OUTPUT_NODES; k++) {
                if (k == i) {
                    *(*(targets + totalCount + j) + k) = 1;
                }
                else {
                    *(*(targets + totalCount + j) + k) = 0;
                }
            }
            fclose(inFilePtr);
        }
        totalCount += count;
    }
    free(filePath);
    free(tempBuffer);
    //부족할 경우 반복 셔플
    shuffle(totalCount);
}

// 테스트 이미지 파일 불러오는 함수
void loadTestFile() {
    imageBuffer = (double**)malloc(totalTestImages * sizeof(double*));
    filePath = (char*)malloc(FILE_PATH_SIZE * sizeof(char));
    targets = (int**)malloc(totalTestImages * sizeof(int*));
    unsigned char* tempBuffer = (unsigned char*)malloc(IMG_SIZE * sizeof(unsigned char));
    if (filePath == NULL || imageBuffer == NULL || targets == NULL || tempBuffer == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    int totalCount = 0;
    for (int i = 0; i < 10; i++) {
        int count = testImageCounts[i];
        for (int j = 0; j < count; j++) {
            snprintf(filePath, FILE_PATH_SIZE, "%s%d/%d-%d.raw", baseTestPath, i, i, j);
            FILE* inFilePtr = fopen(filePath, "rb");
            if (inFilePtr == NULL) {
                fprintf(stderr, "파일 '%s' 열기 실패\n", filePath);
                continue;
            }
            *(imageBuffer + totalCount + j) = (double*)malloc(IMG_SIZE * sizeof(double));
            *(targets + totalCount + j) = (int*)malloc(OUTPUT_NODES * sizeof(int));
            if (*(imageBuffer + totalCount + j) == NULL || *(targets + totalCount + j) == NULL) {
                printf("메모리 할당 실패\n");
                exit(1);
            }
            size_t bytesRead = fread(tempBuffer, sizeof(unsigned char), IMG_SIZE, inFilePtr);
            if (bytesRead != IMG_SIZE) {
                fprintf(stderr, "파일 '%s' 열기 실패\n", filePath);
            }
            else {
                for (int k = 0; k < IMG_SIZE; k++) {
                    *(*(imageBuffer + totalCount + j) + k) = *(tempBuffer + k) / 255.0f;
                }
            }
            for (int k = 0; k < OUTPUT_NODES; k++) {
                if (k == i) {
                    *(*(targets + totalCount + j) + k) = 1;
                }
                else {
                    *(*(targets + totalCount + j) + k) = 0;
                }
            }

            fclose(inFilePtr);
        }
        totalCount += count;
    }
    free(filePath);
    shuffle(totalCount);
}

void train() {
    int count = 0;
    prevWeightsBiasesToZero();
    for (int i = 0; i < totalImages / BATCH_SIZE; i++) {
        saveWeightsBiasesToZero();
        for (int j = 0; j < BATCH_SIZE; j++) {
            forwardPass(*(imageBuffer + count));
            //printPredictions();
            saveWeightsBiases(count, *(imageBuffer + count));
            count++;
        }
        updateWeights();
    }
    printAvgLoss(count);
}

// 평균 손실 출력
void printAvgLoss(int count) {
    printf("%f\n", avgLoss / (count * 10));
    avgLoss = 0.0;
}

// 예측값 출력
void printPredictions() {
    for (int j = 0; j < *(layers + (HIDDEN_LAYERS + 1)); j++) {
        printf("%f ", *(*(predictions + HIDDEN_LAYERS) + j));
    }
    printf("\n");
}

// 노드 개수 입력
void inputNodes() {
    layers = (int*)malloc(sizeof(int) * (HIDDEN_LAYERS + 2));
    if (layers == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    *(layers + 0) = INPUT_NODES;
    *(layers + HIDDEN_LAYERS + 1) = OUTPUT_NODES;
    for (int i = 1; i < HIDDEN_LAYERS + 1; i++) {
        printf("히든 레이어 %d의 노드 수를 입력하세요: ", i);
        if (scanf("%d", layers + i) != 1) {
            printf("잘못된 입력입니다. 정수를 입력해야 합니다.\n");
            exit(1);
        }
    }
}

// 레이어 구성 출력
void printLayers() {
    printf("레이어 구성 \n");
    printf("배치 사이즈: %d \n", BATCH_SIZE);
    printf("입력 레이어: %d 노드\n", *(layers + 0));
    for (int i = 1; i <= HIDDEN_LAYERS; i++) {
        printf("히든 레이어 %d: %d 노드\n", i, *(layers + i));
    }
    printf("출력 레이어: %d 노드\n", *(layers + HIDDEN_LAYERS + 1));
    printf("학습률: %f \n", LEARNING_RATE);
    printf("학습 반복 횟수: %d \n", EPOCH);
}

//메모리 할당 및 초기화
void weightsBiasesMalloc() {
    weights = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));
    biases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    prevBiasUpdates = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    saveWeights = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));
    saveBiases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    gradients = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    accumulatedGradientsWeights = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    accumulatedGradientsBiases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    predictions = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    prevWeightUpdates = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));

    if (weights == NULL || biases == NULL || saveWeights == NULL || prevBiasUpdates == NULL || accumulatedGradientsWeights == NULL
        || saveBiases == NULL || gradients == NULL || predictions == NULL || prevWeightUpdates == NULL || accumulatedGradientsBiases == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        int inputLayerNode = *(layers + i);
        int outputLayerNode = *(layers + (i + 1));

        *(weights + i) = (double**)malloc(inputLayerNode * sizeof(double*));
        *(saveWeights + i) = (double**)malloc(inputLayerNode * sizeof(double*));
        *(prevWeightUpdates + i) = (double**)malloc(inputLayerNode * sizeof(double*));
        *(biases + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(saveBiases + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(prevBiasUpdates + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(gradients + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(accumulatedGradientsWeights + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(accumulatedGradientsBiases + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(predictions + i) = (double*)malloc(outputLayerNode * sizeof(double));

        if (*(weights + i) == NULL || *(biases + i) == NULL || *(predictions + i) == NULL || *(saveWeights + i) == NULL || *(accumulatedGradientsWeights + i) == NULL
            || *(saveBiases + i) == NULL || *(gradients + i) == NULL || *(prevWeightUpdates + i) == NULL || *(prevBiasUpdates + i) == NULL || *(accumulatedGradientsBiases + i) == NULL) {
            printf("메모리 할당 실패\n");
            exit(1);
        }
        for (int j = 0; j < inputLayerNode; j++) {
            *(*(weights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            *(*(saveWeights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            *(*(prevWeightUpdates + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            if (*(*(weights + i) + j) == NULL || *(*(saveWeights + i) + j) == NULL || *(*(prevWeightUpdates + i) + j) == NULL) {
                printf("메모리 할당 실패\n");
                exit(1);
            }
            for (int k = 0; k < outputLayerNode; k++) {
                // -1.0과 1.0 사이의 무작위 값을 생성하여 가중치 초기화
                *(*(*(weights + i) + j) + k) = (double)rand() / RAND_MAX * 2.0 - 1.0;
            }
        }
        for (int j = 0; j < outputLayerNode; j++) {
            *(*(biases + i) + j) = (double)rand() / RAND_MAX * 2.0 - 1.0;
        }
    }
}

//가중치와 편향 누적값 초기화
void saveWeightsBiasesToZero() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        int inputLayerNode = *(layers + i);
        int outputLayerNode = *(layers + (i + 1));

        for (int j = 0; j < inputLayerNode; j++) {
            for (int k = 0; k < outputLayerNode; k++) {
                *(*(*(saveWeights + i) + j) + k) = 0.0;
            }
        }

        for (int j = 0; j < outputLayerNode; j++) {
            *(*(saveBiases + i) + j) = 0.0;
        }
    }
}

void prevWeightsBiasesToZero() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        int inputLayerNode = *(layers + i);
        int outputLayerNode = *(layers + (i + 1));

        for (int j = 0; j < inputLayerNode; j++) {
            for (int k = 0; k < outputLayerNode; k++) {
                *(*(*(prevWeightUpdates + i) + j) + k) = 0.0;
            }
        }

        for (int j = 0; j < outputLayerNode; j++) {
            *(*(prevBiasUpdates + i) + j) = 0.0;
            *(*(accumulatedGradientsWeights + i) + j) = 1.0;
            *(*(accumulatedGradientsBiases + i) + j) = 1.0;
        }
    }
}

// 가중치 메모리 해제
void freeWeightsBiases() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        for (int j = 0; j < *(layers + i); j++) {
            free(*(*(weights + i) + j));
            free(*(*(saveWeights + i) + j));
            free(*(*(prevWeightUpdates + i) + j));
        }
        free(*(weights + i));
        free(*(saveWeights + i));
        free(*(prevWeightUpdates + i));
        free(*(biases + i));
        free(*(gradients + i));
        free(*(accumulatedGradientsWeights + i));
        free(*(saveBiases + i));
        free(*(prevBiasUpdates + i));
        free(*(predictions + i));
        free(*(accumulatedGradientsBiases + i));
    }
    free(weights);
    free(saveWeights);
    free(prevWeightUpdates);
    free(gradients);
    free(accumulatedGradientsWeights);
    free(biases);
    free(saveBiases);
    free(prevBiasUpdates);
    free(accumulatedGradientsBiases);
    free(predictions);

}

void freeImageBufferAndTargets(int imagesCount) {
    for (int i = 0; i < imagesCount; i++) {
        free(*(imageBuffer + i));
        free(*(targets + i));
    }
    free(imageBuffer);
    free(targets);
}


// 순전파 학습
//void forwardPass(double* inputs) {
//    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
//        int inputNodesCount = *(layers + i);
//        int outputNodesCount = *(layers + i + 1);
//
//        for (int j = 0; j < outputNodesCount; j++) {
//            *(*(predictions + i) + j) = *(*(biases + i) + j);
//            for (int k = 0; k < inputNodesCount; k++) {
//                if (i == 0) {
//                    *(*(predictions + i) + j) += *(inputs + k) * *(*(*(weights + i) + k) + j);
//                }
//                else {
//                    *(*(predictions + i) + j) += *(*(predictions + i - 1) + k) * *(*(*(weights + i) + k) + j);
//                }
//            }
//            *(*(predictions + i) + j) = SIGMOID(*(*(predictions + i) + j));
//        }
//    }
//}

// 소프트 맥스 + 시그모이드
void forwardPass(double* inputs) {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        int inputNodesCount = *(layers + i);
        int outputNodesCount = *(layers + i + 1);

        for (int j = 0; j < outputNodesCount; j++) {
            *(*(predictions + i) + j) = *(*(biases + i) + j);
            for (int k = 0; k < inputNodesCount; k++) {
                if (i == 0) {
                    *(*(predictions + i) + j) += *(inputs + k) * *(*(*(weights + i) + k) + j);
                }
                else {
                    *(*(predictions + i) + j) += *(*(predictions + i - 1) + k) * *(*(*(weights + i) + k) + j);
                }
            }
            if (i != HIDDEN_LAYERS) {
                *(*(predictions + i) + j) = SIGMOID(*(*(predictions + i) + j));
            }
        }
        if (i == HIDDEN_LAYERS) {
            softmax(*(predictions + i), outputNodesCount);
        }
    }
}

//드롭아웃 추가
//void forwardPassDropOut(double* inputs) {
//    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
//        int inputNodesCount = *(layers + i);
//        int outputNodesCount = *(layers + i + 1);
//
//        for (int j = 0; j < outputNodesCount; j++) {
//            *(*(predictions + i) + j) = *(*(biases + i) + j);
//            for (int k = 0; k < inputNodesCount; k++) {
//                double weight = (i == 0) ? *(inputs + k) * *(*(*(weights + i) + k) + j)
//                    : *(*(predictions + i - 1) + k) * *(*(*(weights + i) + k) + j);
//
//                // 드롭아웃 적용: 뉴런을 비활성화
//                if ((double)rand() / RAND_MAX > DROPOUT_RATE) {
//                    *(*(predictions + i) + j) += weight;
//                }
//            }
//            if (i != HIDDEN_LAYERS) {
//                *(*(predictions + i) + j) = SIGMOID(*(*(predictions + i) + j));
//            }
//        }
//        if (i == HIDDEN_LAYERS) {
//            softmax(*(predictions + i), outputNodesCount);
//        }
//    }
//}

 //가중치 업데이트 누적 저장
//void saveWeightsBiases(int count, double* inputs) {
//    double error;
//    double gradientUpdate;
//    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
//        for (int j = 0; j < *(layers + i + 1); j++) {
//            if (i == HIDDEN_LAYERS) {
//                error = *(*(targets + count) + j) - *(*(predictions + i) + j);
//                printf("target = %d, output = %f, error = %f\n", *(*(targets + count) + j), *(*(predictions + i) + j), error);
//                avgLoss += error * error;
//
//                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));
//
//                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                }
//                *(*(saveBiases + i) + j) += gradientUpdate;
//            }
//            else {
//                error = 0.0;
//                for (int k = 0; k < *(layers + i + 2); k++) {
//                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
//                }
//
//                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));
//                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    if (i == 0) { // 입력층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
//                    }
//                    else { // 이전 은닉층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                    }
//                }
//                *(*(saveBiases + i) + j) += gradientUpdate;
//            }
//        }
//    }
//}

// 소프트맥스, 교차엔트로피, 시그모이드
//void saveWeightsBiases(int count, double* inputs) {
//    double error;
//    double gradientUpdate;
//    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
//        for (int j = 0; j < *(layers + i + 1); j++) {
//            if (i == HIDDEN_LAYERS) {
//                error = *(*(targets + count) + j) - *(*(predictions + i) + j);
//                //printf("target = %d, output = %f, error = %f\n", *(*(targets + count) + j), *(*(predictions + i) + j), error);
//                avgLoss += error * error;
//
//                *(*(gradients + i) + j) = error;
//
//                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                }
//                *(*(saveBiases + i) + j) += gradientUpdate;
//            }
//            else {
//                error = 0.0;
//                for (int k = 0; k < *(layers + i + 2); k++) {
//                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
//                }
//
//                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));
//                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    if (i == 0) { // 입력층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
//                    }
//                    else { // 이전 은닉층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                    }
//                }
//                *(*(saveBiases + i) + j) += gradientUpdate;
//            }
//        }
//    }
//}

//AdaGrad 적응적 학습률 적용
//void saveWeightsBiases(int count, double* inputs) {
//    double error;
//    double gradientUpdate;
//    double adaptedLearningRate;
//    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
//        for (int j = 0; j < *(layers + i + 1); j++) {
//            if (i == HIDDEN_LAYERS) {
//                error = *(*(targets + count) + j) - *(*(predictions + i) + j);
//                //printf("target = %d, output = %f, error = %f\n", *(*(targets + count) + j), *(*(predictions + i) + j), error);
//                avgLoss += error * error;
//
//                *(*(gradients + i) + j) = error;
//
//                *(*(accumulatedGradientsWeights + i) + j) += error * error;
//                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsWeights + i) + j)) + (1e-8));
//                //printf("learningRate = %f\n", adaptedLearningRate);
//                gradientUpdate = adaptedLearningRate * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                }
//                
//                *(*(accumulatedGradientsBiases + i) + j) += error * error;
//                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsBiases + i) + j)) + (1e-8));
//                *(*(saveBiases + i) + j) += adaptedLearningRate * error;
//            }
//            else {
//                error = 0.0;
//                for (int k = 0; k < *(layers + i + 2); k++) {
//                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
//                }
//
//                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));
//
//                *(*(accumulatedGradientsWeights + i) + j) += error * error;
//                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsWeights + i) + j)) + (1e-8));
//                gradientUpdate = adaptedLearningRate * *(*(gradients + i) + j);
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    if (i == 0) { // 입력층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
//                    }
//                    else { // 이전 은닉층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                    }
//                }
//                *(*(accumulatedGradientsBiases + i) + j) += error * error;
//                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsBiases + i) + j)) + (1e-8));
//                *(*(saveBiases + i) + j) += adaptedLearningRate * error;
//            }
//        }
//    }
//}

// RMSProp
void saveWeightsBiases(int count, double* inputs) {
    double error;
    double gradientUpdate;
    double adaptedLearningRate;
    double decay_rate = 0.9; // RMSprop의 감쇠율 설정
    double max_lr = 0.1;
    double epsilon = 1e-8; // 더 큰 상수값 사용
    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
        for (int j = 0; j < *(layers + i + 1); j++) {
            if (i == HIDDEN_LAYERS) {
                error = *(*(targets + count) + j) - *(*(predictions + i) + j);
                //printf("target = %d, output = %f, error = %f\n", *(*(targets + count) + j), *(*(predictions + i) + j), error);
                avgLoss += error * error;

                *(*(gradients + i) + j) = error;

                // RMSprop 적용: 지수이동평균 사용
                *(*(accumulatedGradientsWeights + i) + j) = decay_rate * *(*(accumulatedGradientsWeights + i) + j) + (1 - decay_rate) * error * error;
                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsWeights + i) + j)) + epsilon);
                adaptedLearningRate = fmin(adaptedLearningRate, max_lr);
                //printf("learningRate = %f\n", adaptedLearningRate);
                gradientUpdate = adaptedLearningRate * *(*(gradients + i) + j);

                for (int k = 0; k < *(layers + i); k++) {
                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
                }

                // RMSprop 적용: 지수이동평균 사용
                *(*(accumulatedGradientsBiases + i) + j) = decay_rate * *(*(accumulatedGradientsBiases + i) + j) + (1 - decay_rate) * error * error;
                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsBiases + i) + j)) + epsilon);
                adaptedLearningRate = fmin(adaptedLearningRate, max_lr);
                *(*(saveBiases + i) + j) += adaptedLearningRate * error;
            }
            else {
                error = 0.0;
                for (int k = 0; k < *(layers + i + 2); k++) {
                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
                }

                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));

                *(*(accumulatedGradientsWeights + i) + j) = decay_rate * *(*(accumulatedGradientsWeights + i) + j) + (1 - decay_rate) * error * error;
                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsWeights + i) + j)) + epsilon);
                adaptedLearningRate = fmin(adaptedLearningRate, max_lr);
                gradientUpdate = adaptedLearningRate * *(*(gradients + i) + j);

                for (int k = 0; k < *(layers + i); k++) {
                    if (i == 0) { // 입력층에 대한 처리
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
                    }
                    else { // 이전 은닉층에 대한 처리
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
                    }
                }
                *(*(accumulatedGradientsBiases + i) + j) = decay_rate * *(*(accumulatedGradientsBiases + i) + j) + (1 - decay_rate) * error * error;
                adaptedLearningRate = LEARNING_RATE / (sqrt(*(*(accumulatedGradientsBiases + i) + j)) + epsilon);
                adaptedLearningRate = fmin(adaptedLearningRate, max_lr);
                *(*(saveBiases + i) + j) += adaptedLearningRate * error;
            }
        }
    }
}

// Adam + 모멘텀 제거
//void saveWeightsBiases(int count, double* inputs) {
//    double error;
//    double gradientUpdate;
//    double adaptedLearningRate;
//    double beta1 = 0.9; // 1차 모멘트 추정의 감쇠율
//    double beta2 = 0.999; // 2차 모멘트 추정의 감쇠율
//    double epsilon = 1e-8; // 안정성을 위한 작은 상수
//    double m; // 1차 모멘트
//    double v; // 2차 모멘트
//    double m_hat; // bias-corrected 1차 모멘트
//    double v_hat; // bias-corrected 2차 모멘트
//    double maxLearningRate = 0.1;
//
//    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
//        for (int j = 0; j < *(layers + i + 1); j++) {
//            if (i == HIDDEN_LAYERS) {
//                error = *(*(targets + count) + j) - *(*(predictions + i) + j);
//                avgLoss += error * error;
//
//                *(*(gradients + i) + j) = error;
//
//                // Adam 적용
//                m = beta1 * *(*(accumulatedGradientsWeights + i) + j) + (1 - beta1) * error;
//                v = beta2 * *(*(accumulatedGradientsBiases + i) + j) + (1 - beta2) * error * error;
//                *(*(accumulatedGradientsWeights + i) + j) = m; // 모멘트 업데이트
//                *(*(accumulatedGradientsBiases + i) + j) = v; // 속도 업데이트
//
//                m_hat = m / (1 - pow(beta1, count + 1)); // bias-correction for m
//                v_hat = v / (1 - pow(beta2, count + 1)); // bias-correction for v
//                adaptedLearningRate = LEARNING_RATE / (sqrt(v_hat) + epsilon);
//                gradientUpdate = adaptedLearningRate * m_hat;
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                }
//
//                // 편향에 대해서도 동일하게 적용
//                m = beta1 * *(*(accumulatedGradientsWeights + i) + j) + (1 - beta1) * error;
//                v = beta2 * *(*(accumulatedGradientsBiases + i) + j) + (1 - beta2) * error * error;
//                *(*(accumulatedGradientsWeights + i) + j) = m; // 모멘트 업데이트
//                *(*(accumulatedGradientsBiases + i) + j) = v; // 속도 업데이트
//
//                m_hat = m / (1 - pow(beta1, count + 1));
//                v_hat = v / (1 - pow(beta2, count + 1));
//                adaptedLearningRate = LEARNING_RATE / (sqrt(v_hat) + epsilon);
//                if (adaptedLearningRate > maxLearningRate) {
//                    adaptedLearningRate = maxLearningRate;
//                }
//
//                //printf("learningRate = %f\n", adaptedLearningRate);
//                *(*(saveBiases + i) + j) += adaptedLearningRate * m_hat;
//            }
//            else {
//                // 은닉층 처리
//                error = 0.0;
//                for (int k = 0; k < *(layers + i + 2); k++) {
//                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
//                }
//                error *= sigmoid_derivative(*(*(predictions + i) + j));
//
//                *(*(gradients + i) + j) = error;
//
//                // Adam 적용: 가중치에 대하여
//                m = beta1 * *(*(accumulatedGradientsWeights + i) + j) + (1 - beta1) * error;
//                v = beta2 * *(*(accumulatedGradientsBiases + i) + j) + (1 - beta2) * error * error;
//                *(*(accumulatedGradientsWeights + i) + j) = m;
//                *(*(accumulatedGradientsBiases + i) + j) = v;
//
//                m_hat = m / (1 - pow(beta1, count + 1));
//                v_hat = v / (1 - pow(beta2, count + 1));
//                adaptedLearningRate = LEARNING_RATE / (sqrt(v_hat) + epsilon);
//
//                gradientUpdate = adaptedLearningRate * m_hat;
//
//                for (int k = 0; k < *(layers + i); k++) {
//                    if (i == 0) { // 입력층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
//                    }
//                    else { // 이전 은닉층에 대한 처리
//                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
//                    }
//                }
//
//                // Adam 적용: 편향에 대하여
//                m = beta1 * *(*(accumulatedGradientsWeights + i) + j) + (1 - beta1) * error;
//                v = beta2 * *(*(accumulatedGradientsBiases + i) + j) + (1 - beta2) * error * error;
//                *(*(accumulatedGradientsWeights + i) + j) = m;
//                *(*(accumulatedGradientsBiases + i) + j) = v;
//
//                m_hat = m / (1 - pow(beta1, count + 1));
//                v_hat = v / (1 - pow(beta2, count + 1));
//                adaptedLearningRate = LEARNING_RATE / (sqrt(v_hat) + epsilon);
//                if (adaptedLearningRate > maxLearningRate) {
//                    adaptedLearningRate = maxLearningRate;
//                }
//                *(*(saveBiases + i) + j) += adaptedLearningRate * m_hat;
//
//            }
//        }
//    }
//}

//void backpropBatchNorm(double* gradInput, double* mean, double* variance, int n) {
//    double gradMean = 0.0;
//    double gradVariance = 0.0;
//    double epsilon = 1e-5;
//    double* gradXhat = (double*)malloc(n * sizeof(double));
//
//    for (int i = 0; i < n; i++) {
//        gradXhat[i] = gradInput[i] * 1.0 / sqrt(variance[i] + epsilon);
//        gradMean += gradInput[i] * -1.0 / sqrt(variance[i] + epsilon);
//        gradVariance += gradInput[i] * -0.5 * (input[i] - mean[i]) / pow(variance[i] + epsilon, 1.5);
//    }
//
//    double gradMeanCorrection = gradMean / n;
//    double gradVarianceCorrection = gradVariance * 2.0 / n;
//
//    for (int i = 0; i < n; i++) {
//        gradInput[i] = gradXhat[i] + gradMeanCorrection + gradVarianceCorrection * (input[i] - mean[i]);
//    }
//
//    free(gradXhat);
//}

// 가중치 업데이트
//void updateWeights() {
//    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
//        for (int j = 0; j < *(layers + i + 1); j++) {
//            for (int k = 0; k < *(layers + i); k++) {
//                *(*(*(weights + i) + k) + j) += *(*(*(saveWeights + i) + k) + j) / BATCH_SIZE;
//                //printf("%d ^^^ %d ^^^ %d ^^^ %f \n", i, k, j, *(*(*(weights + i) + k) + j));
//            }
//            *(*(biases + i) + j) += *(*(saveBiases + i) + j) / BATCH_SIZE;
//        }
//    }
//}

// 가중치 업데이트 + 모멘텀
void updateWeights() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        for (int j = 0; j < *(layers + i + 1); j++) {
            for (int k = 0; k < *(layers + i); k++) {
                *(*(*(prevWeightUpdates + i) + k) + j) = (*(*(*(saveWeights + i) + k) + j) / BATCH_SIZE) + (MOMENTUM * *(*(*(prevWeightUpdates + i) + k) + j));
                //printf("%f\n", *(*(*(prevWeightUpdates + i) + k) + j));

                *(*(*(weights + i) + k) + j) += *(*(*(prevWeightUpdates + i) + k) + j);
            }

            *(*(prevBiasUpdates + i) + j) = (*(*(saveBiases + i) + j) / BATCH_SIZE) + (MOMENTUM * *(*(prevBiasUpdates + i) + j));
            *(*(biases + i) + j) += *(*(prevBiasUpdates + i) + j);
        }
    }
}

// accuracy 측정
void predictTestImages() {
    int totalCorrectPredictions = 0;
    int corretPredictions[10] = { 0 };
    for (int i = 0; i < totalTestImages; i++) {
        forwardPass(*(imageBuffer + i));
        if (isCorrect(i)) {
            int trueLabel = getTrueLabel(i);
            //printf("%d\n", trueLabel);
            totalCorrectPredictions++;
            corretPredictions[trueLabel]++;
        }
    }
    double totalAccuracy = (double)totalCorrectPredictions / totalTestImages;
    printf("accuracy = %f\n", totalAccuracy);
    for (int i = 0; i < 10; i++) {
        double accuracy = (double)corretPredictions[i] / testImageCounts[i];
        //printf("%d\n", corretPredictions[i]);
        //printf("%d\n", testImageCounts[i]);
        printf("%d 의 accuracy = %f\n", i, accuracy);
    }
}

// 테스트 모델의 예측값과 정답을 bool 출력
bool isCorrect(int i) {
    int predictedLabel = 0;
    double maxPrediction = *(*(predictions + HIDDEN_LAYERS) + 0);
    for (int j = 1; j < 10; j++) {
        if (*(*(predictions + HIDDEN_LAYERS) + j) > maxPrediction) {
            maxPrediction = *(*(predictions + HIDDEN_LAYERS) + j);
            predictedLabel = j;
        }
    }
    //printf("%d\n", predictedLabel);
    if (*(*(targets + i) + predictedLabel) == 1) {
        return true;
    }
    else {
        return false;
    }
}
// 목표값 가져오기
int getTrueLabel(int i) {
    for (int j = 0; j < 10; j++) {
        if (*(*(targets + i) + j) == 1) {
            return j;
        }
    }
    return -1;
}
