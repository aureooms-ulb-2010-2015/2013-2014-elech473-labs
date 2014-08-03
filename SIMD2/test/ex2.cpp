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
	size_t size, width, height;
};

enum ACTION{ C, SIMD };

enum MODE{ MIN, MAX };

template<typename T>
T max(const T& a, const T& b){
	return a > b ? a : b;
}

template<typename T>
T min(const T& a, const T& b){
	return a < b ? a : b;
}

void c(const RAW in, const RAW out, MODE mode){

	size_t i, j;

	unsigned char(*opt)(const unsigned char&, const unsigned char&) = mode == MODE::MIN ? &min<unsigned char> : &max<unsigned char>;

	// DOESN'T HANDLE BORDERS

	for (i = 1; i < in.width - 1; ++i){
		for (j = 1; j < in.height - 1; ++j){
			unsigned char O = in.content[(i - 1)*in.width + j - 1];
			O = opt(O, in.content[(i - 1)*in.width + j]);
			O = opt(O, in.content[(i - 1)*in.width + j + 1]);
			O = opt(O, in.content[i*in.width + j - 1]);
			O = opt(O, in.content[i*in.width + j]);
			O = opt(O, in.content[i*in.width + j + 1]);
			O = opt(O, in.content[(i + 1)*in.width + j - 1]);
			O = opt(O, in.content[(i + 1)*in.width + j]);
			O = opt(O, in.content[(i + 1)*in.width + j + 1]);
			out.content[i*in.width + j] = O;
		}
	}

}

void simd(const RAW in, const RAW out, const MODE mode){

	unsigned char* _in = &in.content[1024];
	unsigned char* _out = &out.content[1024 + 1];
	const size_t step = 14;
	const size_t i = (1022 * 1022) / step;

	_asm {
		start:
			mov esi, _in; // datain ptr of the line
			mov ecx, i; // counter
			mov edi, _out; // dataout pointer
			mov edx, 74; // 1024 / 14 + 1

		l0:
			sub edx, 1;
			jnz l1;
			mov edx, 73; // 1024 / 14
			add esi, 2
			add edi, 2
		l1:
			movdqu xmm0, [esi - 1024];  // l1st line
			movdqu xmm1, [esi];         // 2nd line
			movdqu xmm2, [esi + 1024];  // 3rd line 
			pmaxub xmm0, xmm1;  // compare
			pmaxub xmm0, xmm2;  
			movdqu xmm6, xmm0;  // copy 
			movdqu xmm7, xmm0;
			psrldq xmm6, 1;   // shift right because endianess is reversed ...
			psrldq xmm7, 2;
			pmaxub xmm0, xmm6;  // column max
			pmaxub xmm0, xmm7;   
			movdqu[edi], xmm0;  // move result
			add edi, step;  // new result pointer
			add esi, step;  // new src pointer
			sub ecx, 1;
			jnz l0;   // end

		end:
			emms;
	}


}

void pad(const RAW img, const unsigned char color){

	for (size_t j = 0; j < 1024; ++j){
		img.content[j] = color;
		img.content[1023 * 1024 + j] = color;
		img.content[j * 1024] = color;
		img.content[(j + 1) * 1024 - 1] = color;
	}
}

int write_file(const char * path, const RAW out){

	FILE * fd = fopen(path, "wb");

	if (fd == NULL){
		printf("%s\n", path);
		printf("%s\n", "fd 2 null");
		return 12;
	}

	fwrite(out.content, sizeof(unsigned char), out.size, fd);
	return 0;
}

int get_file(const char* path, RAW& in, size_t width){

	FILE * fd;

	fd = fopen(path, "rb");
	if (fd == NULL){
		printf("%s\n", path);
		printf("%s\n", "fd 1 null");
		return 11;
	}

	fseek(fd, 0, SEEK_END);
	in.size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	in.content = (unsigned char*)malloc(in.size);
	fread(in.content, sizeof(unsigned char), in.size, fd);

	in.width = width;
	in.height = in.size / width;

	return 0;
}

int parse_args(const int argc, const char* const argv[], ACTION& action, MODE& mode, size_t& width, size_t& n){



	if (argc < 5){
		printf("argc (%d) < 5\n", argc);
		return 100;
	}


	if (strcmp("C", argv[2]) == 0) action = ACTION::C;
	else if (strcmp("SIMD", argv[2]) == 0) action = ACTION::SIMD;
	else   return 2;

	if (strcmp("MIN", argv[3]) == 0) mode = MODE::MIN;
	else if (strcmp("MAX", argv[3]) == 0) mode = MODE::MAX;
	else   return 33;

	width = atoi(argv[4]);

	if (argc > 5) n = atoi(argv[5]);

	return 0;
}

int do_it(const int argc, const char* const argv[]) {


	// INIT
	ACTION action;
	MODE mode;
	size_t width, n = 1;
	int e;

	e = parse_args(argc - 1, &argv[1], action, mode, width, n);
	if (e != 0) return e;
	RAW in, out;

	// IN
	e = get_file(argv[1], in, width);
	if (e != 0) return e;

	out.content = (unsigned char*)malloc(in.size);
	out.size = in.size;

	// DO
	time_t start = clock();
	if (action == ACTION::SIMD) for (size_t i = 0; i < n; ++i) simd(in, out, mode);
	else                        for (size_t i = 0; i < n; ++i) c(in, out, mode);
	time_t end = clock();
	float dt = (end - start) / (float)(CLOCKS_PER_SEC);
	printf("%f s\n", dt);

	// CLEAN BORDERS
	pad(out, 127);

	// OUT
	free(in.content);
	e = write_file(argv[2], out);
	if (e != 0) return e;
	free(out.content);


	return 0;
}

int main(const int argc, const char* const argv[]) {


	int e = do_it(argc, argv);

	if (e != 0) printf("ERROR CODE: %d", e);

	return e;
}