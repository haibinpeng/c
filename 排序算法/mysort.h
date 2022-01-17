#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define N 10
#define SWAP(a,b){int temp;temp=a;a=b;b=temp;}

void bubble(int* a);
void select(int* a);
void insert(int* a);
int partition(int* a, int left, int right);
void quicksort(int* a, int left, int right);