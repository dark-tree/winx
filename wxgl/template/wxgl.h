
#if defined(WXGL_IGNORE_GUARDS) || !defined(WXGL_H)

#ifndef WXGL_H
#	define WXGL_H
#endif

#if !defined(WXGL_ONLY_SELECTED)
#	define WXGL_EXPOSE_GLAD
#	define WXGL_EXPOSE_WINX
#endif

#ifdef WXGL_EXPOSE_GLAD
//REPLACE_GLAD_HEADER//
#endif

#ifdef WXGL_EXPOSE_WINX
//REPLACE_WINX_HEADER//
#endif

#endif // WXGL_H

