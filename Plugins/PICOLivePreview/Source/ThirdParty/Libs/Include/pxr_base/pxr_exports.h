#pragma once

#if defined(PXR_WIN) || defined(_WINDOWS) || defined(_WINDLL)

#ifdef PXR_EXPORTS
#define PXR_API __declspec(dllexport)
#else
#define PXR_API __declspec(dllimport)
#endif// PXR_EXPORTS

#else
#define PXR_API
#endif// PXR_WIN