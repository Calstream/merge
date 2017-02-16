#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<windows.h>

#define ARR_LENGTH 9
#define MAX_VALUE 10 
#define N_THR 4



typedef struct array
{
	int * data;
	int size;
}array;

typedef struct t_aux
{
	array a;
	int thread_id;
}t_aux, *taux;

typedef struct queue 
{
	array * s_arrays;
	int size;
	int cur_head; //cur
	int last_oc; //free
}queue;

queue bag;

void enqueue(array arr)
{
	int n = arr.size;
	bag.last_oc = (bag.last_oc + 1) % ARR_LENGTH;
	bag.s_arrays[bag.last_oc].data = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		bag.s_arrays[bag.last_oc].data[i] = arr.data[i];
	bag.size++;
}

array dequeue()
{
	array res;
	int n = bag.s_arrays[bag.cur_head].size;
	res.data = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		res.data[i] = bag.s_arrays[bag.cur_head].data[i];
	bag.size--;
	bag.cur_head = (bag.cur_head + 1) % ARR_LENGTH;
}

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

	free(left);
	free(right);
}

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

HANDLE Mutex;

int * is_free_t;

DWORD WINAPI SPLIT(LPVOID Param)
{
	taux tbs = (taux)(Param);
	int length = tbs->a.size;
	int m = length / 2;

	array left;
	array right;
	left.data = malloc(m * sizeof(int));
	right.data = malloc((length - m) * sizeof(int));

	for (int i = 0; i < m; i++)
		left.data[i] = tbs->a.data[i];
	for (int j = 0; j < length - m; j++)
		right.data[j] = tbs->a.data[m + j];

	DWORD wait_res = WaitForSingleObject(Mutex, INFINITE);
	enqueue(left);
	enqueue(right);
	ReleaseMutex(Mutex);
}

DWORD WINAPI MERGE(LPVOID Param) // Param - array
{

}

int get_thread_i(int * a)
{
	for (size_t i = 0; i < N_THR; ++i)
		if (a[i] != 1)
			return i;
	return -1;
}

int main()
{
	clock_t start = clock();
	time_t t;
	srand((unsigned)time(&t));

	array arr;
	arr.size = ARR_LENGTH;
	arr.data = malloc(ARR_LENGTH * sizeof(int));
	for (int i = 0; i < ARR_LENGTH; i++)
	{
		int x = rand() % MAX_VALUE + 1;
		arr.data[i] = x;
	}

	//printf("arr: \n");
	printArray(arr.data);

	queue bag;
	bag.s_arrays = malloc(ARR_LENGTH * ARR_LENGTH * sizeof(int)); // :)))))))))))
	bag.cur_head = 0;
	bag.last_oc = -1;
	enqueue(arr);
	printf("\n");
	printArray(bag.s_arrays[0].data);

	
	HANDLE thread_handle_array_s[N_THR];
	Mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	is_free_t = calloc(N_THR, sizeof(int));

	//split
	while (bag.size < ARR_LENGTH)
	{
		int fi = get_thread_i(is_free_t);
		if (fi != -1)
		{
			array tbs = dequeue();
			t_aux ta;
			ta.a = tbs;
			ta.thread_id = fi;
			thread_handle_array_s[fi] = CreateThread(NULL, 0, SPLIT, &ta, 0, NULL);
			is_free_t[fi] = 1;
		}
		else
		{
			printf("god cant help you now");
		}
	}

	for (int i = 0; i < ARR_LENGTH; i++)
	{
		printf("\n %d: ", i);
		printArray(bag.s_arrays[i].data);
	}
	WaitForMultipleObjects(N_THR, thread_handle_array_s, TRUE, INFINITE);
	for (int ñ = 0; ñ < N_THR; ñ++)
		CloseHandle(thread_handle_array_s[ñ]);

	//merge

	//printf("\nSorted array is \n");
	//printArray(arr);

	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%f", seconds);

	/*int * a1;
	int * a2;
	a1 = calloc(5, sizeof(int));
	a2 = calloc(5, sizeof(int));
	a2[0] = 69;
	a2[1] = 420;
	a1 = a2;
	a2[3] = 73;
	printf("%d ", a1[3]);*/
	getchar();
}