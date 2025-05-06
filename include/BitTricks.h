#pragma once
#include <iostream>
void PrintBits(char num) {
	for (int i = 7; i >= 0; i--) {
		unsigned int bitmask = ((unsigned char)1 << i);
		bool bit = (bitmask & num) == bitmask;
		const char* c = bit ? "1" : "0";
		printf(c);
	}
	printf("\n");
}

void PrintBitsComm(char num, size_t pos) {
	for (int i = 7; i >= 0; i--) {
		unsigned int bitmask = ((unsigned char)1 << i);
		bool bit = (bitmask & num) == bitmask;
		const char* c = bit ? "1" : "0";
		if (pos - 1 == i)
			printf(",");
		printf(c);
	}
}

void printFixed(char num, char fract) {
	std::cout << double(num) / double(1 << fract) << '\n';
}

unsigned int doubleToFixed(double n, size_t f) {
	return (unsigned int)(n * (1 << f));
}

void printFixed(double num, size_t f) {
	char numc = doubleToFixed(num, f);
	PrintBitsComm(numc, f);
}

void printNums() {
	for (double num = -2; num < 2; num += 1.0 / (double)(1 << 7)) {
		printf("%f: ", num);
		printFixed(num, 7);
		printf(" -> %i\n", ((char)doubleToFixed(num, 7)) & 127);
	}
}