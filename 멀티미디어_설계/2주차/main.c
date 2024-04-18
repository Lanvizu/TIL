#define _CRT_SECURE_NO_WARNINGS

#define INPUT_NODES 784 // 입력 계층
#define OUTPUT_NODES 10 // 출력 계층
#define HIDDEN_LAYERS 1 // 히든 레이어 수 1,2,3
#define BATCH_SIZE 32 // 배치 크기
#define LEARNING_RATE 0.05 // 학습률
#define EPOCH 10 // 학습 반복 수
#define IMG_SIZE 784 // 28*28 이미지 크기
#define SIGMOID(x) 1.0 / (1.0 + exp(-x)) // 시그모이드 함수

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

const char* basePath = "../mnist_raw/training/";// 상대 경로 설정

int totalImages = IMAGES_0 + IMAGES_1 + IMAGES_2 + IMAGES_3 + IMAGES_4
+ IMAGES_5 + IMAGES_6 + IMAGES_7 + IMAGES_8 + IMAGES_9; // 전체 이미지 수

int imageCounts[] = {
    IMAGES_0, IMAGES_1, IMAGES_2, IMAGES_3, IMAGES_4,
    IMAGES_5, IMAGES_6, IMAGES_7, IMAGES_8, IMAGES_9
}; // 이미지마다 개수 배열

const char* baseTestPath = "../mnist_raw/testing/";

int totalTestImages = TEST_IMAGES_0 + TEST_IMAGES_1 + TEST_IMAGES_2 + TEST_IMAGES_3 + TEST_IMAGES_4
+ TEST_IMAGES_5 + TEST_IMAGES_6 + TEST_IMAGES_7 + TEST_IMAGES_8 + TEST_IMAGES_9; // 전체 테스트 이미지 수

int testImageCounts[] = {
    TEST_IMAGES_0, TEST_IMAGES_1, TEST_IMAGES_2, TEST_IMAGES_3, TEST_IMAGES_4,
    TEST_IMAGES_5, TEST_IMAGES_6, TEST_IMAGES_7, TEST_IMAGES_8, TEST_IMAGES_9
}; // 테스트 이미지마다 개수 배열

double** imageBuffer = NULL; //이미지 픽셀 저장
int** targets = NULL; // 목표값 저장
char* filePath;
const int FILE_PATH_SIZE = 256;
int* layers; //레이어마다 노드 수 저장
double*** weights; //가중치
double** biases; // 편향
double** predictions; // 예측값
double*** saveWeights; //가중치 누적
double** saveBiases; //편향 누적
double** gradients; // 업데이트 기울기 저장
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

//예측값 출력
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
    saveWeights = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));
    saveBiases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    gradients = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    predictions = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));

    if (weights == NULL || biases == NULL || saveWeights == NULL
        || saveBiases == NULL || gradients == NULL || predictions == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        int inputLayerNode = *(layers + i);
        int outputLayerNode = *(layers + (i + 1));

        *(weights + i) = (double**)malloc(inputLayerNode * sizeof(double*));
        *(saveWeights + i) = (double**)malloc(inputLayerNode * sizeof(double*));
        *(biases + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(saveBiases + i) = (double*)malloc(outputLayerNode * sizeof(double));
        *(gradients + i) = (double*)malloc(*(layers + i + 1) * sizeof(double));
        *(predictions + i) = (double*)malloc(outputLayerNode * sizeof(double));

        if (*(weights + i) == NULL || *(biases + i) == NULL || *(predictions + i) == NULL
            || *(saveWeights + i) == NULL || *(saveBiases + i) == NULL || *(gradients + i) == NULL) {
            printf("메모리 할당 실패\n");
            exit(1);
        }
        for (int j = 0; j < inputLayerNode; j++) {
            *(*(weights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            *(*(saveWeights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            if (*(*(weights + i) + j) == NULL || *(*(saveWeights + i) + j) == NULL) {
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

// 가중치 메모리 해제
void freeWeightsBiases() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        for (int j = 0; j < *(layers + i); j++) {
            free(*(*(weights + i) + j));
            free(*(*(saveWeights + i) + j));
        }
        free(*(weights + i));
        free(*(saveWeights + i));
        free(*(biases + i));
        free(*(saveBiases + i));
        free(*(predictions + i));
    }
    free(weights);
    free(saveWeights);
    free(biases);
    free(saveBiases);
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

// 시그모이드 미분
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// 순전파 학습
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
            *(*(predictions + i) + j) = SIGMOID(*(*(predictions + i) + j));
        }
    }
}

// 가중치 업데이트 누적 저장
void saveWeightsBiases(int count, double* inputs) {
    double error;
    double gradientUpdate;
    for (int i = HIDDEN_LAYERS; i >= 0; i--) {
        for (int j = 0; j < *(layers + i + 1); j++) {
            if (i == HIDDEN_LAYERS) {
                error = *(*(targets + count) + j) - *(*(predictions + i) + j);

                avgLoss += error * error;

                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));

                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);

                for (int k = 0; k < *(layers + i); k++) {
                    *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
                }
                *(*(saveBiases + i) + j) += gradientUpdate;
            }
            else {
                error = 0.0;
                for (int k = 0; k < *(layers + i + 2); k++) {
                    error += *(*(gradients + i + 1) + k) * *(*(*(weights + i + 1) + j) + k);
                }

                *(*(gradients + i) + j) = error * sigmoid_derivative(*(*(predictions + i) + j));
                gradientUpdate = LEARNING_RATE * *(*(gradients + i) + j);

                for (int k = 0; k < *(layers + i); k++) {
                    if (i == 0) { // 입력층에 대한 처리
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
                    }
                    else { // 이전 은닉층에 대한 처리
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
                    }
                }
                *(*(saveBiases + i) + j) += gradientUpdate;
            }
        }
    }
}

// 가중치 업데이트
void updateWeights() {
    for (int i = 0; i < HIDDEN_LAYERS + 1; i++) {
        for (int j = 0; j < *(layers + i + 1); j++) {
            for (int k = 0; k < *(layers + i); k++) {
                *(*(*(weights + i) + k) + j) += *(*(*(saveWeights + i) + k) + j) / BATCH_SIZE;
                //printf("%d ^^^ %d ^^^ %d ^^^ %f \n", i, k, j, *(*(*(weights + i) + k) + j));
            }
            *(*(biases + i) + j) += *(*(saveBiases + i) + j) / BATCH_SIZE;
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

void saveNNMeta(const char* filename, const NeuralNetwork* nn) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "파일 열기 실패: %s\n", filename);
        return;
    }

    fprintf(file, "InputNodes: %d\n", nn->inputNodes);
    fprintf(file, "HiddenLayers: %d\n", nn->hiddenLayers);
    for (int i = 0; i < nn->hiddenLayers; i++) {
        fprintf(file, "HiddenLayer%d: %d\n", i + 1, nn->hiddenNodes[i]);
    }
    fprintf(file, "OutputNodes: %d\n", nn->outputNodes);
    fprintf(file, "LearningRate: %f\n", nn->learningRate);
    fprintf(file, "BatchSize: %d\n", nn->batchSize);

    // 예시: 각 레이어의 가중치 저장 (실제 구현에서는 실제 가중치 데이터를 처리해야 함)
    // 이 부분은 실제 가중치 데이터에 따라 매우 다를 수 있으며, 여기서는 단순화된 예시를 제공합니다.
    // 예: fprintf(file, "WeightsLayer1: %f, %f, ...\n", weights[0], weights[1]);

    fclose(file);
}