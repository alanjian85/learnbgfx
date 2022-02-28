function(add_bgfx_shader NAME)
	list(SUBLIST ARGV 1 -1 FILES)
	set(OUTPUT_LIST "")
    
	foreach(FILE ${FILES})
		set(FILE "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")

        get_filename_component(FILENAME "${FILE}" NAME_WE)

		string(SUBSTRING "${FILENAME}" 0 2 TYPE)
        if("${TYPE}" STREQUAL "fs")
            set(TYPE "FRAGMENT")
        elseif("${TYPE}" STREQUAL "vs")
            set(TYPE "VERTEX")
        elseif("${TYPE}" STREQUAL "cs")
            set(TYPE "COMPUTE")
        else()
            message(FATAL_ERROR "Shaders filename must have fs, vs or cs prefix. Got ${FILENAME}")
        endif()

		set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.bin")
		set(DEPENDS_PATH "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.bin.d")

        add_custom_command(
            OUTPUT "${OUTPUT_PATH}"
			BYPRODUCTS "${DEPENDS_PATH}"
		    COMMAND shaderc
            ARGS -f "${FILE}" -o "${OUTPUT_PATH}" --type "${TYPE}" --depends --platform linux
            COMMENT "Building shader ${FILE}"
            DEPENDS "${FILE}"
            VERBATIM
        )
        list(APPEND OUTPUT_LIST ${OUTPUT_PATH})
    endforeach()

    add_custom_target(bgfx_shader_${NAME} ALL DEPENDS ${OUTPUT_LIST})
    add_dependencies(${NAME} bgfx_shader_${NAME})
endfunction()

function(add_bgfx_texture NAME)
    list(SUBLIST ARGV 1 -1 FILES)
    set(OUTPUT_LIST "")

    foreach(FILE ${FILES})
        set(FILE "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")

        get_filename_component(FILENAME "${FILE}" NAME_WE)

        set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.dds")

        add_custom_command(
            OUTPUT "${OUTPUT_PATH}"
            COMMAND texturec
            ARGS -f "${FILE}" -o "${OUTPUT_PATH}"
            COMMENT "Converting texture ${FILE}"
            DEPENDS "${FILE}"
            VERBATIM
        )
        list(APPEND OUTPUT_LIST ${OUTPUT_PATH})
    endforeach()

    add_custom_target(bgfx_texture_${NAME} ALL DEPENDS ${OUTPUT_LIST})
    add_dependencies(${NAME} bgfx_texture_${NAME})
endfunction()