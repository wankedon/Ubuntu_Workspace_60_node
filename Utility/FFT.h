#pragma once

#ifdef __cplusplus
extern "C" {
#endif
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FASTFFT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FASTFFT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef FASTFFT_EXPORTS
#define FASTFFT_API __declspec(dllexport)
#else
#define FASTFFT_API __declspec(dllimport)
#endif

typedef enum WINDOW_SHAPE
{
	WS_RECT,
	WS_HANNING,
	WS_HAMMING,
	WS_BLACKMAN,
	WS_KAISER,
	WS_GUASS,
	WS_SIN,
}WINDOW_SHAPE;

typedef enum _FFTDataType
{
	FDT_TIME,
	FDT_FREQ,
}FFTDataType;

FASTFFT_API uint32_t createFFTer(int fftPoints);
FASTFFT_API void deleteFFTer(uint32_t handle);
FASTFFT_API void addWindow(uint32_t handle, WINDOW_SHAPE win);
FASTFFT_API void FFTInput(uint32_t handle, float* src, FFTDataType dataType);
FASTFFT_API void FFTTransTo(uint32_t handle, FFTDataType dataType);
FASTFFT_API void FFTHighResTrans(uint32_t handle, double sampleRate, double freqOffset, double bw);	//time to freq convertion in high resolution
FASTFFT_API void FFTOutput(uint32_t handle, float* dst, FFTDataType dataType);

#ifdef __cplusplus
}
#endif