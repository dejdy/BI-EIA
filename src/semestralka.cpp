#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/time.h>
#include<ctime>
#include<ratio>
#include<chrono>
#define MAX_EL 100
#define DEFAULT_ARR_SIZE 50
#define THRESHOLD 2 // MUST BE LARGER THEN 0!!!
#define ALG INSERTION

int swapNum = 0;

typedef enum
{
    INSERTION, MERGE
} alg;

void printArray(int * a, int s) // Prints array a, containing s elements
{
    for(int i = 0; i<s; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int * allocateArray(int s)
{
    int * arr = (int*) malloc(s*sizeof(int));
    return arr;
}

int * fillArrayRand(int s)
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);

    int * array = allocateArray(s);
    for(int i = 0; i < s; i++)
    {
        array[i] = rand() % MAX_EL + 1;
    }
    return array;
}

char * parseArguments(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("No arguments passed. Randomly generated array of size %d will be used.\n\n", DEFAULT_ARR_SIZE);// No arguments passed
        return NULL;
    }
    else if (argc ==2 ) return argv[1];
    else
    {
        printf("Too many arguments passed. Terminating...\n");
        exit(0);
    }
    return NULL;
}

void swap(int * xs, int i, int j)
{
    swapNum++;
    int tmp = xs[i];
    xs[i] = xs[j];
    xs[j] = tmp;
}





/*Algoritmus podle Elementary Algorithms od Larry LIU Xinyu*/

/*
 * merge two sorted subs xs[i, m) and xs[j...n) to working area xs[w...]
 */
void wmerge(int * xs, int i, int m, int j, int n, int w)
{
    while (i < m && j < n)
    {
        if(xs[i] < xs[j])
        {
            swap(xs, w, i);
            i++;
        }
        else
        {
            swap(xs, w, j);
            j++;
        }
        w++;
    }

    while (i < m)
    {
        swap(xs, w++, i++);
    }

    while (j < n)
    {
        swap(xs, w++, j++);
    }
}

void imsort(int * xs, int l, int u);

/*
 * sort xs[l, u), and put result to working area w.
 * constraint, len(w) == u - l
 */
void wsort(int * xs, int l, int u, int w)
{
    int m;
    if (u - l > 1)
    {
        m = l + (u - l) / 2;
        imsort(xs, l, m);
        imsort(xs, m, u);
        wmerge(xs, l, m, m, u, w);
    }
    else
    {
        while (l < u)
        {
            swap(xs, l, w);
            l++;
            w--;
        }
    }
}

void insertionSort(int * xs, int l, int u)
{
    int i, j;
    for (i = l; i < u; i++)
    {
        j = i;
        while (j > l && xs[j - 1] > xs[j])
        {
            swap(xs, j, j-1);
            j--;
        }
    }
}


void kmerge(int * xs, int * ys, int l, int m, int u) {
    int i, j, k;
    i = l;
    k = l; j = m;
    while (i < m && j < u)
        ys[k++] = xs[i] < xs[j] ? xs[i++] : xs[j++];
    while (i < m)
        ys[k++] = xs[i++];
    while (j < u)
        ys[k++] = xs[j++];
    for(; l < u; ++l)
        xs[l] = ys[l];
}

void kmsort(int * xs, int * ys, int l, int u) {
    int m;
    if (u - l > 1) {
        m = l + (u - l) / 2;
        kmsort(xs, ys, l, m);
        kmsort(xs, ys, m, u);
        kmerge(xs, ys, l, m, u);
    }
}

void mergeSort(int * xs, int l, int u) {
    int * ys = (int *) malloc(sizeof(int) * (u));
    kmsort(xs, ys, l, u);
    free(ys);
}


void smallSort(int * xs, int l, int u)
{

    switch (ALG)
    {
        case INSERTION:
        insertionSort(xs, l, u);
        break;

        case MERGE:
        mergeSort(xs, l, u);
        break;
    }
}

void imsort(int * xs, int l, int u)
{
    int m, n, w;
    if (u - l > 1)
    {
        m = l + (u - l) / 2; /* half of the array part */
        w = l + u - m; /* working area position */
        wsort(xs, l, m, w); /* the last half contains sorted elements */
        while (w - l > THRESHOLD)
        {
            n = w;
            w = l + (n - l + 1) / 2;
            wsort(xs, w, n, l);  /* the first half of the previous working area contains sorted elements */
            wmerge(xs, l, l + n - w, n, u, w);
        }
        // Here we switch to another sorting algorithm after we have reduced the array to THRESHOLD
        smallSort(xs, l, u);

    }
}

void sort(int * arr, int arrSize)
{
    swapNum = 0;
    //printf("Before sorting: \n");
    //printArray(arr, arrSize);
    printf("\n");
    imsort(arr, 0, arrSize);
    //mergeSort(arr, 0, arrSize);
    //printf("After sorting: \n");
    printArray(arr, arrSize);
    printf("\nNumber of swaps: %d\n", swapNum);
}


int * fillArray(char * path, int * size)
{
    *size = DEFAULT_ARR_SIZE;
    if(!path) return fillArrayRand(DEFAULT_ARR_SIZE);
    int temp;

    int alloc = 10;
    int used = 0;
    int * arr = (int *)malloc(alloc * sizeof(int));
    FILE * input;
    input = fopen(path, "r");
    if(!input)
    {
        printf("No such file!\n");
        free(arr);
        exit(1);
    }

    while (fscanf(input, "%d", &temp) != EOF)
    {
        arr[used] = temp;
        used++;
        if(used+2 >= alloc)
        {
            alloc *= 2;
            arr = (int*)realloc(arr, alloc*sizeof(int));
        }
    }
    fclose(input);
    *size = used;
    printf("Array size: %d\n", used);
    return arr;
}

int main(int argc, char *argv[])
{
    using namespace std::chrono;
    char * path = parseArguments(argc, argv);

    int arrSize;
    int * array = fillArray(path, &arrSize);


    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    sort(array, arrSize);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    //printArray(array, arrSize);

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds.";


    free(array);

    return 0;
}
