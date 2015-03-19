#ifdef __cplusplus
#define ASMATH_EXT_LANG extern "C"
#else
#define ASMATH_EXT_LANG
#endif

#if   defined(__i386__)
	#define ASMATH_ARCH x86
#elif defined(__x86_64__)
	#define ASMATH_ARCH x86_64
#elif defined(__arm__)
	#define ASMATH_ARCH arm
#elif defined(__aarch64__)
	#define ASMATH_ARCH arm64
#else
	#error Unsupported platform.
#endif
