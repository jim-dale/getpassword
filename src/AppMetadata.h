#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define VERSION(x) "2.0.0-" STRINGIFY(x)

static const PTCHAR ProgramName = _T("getpassword");
static const PTCHAR ProgramVersion = _T(VERSION(BUILDNUM));

#if defined(_M_IX86)
static const PTCHAR ProgramConfig = _T("X86");
#elif defined(_M_X64)
static const PTCHAR ProgramConfig = _T("X64");
#else
static const PTCHAR ProgramConfig = _T("Undefined");
#endif
