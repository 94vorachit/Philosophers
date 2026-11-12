/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 13:46:33 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 13:46:40 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 100000

int shared_counter = 0;
pthread_mutex_t lock;

void* increment_task(void* arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        // Lock the mutex before entering the critical section
        pthread_mutex_lock(&lock);
        
        shared_counter++; // Critical section (shared resource)
        
        // Unlock the mutex immediately after
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // 1. Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    // 2. Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_task, NULL);
    }

    // 3. Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 4. Clean up the mutex
    pthread_mutex_destroy(&lock);

    printf("Expected counter: %d\n", NUM_THREADS * INCREMENTS);
    printf("Actual counter:   %d\n", shared_counter);

    return 0;
}
