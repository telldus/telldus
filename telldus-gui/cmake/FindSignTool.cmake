
IF(WIN32 OR APPLE)
	SET(SIGN_FILES FALSE CACHE BOOL "Sign files ofter build")
ENDIF()

FUNCTION(SIGN TARGET)
	IF (NOT WIN32 AND NOT APPLE)
		RETURN()
	ENDIF()
	IF (NOT SIGN_FILES)
		RETURN()
	ENDIF()
	GET_TARGET_PROPERTY(file ${TARGET} LOCATION)
	GET_FILENAME_COMPONENT(filename ${file} NAME)
	IF (WIN32)
		ADD_CUSTOM_COMMAND( TARGET ${TARGET} POST_BUILD
			COMMAND signtool.exe sign /a /t http://timestamp.verisign.com/scripts/timstamp.dll ${file}
			COMMENT "Signing file ${filename}"
		)
	ELSEIF(APPLE)
		ADD_CUSTOM_COMMAND( TARGET ${TARGET} POST_BUILD
			COMMAND codesign -v --force --sign "Developer ID Application: Telldus Technologies AB" ${file}
			COMMENT "Signing file ${filename}"
		)
	ENDIF()
ENDFUNCTION()
