// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004-2008 Forgotten and the VBA development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "filters.h"

      //
      // Screen filters
      //

extern int Init_2xSaI(u32);
extern void hq2x_init(unsigned);
extern bool sdlStretchInit(int colorDepth, int sizeMultiplier, int srcWidth);

extern void sdlStretch1x(u8*,u32,u8*,u8*,u32,int,int);
extern void sdlStretch2x(u8*,u32,u8*,u8*,u32,int,int);
extern void sdlStretch3x(u8*,u32,u8*,u8*,u32,int,int);
extern void sdlStretch4x(u8*,u32,u8*,u8*,u32,int,int);
extern void _2xSaI(u8*,u32,u8*,u8*,u32,int,int);
extern void _2xSaI32(u8*,u32,u8*,u8*,u32,int,int);
extern void Super2xSaI(u8*,u32,u8*,u8*,u32,int,int);
extern void Super2xSaI32(u8*,u32,u8*,u8*,u32,int,int);
extern void SuperEagle(u8*,u32,u8*,u8*,u32,int,int);
extern void SuperEagle32(u8*,u32,u8*,u8*,u32,int,int);
extern void Pixelate(u8*,u32,u8*,u8*,u32,int,int);
extern void Pixelate32(u8*,u32,u8*,u8*,u32,int,int);
extern void AdMame2x(u8*,u32,u8*,u8*,u32,int,int);
extern void AdMame2x32(u8*,u32,u8*,u8*,u32,int,int);
extern void Bilinear(u8*,u32,u8*,u8*,u32,int,int);
extern void Bilinear32(u8*,u32,u8*,u8*,u32,int,int);
extern void BilinearPlus(u8*,u32,u8*,u8*,u32,int,int);
extern void BilinearPlus32(u8*,u32,u8*,u8*,u32,int,int);
extern void Scanlines(u8*,u32,u8*,u8*,u32,int,int);
extern void Scanlines32(u8*,u32,u8*,u8*,u32,int,int);
extern void ScanlinesTV(u8*,u32,u8*,u8*,u32,int,int);
extern void ScanlinesTV32(u8*,u32,u8*,u8*,u32,int,int);
extern void hq2x(u8*,u32,u8*,u8*,u32,int,int);
extern void hq2x32(u8*,u32,u8*,u8*,u32,int,int);
extern void lq2x(u8*,u32,u8*,u8*,u32,int,int);
extern void lq2x32(u8*,u32,u8*,u8*,u32,int,int);
extern void hq3x16(u8*,u32,u8*,u8*,u32,int,int);
extern void hq4x16(u8*,u32,u8*,u8*,u32,int,int);
extern void hq3x32_32(u8*,u32,u8*,u8*,u32,int,int);
extern void hq4x32_32(u8*,u32,u8*,u8*,u32,int,int);

struct FilterDesc {
	char name[30];
	int enlargeFactor;
	FilterFunc func16;
	FilterFunc func24;
	FilterFunc func32;
};

const FilterDesc Filters[] = {
  { "Stretch 1x", 1, sdlStretch1x, sdlStretch1x, sdlStretch1x },
  { "Stretch 2x", 2, sdlStretch2x, sdlStretch2x, sdlStretch2x },
  { "2xSaI", 2, _2xSaI, 0, _2xSaI32 },
  { "Super 2xSaI", 2, Super2xSaI, 0, Super2xSaI32 },
  { "Super Eagle", 2, SuperEagle, 0, SuperEagle32 },
  { "Pixelate", 2, Pixelate, 0, Pixelate32 },
  { "AdvanceMAME Scale2x", 2, AdMame2x, 0, AdMame2x32 },
  { "Bilinear", 2, Bilinear, 0, Bilinear32 },
  { "Bilinear Plus", 2, BilinearPlus, 0, BilinearPlus32 },
  { "Scanlines", 2, Scanlines, 0, Scanlines32 },
  { "TV Mode", 2, ScanlinesTV, 0, ScanlinesTV32 },
  { "lq2x", 2, lq2x, 0, lq2x32 },
  { "hq2x", 2, hq2x, 0, hq2x32 },
  { "Stretch 3x", 3, sdlStretch3x, sdlStretch3x, sdlStretch3x },
  { "hq3x", 3, hq3x16, 0, hq3x32_32 },
  { "Stretch 4x", 4, sdlStretch4x, sdlStretch4x, sdlStretch4x },
  { "hq4x", 4, hq4x16, 0, hq4x32_32 }
};

int getFilterEnlargeFactor(const Filter f)
{
	return Filters[f].enlargeFactor;
}

char* getFilterName(const Filter f)
{
	return (char*)Filters[f].name;
}

FilterFunc initFilter(const Filter f, const int colorDepth, const int srcWidth)
{
  FilterFunc func;

  switch (colorDepth) {
    case 15:
    case 16:
      func = Filters[f].func16;
      break;
    case 24:
      func = Filters[f].func24;
      break;
    case 32:
      func = Filters[f].func32;
      break;
    default:
	  func = 0;
      break;
  }

  if (func)
    switch (f) {
      case kStretch1x:
        sdlStretchInit(colorDepth, 0, srcWidth);
        break;
      case kStretch2x:
        sdlStretchInit(colorDepth, 1, srcWidth);
        break;
      case kStretch3x:
        sdlStretchInit(colorDepth, 2, srcWidth);
        break;
      case kStretch4x:
        sdlStretchInit(colorDepth, 3, srcWidth);
        break;
      case k2xSaI:
      case kSuper2xSaI:
      case kSuperEagle:
        if (colorDepth == 15) Init_2xSaI(555);
        else if (colorDepth == 16) Init_2xSaI(565);
        else Init_2xSaI(colorDepth);
        break;
      case khq2x:
      case klq2x:
        hq2x_init(colorDepth);
        break;
      default:
        break;
    }

  return func;
}

      //
      // Interframe blending filters
      //

extern void SmartIB(u8*,u32,int,int);
extern void SmartIB32(u8*,u32,int,int);
extern void MotionBlurIB(u8*,u32,int,int);
extern void MotionBlurIB32(u8*,u32,int,int);

struct IFBFilterDesc {
	char name[30];
	IFBFilterFunc func16;
	IFBFilterFunc func32;
};

const IFBFilterDesc IFBFilters[] = {
  { "No interframe blending", 0, 0 },
  { "Interframe motion blur", MotionBlurIB, MotionBlurIB32 },
  { "Smart interframe blending", SmartIB, SmartIB32 }
};

IFBFilterFunc initIFBFilter(const IFBFilter f, const int colorDepth)
{
  IFBFilterFunc func;

  switch (colorDepth) {
    case 15:
    case 16:
      func = IFBFilters[f].func16;
      break;
    case 32:
      func = IFBFilters[f].func32;
      break;
    case 24:
    default:
	  func = 0;
      break;
  }

  return func;
}

char* getIFBFilterName(const IFBFilter f)
{
	return (char*)IFBFilters[f].name;
}

      //
      // Optimized stretchers implementation
      //

#ifndef C_CORE
u8 sdlStretcher[16384];

#ifdef _MSC_VER
#define SDL_CALL_STRETCHER \
  {\
    __asm mov eax, stretcher\
    __asm mov edi, destPtr\
    __asm mov esi, srcPtr\
    __asm call eax\
  }
#else
#define SDL_CALL_STRETCHER \
        asm volatile("call *%%eax"::"a" (stretcher),"S" (srcPtr),"D" (dstPtr))
#endif

#define SDL_LONG(val) \
  *((u32 *)&sdlStretcher[sdlStretcherPos]) = val;\
  sdlStretcherPos+=4;

#define SDL_AND_EAX(val) \
  sdlStretcher[sdlStretcherPos++] = 0x25;\
  SDL_LONG(val);

#define SDL_AND_EBX(val) \
  sdlStretcher[sdlStretcherPos++] = 0x81;\
  sdlStretcher[sdlStretcherPos++] = 0xe3;\
  SDL_LONG(val);

#define SDL_OR_EAX_EBX \
  sdlStretcher[sdlStretcherPos++] = 0x09;\
  sdlStretcher[sdlStretcherPos++] = 0xd8;

#define SDL_LOADL_EBX \
  sdlStretcher[sdlStretcherPos++] = 0x8b;\
  sdlStretcher[sdlStretcherPos++] = 0x1f;

#define SDL_LOADW \
  sdlStretcher[sdlStretcherPos++] = 0x66;\
  sdlStretcher[sdlStretcherPos++] = 0x8b;\
  sdlStretcher[sdlStretcherPos++] = 0x06;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc6;\
  sdlStretcher[sdlStretcherPos++] = 0x02;

#define SDL_LOADL \
  sdlStretcher[sdlStretcherPos++] = 0x8b;\
  sdlStretcher[sdlStretcherPos++] = 0x06;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc6;\
  sdlStretcher[sdlStretcherPos++] = 0x04;

#define SDL_LOADL2 \
  sdlStretcher[sdlStretcherPos++] = 0x8b;\
  sdlStretcher[sdlStretcherPos++] = 0x06;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc6;\
  sdlStretcher[sdlStretcherPos++] = 0x03;

#define SDL_STOREW \
  sdlStretcher[sdlStretcherPos++] = 0x66;\
  sdlStretcher[sdlStretcherPos++] = 0x89;\
  sdlStretcher[sdlStretcherPos++] = 0x07;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc7;\
  sdlStretcher[sdlStretcherPos++] = 0x02;

#define SDL_STOREL \
  sdlStretcher[sdlStretcherPos++] = 0x89;\
  sdlStretcher[sdlStretcherPos++] = 0x07;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc7;\
  sdlStretcher[sdlStretcherPos++] = 0x04;

#define SDL_STOREL2 \
  sdlStretcher[sdlStretcherPos++] = 0x89;\
  sdlStretcher[sdlStretcherPos++] = 0x07;\
  sdlStretcher[sdlStretcherPos++] = 0x83;\
  sdlStretcher[sdlStretcherPos++] = 0xc7;\
  sdlStretcher[sdlStretcherPos++] = 0x03;

#define SDL_RET \
  sdlStretcher[sdlStretcherPos++] = 0xc3;

#define SDL_PUSH_EAX \
  sdlStretcher[sdlStretcherPos++] = 0x50;

#define SDL_PUSH_ECX \
  sdlStretcher[sdlStretcherPos++] = 0x51;

#define SDL_PUSH_EBX \
  sdlStretcher[sdlStretcherPos++] = 0x53;

#define SDL_PUSH_ESI \
  sdlStretcher[sdlStretcherPos++] = 0x56;

#define SDL_PUSH_EDI \
  sdlStretcher[sdlStretcherPos++] = 0x57;

#define SDL_POP_EAX \
  sdlStretcher[sdlStretcherPos++] = 0x58;

#define SDL_POP_ECX \
  sdlStretcher[sdlStretcherPos++] = 0x59;

#define SDL_POP_EBX \
  sdlStretcher[sdlStretcherPos++] = 0x5b;

#define SDL_POP_ESI \
  sdlStretcher[sdlStretcherPos++] = 0x5e;

#define SDL_POP_EDI \
  sdlStretcher[sdlStretcherPos++] = 0x5f;

#define SDL_MOV_ECX(val) \
  sdlStretcher[sdlStretcherPos++] = 0xb9;\
  SDL_LONG(val);

#define SDL_REP_MOVSB \
  sdlStretcher[sdlStretcherPos++] = 0xf3;\
  sdlStretcher[sdlStretcherPos++] = 0xa4;

#define SDL_REP_MOVSW \
  sdlStretcher[sdlStretcherPos++] = 0xf3;\
  sdlStretcher[sdlStretcherPos++] = 0x66;\
  sdlStretcher[sdlStretcherPos++] = 0xa5;

#define SDL_REP_MOVSL \
  sdlStretcher[sdlStretcherPos++] = 0xf3;\
  sdlStretcher[sdlStretcherPos++] = 0xa5;

void sdlMakeStretcher(int width, int sizeOption)
{
  int sdlStretcherPos;
  sdlStretcherPos = 0;
  switch(systemColorDepth) {
  case 16:
    if(sizeOption) {
      SDL_PUSH_EAX;
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      for(int i = 0; i < width; i++) {
        SDL_LOADW;
        SDL_STOREW;
        SDL_STOREW;
        if(sizeOption > 1) {
          SDL_STOREW;
        }
        if(sizeOption > 2) {
          SDL_STOREW;
        }
      }
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_POP_EAX;
      SDL_RET;
    } else {
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      SDL_PUSH_ECX;
      SDL_MOV_ECX(width);
      SDL_REP_MOVSW;
      SDL_POP_ECX;
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_RET;
    }
    break;
  case 24:
    if(sizeOption) {
      SDL_PUSH_EAX;
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      int w = width - 1;
      for(int i = 0; i < w; i++) {
        SDL_LOADL2;
        SDL_STOREL2;
        SDL_STOREL2;
        if(sizeOption > 1) {
          SDL_STOREL2;
        }
        if(sizeOption > 2) {
          SDL_STOREL2;
        }
      }
      // need to write the last one
      SDL_LOADL2;
      SDL_STOREL2;
      if(sizeOption > 1) {
        SDL_STOREL2;
      }
      if(sizeOption > 2) {
        SDL_STOREL2;
      }
      SDL_AND_EAX(0x00ffffff);
      SDL_PUSH_EBX;
      SDL_LOADL_EBX;
      SDL_AND_EBX(0xff000000);
      SDL_OR_EAX_EBX;
      SDL_POP_EBX;
      SDL_STOREL2;
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_POP_EAX;
      SDL_RET;
    } else {
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      SDL_PUSH_ECX;
      SDL_MOV_ECX(3*width);
      SDL_REP_MOVSB;
      SDL_POP_ECX;
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_RET;
    }
    break;
  case 32:
    if(sizeOption) {
      SDL_PUSH_EAX;
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      for(int i = 0; i < width; i++) {
        SDL_LOADL;
        SDL_STOREL;
        SDL_STOREL;
        if(sizeOption > 1) {
          SDL_STOREL;
        }
        if(sizeOption > 2) {
          SDL_STOREL;
        }
      }
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_POP_EAX;
      SDL_RET;
    } else {
      SDL_PUSH_ESI;
      SDL_PUSH_EDI;
      SDL_PUSH_ECX;
      SDL_MOV_ECX(width);
      SDL_REP_MOVSL;
      SDL_POP_ECX;
      SDL_POP_EDI;
      SDL_POP_ESI;
      SDL_RET;
    }
    break;
  }
}

#else // C_CORE

void (*sdlStretcher)(u8 *, u8*, int) = 0;

#define SDL_CALL_STRETCHER \
       sdlStretcher(srcPtr, dstPtr, width)

template<typename T>
void sdlStretchx1(u8 *src, u8 *dest, int width)
{
  T *s = (T *)src;
  T *d = (T *)dest;
  for(int i = 0; i < width; i++)
    *d++ = *s++;
}

template<typename T>
void sdlStretchx2(u8 *src, u8 *dest, int width)
{
  T *s = (T *)src;
  T *d = (T *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *s++;
  }
}

template<typename T>
void sdlStretchx3(u8 *src, u8 *dest, int width)
{
  T *s = (T *)src;
  T *d = (T *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *s;
    *d++ = *s++;
  }
}

template<typename T>
void sdlStretchx4(u8 *src, u8 *dest, int width)
{
  T *s = (T *)src;
  T *d = (T *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *s;
    *d++ = *s;
    *d++ = *s++;
  }
}

void (*sdlStretcher16[4])(u8 *, u8 *, int) = {
  sdlStretchx1<u16>,
  sdlStretchx2<u16>,
  sdlStretchx3<u16>,
  sdlStretchx4<u16>
};

void (*sdlStretcher32[4])(u8 *, u8 *, int) = {
  sdlStretchx1<u32>,
  sdlStretchx2<u32>,
  sdlStretchx3<u32>,
  sdlStretchx4<u32>
};

void sdlStretch24x1(u8 *src, u8 *dest, int width)
{
  u8 *s = src;
  u8 *d = dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s++;
    *d++ = *s++;
    *d++ = *s++;
  }
}

void sdlStretch24x2(u8 *src, u8 *dest, int width)
{
  u8 *s = (u8 *)src;
  u8 *d = (u8 *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
  }
}

void sdlStretch24x3(u8 *src, u8 *dest, int width)
{
  u8 *s = (u8 *)src;
  u8 *d = (u8 *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
  }
}

void sdlStretch24x4(u8 *src, u8 *dest, int width)
{
  u8 *s = (u8 *)src;
  u8 *d = (u8 *)dest;
  for(int i = 0; i < width; i++) {
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
    *d++ = *s;
    *d++ = *(s+1);
    *d++ = *(s+2);
    s += 3;
  }
}

void (*sdlStretcher24[4])(u8 *, u8 *, int) = {
  sdlStretch24x1,
  sdlStretch24x2,
  sdlStretch24x3,
  sdlStretch24x4
};

#endif // C_CORE

bool sdlStretchInit(int colorDepth, int sizeMultiplier, int srcWidth)
{
#ifndef C_CORE
  sdlMakeStretcher(srcWidth, sizeMultiplier);
#else
  switch(colorDepth) {
  case 16:
    sdlStretcher = sdlStretcher16[sizeMultiplier];
    break;
  case 24:
    sdlStretcher = sdlStretcher24[sizeMultiplier];
    break;
  case 32:
    sdlStretcher = sdlStretcher32[sizeMultiplier];
    break;
  default:
    return false;
  }
#endif
  return true;
}

void sdlStretch1x(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */, u8 *dstPtr, u32 dstPitch, int width, int height) {
    int i;
#ifndef C_CORE
    u32 *stretcher = (u32 *)sdlStretcher;
#endif
      for(i = 0; i < height; i++) {
        SDL_CALL_STRETCHER;
        srcPtr += srcPitch;
        dstPtr += dstPitch;
	  }
}
void sdlStretch2x(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */, u8 *dstPtr, u32 dstPitch, int width, int height) {
    int i;
#ifndef C_CORE
    u32 *stretcher = (u32 *)sdlStretcher;
#endif
      for(i = 0; i < height; i++) {
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        srcPtr += srcPitch;
        dstPtr += dstPitch;
      }
}
void sdlStretch3x(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */, u8 *dstPtr, u32 dstPitch, int width, int height) {
    int i;
#ifndef C_CORE
    u32 *stretcher = (u32 *)sdlStretcher;
#endif
      for(i = 0; i < height; i++) {
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        srcPtr += srcPitch;
        dstPtr += dstPitch;
      }
}
void sdlStretch4x(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */, u8 *dstPtr, u32 dstPitch, int width, int height) {
    int i;
#ifndef C_CORE
    u32 *stretcher = (u32 *)sdlStretcher;
#endif
      for(i = 0; i < height; i++) {
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        dstPtr += dstPitch;
        SDL_CALL_STRETCHER;
        srcPtr += srcPitch;
        dstPtr += dstPitch;
      }
}


