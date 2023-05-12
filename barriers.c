#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


struct barrier {
  int count;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

void barrier_init(struct barrier *barrier, int count) {
  barrier->count = count;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->cond, NULL);
}

void barrier_wait(struct barrier *barrier) {
  pthread_mutex_lock(&barrier->mutex);

  barrier->count--;


  if (barrier->count == 0) {
    pthread_cond_broadcast(&barrier->cond);
  } else {
    pthread_cond_wait(&barrier->cond, &barrier->mutex);
  }

  pthread_mutex_unlock(&barrier->mutex);
}

void barrier_destroy(struct barrier *barrier) {
  pthread_mutex_destroy(&barrier->mutex);
  pthread_cond_destroy(&barrier->cond);
}

void *thread_function(void *arg) {
  printf("Thread %ld: started\n", (long)arg);

  sleep(rand() % 10);

  barrier_wait(&barrier);

  printf("Thread %ld: reached barrier\n", (long)arg);

  return NULL;
}

int main() {
  struct barrier barrier;
  barrier_init(&barrier, 3);

  pthread_t threads[3];
  for (int i = 0; i < 3; i++) {
    pthread_create(&threads[i], NULL, thread_function, (void *)i);
  }

  for (int i = 0; i < 3; i++) {
    pthread_join(threads[i], NULL);
  }

  barrier_destroy(&barrier);
  return EXIT_SUCCESS;
}
