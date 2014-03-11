# MSVC PrecompiledHeader 
MACRO( MSVC_SET_PCH Target PrecompiledHeader PrecompiledSource)
  IF( MSVC )
    GET_FILENAME_COMPONENT(PrecompiledBasename
       ${PrecompiledHeader} NAME)

    SET_PROPERTY(SOURCE ${PrecompiledSource}
      APPEND
      PROPERTY
        COMPILE_FLAGS
          "/Yc\"${PrecompiledBasename}\"")

	SET_PROPERTY(TARGET ${Target}
      APPEND
      PROPERTY
        COMPILE_FLAGS
          "/Yu\"${PrecompiledBasename}\"")
  ENDIF( MSVC )
ENDMACRO( MSVC_SET_PCH )