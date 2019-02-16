<h1 align="center">
    <p>Prime Sieve</p>
    <img src="https://img.icons8.com/cotton/64/000000/cloud-binary-code.png"
    style="width: auto; height: auto; max-width: 100%; max-height: 100%">
</h1>

<div align="center">
  <p>An implementation of parallel prime sieve using Sieve-of-Eratosthenes algorithm with pthreads.</p>
</div>

- [Implementation](#implementation)
- [How to run](#how-to-run)
- [Improvements](#improvements)
- [Reference](#reference)

## Implementation

Prime sieve is an attractive research field, since prime numbers are largely used in encryption/decryption
and high performance benchmark field. The Sieve-of-Eratosthenes is a well-known way to find all prime numbers in a
given range.

The parallel way of this program is straightforward, the range will be divided into many parts, each
part will be handled by one thread.
 
## How to run

Make sure the system has a `cmake` with a 2.8 or higher version, `gcc` with a 4.8 or higher version.

Run the cmake to generate makefile:

```sh
$ cmake .
```

Makefile has been created, just run `make` command in terminal:

```sh
$ make
```

An executable program called `sieve_of_eratosthenes` is generated.

Then you can run it and there is a CLI for the program:

```sh
$ ./sieve_of_eratosthenes 

A CLI to find Primes using Sieve of Eratosthenes
Usage:
  ./sieve_of_eratosthenes [OPTION...]

  -m, --max arg     maximum value of the number
  -t, --thread arg  number of threads
      --help        Print help
```

Run the program with the specific arguments:

```sh
$ ./sieve_of_eratosthenes -m 1000 -t 1
```

There is a test script called `job.sh` provided within the project root, you can run it instead:

```sh
$ ./job.sh
```

It will run multiple pre-set test jobs.

## Improvements

This program has some places that can improve and optimize:

1. Algorithm

   The Sieve-of-Eratosthenes is poor at finding large prime numbers. There is a more fast and modern algorithm called
   Sieve of Atkin.
   
2. Workload Balance
    
    The workload of each thread is not allocate in a fair strategy. The later threads have huge workload
    to calculate many big prime numbers, a more reasonable partition strategy should be considered.

## Reference

[Parallel Prime Sieve](http://www.massey.ac.nz/~mjjohnso/notes/59735/seminars/01077635.pdf)

 <a href="https://icons8.com/icon/91875/cloud-binary-code" style="display:none;">Cloud Binary Code icon by Icons8</a>