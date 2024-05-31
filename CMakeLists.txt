cmake_minimum_required(VERSION 3.5)

project(GSPCore)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)


message("\nBuilding GSPCore")

add_compile_definitions(DEBUG)

if(EMSCRIPTEN)
    message("Building GSPCore for WASM")

    set(GSPCORE_SRC
        src/platform/wasm/GSPCore.c
        src/platform/wasm/GWindow.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/UI/GView.c
        src/rendering/webgl/GWebGLRenderer.c
        #src/graphics/unix/GFrame_gl.c
        #src/graphics/unix/GVertexBuffer_gl.c
        #src/graphics/unix/GShader_gl.c
        #src/graphics/unix/GTexture_gl.c
        #src/graphics/unix/unix_GTextProvider.c

        #src/GRenderManager.c
        #src/graphics/vulkan/GHiperRenderManager.c
    )

    add_library(GSPCore STATIC ${GSPCORE_SRC})
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_FREETYPE=1 --preload-file ../resources/wasm/assets")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set_target_properties(GSPCore PROPERTIES LINK_FLAGS "-s USE_WEBGL2=1 -s USE_SDL=0 -s USE_SDL_TTF=2 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS=['_malloc']")

    target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    # Add executable target
    add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
    target_link_libraries(Test PRIVATE GSPCore)
    target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)
    
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(Test PROPERTIES OUTPUT_NAME "index")
    #set_target_properties(Test PROPERTIES LINK_FLAGS "-s USE_WEBGL2=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS=['_main', '_malloc']")
    set_target_properties(Test PROPERTIES LINK_FLAGS "-s USE_WEBGL2=1  -s USE_SDL=2 -s USE_SDL_TTF=2  -s EXPORTED_FUNCTIONS=\"['_main', '_malloc']\" -s EXPORTED_RUNTIME_METHODS=\"['ccall','cwrap']\"")


    # Copy custom HTML file to the output directory
    add_custom_command(
        TARGET Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/wasm/gspcore.html ${CMAKE_BINARY_DIR}/index.html
    )

elseif($ENV{GSP_IOS})
    message("Building GSPCore for iOS")

    set(CMAKE_SYSTEM_NAME iOS)
    set(CMAKE_OSX_SYSROOT iphoneos)

    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_IOS_INSTALL_COMBINED YES)

    add_executable(GSP MACOSX_BUNDLE testing/helloworld.c)
    set_target_properties(GSP PROPERTIES XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.mycompany.myexampleapp3" XCODE_ATTRIBUTE_EXECUTABLE_FILE "GSP" MACOSX_BUNDLE YES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/build/minimal.plist)

elseif(APPLE)

    message("Building GSPCore for MACOS")

    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

    add_executable(GSP MACOSX_BUNDLE testing/helloworld.c)
    set_target_properties(GSP PROPERTIES XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.mycompany.myexampleapp3" XCODE_ATTRIBUTE_EXECUTABLE_FILE "GSP" MACOSX_BUNDLE YES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/build/minimal.plist)

elseif (UNIX)

    message("Building GSPCore for UNIX/Linux")

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

    # Find X11 library
    find_package(X11 REQUIRED)
    find_package(Cairo REQUIRED)
        find_package(Vulkan REQUIRED)

    set(GSPCORE_SRC
        src/platform/x11/GSPCore.c
        src/platform/x11/GWindow.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/graphics/unix/GFrame_gl.c
        src/graphics/unix/GVertexBuffer_gl.c
        src/graphics/unix/GShader_gl.c
        src/graphics/unix/GTexture_gl.c
        src/graphics/unix/unix_GTextProvider.c

        src/GRenderManager.c
        src/graphics/vulkan/GHiperRenderManager.c
    )

    # Set the C compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

    add_library(GSPCore SHARED ${GSPCORE_SRC})

    target_include_directories(GSPCore PUBLIC ${CAIRO_INCLUDE_DIRS}  ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    # Link X11 library
    target_link_libraries(GSPCore ${X11_LIBRARIES} GL GLEW m ${CAIRO_LIBRARIES} ${Vulkan_LIBRARIES})

    # Add executable target
    add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
    target_link_libraries(Test PRIVATE GSPCore)

    target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)

elseif(WIN32)

    message("Building GSPCore for Microsoft Windows")

    find_package(Vulkan REQUIRED)
    set(CMAKE_GENERATOR_PLATFORM x64)

    set(GSPCORE_SRC
        src/platform/win32/GSPCore.c
        src/platform/win32/GWindow.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/GRenderManager.c
        src/graphics/vulkan/GHiperRenderManager.c
    )

    add_library(GSPCore STATIC ${GSPCORE_SRC})
    target_include_directories(GSPCore PUBLIC  $ENV{VK_SDK_PATH}/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(GSPCore PRIVATE gdi32 ${Vulkan_LIBRARIES})
    
    # Add executable target
    add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
    target_link_libraries(Test PRIVATE GSPCore)

    target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)

else()
    message("Unknown platform" ${CMAKE_PLATFORM})
endif()






