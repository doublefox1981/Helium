#pragma once

#ifdef PIPELINE_API
# ifdef PIPELINE_EXPORTS
#  define PIPELINE_API __declspec (dllexport)
# else
#  define PIPELINE_API __declspec (dllimport)
# endif
#else
# define PIPELINE_API
#endif
