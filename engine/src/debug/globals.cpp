#include "globals.h"

#include "platforms/win64/win64-dev.h"

namespace err
{
   void CreateHostMessageBox(const char* title, const char* message)
   {
#ifdef WINDOWS
      MessageBoxA(NULL, message, title, MB_OK);
#endif
   }
}