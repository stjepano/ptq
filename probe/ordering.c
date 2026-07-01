#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>

int data = 0;
_Atomic bool ready = false;

int producer(void *ignored)
{
    int i;
    for (i = 0; i < 42; i++)
    {
    }
    data = i;
    atomic_store_explicit(&ready, true, memory_order_release);
    return 0;
}

int consumer(void *ignored)
{
    while (!atomic_load_explicit(&ready, memory_order_acquire))
    {
        // spin
    }
    printf("data: %d\n", data);
    return 0;
}

int main(int argc, char **argv)
{
    printf("hello world\n");

    thrd_t producer_thread, consumer_thread;
    thrd_create(&consumer_thread, consumer, nullptr);
    thrd_create(&producer_thread, producer, nullptr);

    thrd_join(consumer_thread, nullptr);

    return 0;
}
