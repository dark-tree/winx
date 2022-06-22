
#define WXGL_IGNORE_GUARDS
#define WXGL_ONLY_SELECTED

#define WXGL_EXPOSE_GLAD
#include "wxgl.h"
#undef WXGL_EXPOSE_GLAD

//REPLACE_GLAD_SOURCE//

#define WXGL_EXPOSE_WINX
#include "wxgl.h"
#undef WXGL_EXPOSE_WINX

#define WINX_CREATION_HOOK gladLoadGL()

//REPLACE_WINX_SOURCE//

