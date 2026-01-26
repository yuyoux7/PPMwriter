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
    ColorVector3* Data;
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
}PPMImage;

bool InitializationPPMImage(PPMImage* self);
bool OpenPPMFile(PPMImage* self, const char* filename);
void SetPPMSize(PPMImage* self, int Width, int hight);
void WritePPM_C(PPMImage* self, int r, int g, int b);
void WritePPM_V3(PPMImage* self, ColorVector3 color);
bool ClosePPMFile(PPMImage* self);
bool FreePPMImage(PPMImage* self);

bool InitializationPPMImage(PPMImage* self)
{
    self->Height = 0;
    self->Width = 0;
    self->Data = nullptr;
    self->File = nullptr;
    return true;
}

bool OpenPPMFile(PPMImage* self, const char* filename)
{
    self->File = fopen(filename, "wb");
    if (!self->File)
        return false;
    fputs("P6\n", self->File);   
    return true;
}

void SetPPMSize(PPMImage* self, int Width, int hight)
{
    if (Width && hight) {
        self->Width = Width;
        self->Height = hight;
        char flash[256];
        sprintf(flash, "%d %d\n", Width, hight);
        if (self->File){
            fputs(flash, self->File); 
            fputs("255\n", self->File);  
        }
        else {
            fprintf(stderr, "Flie not opens");
        }
        self->Data = (Pixel*)malloc(sizeof(Pixel));
        if (!self->Data)
        {
            fprintf(stderr, "Memory self->Data error");
        }
        else {
            self->Data->_Size_ = Width * hight * 3;
            self->Data->Data.Data = (ColorVector3*)malloc(sizeof(ColorVector3) * self->Data->_Size_);
            if (!self->Data->Data.Data)
                fprintf(stderr, "Memory self->Data->Data.Data error");
            self->Data->Data._Size_ = 0;
        }
    }
}

void WritePPM_C(PPMImage* self, int r, int g, int b)
{
    if (self->Data && (((self->Data->Data._Size_ - 2) < self->Data->_Size_) || self->Data->Data._Size_ < 2))
    {
        *(self->Data->Data.Data + self->Data->Data._Size_) = {r, g, b};
        fputc(r, self->File);
        fputc(g, self->File);
        fputc(b, self->File);
        self->Data->Data._Size_++;
    }
}

void WritePPM_V3(PPMImage *self, ColorVector3 color)
{
    WritePPM_C(self, color.r, color.g, color.b);
}

bool ClosePPMFile(PPMImage* self)
{
    if (self->File)
        if (fclose(self->File))
            return false;
    self->File = nullptr;
    return true;
}

bool FreePPMImage(PPMImage* self)
{
    if (self->Data){
        free(self->Data);
    }
    self->Data = nullptr;
    return true;
}

#if __cplusplus
}
#endif