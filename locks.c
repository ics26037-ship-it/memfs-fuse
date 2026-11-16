#include <pthread.h>
#include "locks.h"

// Simple lock interface.
void mutex_init(struct mutex *lock)
{
  pthread_mutex_init(&(m->lock), NULL);
}

void mutex_lock(struct mutex *lock)
{
  pthread_mutex_lock(&(m->lock));
}

void mutex_unlock(struct mutex *lock)
{
  pthread_mutex_unlock(&(m->lock));
}

void mutex_destroy(struct mutex *lock)
{
  pthread_mutex_destroy(&(m->lock));
}

// Read-write lock interface.
void rwmutex_init(struct rwmutex *rwlock)
{
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->read_cv, NULL);
    pthread_cond_init(&rw->write_cv, NULL);
    rw->readers = 0;
    rw->writers = 0;
    rw->waiting_writers = 0;
}

void rwmutex_read_lock(struct rwmutex *rwlock)
{
    pthread_mutex_lock(&rw->mutex);
    
    while (rw->writers > 0 || rw->waiting_writers > 0) {
        pthread_cond_wait(&rw->read_cv, &rw->mutex);
    }
    rw->readers++;
    pthread_mutex_unlock(&rw->mutex);
}

void rwmutex_write_lock(struct rwmutex *rwlock)
{
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;
   
    while (rw->writers > 0 || rw->readers > 0) {
        pthread_cond_wait(&rw->write_cv, &rw->mutex);
    }
    rw->waiting_writers--;
    rw->writers = 1;
    pthread_mutex_unlock(&rw->mutex);
}

void rwmutex_read_unlock(struct rwmutex *rwlock)
{
    pthread_mutex_lock(&rw->mutex);
    rw->readers--;
    if (rw->readers == 0 && rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->write_cv);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void rwmutex_write_unlock(struct rwmutex *rwlock)
{
    pthread_mutex_lock(&rw->mutex);
    rw->writers = 0;
    if (rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->write_cv);
    } else {
        pthread_cond_broadcast(&rw->read_cv);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void rwmutex_destroy(struct rwmutex *rwlock)
{
   pthread_mutex_destroy(&rw->mutex);
   pthread_cond_destroy(&rw->read_cv);
   pthread_cond_destroy(&rw->write_cv);
}

