# PPMwriter
A Image file type Writer
# How to Using
```C
#include "PPM.h"

int main()
{
    PPMImage picture = {};    
    InitializationPPMImage(&picture);
    OpenPPMFile(&picture, "example.ppm");
    SetPPMSize(&picture, 255, 255);    
    for (int j = 255; j > 0; j--)
    {
        for (int i = 0; i < 255; i++)
        {
            WritePPM_C(&picture, (((j - i) > 0) ? (j - i) : (i - j)), (i), (255 - j));
        }
    }
    ClosePPMFile(&picture);
    FreePPMImage(&picture);
}
```

# Function Introduction

<h4>Initialization PPM Image Object</h4>

<p5>If it Initialization fail is back false</p5>

```C
bool InitializationPPMImage(PPMImage* self);
```

<h4>Open PPM to catch file Handle</h4>

<p5>If it Open Fail it back false </p5>

```C
bool OpenPPMFile(PPMImage* self, const char* filename);
```

<h4>Set PPM image size</h4>

```C
void SetPPMSize(PPMImage* self, int Width, int hight);
```

<h4>Write PPM </h4>

<p5>Write RGB in PPM in last</p5>

```C
typedef struct {
  int r;
  int g;
  int b;
} ColorVector3;

void WritePPM_C(PPMImage* self, int r, int g, int b);
void WritePPM_V3(PPMImage* self, ColorVector3 color);
```

<h4>Close PPM file Headle</h4>

<p5>If it file no open or can't close it back fales</p5>

```C
bool ClosePPMFile(PPMImage* self);
```

<h4>Free PPM Object</h4>

<p5>If it is can't free it back false</p5>

```C
bool FreePPMImage(PPMImage* self);
```
