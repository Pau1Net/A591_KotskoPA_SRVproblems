#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>

// Определил константу для перевода секунд в наносекунды
#define NANOSECONDS_IN_SECOND 1000000000LL

//функция потока, которая будет выполняться более 10 секунд
void *extended_thread(void *arg)
{
    printf("Thread will run for more than 10 seconds.\n");
    sleep(20);
}

int main(void)
{
    uint64_t wait_time;
    struct sigevent sig_event;
    int ret_val;
    pthread_t thr_id;

    printf("Timeout program.\n");

    //sigevent для использования SIGEV_UNBLOCK
    sig_event.sigev_notify = SIGEV_UNBLOCK;

    //поток для функции extended_thread
    pthread_create(&thr_id, NULL, extended_thread, NULL);

    //время ожидания для таймера
    wait_time = 10LL * NANOSECONDS_IN_SECOND;

    //таймер с настройками таймаута
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &sig_event, &wait_time, NULL);

    // присоединение к потоку и проверка истекло ли время ожидания
    ret_val = pthread_join(thr_id, NULL);
    if (ret_val == ETIMEDOUT)
    {
        printf("10 seconds passed, thread %d still running!\n", thr_id);
    }

    sleep(5);

    //снова инициировал таймер с настройками таймаута
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &sig_event, &wait_time, NULL);

    // Опять проверка
    ret_val = pthread_join(thr_id, NULL);
    if (ret_val == ETIMEDOUT)
    {
        printf("Thread %d took longer than 25 seconds!\n", thr_id);
    }
    else
    {
        printf("Thread %d finished as expected.\n", thr_id);
    }

    return 0;
}
