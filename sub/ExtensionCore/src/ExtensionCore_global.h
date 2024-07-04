#pragma once

#ifdef ExtensionCore_PROJECT

#define EXTENSIONCORE_API __declspec(dllexport)
#else
#define EXTENSIONCORE_API __declspec(dllimport)

#endif // ExtensionCore

