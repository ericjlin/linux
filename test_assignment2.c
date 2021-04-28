#include <stdio.h>
#include <cpuid.h>
#include <stdint.h>

int main(int argc, char **argv) {
	uint32_t eax, ebx, ecx, edx;
	unsigned long long cycles;
	eax = 0x4fffffff;

	__cpuid(0x4fffffff, eax, ebx, ecx, edx);

	cycles = ebx;
	cycles = cycles << 32;
	cycles += ecx;

	printf("CPUID, exits %u, cycles spent in exit=%llu\n", eax, cycles);

 	return 0;
}
