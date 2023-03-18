/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define FILE_NAME "output.txt"

// Функция потока, которая записывает сообщение в файл 
void *thread_function(void *arg) {
    int thread_num;
    char message[100];
    FILE *file;

    // Получение номера потока из аргумента 
    thread_num = *((int *)arg);

    // Формирование сообщения 
    sprintf(message, "Thread %d: Hello from thread %d!\n", thread_num, thread_num);

    // Открытие файла для добавления сообщения 
    file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return NULL;
    }

    // Запись сообщения в файл и закрытие файла
    fputs(message, file);
    fclose(file);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread_num1 = 1, thread_num2 = 2;
    int priority = 10;
    int policy;
    struct sched_param param;
    FILE *file;
    char buffer[256];

    // Создание потоков 
    pthread_create(&thread1, NULL, thread_function, &thread_num1);
    pthread_create(&thread2, NULL, thread_function, &thread_num2);

    // Установка приоритета для обоих потоков 
    pthread_getschedparam(thread1, &policy, &param);
    param.sched_priority = priority;
    pthread_setschedparam(thread1, policy, &param);

    pthread_getschedparam(thread2, &policy, &param);
    param.sched_priority = priority;
    pthread_setschedparam(thread2, policy, &param);

    // Ожидание завершения потоков 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Чтение содержимого файла и вывод его на терминал 
    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);

    return 0;
}
