
IF(WIN32)
	SET(SIGN_FILES FALSE CACHE BOOL "Sign files ofter build")
ENDIF()

FUNCTION(SIGN TARGET)
	IF (NOT WIN32)
		RETURN()
	ENDIF()
	IF (NOT SIGN_FILES)
		RETURN()
	ENDIF()
	GET_TARGET_PROPERTY(file ${TARGET} LOCATION)
	GET_FILENAME_COMPONENT(filename ${file} NAME)
	ADD_CUSTOM_COMMAND( TARGET ${TARGET} POST_BUILD
		COMMAND signtool.exe sign /a /t http://timestamp.verisign.com/scripts/timstamp.dll ${file}
		COMMENT "Signing file ${filename}"
	)
ENDFUNCTION()
