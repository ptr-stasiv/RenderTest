#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

   void InitializeGUI(const char* url, const int width, const int height);

   void GetWebWindowInfo(int* width, int* height, void** winPixels);

#ifdef __cplusplus
}
#endif