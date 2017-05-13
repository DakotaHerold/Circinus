#pragma once

//#define RUNTIME_BUILD

#if !defined(RUNTIME_BUILD) && !defined(BENCHMARK_BUILD)
#define EDITOR_BUILD
#endif

#ifdef BENCHMARK_BUILD
#define HAS_GUI
#endif

#ifdef EDITOR_BUILD
#define HAS_GUI
#define HAS_EDITOR
#endif
