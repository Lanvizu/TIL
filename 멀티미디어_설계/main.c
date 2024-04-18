#define _CRT_SECURE_NO_WARNINGS

#define INPUT_NODES 784 // �Է� ����
#define OUTPUT_NODES 10 // ��� ����
#define HIDDEN_LAYERS 1 // ���� ���̾� �� 1,2,3
#define BATCH_SIZE 32 // ��ġ ũ��
#define LEARNING_RATE 0.05 // �н���
#define EPOCH 10 // �н� �ݺ� ��
#define IMG_SIZE 784 // 28*28 �̹��� ũ��
#define SIGMOID(x) 1.0 / (1.0 + exp(-x)) // �ñ׸��̵� �Լ�

#define IMAGES_0 5923 //0 �̹���
#define IMAGES_1 6742 //1 �̹���
#define IMAGES_2 5958 //2 �̹���
#define IMAGES_3 6131 //3 �̹���
#define IMAGES_4 5842 //4 �̹���
#define IMAGES_5 5421 //5 �̹���
#define IMAGES_6 5918 //6 �̹���
#define IMAGES_7 6265 //7 �̹���
#define IMAGES_8 5851 //8 �̹���
#define IMAGES_9 5949 //9 �̹���

#define TEST_IMAGES_0 980 //0 �׽�Ʈ �̹���
#define TEST_IMAGES_1 1135 //1 �׽�Ʈ �̹���
#define TEST_IMAGES_2 1032 //2 �׽�Ʈ �̹���
#define TEST_IMAGES_3 1010 //3 �׽�Ʈ �̹���
#define TEST_IMAGES_4 982 //4 �׽�Ʈ �̹���
#define TEST_IMAGES_5 892 //5 �׽�Ʈ �̹���
#define TEST_IMAGES_6 958 //6 �׽�Ʈ �̹���
#define TEST_IMAGES_7 1028 //7 �׽�Ʈ �̹���
#define TEST_IMAGES_8 974 //8 �׽�Ʈ �̹���
#define TEST_IMAGES_9 1009 //9 �׽�Ʈ �̹���

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

const char* basePath = "../mnist_raw/training/";// ��� ��� ����

int totalImages = IMAGES_0 + IMAGES_1 + IMAGES_2 + IMAGES_3 + IMAGES_4
+ IMAGES_5 + IMAGES_6 + IMAGES_7 + IMAGES_8 + IMAGES_9; // ��ü �̹��� ��

int imageCounts[] = {
    IMAGES_0, IMAGES_1, IMAGES_2, IMAGES_3, IMAGES_4,
    IMAGES_5, IMAGES_6, IMAGES_7, IMAGES_8, IMAGES_9
}; // �̹������� ���� �迭

const char* baseTestPath = "../mnist_raw/testing/";

int totalTestImages = TEST_IMAGES_0 + TEST_IMAGES_1 + TEST_IMAGES_2 + TEST_IMAGES_3 + TEST_IMAGES_4
+ TEST_IMAGES_5 + TEST_IMAGES_6 + TEST_IMAGES_7 + TEST_IMAGES_8 + TEST_IMAGES_9; // ��ü �׽�Ʈ �̹��� ��

int testImageCounts[] = {
    TEST_IMAGES_0, TEST_IMAGES_1, TEST_IMAGES_2, TEST_IMAGES_3, TEST_IMAGES_4,
    TEST_IMAGES_5, TEST_IMAGES_6, TEST_IMAGES_7, TEST_IMAGES_8, TEST_IMAGES_9
}; // �׽�Ʈ �̹������� ���� �迭

double** imageBuffer = NULL; //�̹��� �ȼ� ����
int** targets = NULL; // ��ǥ�� ����
char* filePath;
const int FILE_PATH_SIZE = 256;
int* layers; //���̾�� ��� �� ����
double*** weights; //����ġ
double** biases; // ����
double** predictions; // ������
double*** saveWeights; //����ġ ����
double** saveBiases; //���� ����
double** gradients; // ������Ʈ ���� ����
double avgLoss; // ��� �ս�

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
    start = clock(); // �Լ� ���� �� �ð� ���� ����

    inputNodes();
    weightsBiasesMalloc();
    loadFile();
    trainEpoch();
    freeImageBufferAndTargets(totalImages);

    //accuracy ����
    loadTestFile();
    predictTestImages();
    freeImageBufferAndTargets(totalTestImages);

    freeWeightsBiases();

    end = clock(); // �Լ� ���� �� �ð� ���� ����

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // ������ �ð��� �� ������ ��ȯ

    printLayers();

    printf("�ɸ� �ð� = %f \n", cpu_time_used);
}

void trainEpoch() {
    for (int i = 0; i < EPOCH; i++) {
        printf("\n");
        printf("EPOCH %d", i + 1);
        printf("\n");
        train();
    }
}

// �̹��� ���� ���� �Լ�
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

// �̹��� ���� �ҷ����� �Լ�
void loadFile() {
    imageBuffer = (double**)malloc(totalImages * sizeof(double*));
    filePath = (char*)malloc(FILE_PATH_SIZE * sizeof(char));
    targets = (int**)malloc(totalImages * sizeof(int*));
    unsigned char* tempBuffer = (unsigned char*)malloc(IMG_SIZE * sizeof(unsigned char));
    if (filePath == NULL || imageBuffer == NULL || targets == NULL || tempBuffer == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    int totalCount = 0;
    for (int i = 0; i < 10; i++) {
        int count = imageCounts[i];
        for (int j = 0; j < count; j++) {
            snprintf(filePath, FILE_PATH_SIZE, "%s%d/%d-%d.raw", basePath, i, i, j);
            FILE* inFilePtr = fopen(filePath, "rb");
            if (inFilePtr == NULL) {
                fprintf(stderr, "���� '%s' ���� ����\n", filePath);
                continue;
            }
            *(imageBuffer + totalCount + j) = (double*)malloc(IMG_SIZE * sizeof(double));
            *(targets + totalCount + j) = (int*)malloc(OUTPUT_NODES * sizeof(int));
            if (*(imageBuffer + totalCount + j) == NULL || *(targets + totalCount + j) == NULL) {
                printf("�޸� �Ҵ� ����\n");
                exit(1);
            }
            size_t bytesRead = fread(tempBuffer, sizeof(unsigned char), IMG_SIZE, inFilePtr);
            if (bytesRead != IMG_SIZE) {
                fprintf(stderr, "���� '%s' ���� ����\n", filePath);
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
    //������ ��� �ݺ� ����
    shuffle(totalCount);
}

// �׽�Ʈ �̹��� ���� �ҷ����� �Լ�
void loadTestFile() {
    imageBuffer = (double**)malloc(totalTestImages * sizeof(double*));
    filePath = (char*)malloc(FILE_PATH_SIZE * sizeof(char));
    targets = (int**)malloc(totalTestImages * sizeof(int*));
    unsigned char* tempBuffer = (unsigned char*)malloc(IMG_SIZE * sizeof(unsigned char));
    if (filePath == NULL || imageBuffer == NULL || targets == NULL || tempBuffer == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    int totalCount = 0;
    for (int i = 0; i < 10; i++) {
        int count = testImageCounts[i];
        for (int j = 0; j < count; j++) {
            snprintf(filePath, FILE_PATH_SIZE, "%s%d/%d-%d.raw", baseTestPath, i, i, j);
            FILE* inFilePtr = fopen(filePath, "rb");
            if (inFilePtr == NULL) {
                fprintf(stderr, "���� '%s' ���� ����\n", filePath);
                continue;
            }
            *(imageBuffer + totalCount + j) = (double*)malloc(IMG_SIZE * sizeof(double));
            *(targets + totalCount + j) = (int*)malloc(OUTPUT_NODES * sizeof(int));
            if (*(imageBuffer + totalCount + j) == NULL || *(targets + totalCount + j) == NULL) {
                printf("�޸� �Ҵ� ����\n");
                exit(1);
            }
            size_t bytesRead = fread(tempBuffer, sizeof(unsigned char), IMG_SIZE, inFilePtr);
            if (bytesRead != IMG_SIZE) {
                fprintf(stderr, "���� '%s' ���� ����\n", filePath);
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

// ��� �ս� ���
void printAvgLoss(int count) {
    printf("%f\n", avgLoss / (count * 10));
    avgLoss = 0.0;
}

//������ ���
void printPredictions() {
    for (int j = 0; j < *(layers + (HIDDEN_LAYERS + 1)); j++) {
        printf("%f ", *(*(predictions + HIDDEN_LAYERS) + j));
    }
    printf("\n");
}

// ��� ���� �Է�
void inputNodes() {
    layers = (int*)malloc(sizeof(int) * (HIDDEN_LAYERS + 2));
    if (layers == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    *(layers + 0) = INPUT_NODES;
    *(layers + HIDDEN_LAYERS + 1) = OUTPUT_NODES;
    for (int i = 1; i < HIDDEN_LAYERS + 1; i++) {
        printf("���� ���̾� %d�� ��� ���� �Է��ϼ���: ", i);
        if (scanf("%d", layers + i) != 1) {
            printf("�߸��� �Է��Դϴ�. ������ �Է��ؾ� �մϴ�.\n");
            exit(1);
        }
    }
}

// ���̾� ���� ���
void printLayers() {
    printf("���̾� ���� \n");
    printf("��ġ ������: %d \n", BATCH_SIZE);
    printf("�Է� ���̾�: %d ���\n", *(layers + 0));
    for (int i = 1; i <= HIDDEN_LAYERS; i++) {
        printf("���� ���̾� %d: %d ���\n", i, *(layers + i));
    }
    printf("��� ���̾�: %d ���\n", *(layers + HIDDEN_LAYERS + 1));
    printf("�н���: %f \n", LEARNING_RATE);
    printf("�н� �ݺ� Ƚ��: %d \n", EPOCH);
}

//�޸� �Ҵ� �� �ʱ�ȭ
void weightsBiasesMalloc() {
    weights = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));
    biases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    saveWeights = (double***)malloc((HIDDEN_LAYERS + 1) * sizeof(double**));
    saveBiases = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    gradients = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));
    predictions = (double**)malloc((HIDDEN_LAYERS + 1) * sizeof(double*));

    if (weights == NULL || biases == NULL || saveWeights == NULL
        || saveBiases == NULL || gradients == NULL || predictions == NULL) {
        printf("�޸� �Ҵ� ����\n");
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
            printf("�޸� �Ҵ� ����\n");
            exit(1);
        }
        for (int j = 0; j < inputLayerNode; j++) {
            *(*(weights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            *(*(saveWeights + i) + j) = (double*)malloc(outputLayerNode * sizeof(double));
            if (*(*(weights + i) + j) == NULL || *(*(saveWeights + i) + j) == NULL) {
                printf("�޸� �Ҵ� ����\n");
                exit(1);
            }
            for (int k = 0; k < outputLayerNode; k++) {
                // -1.0�� 1.0 ������ ������ ���� �����Ͽ� ����ġ �ʱ�ȭ
                *(*(*(weights + i) + j) + k) = (double)rand() / RAND_MAX * 2.0 - 1.0;
            }
        }
        for (int j = 0; j < outputLayerNode; j++) {
            *(*(biases + i) + j) = (double)rand() / RAND_MAX * 2.0 - 1.0;
        }
    }
}

//����ġ�� ���� ������ �ʱ�ȭ
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

// ����ġ �޸� ����
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

// �ñ׸��̵� �̺�
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// ������ �н�
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

// ����ġ ������Ʈ ���� ����
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
                    if (i == 0) { // �Է����� ���� ó��
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(inputs + k);
                    }
                    else { // ���� �������� ���� ó��
                        *(*(*(saveWeights + i) + k) + j) += gradientUpdate * *(*(predictions + (i - 1)) + k);
                    }
                }
                *(*(saveBiases + i) + j) += gradientUpdate;
            }
        }
    }
}

// ����ġ ������Ʈ
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

// accuracy ����
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
        printf("%d �� accuracy = %f\n", i, accuracy);
    }
}

// �׽�Ʈ ���� �������� ������ bool ���
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
// ��ǥ�� ��������
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
        fprintf(stderr, "���� ���� ����: %s\n", filename);
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

    // ����: �� ���̾��� ����ġ ���� (���� ���������� ���� ����ġ �����͸� ó���ؾ� ��)
    // �� �κ��� ���� ����ġ �����Ϳ� ���� �ſ� �ٸ� �� ������, ���⼭�� �ܼ�ȭ�� ���ø� �����մϴ�.
    // ��: fprintf(file, "WeightsLayer1: %f, %f, ...\n", weights[0], weights[1]);

    fclose(file);
}