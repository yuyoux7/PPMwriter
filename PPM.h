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
#if __has_include(<cmath>)
#include <cmath>
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
#if __has_include(<math.h>)
#include <math.h>
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
    bool lock;
    bool init;
    FILE* File;
}PPMImage;

bool InitializationPPMImage(PPMImage* self);
bool OpenPPMFile(PPMImage* self, const char* filename);
void SetPPMSize(PPMImage* self, int Width, int hight);
void WritePPM_C(PPMImage* self, int r, int g, int b);
void WritePPM_V3(PPMImage* self, ColorVector3 color);
void ReadPPM(PPMImage* self, const char* filename);
bool ClosePPMFile(PPMImage* self);
bool FreePPMImage(PPMImage* self);

bool InitializationPPMImage(PPMImage* self)
{
    if (self->init)
        return false;
    self->Height = 0;
    self->Width = 0;
    self->Data = nullptr;
    self->File = nullptr;
    self->lock = false;
    self->init = true;
    return true;
}

bool OpenPPMFile(PPMImage* self, const char* filename)
{
    if (!self->init)
        return false;
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
            fprintf(stderr, "Memory error");
        }
        else {
            self->lock = true;
            self->Data->_Size_ = Width * hight + 1;
            self->Data->Data.Data = (ColorVector3*)calloc(self->Data->_Size_, sizeof(ColorVector3));
            if (!self->Data->Data.Data)
                fprintf(stderr, "Memory error");
            self->Data->Data._Size_ = 0;
        }
    }
}

void WritePPM_C(PPMImage* self, int r, int g, int b)
{
    if (self->lock && (((self->Data->Data._Size_ - 2) < self->Data->_Size_) || self->Data->Data._Size_ < 2))
    {
        *(self->Data->Data.Data + self->Data->Data._Size_) = (ColorVector3){r, g, b};
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
    self->init = false;
    return true;
}

void ReadPPM(PPMImage* self, const char* filename)
{
    ClosePPMFile(self);
    InitializationPPMImage(self);
    self->File = fopen(filename, "rb");
    char *flash = (char*)malloc(sizeof(char) * 0xFFFFFF);
    int local = 0, hlc = 0, h1 = 0, h2 = 0, h3 = 0;
    while (!feof(self->File))
    {
        (*(flash + local) = fgetc(self->File));
        local++;
    }
    *(flash + (local + 1)) = '\0';
    char* header = (char*)malloc(sizeof(char) * (local + 2));
    for (int i = 0;*(flash + i) != '\0';i++)
    {
        if (*(flash + i) == '#')
            while (*(flash + i) != '\n') { i++; }
        if (*(flash + i) != '\n')
        {
            *(header + hlc) = *(flash + i);
            hlc++;
        }
        else
            if (!h1)
                h1 = hlc;
            else if(!h2)
                h2 = hlc;
            else if(!h3)
            {
                h3 = hlc;
                break;
            }
    }
    char lsw[32] = {};
    char lsh[32] = {};
    int i = 0, wl = 0, hl = 0, width = 0, height = 0, cw = 0;
    for (; *(header + (h1 + i)) != ' '; i++)
    {
        lsw[wl] = *(header + (h1 + i));
        wl++;
    }
    i++;
    for (; (h1 + i) < h2; i++)
    {
        lsh[hl] = *(header + (h1 + i));
        hl++;
    }
    for (int j = 0; j < wl; j++)
    {
        width += (lsw[j] - '0') * pow(10, ((wl - j) - 1));
    }
    for (int j = 0; j < hl; j++)
    {
        height += (lsh[j] - '0') * pow(10, ((hl - j) - 1));
    }
    SetPPMSize(self, width, height);
    for (int j = 0; (h2 + j) < h3; j++)
    {
        cw += ((*(header + (h2 + j))) - '0') * pow(10, h3 - (h2 + j));
    }
    for (int j = 0; j < (local - h3) / 3; j++)
    {
        ColorVector3 gcolor = {0, 0, 0};
        gcolor.r = *(flash + h3 + 3 + j * 3) & 0xFF;
        gcolor.g = *(flash + h3 + 3 + j * 3 + 1) & 0xFF;
        gcolor.b = *(flash + h3 + 3 + j * 3 + 2) & 0xFF;
        self->Data->Data.Data[j] = gcolor; 
    }
    free(flash);
    free(header);
}

bool FreePPMImage(PPMImage* self)
{
    if (self->lock) {
        free(self->Data);
    }
    else
    {
        return false;
    }
    self->Data = nullptr;
    self->lock = false;
    return true;
}

#if __cplusplus
}
#endif