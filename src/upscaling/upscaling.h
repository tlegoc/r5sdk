//
// Created by theo on 29/12/2023.
//

#ifndef R5SDK_UPSCALING_H
#define R5SDK_UPSCALING_H

#include "windows/id3dx.h"

extern BOOL g_bIsUpscalingInitialized;
extern ID3D11Texture2D* p_upscalingInput;
extern ID3D11ShaderResourceView* p_upscalingInput_SRV;

void Upscaling_Init();
void Upscaling_Shutdown();

void Upscaling_Run();
void Upscaling_Resize(UINT newWidth, UINT newHeight);

void Upscaling_DebugUI();

#endif //R5SDK_UPSCALING_H
