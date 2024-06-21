cmake_minimum_required(VERSION 3.5)

project(GSPCore)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)

#[[
    DEPENDENCIES:

        LINUX:
            gcc
            g++
            cmake

            X11 development package: (fedora libX11-devel)
            Cairo development package: (fedora cairo-devel)
            Vulkan SDK (follow LunarG instructions), (also fedora vulkan-validation-layers)

]]


message("\nBuilding GSPCore")

add_compile_definitions(DEBUG)

if(EMSCRIPTEN)
    message("Building GSPCore for WASM")

    set(GSPCORE_SRC
        src/Application/GApplication.c
        src/Debug/GLog.c

        src/Windowing/GWindowManager.c
        src/Windowing/Platforms/Web/GWebWindowManager.c

        src/Graphics/GRenderManager.c
        src/Graphics/Backends/WebGL/GWebGlRenderManager.c

        src/Resource/GResourceManager.c
    )

    add_library(GSPCore STATIC ${GSPCORE_SRC})
    target_compile_definitions(GSPCore PUBLIC -DGSPCORE_BUILD -DGSPCORE_BUILD_WEB)


    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_FREETYPE=1 --preload-file ../resources/wasm/assets --preload-file ../resources/wasm/shaders")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set_target_properties(GSPCore PROPERTIES LINK_FLAGS "-s USE_WEBGL2=1 -s USE_SDL=0 -s USE_SDL_TTF=2 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS=['_malloc']")

    target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    # Add executable target
    add_executable(Test testing/main.c)
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
    # export GSP_IOS=true     

    message("Building GSPCore for iOS")

    set(CMAKE_SYSTEM_NAME iOS)
    set(CMAKE_OSX_SYSROOT iphoneos)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0")

    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_IOS_INSTALL_COMBINED YES)

    set(CMAKE_C_FLAGS "-x objective-c")


    #find_library(UIKIT_LIBRARY UIKit)
    #find_library(COREVIDEO_FRAMEWORK CoreVideo)
    find_library(METAL_FRAMEWORK Metal)
    find_library(METALKIT_FRAMEWORK MetalKit)

    set(GSPCORE_SRC 
        src/Basic/GApplication/ios/GApplication.m
        src/Controllers/GApplicationController/ios/GApplicationController.m
    )

    add_library(GSPCore ${GSPCORE_SRC})
    target_compile_definitions(GSPCore PUBLIC -DGSPCORE_BUILD_IOS)
    target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(GSPCore)

    set(GSP_TEST_SRC
        testing/helloGspInterface2.c
    )

    add_executable(GSP MACOSX_BUNDLE ${GSP_TEST_SRC})

    set_target_properties(GSP PROPERTIES XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.mycompany.myexampleapp3" XCODE_ATTRIBUTE_EXECUTABLE_FILE "GSP" MACOSX_BUNDLE YES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/build/minimal.plist)
    
    target_include_directories(GSP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(GSP PRIVATE GSPCore)

elseif(APPLE)

    message("Building GSPCore for MACOS")
    #add_compile_definitions(GSPCORE_BUILD_MACOS)

    # temporarily setting the language to objc for all c files.
    # this is because GSPCORE_BUILD_MACOS is defined for C files as well as Objc.
    # ideally what should happen is the variants that have objc code are compiled with GSPCORE_BUILD_MACOS
    # and then this definition is removed to compile business logic C.
    set(CMAKE_C_FLAGS "-x objective-c")

    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_FRAMEWORK Metal)
    find_library(METALKIT_FRAMEWORK MetalKit)

    set(GSPCORE_SRC 
        src/GSPCore.c
        src/Debug/GLog.c

        src/Application/GApplicationManager.c

        src/Window/GWindowManager.c
        src/Window/Platforms/Cocoa/GCocoaWindowManager.m

        src/Render/GRenderManager.c
        src/Render/Backends/Metal/GMetalRenderManager.m

        src/Resource/GResourceManager.c
        src/Resource/Providers/Cocoa/GCocoaResourceManager.m
    )

    add_library(GSPCore ${GSPCORE_SRC})
    target_compile_definitions(GSPCore PUBLIC -DGSPCORE_BUILD_MACOS)

    target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(GSPCore ${COCOA_LIBRARY} ${METAL_FRAMEWORK} ${METALKIT_FRAMEWORK})



    set(GSP_TEST_SRC
        testing/main.c
    )

    add_executable(GSP MACOSX_BUNDLE ${GSP_TEST_SRC})

    set_target_properties(GSP PROPERTIES XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.mycompany.myexampleapp3" XCODE_ATTRIBUTE_EXECUTABLE_FILE "GSP" MACOSX_BUNDLE YES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/build/minimal.plist)

    target_include_directories(GSP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(GSP PRIVATE GSPCore)
elseif (UNIX)

    message("Building GSPCore for UNIX/Linux")

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

    # Find X11 library
    find_package(X11 REQUIRED)
    find_package(Cairo REQUIRED)
    find_package(Vulkan REQUIRED)

    set(GSPCORE_SRC
        src/GSPCore.c
        src/Debug/GLog.c

        src/Application/GApplicationManager.c
        
        src/Window/GWindowManager.c
        src/Window/Platforms/X11/GX11WindowManager.c
        src/Window/Platforms/Wayland/GWaylandWindowManager.c
        src/Window/Platforms/Wayland/xdg-shell-protocol.c
        src/Window/Platforms/Wayland/kde-server-decoration.c

        src/Render/GRenderManager.c
        src/Render/Backends/Vulkan/GVulkanRenderManager.c

        src/View/GViewManager.c
        
        src/Server/GSPServer.c
    )

    # Set the C compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

    add_library(GSPCore SHARED ${GSPCORE_SRC})
    target_compile_definitions(GSPCore PUBLIC -DGSPCORE_BUILD -DGSPCORE_BUILD_UNIX)


    target_include_directories(GSPCore PUBLIC ${CAIRO_INCLUDE_DIRS}  ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    # Link X11 library
    target_link_libraries(GSPCore ${X11_LIBRARIES} m wayland-client X11 ${CAIRO_LIBRARIES} ${Vulkan_LIBRARIES})

    # Add executable target
    add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
    target_link_libraries(Test PRIVATE GSPCore)

    target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)

elseif(WIN32)

    message("Building GSPCore for Microsoft Windows")

    find_package(Vulkan REQUIRED)
    set(CMAKE_GENERATOR_PLATFORM x64)

    set(GSPCORE_SRC
        src/Application/GApplication.c
        src/Debug/GLog.c

        src/Windowing/GWindowManager.c
        src/Windowing/Platforms/Win32/GWin32WindowManager.c

        src/Graphics/GRenderManager.c
        src/Graphics/Backends/Vulkan/GVulkanRenderManager.c
    )

    add_library(GSPCore STATIC ${GSPCORE_SRC})
    target_compile_definitions(GSPCore PUBLIC -DGSPCORE_BUILD -DGSPCORE_BUILD_WIN32)

    target_include_directories(GSPCore PUBLIC  $ENV{VK_SDK_PATH}/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(GSPCore PRIVATE gdi32 ${Vulkan_LIBRARIES})
    
    # Add executable target
    add_executable(Test testing/main.c)
    target_link_libraries(Test PRIVATE GSPCore)

    target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)

else()
    message("Unknown platform" ${CMAKE_PLATFORM})
endif()






