#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>

_Atomic int counter = 0;

int threadFunc(void *arg)
{
    printf("hello from thread %s\n", (const char *)arg);
    atomic_fetch_add_explicit(&counter, 1, memory_order_release);
    return 0;
}

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
int main(int argc, char **argv)
{
    printf("hello world\n");
    const char *thread_names[] = {"thread 1", "thread 2", "thread 3",
                                  "thread 4", "thread 6", "thread 7",
                                  "thread 8", "thread 9", "thread 10"};
    thrd_t threads[ARR_LEN(thread_names)];

    for (int i = 0; i < ARR_LEN(thread_names); i++)
    {
        thrd_create(&threads[i], threadFunc, (void *)thread_names[i]);
    }

    printf("waiting for threads ...\n");
    // NOTE: if any of threads would break this would spin lock forever
    while (atomic_load_explicit(&counter, memory_order_acquire) !=
           ARR_LEN(thread_names))
    {
        // spin lock
    }
    printf("all threads done ...\n");

    return 0;
}
