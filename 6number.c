// Комментарии добавлены мной ( не профессиональным разработчиком на C) и могут отличаться от реальности, но, как мне видится, незначительно.

#include <stdio.h>
#include <pthread.h>

// Инициализация глобальных переменных.
int info = 0;  // Переменная для хранения информации, передаваемой от производителя к потребителю.
int is_data_ready = 0;  // Флаг, указывающий, готовы ли данные к передаче от производителя к потребителю.
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;  // Мьютекс для синхронизации доступа к данным.
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;  // Условная переменная для сигнализации о готовности данных.

// Функция потребителя.
void *consume_data(void *param)
{
    puts("Потребитель активирован.");  // Уведомление о запуске потребителя.
    while(1)  // Бесконечный цикл потребителя.
    {
        pthread_mutex_lock(&data_mutex);  // Блокировка мьютекса перед проверкой данных.
        puts("Начало ожидания 1.");  // Уведомление о начале ожидания.
        while (!is_data_ready)  // Пока данные не готовы...
        {
            puts("Начало ожидания 2.");  // Уведомление о начале ожидания.
            pthread_cond_wait(&condition_var, &data_mutex);  // Ожидание сигнала о готовности данных.
            puts("Конец ожидания.");  // Уведомление о конце ожидания.
        }
        printf("Данные от производителя = %d.\n", info);  // Вывод полученных данных.
        is_data_ready = 0;  // Сброс флага готовности данных.
        pthread_cond_signal(&condition_var);  // Сигнализация производителю о готовности к приему новых данных.
        pthread_mutex_unlock(&data_mutex);  // Разблокировка мьютекса.
    }
}

// Функция производителя.
void *produce_data(void *param)
{
    puts("Производитель активирован.");  // Уведомление о запуске производителя.
    while(1)  // Бесконечный цикл производителя.
    {
        sleep(2);  // Приостановка выполнения на 2 секунды.
        printf("Производитель получил данные от оборудования = %d.\n", info);  // Вывод полученных данных.
        pthread_mutex_lock(&data_mutex);  // Блокировка мьютекса перед изменением данных.
        puts("Начало ожидания производителя 1.");  // Уведомление о начале ожидания.
        while (is_data_ready)  // Пока потребитель не готов принять новые данные...
        {
            puts("Начало ожидания производителя 2.");  // Уведомление о начале ожидания.
            pthread_cond_wait(&condition_var, &data_mutex);  // Ожидание сигнала о готовности к приему новых данных.
        }
        is_data_ready = 1;  // Установка флага готовности данных.
        info++;  // Увеличение счетчика данных.
        puts("Конец ожидания производителя.");  // Уведомление о конце ожидания.
        pthread_cond_signal(&condition_var);  // Сигнализация потребителю о готовности данных.
        pthread_mutex_unlock(&data_mutex);  // Разблокировка мьютекса.
    }
}

// Главная функция.
int main(void)
{
    puts("Начало работы.");  // Уведомление о начале работы программы.
    pthread_t consumer_thread, producer_thread;  // Объявление идентификаторов потоков.
    pthread_create(&consumer_thread, NULL, consume_data, NULL);  // Создание потока потребителя.
    pthread_create(&producer_thread, NULL, produce_data, NULL);  // Создание потока производителя.
    sleep(10);  // Приостановка основного потока на 10 секунд, чтобы дать потокам производителя и потребителя выполнить свою работу.
    return 0;  // Завершение работы программы.
}
