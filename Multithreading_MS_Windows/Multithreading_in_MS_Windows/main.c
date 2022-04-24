#define _CRT_SECURE_NO_WARNINGS
#define MAX_THREADS_HANDLES 50
#define BIG_SIZE 100000
#define SMALL_SIZE 20
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <math.h>
# pragma argsused

HANDLE threads[MAX_THREADS_HANDLES];

struct thread_data
{
	double beg;
	double end;
	int degree;
	int rect_number;
	double result;
};

void gotoxy(short x, short y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

DWORD WINAPI thread_function(void* arguments) {
	struct thread_data* data = (struct thread_data*)arguments;
	double rect_width = (data->end - data->beg) / data->rect_number;
	double surface_area = 0;
	for (int i = 0; i < data->rect_number; i++) {
		surface_area += pow((data->beg + i * rect_width), data->degree) * rect_width;
	}
	data->result = surface_area;
}

void calculate(int beg, int end, int degree, int rect_number, int threads_number) {
	DWORD id;

	double x = end - beg;
	double part_for_thread = x / threads_number;

	struct thread_data* data = (struct thread_data*)malloc(threads_number * sizeof(struct thread_data));

	for (int i = 0; i < threads_number; i++) {
		data[i].beg = beg + i * part_for_thread;
		data[i].end = beg + (i + 1) * part_for_thread;
		data[i].degree = degree;
		data[i].rect_number = rect_number / threads_number;
	}

	for (int i = 0; i < threads_number; i++)
	{
		threads[i] = CreateThread(
			NULL,
			0,
			thread_function,
			(void*)&data[i],
			0,
			&id);

		if (threads[i] != INVALID_HANDLE_VALUE)
			printf("Thread (id: %d) has been created.\n ", id);
	}

	double result = 0;
	WaitForMultipleObjects(threads_number, threads, 1, INFINITE);
	for (size_t i = 0; i < threads_number; i++) {
		CloseHandle(threads[i]);
		result += data[i].result;
	}

	printf("Result of integral from %d to %d = %lf\n", beg, end, result);

	free(data);
}

int priorities[3] = { THREAD_PRIORITY_BELOW_NORMAL ,
THREAD_PRIORITY_NORMAL , THREAD_PRIORITY_ABOVE_NORMAL
};

int main(int argc, char** argv)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	CreateProcess(TEXT("C:\\Windows\\System32\\notepad.exe"),
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	
	system("cls");

	int threads_numbers[SMALL_SIZE];
	float time_measurements[SMALL_SIZE];

	int threads_number = 1;
	clock_t beg = clock();
	calculate(0, 5, 3, BIG_SIZE, threads_number);
	clock_t end = clock();
	float time = (float)(end - beg) / CLOCKS_PER_SEC;
	threads_numbers[0] = threads_number;
	time_measurements[0] = time;

	threads_number = 2;
	beg = clock();
	calculate(0, 5, 3, BIG_SIZE, threads_number);
	end = clock();
	time = (float)(end - beg) / CLOCKS_PER_SEC;
	threads_numbers[1] = threads_number;
	time_measurements[1] = time;

	threads_number = 4;
	beg = clock();
	calculate(0, 5, 3, BIG_SIZE, threads_number);
	end = clock();
	time = (float)(end - beg) / CLOCKS_PER_SEC;
	threads_numbers[2] = threads_number;
	time_measurements[2] = time;

	for (int i = 3; i < SMALL_SIZE; i++) {
		threads_number = 2 + i;
		beg = clock();
		calculate(0, 5, 3, BIG_SIZE, threads_number);
		end = clock();
		time = (float)(end - beg) / CLOCKS_PER_SEC;
		threads_numbers[i] = threads_number;
		time_measurements[i] = time;
	}

	printf("Number of threads: ");
	for (int i = 0; i < SMALL_SIZE; i++)
		printf("%d ", threads_numbers[i]);
	printf("\nElapsed time: ");
	for (int i = 0; i < SMALL_SIZE; i++)
		printf("%f ", time_measurements[i]);

	Sleep(20000);
	return 0;
}