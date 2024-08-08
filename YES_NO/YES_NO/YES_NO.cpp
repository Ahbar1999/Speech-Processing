// YES_NO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <climits>

#define ll long long  

const int window_size = 200;
const int buf_size = 10000;
int arr[buf_size];

ll arr_energy_base[buf_size / window_size];
ll arr_zcr_base[buf_size / window_size];
// ll avg_zcr_base = 0; 
// ll avg_energy_base = 0;

ll th_energy = 0;
ll th_zcr = 0;  

ll arr_energy_no[buf_size / window_size];
ll arr_zcr_no[buf_size / window_size];
ll avg_energy_no = 0;
ll zcr_no = 0; 

ll arr_energy_yes[buf_size / window_size];
ll arr_zcr_yes[buf_size / window_size];
ll avg_energy_yes = 0;
ll zcr_yes = 0; 

ll arr_energy_test[buf_size / window_size];
ll arr_zcr_test[buf_size / window_size];
ll avg_energy_test = 0;
ll zcr_test = 0;

const char* base_file = "base.txt";
const char* yes_file = "yes2.txt";
const char* no_file = "no2.txt";
const char* test_file = "no2.txt";

void init(int size, ll* arr, ll val) {
	for(int i =0; i < size; i++) arr[i] = val;
}

// sum of squares
void ENERGY(int n, ll* arr_energy) {
	init(buf_size / window_size, arr_energy, th_energy);
	int i = 0;
	ll energy = 0;
	for (int j =0; j < n; j++) {
		energy += (ll)arr[j] * (ll)arr[j];
		if ((j - i + 1) % window_size == 0) {
			// printf("%d\n", j / window_size);
			arr_energy[j / window_size] = energy;
			i = j + 1;
			energy = 0;
		}
	}
	// note that we are discarding the last frame
}

// number of times the signal crosses the x axis
void ZCR(int n, ll* arr_zcr) {
	init(buf_size / window_size, arr_zcr, th_zcr);
	int cnt =0;
	int i =0; 
	for (int j =0; j < n - 1; j++) {
		if ((ll)arr[j] * (ll)arr[j + 1] < 0) cnt++;
		if ((j - i + 1) % window_size == 0) {
			arr_zcr[(j + 1) / window_size] = cnt;
			i = j + 1;
			cnt =0;
		}
	}
}

void average_out(int n, ll* store, ll* arr) {
	ll average = 0;
	for (int i =0; i < n / window_size; i++) {
		average += arr[i];
	}
	*store = average / (n / window_size);
}

void sum(int n, ll* store, ll* arr) {
	ll acc = 0;
	for (int i =0; i < n / window_size; i++) {
		acc += arr[i];
	}
	*store = acc;
}

int read(const char* filepath) { 
	memset(arr, 0, sizeof(arr));
	FILE* file = fopen(filepath, "r");

	if (file == NULL) {
		printf("Could not open the %s\n", filepath);
		return 1;
	}
	int n = 0;	// number of integers read
	for (int i =0; i < buf_size && !feof(file); i++) {
		fscanf(file, "%d", &arr[i]);
		n++;
	}
	fclose(file);

	return n;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	n = read(base_file);
	ENERGY(n, arr_energy_base);
	average_out(n, &th_energy, arr_energy_base);
	ZCR(n, arr_zcr_base);
	average_out(n, &th_zcr, arr_zcr_base);
	
	n = read(yes_file);
	ENERGY(n, arr_energy_yes);
	average_out(n, &avg_energy_yes, arr_energy_yes);
	ZCR(n, arr_zcr_yes);
	sum(n, &zcr_yes, arr_zcr_yes);

	//for (int i =0; i < buf_size / window_size; i++) printf("%d\n", arr_energy_yes[i]);
	//for (int i =0; i < buf_size / window_size; i++) printf("%d\n", arr_zcr_yes[i]);

	n = read(no_file);
	ENERGY(n, arr_energy_no);
	average_out(n, &avg_energy_no, arr_energy_no);
	ZCR(n, arr_zcr_no);
	sum(n, &zcr_no, arr_zcr_no);

	n = read(test_file);
	ENERGY(n, arr_energy_test);
	average_out(n, &avg_energy_test, arr_energy_test);
	ZCR(n, arr_zcr_test);
	sum(n, &zcr_test, arr_zcr_test);
	
	printf("\n\nRESULT: "); 
	if (avg_energy_test > avg_energy_no && avg_energy_test < avg_energy_yes + th_energy 
		&& zcr_test < zcr_no) {
		printf("YES\n");	
	} else if (avg_energy_test < avg_energy_yes && zcr_test > zcr_yes) {
		printf("NO\n");	
	} else {
		printf("Unrecognizable speech\n");
	}
	
	return 0;
}
