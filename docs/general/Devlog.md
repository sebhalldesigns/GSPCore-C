# GSP Devlog



## 09/03/2024

Have been away for a few weeks so getting back into GSP with some new ideas and plans. These are:

- Prioritize moving forward (rather than constantly rewriting stuff).

- Promote simpler designs and lower-level structures (as much C as possible).

- Functional interfaces (non-OOP) are the way to go.

  - These let you have freedom of language and allow for things like FFI and dynamic linking more easily (e.g make a DLL and load it in Python).

- Compatibility and simplicity good targets - having both forwards/backwards compatibility is ideal.

- Try not to hate on Rust - very mixed feelings but it is built on solid principles and companies are moving to it for a reason.

- CPU rendering isn't the worst thing ever.

- Potential confusion over GSP/UniversalUI naming and project scopes. Rough possible clarity is that UniversalUI is built on top of GSP and can be sort of seen like UIKit built on top of Quartz and Cocoa or WPF built on top of MIL and Win32. What this means is that apps can be built with GSP but without UniversalUI, for instance if a different rendering paradigm is desired or if the application has no GUI. Why use GSP if a GUI is not needed? That will come down to things like the build environment, libraries (like image processing and audio), performance metrics and compatibility.

  - UniversalUI ≈ UIKit/WPF
  - GSP ≈ Quartz + Cocoa/MIL + Win32 

- 'Modular' building of GSP where it doesn't necessarily need things like GPU rendering or windowing. Statically link the required modules into a single tailor-made DLL that saves on space.

  

#### Rough Rendering Architecture Plan

Somewhat inspired by CoreGraphics

- Define system-memory pixel buffers

- Render basic views to these buffers using CPU
  - Need a vector-geometry approach to building these
  - Preferably exportable to SVG or something similar.
  
- Under normal operation, upload these to GPU for compositing.
  - Legacy fallback, composite on CPU.
  
    

## 10/03/2024

Further thoughts on rendering architecture:

* Graphics drawn with GVG (GSP Vector Graphics) on CPU.

* Use batching and multithreading to improve efficiency
  * Batch commands so things like scan-fill passes and drawing can be done in parallel.
  
  * Process canvases in parallel.
  
  * Use sliced rendering to produce more efficient CPU compositing.
  
    

## 13/03/2024

Some ideas for GSP layout and scope:

- Move SVG into a 'tools' subdirectory
- Implement build system as core component
- GSP runtime?

Other ideas for defining the validity of an idea for refactoring development:

* Is this necessary?
* Does it provide a significant, meaningful increase to performance?
* Could it be done at a later date?
* How much time will this take to develop?
* Can it feasibly be done at a later stage if required?

GVG should simply be a (very efficient) vector graphics rasterizer. Separately have GCOMP as a simple (but very efficient) bitmap compositor (using tiling). GSP windowing then provides an interface for presenting a bitmap as the window contents, or for compositing on a GPU window surface. Keep it simple. 



## 16/03/2024

Hoping to make meaningful progress this weekend.

Targets are as follows:

- Embrace C as a lightweight, flexible, portable and powerful language. In the future, there may be sections rewritten in Rust. If linux can be written in C though, so can GSP.

- Use the newly proposed (flat, general-purpose) project structure.

- Implement basic working versions of GVg and GWin.

  - GVg should include text, and rectangles as a minimum. It should also include a tiling compositor.
  - GWin should support creating windows, presenting a GVg bitmap, handling mouse input and text input.

- This should be working on both Windows (Win32) and Linux (X11) as a minimum.

  

Some more detailed notes:

- Keep modules as simple as possible - Occam's razor, KISS etc.
  - They should be stateless (don't require initialization).
  - They should be completely independent of other GSP components and function perfectly as isolated libraries. E.g any application should be able to import GVg and use it as a vector graphics rasterizer.
  
- Modules should work as both static and dynamic libraries.

  

## 17/03/2024

Good progress with the project since the start of yesterday. All architectural decisions taken recently are proving to be beneficial.

### gWin

Similar in concept to previous versions in GSP and UniversalUI. Initially targeting Win32 and X11 seems easy and fairly simple. Currently ignoring mouse and keyboard events as they are fairly unrelated to graphics (and graphics appear to be both far more complex and far more important to the success of OpenGSP).

### gVG

Interestingly, despite going down the route of complete CPU rendering, the pipeline and overall architecture of gVG actually makes it somewhat easy to implement GPU rendering in the future. The pipeline is as follows:

- **Input/Encoding** - drawing commands are submitted to gVG and packed into consistently formatted 'Job' packets, which are set to different stages of the pipeline depending on their content.
- **1: Path Generation** - the first main pipeline stage. Shapes and vector geometry are converted into paths made up of straight lines.
- **2: Path Rendering** - the second pipeline stage. Processed paths are drawn onto canvas layers, including features such as filling and path tracing.
- **3: Composition** - the final pipeline stage. The bitmaps generated for vector elements are combined with pre-rendered bitmaps (such as text and images) to produce the final result.

Stages 2 and 3 can both be implemented efficiently on a GPU. This is likely something that will be implemented in the future, especially once 3D graphics become part of the project. They both have some basic versions implemented, while the input and path generation stages are both unimplemented.

A fair amount of today's work was spent starting the implementation of a thread and job pool. The system currently allocates one persistent thread for each logical processor in the host device. These are intended to be where the actual pipeline processing takes place, leaving the main thread free to 

### Next Steps

Further implementation of the job pool system and implementation of the first two stages (input and path generation). These are the ones that can't be implemented on GPU at all, so should be especially well implemented.

Additionally, it might be good to start working on a gUI module, as a reference implementation of views etc. This is the start of the new UniversalUI (v0.100), built on top of OpenGSP.

## 18/03/2024 

Was thinking earlier that it might be better to shelve the thread pool system for now, and focus on single-threaded rendering. This is more simple, will achieve results faster and encourages more efficient implementation of algorithms.

Work on GEOMOD should also probably continue in parallel so that it doesn't get ignored and pushed to the background for the next year. Lessons learned in UniversalUI are also valid.

## 25/03/2024

A couple thoughts:

- Likely a good idea to revisit studying existing cross-platform GUI frameworks such as Qt, GDK/GTK etc
- Possibly simplify the gVG pipeline into a line & polygon preprocessor, and then move the actual drawing into a software opengl/vulkan/wgpu implementation. Doing this simplifies the requirements for drawing (by restricting it to opengl style commands), and also allows for 3D things to potentially be rendered as well.

## 01/04/2024

Starting work on a basic iOS editor client because the current options are shite

Also planning on targeting macOS a bit for now becuase of Metal being great and support from GSP being behind on Apple platforms

## 06/04/2024

Attempted to get window working nicely on macOS and it was behaving strangely. Identified that this was because macOS was treating the built application as a 'command line tool' instead of an 'application', and that the solution is to use cmake Xcode generation (cmake -g xcode).

Then, found a problem that I had faced in the past which was to do with code signing and the error message

```
resource fork, Finder information, or similar detritus not allowed Command CodeSign failed with a nonzero exit code
```

This was to do with metadata/attributes on the .app bundle, which showed to be from finder and apple file system. I tried many different solutions online using the xattr tool, but it just wouldnt delete the attributes related to finder and the file system.

Then I found the solution which was painfully simple. I was working on the project in the Documents folder on my mac which I have synced to iCloud. iCloud sync was causing these attributes to be automatically added, and that was why I couldn't build! Moving the project out of the iCloud synced folder completely solves this issue.

Started work also on the web port and emscripten is shockingly easy. To deploy you can compile to a .js file and simply add that as a script to the html directly. In order to interact with the web environment from C, you can simply use EM_ASM() which lets you add JS code inline in C!! WebGL is ridiculously easy to set up.

## 15-16/04/2024

Was away for the weekend, and did some more reading on CAD kernels. This lead me to start a revisit to Rust and the GEOMOD project, this time ignoring 3D graphics and GUI, and aiming only for a CLI style system. Think it could be a good idea to summarise projects as follows: 

- **OpenGSP Core** - thin & lightweight system abstraction layer, distinct versions as follows:
  - Windows 
  - Linux
  - macOS
  - iOS
  - Web
  - Android
  - **Should be constructed as a library for either dynamic or static linking, can be implemented in various different languages, such as C, Objective-C, Rust or Kotlin**.
- **OpenGSP GPI** (Graphics Programming Interface) - thin & lightweight graphics API abstraction layer, distinct versions as follows:
  - Vulkan
  - Metal
  - WebGL
  - OpenGSP SGR (see below)
  - **Should be constructed as a library for either dynamic or static linking, can be implemented in various different languages, such as C, Objective-C, Rust or Kotlin**.
- **OpenGSP SGR** (Software Graphics Renderer) - lightweight software renderer to be used as either a lightweight or legacy fallback, or for embedded systems. **Could be implemented in Rust**.
- **OpenGSP VDK** (Vector Drawing Kit) - layer above GPI to draw vector graphics. **Could be implemented in Rust**.
- **OpenGSP FSK** (File Serialization Kit) - set of efficient file serialization and deserialization libraries.

Was also thinkin about using a potential client-server socket architecture which could be used to isolate the 'unsafe' C code from Rust code, but I think that this would incur a significant amount of overhead, especially for embedded and low-power systems. It could be implemented as an optional system, but keeping the 'unsafe' parts thin, well defined ad extensively tested is likely a better all round solution.

## 17/04/2024

Started working on Rust implementations of OpenGSP, and registered crates with crates.io

## 20/04/2024

Been working on the Rust implementation quite a lot since yesterday. Spent a few hours (approx. 6) messing around with Objective-C/Rust interop on macOS and finally got it working. Wanted to be able to work with raw objc interfaces so reduce the number of dependencies and can use any objc API (rather than requiring an additional dedicated crate).

Then switched to Windows and successfully got Win32 window creation and events working. Decided upon using a window state tied to the 'user data' pointer of the HWND window, because Rust hates global variables and the mutex I was trying to use in the WndProc was just crashing the app.

Been thinking a bit about API choices and next steps for graphics. For the latter, I'm considering using WGPU as an off-the-shelf API that does work seeing as we are on Rust. Not too fussed for now though.

As for API choices, the current implementation is very similar to what it was before Rust Rewrite II (tm) - that is, a set of very basic structs and functions. Things like:

```c
gsp_window_size_t gsp_set_window_will_resize_callback(gsp_window_handle_t window_handle, gsp_window_size_t proposed_size);
```

Which has the benefit of being very lightweight, simple and highly compatible, but the disadvantage of being a lot of words, and a very slow and painful interface to work with.

Some alternative ideas:

- Create a delegate-style API where instead of passing individual callbacks, users create and register a delegate, then assign that delegate to windows and views.
- Create a 'message-queue' style API like what Win32 and X11 use anyway, which has the advantage of being able to run in a server process and isolate business and ui code.

## 21/04/2024

Having thought about it a bit more, I don't think the low-level C-style API implementation is that flawed. It is very simple and easy to use (aside from the long and explicit naming schemes). A good addition would be a user data pointer.

## 23/04/2024

Used WGPU tutorial to render a triangle to the screen. Am not necessarily convinced WGPU is the way to go though. Binary size is increasing fast and although this isn't a good measure of performance, it isn't building confidence. 

Some thoughts about Rust vs C:

- example from a while ago that opened OpenGL 1.1 on windows 7 was a much smaller binary:

  - Rust Hello World: 160 KB
  - Rust basic Win32 window: 1MB
  - Rust basic Graphics drawing: 16MB
  - C basic graphics drawing: 29KB

- This obviously isn't a particularly fair comparison but for the exact purposes trying to be achieved here, Rust doesn't seem to be offering the best solution. Due to a few reasons.

  

  **Pros of Rust for the project:**

  1. Cargo distribution is great
  2. Zed is a nice IDE
  3. Automatic dependency management is nice
  4. Cargo build system is painless
  5. Syntax can be nice
  6. Should be more stable?
  7. Sounds impressive
  8. People like the idea of using Rust
  9. Has a community that is helpful

  

  **Cons of Rust for the project:**

  1. LOTS of unsafe code (using C interfaces & callbacks & objective C is making this happen)
  2. Big binary sizes?
  3. Slow compile times
  4. Many libraries very experimental
  5. Many libraries badly documented
  6. Objective C integration could not be more janky
  7. Lack of classes and OOP is annoying 
  8. Lack of global variables is very annoying
  9. Lots of things happen implicitly which I don't like 
  10. Syntax can be ugly - chaining of structs and generics needed to make things happen
      1. Why so many generics??
  11. Lots of the language is still quite underdeveloped
  12. Simple things take way too long

Proposed next steps:

Return to majority C/C++ codebase. Probably C.

Rust implementation taught a few lessons:

- Newer != better
- More safety nets != safer -> have to use unsafe code to interface with lots of things, using unsafe code in Rust makes it harder to diagnose issues when it could just be normal C code.
- Objective C is crappy
- Having a modern build system is great.
- I miss C. Simulink and Rust both abstract a lot of fundamentals away and I don't like it.
- Surely there can be ways to make C-style languages more robust without throwing everything in the bin and starting from scratch like Rust has done?
- A lot of progress can be made in a short time - it's only been 6 days (4 of them work days) and we already have applications building on 2 operating systems, interfacing with 3 different languages and making some GPU stuff happen.

Graphics APIs they will be as follows:

- Apple - Metal
- Windows - Vulkan, SGR fallback (with backwards compatibility in mind)
- Linux - Vulkan, SGR fallback
- Android - Vulkan
- Web - WebGL
- Embedded - SGR

Also note: buying a RP2040 display for embedded testing

## 5/05/2024

Spent the last couple of days on a big C refactor, using the codebase from before Rust as a basis. Haven't been good at keeping a devlog so will try and summarise progress.

- Decided to use pure C - as self-contained as possible. This just makes everything feel much faster and it's quite a nice way of designing imo.
- Worked out a pseudo-objective interface - keeping it C allows for easy and convenient language bindings.
- Focusing on X11 for now, as it's quite a good middle ground between Win32 and macOS (and those two are both much more documented).
- Focusing on OpenGL and SGI for now - leaving vulkan to the side for now as it is a) a lot of work b) actually slower to launch? Metal is easy in comparison.
- One of the big hurdles approaching soon is text rendering. Considering using stb_truetype as truetype is a far bigger library with fairly small improvement over stb_truetype. It's most noticeable when using small font sizes, but a workaround might be possible to overcome this. Need to think how to improve for lightweight embedded situations. Maybe stb_truetype is perfect. I'm not sure.

## 6/05/2024

Good progress so far this weekend. Ideally spend a few hours today planning next steps and getting the project in a good state to maybe not work on it properly for a few weeks. A few questions:

1. What and where is the usp/ip/value added by GSPCore? Setting up an OpenGL context is something that can be done by basically anyone without much difficulty. OpenGL itself is currently providing all the heavy lifting.

2. How does UniversalUI fit into the picture with GSP and what is it doing exactly?

**Update** - after some more research, a test program and some ChatGPTing, it makes a huge amount of sense to take more advantage of the existing platform resources. It also clarifies the dual-mode configuration:

- **Linux**
  - Standard Mode - OpenGL for rendering, Cairo for text.
  - Legacy Mode - Software rendering, built-in bitmap fonts.
- **Windows**
  - Standard Mode - DirectX for rendering,  DirectWrite for text.
  - Legacy Mode - Software rendering, built-in bitmap fonts.
- **macOS**
  - Standard Mode - Metal for rendering, CoreText for text.
  - Legacy Mode - Software rendering, built-in bitmap fonts.
- **iOS and Other Apple**
  - Metal for rendering, CoreText for text.
- **WASM**
  - WebGL for rendering, HTML5 c anvas for text.
- **Embedded**
  - Standard Mode - Software rendering, bitmap fonts.

Notes: 

- Legacy mode is intended for backwards compatibility on older systems, but as an optional feature that can be disabled.
- Using built-in libraries for text rendering such as Cairo, CoreText and DirectWrite has a few huge advantages, including:
  - Should be quick and easy to implement.
  - Likely very good runtime performance.
  - Text should look consistent with other text on the same platform. This is quite important as different rendering systems handle subpixel rendering differently, and it avoids them looking weird.
  - Reduces size of binaries and number of libraries that need to be downloaded to build and run GSPCore.
  - Don't have to worry about text shaping and layout as it's handled by the system. Harfbuzz not going to be used.
- Planning to make a tool that can take a ttf font and generate a bitmap font header file. Will then bundle 1 sans-serif and 1 monospace font with the library.

### Next Steps:

Define an external interface for text rendering and conversion into a texture etc. Hook up to cairo and test some OpenGL/Cairo integration. 

Possibly try and get some more Windows functionality working.

Start working on vector graphics rendering such as shape and path rendering.

Add 9-slice functionality as a core component (or some other responsive rendering implementation that doesn't require re-rendering things like a box being stretched.)

## 10/05/2024

Another weekend, another push for OpenGSP.

Pleased with progress last weekend, definitely helped make a clearer picture of the project from a technical standpoint. Going to start of this weekend with an early night and hopefully start work early tomorrow. Here are some aims/plans:

- Stick with Linux for now. Have a technical idea of how to do stuff on other platforms but spending another couple of days API-wrangling (with a lot of overlap with previous versions) isn't very productive. What is far more useful is to focus on the new OpenGSP IP, the stuff that will make it stand out and be (hopefully) successful - the rendering system, scene graph and application semantics.
- Start the new UniversalUI repo as a dependant project on GSPCore. Use this as a better way to drive project direction.
- Focus on rendering of text, images and very basic provision for polygons.
- Try and get a more app-like state by the end of the weekend with basic drawing and layout capabilities and pointer interaction.

Immediate first steps:

- Write a basic texture compositor. I think this is partially implemented already but needs improving.

  

## 11/05/2024

Started nice and early which is good.

Created new repo - GSPUI. This is the 2nd OpenGSP library to be created after GSPCore. Other ones in the future to potentially include audio tools, server tools, database and things like that. Ultimately want OpenGSP to be the go-to collection of software libraries for basically any purpose. GSPCore being the underlying library that is a complete platform abstraction. None of the higher libraries should interface with any native library. All other GSP libraries should depend only on GSPCore. 

Also possibly make GEOMOD an OpenGSP library - GSPMod (OpenGSP Modelling Toolkit) or something. Maybe GSPMK (OpenGSP Modelling Kernel).

Have a bit of a dilemma - what should UniversalUI be in this context? I have the domain so feel like it should become something. It is a long name though, and GSPUI fits in well with the rest of the OpenGSP project. 

****

Now time for some actual design.

UView to contain a very large propotion of UI logic. It is basically a conversion layer between abstract UI concepts and direct rendering and system commands. This includes everything from view heirarchies to layout constraints, transformations and graphics abstractions. 



Some notes on potential ideas for GSPCore additions:

- ARC - a smart pointer that uses reference counting and is passed a free function to run when the reference count reaches 0.
- Events - an events system that uses handles to functions with common signatures for subscribing and publishing to events.

Layout constraints to be based off GSPCore object - GSolver evaluates arbitrary constraints. The initial interface has been defined but needs implementation.

Note on object-oriented functionality in other languages:

- Use a single base class for all views - try and create a UView superclass

- Find a way to implement system controls in C and still work with the UView superclass - probably something along the lines of USystemView: UView which then pulls rendering and interaction data from C.

  On second thoughts the suggested implementation is quite bad.

Will make a set of base objects that split up the planned UView functionality so that they arent a single object, and just accept that they should be object-oriented in structure.

Another update:

I re-created the UniversalUI repository, as it makes sense to me for UniversalUI to be a set of language-specific application framework libraries that share a common API. This is distinct from the other OpenGSP libraries, as they aim to be single, language agnostic C interfaces that can be used by any application or language. Therefore it makes sense for UniversalUI to be structured and named differently.

Successfully got C# and C++ native object-oriented functionality working, although the C++ seems to require a pattern called CRTP which basically makes the superclass compile-time dependant on the subclass, and requires it to be a header-only library for now. Took a while to get the fundamentals working but now they do, and should be able to stick with those designs for now.

Next steps:

Back to raw C stuff, need to get some objects defined for view graph. This basically includes all view stuff except rendering - subviews, layouts and more.

Separately, get simple rendering working better. I.e image loading and compositing.

The view graph will be quite a big thing, but after that rendering will be the main focus.

Update an couple hours later:

Been working on getting some actual rendering and compositing happening. Simple test (but really useful as a benchmark and idea of rough performance) was to have a window handling it's draw and mouse position callbacks. It would then draw some pre-rendered frames onto the window frame at the mouse location. After some rearranging of the main loop, I got it to the point where 2.5k small frames could follow the mouse around the window at approximately 144Hz. This was very reassuring and very rewarding as it shows this approach can definitely work well, and also doesn't need to be crazy complicated!

The big issue that was originally causing poor performance was X11 sending mouse move events at a very high rate, far higher than the rate it could be rendered at. As a guess, maybe 10kHz. This meant that the mouse move events would stack up and the boxes on screen would be further behind the mouse the longer it was moved for. Some improvements to the GWindow loop improved this a lot, they were:

- Limiting the callback rate - although this might not be a good idea.
- Changing the redraw request to a flag (rather than a function call)
- Redrawing the window if there are no events to process and there is a redraw request flag, then resetting the flag.

This does rely on events being processed rapidly, otherwise a redraw would never happen.

Could possibly implement a minimum redraw rate.

Or a better system:

- Define a cache of event properties (window size, mouse location etc)
- Each property should have an update flag
- When properties arrive, they update the cache and set the flag.
- Events can then be called at a later date (or at a fixed rate) and ensure they have the latest information.

This prevents 

​	a) Events only being called at a fixed rate and clients missing information

​	b) High CPU load due to long processing of events in clients

​	c) Event backlog - very bad!

How about making a 'UI Pipeline'?

Events -> State -> State Updates -> Changes etc

Where things can happen at different rates.

## 13/5/2024 

Got Vulkan successfully running on windows with fairly minor changes (~30mins fixing). Launch times are reasonable which is great. 

Things needed to investigate in due course:

- MSVC won't compile the C++ function casts. This is ok as it was a bit of a hack anyway. Needs more complexity.

- Windows 7 running does work once it has some dll dependencies **including** the vulkan DLL. We do not want this. It should not require the vulkan dll. 

To add to the C++ function casts - apparently in C++ the this pointer in the function signatures isn't a normal pointer, instead it can have a different calling convention and can be larger than a normal pointer. It is fundamentally different apparently. MSVC is preferable on windows for sure.

## 15/5/2024

Thinking today about what the next steps are. The MSVC issue is probably a bit more significant than it seems because it forces/encourages a thicker translation layer between the framework and clients.

If GSPCore were only being designed for desktop use, a server-client architecture doesn't seem like a bad idea at all. Aside from the performance hit of string operations and communications, it achieves some good advantages:

- Completely language agnostic
- Separation of logic between possibly unsafe C APIs and possibly more bulletproof code like Rust
- Server code can be installed once and shared between clients, meaning they can be lighter.

These aren't applicable, however, to any embedded, mobile or web system as are also targeted by GSPCore. Because these systems are also being targeted, the GSPCore library must be statically linkable to an executable file. It must also be small and lightweight in order to fit onto embedded clients. 

I'm thinking it's possible to define an interface that would work both statically linked and as a server/client process, with a defined 'packet' size and operations etc.  The API would revolve around objects - creation, destruction, getting and setting properties. It might be possible to define it in a way where function calls aren't necessary, or at least something as simple as a messages arrived function call.

Need to be careful not to overcomplicate the framework, especially seeing as progress has been good recently. Want to be able to have a very fast, reliable and lightweight rendering system though, and this can benefit greatly from having a further separation from client code. UI stuff will then be able to run at the same speed, regardless of whether the application logic is in Python or C.

## 16/5/2024

To ChatGPT:

> I'm working on a cross-platform application framework that ultimately has two goals - 1 ) to create a platform on which I can build a variety of commercial and open-source applications, and 2) to create the go-to platform for others to build their software on. The scope of the project is to create a platform abstraction layer and graphics rendering system, allowing the same application code to run on anything from a desktop linux pc to an iphone, web browser or microcontroller. So far I have spent quite a lot of time figuring out technical details of the windowing and input side and have made various different working levels of compatibility on windows, linux and macos (with iOS). Most recently, I was focusing on getting Vulkan rendering working on windows and Linux which I did successfully. This was going great until I tried to recompile my code using MSVC for windows (I had been using GCC on linux) and a trick that I had been using to make my C library work with C++ member functions (by using reinterpret_cast and adding the this pointer to the c function signature) wouldn't be compiled by MSVC. Apparently they have different calling conventions or something like that. Anyway this means I need to have some kind of thicker interface than I originally planned. Then for the rendering side, I want to target 4 different backends for rendering - Vulkan (windows, linux and android default), Metal (macOS and iOS etc), WebGL (web apps) and finally a software/CPU custom renderer for use on systems that lack sufficient hardware or software for GPU rendering, such as microcontroller systems. I have some creative ideas about rendering in more efficient ways that prioritise responsiveness and power usage, including extensive usage of sliced rendering and building more functionality to be inherant to the rendering system, minimising callbacks to business logic. The areas where I'm hoping my project will stand out are to be it's efficiency, small size, intuitive API, backwards and forwards compatibity, support of every major platform, support of  many different programming languages, and finally built-in support for 3D rendering. In order to achieve these things I'm writing my framework as a self-contained C-only library that can be either statically or dynamically linked. It makes extensive use of native platform technologies where possible such as Cairo, CoreText etc for text rendering which simplifies project scope, improves the native look and feel, and reduces the binary size. I've been thinking about how to progress because my current dilemma is as follows: I need to define a different API than C callbacks. I also need to come up with some better definition of how graphics work and how they can be described in a language-neutral manner that is also very efficient. Outside of my project I work as an automotive electronics engineer, so I work extensively with protocols such as CANopen and XCP. I'm thinking of designing my framework API to resemble something similar to how these work: define a 'packet' format, that is oriented around defined objects and IDs, so no requirement of strings for communication and the API. This approach would allow my API to be entirely memory-safe, be completely consistent across different programming languages, reduce the overhead compared to string operations hugely, allow it to be statically linked, and yet still work as a dynamically linked library or as a client/server design to separate application logic and graphics logic. What are your comments on my project as a whole? Do you have any suggestions for high-level changes? What do you think of my idea for a data-oriented/numeric API? How would you suggest I get started with writing my API?

## 19/5/2024

Thought a lot about the API interface and decided it definitely isn't the priority for now. The priority is definitely getting GSPCore working as a statically linked library in pure C and demonstrating how it can be used to render application views.

Started working on defining the drawing interface, which wants to be:

- Simple and intuitive
- Stateful
- Inherently responsive

Also thinking that 'rendering' into HTML would be a great feature. The paradigms need to work well translating between HTML/CSS and rendered views. WebGL is an important feature for web, and it should work seamlessly with the rest of the system. Thinking there should be 'frame providers' or something that can differentiate between high-performance geometry rendering such as WebGL and standard view rendering.

If GSPCore is aimed at using more underlying graphics technologies (possibly a great idea as it's what React Native does and RN is very popular), it then differentiates itself by being a C API, supporting older systems, high performance GPU rendering, and being nice and intuitive to use.

## 21/05/2024

Thinking that it makes sense to put a big focus on C# as the recommended language for general-purpose app development. Large number of C# developers, it can be compiled to native, is reasonably performant and memory safe.

## 22/05/2024

Thought about the view/scene model, and I think it makes a lot of sense to have a generic 'GView' object that can represent any function. This would make up for a lack of OOP functionality in C.

Just did a test with 10k small views each with a 2D canvas rendered image in divs, using a css flexbox to lay out. This works fine for layout but takes a long time to render (e.g 1000ms). Probably wise to use WebGL2 and render text with 2D canvas. Then can use 1 set of view layout algorithms etc between all different rendering systems.

## 23/05/2024

Working on making GSPCore a complete abstraction layer will require implementing features from scratch on some platforms that lack them.

## 27/05/2024

Today got a few hours to spend on the project. Want to get some semantic stateful rendering working today, with a design that can be easily implemented for different platforms, interfaces and rendering systems. 

Also a quick note on the Object-Oriented vs C issue, I think I have a good solution:

- Types prefixed with 'G' are implemented in C only, so are opaque types with defined interfaces.
- Object-Oriented functionality such as in UniversalUI should have a different prefix such as 'U'

## 01/06/2024

A couple days ago (28th maybe), I finally got SDL WASM rendering working as I wanted. Took way too long but it was an annoyingly simple issue which was that I was rendering to a grayscale bitmap rather than an RGBA one. After using a different function it basically worked straight away.

Now I have a whole weekend and part of the next one to put some time into the project and hopefully get it into a better state that can actually be used for some basic stuff, and allow me to move on to another project for a few weeks or in parallel.

To recap, the following capabilities are implemented and somewhat tested in the current working copy of GSPCore:

### WASM	

- Creation of a GL canvas
- Rendering of text via SDL TTF
- Basic rendering of text bitmap onto viewport (good aliasing but poor quality layout?? ok for now)

### Win32

- Creation of windows and some window events with callbacks
- Vulkan initialisation and clear screen rendering
- Sample rendering of text via GDI (poor quality aliasing but probably ok for now)

### Linux

- Creation of X11 windows with some events with callbacks
- Vulkan initialisation and clear screen rendering
- Sample rendering of text via Cairo (good quality)

### macOS

- Very basic hello world example

### iOS

- Very basic hello world example

It still very much feels like the project lacks a definitive structure and API style, so I think this is an important thing to do.

The areas where the current structure seems lacking are:

- Events system is inconsistent and inelegant
- Separate interface and definition files is strange and can be annoying
- More files than necessary

Solutions:

- Make use of 'Controller' style model, where a controller object is responsible for handling events. This can be wrapped in a number of ways but separates out event control into a separate object which I think is neat.
- Use only 1 header file for each object, but make use of an internal GSPCORE_BUILD symbol to hide internal structures and functions when they are not wanted.
- Be strict about the 1:1 ratio of header files to source files and keep a mirrored structure with variants declared in the header file.

## 02/06/2024

Focusing on going as far with a 'full stack' implementation as possible in the WASM environment.

In particular, this is an implementation of GRenderer for WebGL and a cross-platform GView implementation.

Haven't written much in the log today but this is because a lot of very productive coding was done. There is now a working WASM implementation of GSPCore that has Application, Window and View all implemented at a basic level with their controllers. In addition, the Renderer class is capable of selectively rendering views to textures and then compositing all at the right location when the screen is rendered. A basic view tree structure allows the propagation of events such as mouse enter and exit events, and a full window render can be requested by any view. The beginnings of a layout system is also implemented, currently only with a DockPanel style layout, but this has DOCK_LEFT, DOCK_TOP, DOCK_RIGHT and DOCK_BOTTOM all working as expected which is great.

For the first time, it actually seems like GSPCore is making serious progress and should actually become something usable in not too long.

Here are some of the next steps to do:

- Separate out the platform-dependant from platform-independant code. There is currently a lot of shared code between different implementations mixed up with unique code. This makes it a pain to track changes and keep everything working well, so there should be a better distinction between fully cross-platform functions and fully platform-specific functions.
- A big one is to start looking at the drawing API. This probably wants to be similar to SVG and will likely use hardware techniques to reduce code complexity and execution time. Examples include turning a rounded rect from an 8 sided shape (4 straight sides, 4 arcs) to be an e.g 44 sided shape (4 straight sides, 10 sides at each corner for a 10px radius). Testing is necessary to decide which methods are best, although the most important thing is to have an ok implementation that works as expected.
  - The API should definitely involve a caching system and some kind of responsive layout. This will reduce the number of times a view's drawing commands are updated and that's going to be a big factor in improving performance.
- At some point, bringing the Vulkan and Metal implementations up to speed with WebGL will be important. This isn't as critical as making the WebGL implementation more fleshed out I think, but the key to making the other backend implementations feasible will be the abstraction of dependent code out of independent code. 
- Other aspects that are as yet completely ignored are: 
  - Text input
  - Localisation
  - Animation
  - Accessibility
  - DPI awareness
  - File parsing
  - Software graphics rendering (low priority)

## 09/06/2024

Had some great architectural progres today. Looking at drawing APIs such as Skia and how to design one for GSPCore that can be represented in a format that is portable and relatively easy to implement in a fragment shader. The solution - a 4-byte serialization stream (called AG4 for now - Abstract Graphics 4-byte serialization).

This is roughly as follows:

The stream is made up of command units and data units. Command units have a 2-byte command ID, a 1-byte parameter count and a flag/modifier byte. Elements (such as rectangles, lines etc) are created with the 0x0001 AG4_BEGIN_ELEMENT command, and committed with the AG4_END_ELEMENT command. Between these two commands, a number of field parameters can be inserted such as AG4_SET_FRAME which is followed by 4 floating point units for the x, y, width and height. 

While there are of course many considerations with this approach (such as fixed buffer sizes in shaders), it offers some great advantages:
- Platform and renderer independance
- Variable sizing for elements, so efficient packing.
- Ability to save graphics to a file.
- Room for expansion with future versions.
- Should be quite efficient to run although this depends on implementation.

Next:

- Need to start working on other projects a bit more.
- Want to revisit C object structure as it's messy, time consuming and possibly error prone. Requires a lot of NULL checks which bloat code a lot.
    - Possibly consider move to C++ or less use of pointers/a better C API. Still think not using C is a skill issue.
- Implement a stacked graphical attachment system for views which can be AG4 graphics, image resources or text. These can then be given different resizing characteristics - a good example of this would be a rounded button where the background is a 9-sliced AG4 texture and the text is centred.
- Look at restructuring code so that there is less duplicated code between platforms and it becomes only platform specific code that isn't shared. Possibly use #ifdef GSPCORE_BUILD_MACOS etc within C files to have only 1 source file per class rather than multiple, with sections for each platform and lots of #ifdef macros.
- Continue focusing on WebGL version, but start to bring other platforms up to speed.
- The reduction in shared code also applies to this, where the GRenderer code for instance should contain a lot of shared code.


## 11/06/2024

Today had another look at code structure and languages, still happy that C is a good choice, mainly due to the Objective-C side of things. Otherwise Rust could be a good choice. Will save the pain and setback of refactoring in Rust for now.

Thinking of implementing a custom memory pool to reduce the number of NULL checks. If it's in the memory pool, the worst that can happen is corrupted memory rather than a segfault.

Also thinking about smart pointers.

## 13/06/2024

Not going to do any development today, but will make sure to finish work early enough tomorrow that I can spend a good amount of time on the project. Just installed Fedora which had been great on Asahi linux and seems to be just as good on my desktop.

Plans for the next steps:

- add back in wayland code
- start refactoring C code so that there are fewer objects created by the user, more by the system.
- possibly look into a proxy ID system (rather than giving the user raw pointers that contain all the handles to objects, keep them internally).
- use less (if any) dynamic memory allocation, instead use linked lists to do a lot more stuff as these are much more performant.
- separate out duplicated code and define platform specific code better.
- get Vulkan implementation to a better level.

## 14/06/2024

Refactoring going well. Simplified a few things already which has made the system significantly simpler already. Things like not having an application object but instead just a struct with a controller as a field. Taking inspiration from GLFW with their opaque struct GLFWwindow - the GWindow is also an opaque struct. GLFWwindow also just uses a linked list rather than an array of windows - this saves complexity, memory and time! Probably going to delete GVector because it's lazy programming TBH (and slow).

Update at end of day:
 
Now have a wayland window rendering with vulkan.
Newer code is much cleaner, more maintainable and less overlap.

Main issue right now is that there is a complete mix of old and new code. Need to do a purge of old code and refactor WASM and Win32 into new structure (macos + ios are behind enough that it doesn't really matter for them).

This should be a priority tomorrow - get those cleaned up.
Then start to get vulkan rendering more pinned down for 2D compositor stuff.
Then implement fragment vector shader and text and we're in a much better position.

## 15/06/2024

Have some time today although not completely free.
Right now going to do a purge of old code, try and get a simple baseline functionality working for Linux (Wayland and X11), Win32 and WebGL.
That is - app launch, window and rendermanager creation.

GWindow is to be one of very few (possibly the only) items allocated dynamically. This is because I don't want to impose arbitrary restrictions on the number of windows that can be created. It will be tracked with a linked list, which will be used to determine if a window pointer is valid or not.

Update a few hours later:

Achieved a good level of win32/webgl support, both now have application callbacks and win32 has vulkan rendering working again which is great. Not going to spend any more time yet on WebGL as it's comparitively easy and already have source examples.

Just added some wayland functionality so that pointer enter, exit, move and button presses are now all captured (although they are not sent anywhere).

The view API is still a big question so I'm going to focus on rendering for now - this is a concrete, objective functionality that is also very important. The view API is much more of a convenience thing and doesn't even need to be part of GSPCore. Instead it could be implemented on the client side.

  This is actually quite a big deal - the semantics of how to create a view API are very platform dependant and also vary significantly. It makes sense to focus on GSPCore as being a platform abstraction layer, events broker and rendering system.
  