#pragma once
#define PPMWRITE
#if __cplusplus
#if __has_include(<cstdio>)
#include <cstdio>
#endif
#if __has_include(<cstddef>)
#include <cstddef>
#endif
#if __has_include(<malloc.h>)
#include <malloc.h>
#endif
#if __has_include(<cstdlib>)
#include <cstdlib>
#endif
extern "C" {
#else
#define nullptr (void*)0x0
#if __has_include(<stdio.h>)
#include <stdio.h>
#endif
#if __has_include(<stddef.h>)
#include <stddef.h>
#endif
#if __has_include(<stdbool.h>)
#include <stdbool.h>
#endif
#if __has_include(<stdint.h>)
#include <stdint.h>
#endif
#if __has_include(<malloc.h>)
#include <malloc.h>
#endif
#if __has_include(<stdlib.h>)
#include <stdlib.h>
#endif
#endif
#define ColorRange 256

typedef struct
{
    int r;
    int g;
    int b;
}ColorVector3;

typedef struct 
{
    char** Data;
    size_t _Size_; // pool size
}DataVector2;

typedef struct 
{
    DataVector2 Data;
    size_t _Size_; // Width * Height * Pixel(r, g, b)
}Pixel;

typedef struct 
{
    int Width;
    int Height;
    Pixel* Data;
    FILE* File;
}Image;

bool InitializationImage(Image* self);
bool OpenPPMFile(Image* self, const char* filename);
void SetPPMSize(Image* self, int Width, int hight);
char* IntToStr(int Data);
void WritePPM_C(Image* self, int r, int g, int b);
void WritePPM_V3(Image* self, ColorVector3 color);
bool ClosePPMFile(Image* self);
bool FreeImage(Image* self);

bool InitializationImage(Image* self)
{
    self->Height = 0;
    self->Width = 0;
    self->Data = nullptr;
    self->File = nullptr;
    return true;
}

bool OpenPPMFile(Image* self, const char* filename)
{
    self->File = fopen(filename, "wb");
    if (!self->File)
        return false;
    fputs("P6\n", self->File);   
    return true;
}

void SetPPMSize(Image* self, int Width, int hight)
{
    self->Width = Width;
    self->Height = hight;
    char flash[256];
    sprintf(flash, "%d %d\n", Width, hight);
    if (self->File){
        fputs(flash, self->File); 
        fputs("255\n", self->File);  
    }
    else {
        fprintf(stderr, "flie not open to write");
    }
    self->Data = (Pixel*)malloc(sizeof(Pixel));
    if (!self->Data)
    {
        fprintf(stderr, "Memory self->Data error");
    }
    else {
        self->Data->_Size_ = Width * hight * 3;
        self->Data->Data.Data = (char**)malloc(Width * hight * sizeof(char*) * 8);
        *self->Data->Data.Data = (char*)malloc(Width * hight * sizeof(char));
        if (!self->Data->Data.Data)
        {
            fprintf(stderr, "Memory self->Data->Data.Data error");
        }
        self->Data->Data._Size_ = 0;
    }
}

char* IntToStr(int Data)
{
    int _Size_ = 0;
    char flash[3];
    char* recflash;
    for (int i = (Data % ColorRange); i; i/=10)
    {
        flash[_Size_] = ((i % 10) + '0');
        _Size_++;
    }
    if (!Data)
    {
        recflash = (char*)malloc(sizeof(char));
        *recflash = '0';
        return recflash;
    }
    recflash = (char*)malloc(sizeof(char) * _Size_);
    for (int i = 0; i < _Size_; i++)
    {
        *(recflash + i) = flash[(_Size_ - i) - 1];
    }
    return recflash;
}

void WritePPM_C(Image* self, int r, int g, int b)
{
    if (self->Data && (((self->Data->Data._Size_ - 2) < self->Data->_Size_) || self->Data->Data._Size_ < 2))
    {
        *(self->Data->Data.Data + self->Data->Data._Size_) = IntToStr(r);
        fputc(r, self->File);
        self->Data->Data._Size_++;
        *(self->Data->Data.Data + self->Data->Data._Size_) = IntToStr(g);
        fputc(g, self->File);
        self->Data->Data._Size_++;
        *(self->Data->Data.Data + self->Data->Data._Size_) = IntToStr(b);
        fputc(b, self->File);
        self->Data->Data._Size_++;
    }
}

void WritePPM_V3(Image *self, ColorVector3 color)
{
    WritePPM_C(self, color.r, color.g, color.b);
}

bool ClosePPMFile(Image* self)
{
    if (self->File)
        if (fclose(self->File))
            return false;
    self->File = nullptr;
    return true;
}

bool FreeImage(Image* self)
{
    if (self->Data){
        if (self->Data->Data.Data)
           free(self->Data->Data.Data);
        free(self->Data);
    }
    self->Data = nullptr;
    return true;
}

#if __cplusplus
}
#endif