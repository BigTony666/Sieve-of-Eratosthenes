/**********************************
 * Parallel Sieve of Eratosthenes in prthreads
 *
 * DESCRIPTION: A program to find primes in a parallel way by using pthreads
 *
 * Author: Kejie Zhang
 * LAST UPDATED: 02/01/2019
 *
 * USEFUL REFERENCE:
 *    -> Pthreads: https://computing.llnl.gov/tutorials/pthreads/
**********************************/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "cxxopts.hpp"

/** Define global variables */
pthread_mutex_t mutex_sum;
long sum;
std::vector<int> primes;

struct thread_data {
    int thread_id;
    int thread_total;
};

/** Parse the command line parameters */
cxxopts::ParseResult
parse(int argc, char *argv[]) {
    try {
        cxxopts::Options options(argv[0], "A CLI to find Primes using Sieve of Eratosthenes");
        options
                .positional_help("[optional args]")
                .show_positional_help();

        options
                .allow_unrecognised_options()
                .add_options()
                        ("m, max", "maximum value of the number", cxxopts::value<long>())
                        ("t, thread", "number of threads", cxxopts::value<int>())
                        ("help", "Print help")
#ifdef CXXOPTS_USE_UNICODE
#endif
                ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        return result;

    } catch (const cxxopts::OptionException &e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}

/** Time conversion helper */
float transferTime(clock_t &tempo) {
    return (float) tempo / CLOCKS_PER_SEC;
}

/** Find primes worker function */
void *findPrime(void *thread_arg) {
    /** Define and use local variables for convenience */
    struct thread_data *local_data = (struct thread_data *) thread_arg;
    int partition = local_data->thread_id;
    int thread_total = local_data->thread_total;
    int i, start, end, range;
    long local_sum = 0;

    /** Calculate the partition range */
    printf("Thread %d is created and working...\n", partition);
    range = primes.size() / thread_total;
    if (range * thread_total < primes.size()) {
        range += 1;
    }
    start = 1 + partition * range;
    end = start + range;
    if (partition == thread_total - 1) {
        end = primes.size();
    }

    /** Sieve of Eratosthenes */
    for (i = start; i < end; i++) {
        if ( i < 3) {
            if (i == 1) {
                primes[i] = 1;
            } else {
                primes[i] = 0;
            }
        } else {
            if (i % 2 == 0) {
                primes[i] = 1;
            }
        }
    }

    for (i = start; i < end; i++) {
        if (i < 3) {
            continue;
        } else
        {
            if (primes[i] != 1) {
                for (int j = 3; j < i; j += 2) {
                    if (j * j > i)
                        break;
                    if (i % j == 0) {
                        primes[i] = 1;
                        break;
                    }
                }
            }
        }
    }

    for (i = start; i < end; i++) {
        if (i > 1 && primes[i] == 0) {
            local_sum++;
        }
    }

    pthread_mutex_lock(&mutex_sum);
    sum += local_sum;
    pthread_mutex_unlock(&mutex_sum);


    pthread_exit((void *) 0);
}

/** Main function */
int main(int argc, char *argv[]) {
    long max = 100;
    int num_threads = 8;
    clock_t start, end, tempo;
    float time;

    /*** Parse the command line parameters */
    auto result = parse(argc, argv);
    if (result.count("max")) {
        max = result["max"].as<long>();
    }
    if (result.count("thread")) {
        num_threads = result["thread"].as<int>();
    }

    pthread_attr_t attr;
    struct thread_data thread_data_array[num_threads];
    pthread_t threads[num_threads];
    void *status;

    primes.assign(max, 0);

    pthread_mutex_init(&mutex_sum, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    /** Create threads and start work */
    std::cout << "Creating " << num_threads << " threads........." << std::endl;
    start = clock();
    for (int tid = 0; tid < num_threads; tid++) {
        thread_data_array[tid].thread_id = tid;
        thread_data_array[tid].thread_total = num_threads;

        int rc = pthread_create(&threads[tid], &attr, findPrime, (void *) &thread_data_array[tid]);

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /**
     * Wait for all threads to complete their work.
     * Free attribute and wait for the other threads
     */
    for (int tid = 0; tid < num_threads; tid++) {
        int rc = pthread_join(threads[tid], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            return -1;
        }
    }
    end = clock();
    tempo = end - start;

    /** Collect time */
    time = transferTime(tempo);

    /** Log the result */
    std::cout << "Total number of primes: " << sum << std::endl;
    std::cout << "Total time: " << time << std::endl;

    pthread_mutex_destroy(&mutex_sum);
    pthread_exit(NULL);

}