#define CONFIG_CCDIR "/usr"
#define CONFIG_CC_CRT_PREFIX "/usr/lib"
#define GCC_MAJOR 2
#define HOST_I386 1
#define CC_VERSION "0.1"
#define CC_TARGET_PE
#undef _WIN32

#if defined(__linux)
#define CC_PLATFORM "Linux"
#elif defined(_MSC_VER)
#define CC_PLATFORM "Windows"
#else
#define CC_PLATFORM "ckernel"
#endif
