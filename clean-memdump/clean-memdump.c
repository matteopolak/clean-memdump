#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>

#define BUFFER_SIZE 4196
#define EQUAL_COUNT 79

char* countEquals(char* p, unsigned int* count) {
	unsigned int nEquals = 0;

	while (*p == '=') {
		++nEquals;
		++p;
	}

	if (*p == '\0')
		*count += nEquals;
	else {
		if (nEquals == EQUAL_COUNT)
			*count = EQUAL_COUNT;
		else
			*count = 0;
	}

	return p;
}

char* findEqual(char* buffer, unsigned int count) {
	unsigned int n = 0;
	char* p = buffer;

	while (n < count && (*p != '=')) {
		++p;
		++n;
	}

	return p;
}

char* findBoundary(char* buffer, size_t remaining, unsigned int* count) {
	char* result = NULL;
	char* p = buffer;
	size_t n = 0;
	unsigned int c = *count;

	while (n < remaining) {
		++n;

		if (*p == '\0' || *p == '\n') {
			c = 0;
			++p;

			continue;
		}

		++p;

		if (c > 71) {
			c = c;
		}

		if (++c == 72 && (*p == '\n' || *p == '\r')) {
			result = p - 73;
			break;
		}
	}

	*count = c;

	return result;
}

void cleanBuffer(char* buffer, size_t N) {
	char* p = buffer;

	for (size_t n = 0; n < N; ++n, ++p) {
		if (*p == '\0' || *p == '�') {
			*p = ' ';
		}
	}
}

int main(int argc, char** argv) {
	char buffer[BUFFER_SIZE + 1];
	FILE* fin;

	unsigned int nEquals = 0;
	long start = -1;
	char* p = NULL;
	size_t N;

	fin = fopen(argv[1], "rb");

	if (fin == NULL)
		return -1;

	buffer[BUFFER_SIZE] = '\0';

	while (!feof(fin)) {
		N = fread(buffer, 1, BUFFER_SIZE, fin);

		p = findEqual(buffer, BUFFER_SIZE);

		while (*p != '\0') {
			p = countEquals(p, &nEquals);
			if (nEquals == EQUAL_COUNT) {
				start = ftell(fin) - ((long) N - (long) (p - buffer));
				nEquals = 0;
				++j;
			}
			p = findEqual(p, (unsigned int) N - (unsigned int) (p - buffer));
		}
	}

	if (start >= 0) {
		fseek(fin, start, SEEK_SET);

		unsigned int count = 0;

		while (!feof(fin)) {
			N = fread(buffer, 1, BUFFER_SIZE, fin);
			p = findBoundary(buffer, N, &count);

			cleanBuffer(buffer, N);

			if (!p) {
				fwrite(buffer, 1, N, stdout);
			} else {
				fwrite(buffer, 1, p - buffer, stdout);

				break;
			}
		}
	}

	fclose(fin);
}