#ifndef JSON_VISUAL_C_HEADER
#define JSON_VISUAL_C_HEADER

#ifdef _MSC_VER

    #if !defined (json_fallthrough) && defined (__cpp_attributes) && defined (__has_cpp_attribute)
	    #if __has_cpp_attribute(fallthrough)
	        #define json_fallthrough [[fallthrough]]
		#endif
	#endif

    #if !defined (json_fallthrough)
        #define json_fallthrough /* FALLTHRU */
    #endif

    #if !defined (json_deprecated)
        #if defined (__cpp_attributes) && defined (__has_cpp_attribute)
            #if __has_cpp_attribute(deprecated)
                #define json_deprecated(method, warning) method [[deprecated(warning)]]
            #endif
        #endif
        #if !defined (json_deprecated)
            #define json_deprecated(method, warning) __declspec(deprecated(warning)) method
        #endif
    #else
        #define json_deprecated(method, warning) method
    #endif


    #define json_nothrow
    #define json_throws(x)
    #define json_pure json_nothrow
    #define json_read_priority
    #define json_write_priority
    #define json_malloc_attr json_nothrow
    #define json_hot
    #define json_cold
    #define json_likely(x) x
    #define json_unlikely(x) x

    #ifdef JSON_LESS_MEMORY
	   #define PACKED(x) :x
	   #define BITS(x) :x
    #endif

#endif

#endif
