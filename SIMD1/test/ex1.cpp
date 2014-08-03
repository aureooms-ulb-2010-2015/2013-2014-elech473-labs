// test.cpp : Defines the entry point for the console application.
//

#include <fcntl.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "StdAfx.h"
#include <stdint.h>




struct RAW {
	unsigned char * content;
	size_t size;
};

enum ACTION{ C, SIMD };

void c(const RAW file, const unsigned char threshold){

	for (size_t i = 0; i < file.size; ++i){
		file.content[i] = file.content[i] > threshold ? 255 : 0;
	}

}

void simd(const RAW file, const unsigned char threshold){

	unsigned char comp[16];
	for (size_t i = 0; i < 16; ++i) comp[i] = threshold;
	unsigned char* _comp = &comp[0];

	uint64_t sign[2];
	sign[0] = 0x8080808080808080;
	sign[1] = 0x8080808080808080;
	uint64_t* _sign = &sign[0];

	size_t l = file.size / 16;
	unsigned char * _ptr = &file.content[0];

	_asm{
		mov esi, _comp;
		movdqu xmm1, [esi];
		mov esi, _sign;
		movdqu xmm2, [esi];
		paddb xmm1, xmm2;
		mov ecx, l;
		mov esi, _ptr;

	label1:
		movdqu xmm0, [esi];
		paddb xmm0, xmm2;
		pcmpgtb xmm0, xmm1;
		movdqu[esi], xmm0;
		add esi, 16;
		sub ecx, 1;
		jnz label1;

		emms;
	}

	for (size_t i = 0; i < file.size % 16; ++i){
		file.content[i] = file.content[i] > threshold ? 255 : 0;
	}


}

int write_file(const char * path, const RAW file){

	FILE * fd = fopen(path, "wb");

	if (fd == NULL){
		printf("%s\n", path);
		printf("%s\n", "fd 2 null");
		return 12;
	}

	fwrite(file.content, sizeof(unsigned char), file.size, fd);
	return 0;
}

int get_file(const char* path, RAW& file){

	FILE * fd;

	fd = fopen(path, "rb");
	if (fd == NULL){
		printf("%s\n", path);
		printf("%s\n", "fd 1 null");
		return 11;
	}

	fseek(fd, 0, SEEK_END);
	file.size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	file.content = (unsigned char*)malloc(file.size);
	fread(file.content, sizeof(unsigned char), file.size, fd);

	return 0;
}

int parse_args(const int argc, const char* const argv[], ACTION& action, unsigned char& threshold, size_t& n){



	if (argc < 4){
		printf("argc (%d) < 4\n", argc);
		return 100;
	}


	if (strcmp("C", argv[2]) == 0) action = ACTION::C;
	else if (strcmp("SIMD", argv[2]) == 0) action = ACTION::SIMD;
	else   return 2;

	threshold = atoi(argv[3]);

	if (argc > 4) n = atoi(argv[4]);

	return 0;
}

int do_it(const int argc, const char* const argv[]) {


	// INIT
	ACTION action;
	unsigned char threshold;
	size_t n = 1;
	int e;
	
	e = parse_args(argc - 1, &argv[1], action, threshold, n);
	if (e != 0) return e;
	RAW file;

	// IN
	e = get_file(argv[1], file);
	if (e != 0) return e;

	// DO
	time_t start = clock();
	if (action == ACTION::SIMD) for (size_t i = 0; i < n; ++i) simd(file, threshold);
	else                        for (size_t i = 0; i < n; ++i) c(file, threshold);
	time_t end = clock();
	float dt = (end - start) / (float)(CLOCKS_PER_SEC);
	printf("%f s\n", dt);

	// OUT
	e = write_file(argv[2], file);
	if (e != 0) return e;
	free(file.content);


	return 0;
}

int main(const int argc, const char* const argv[]) {


	int e = do_it(argc, argv);

	if (e != 0) printf("ERROR CODE: %d", e);

	return e;
}