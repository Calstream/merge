#include<stdlib.h>
#include<stdio.h>
#include <time.h>

#define ARR_LENGTH 20
#define MAX_VALUE 100 

// First subarray is arr[li..mi]
// Second subarray is arr[mi+1..ri]
void merge(int arr[], int li, int mi, int ri)
{
	int i, j, k; //counters
	int n1 = mi - li + 1;
	int n2 = ri - mi;

	/* temp */
	int * left;
	int * right;
	left = malloc(n1 * sizeof(int));
	right = malloc(n2 * sizeof(int));
	
	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		left[i] = arr[li + i];						// ??? TODO
	for (j = 0; j < n2; j++)
		right[j] = arr[mi + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = li; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (left[i] <= right[j])
		{
			arr[k] = left[i];
			i++;
		}
		else
		{
			arr[k] = right[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		arr[k] = left[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		arr[k] = right[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		int m = (l + r) / 2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}


void printArray(int * arr)
{
	int i;
	for (i = 0; i < ARR_LENGTH; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int main()
{
	clock_t start = clock();
	time_t t;
	srand((unsigned)time(&t));

	int * arr;
	arr = malloc(ARR_LENGTH * sizeof(int));
	for (int i = 0; i < ARR_LENGTH; i++)
	{
		int x = rand() % MAX_VALUE + 1;
		arr[i] = x;
	}

	//printf("arr: \n");
	//printArray(arr);

	mergeSort(arr, 0, ARR_LENGTH - 1);

	//printf("\nSorted array is \n");
	//printArray(arr);

	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%f", seconds);
	getchar();
}