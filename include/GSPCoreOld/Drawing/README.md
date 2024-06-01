# GSPCore Drawing

The drawing system is designed to map well onto hardware rendering APIs such as WebGL, Metal, Vulkan etc.

**GDrawElement is analagous to a GPU Texture.** A GDrawElement will contain a handle to a texture when created on platforms using hardware rendering. It corresponds to two tris that are rendered to the viewport with an x,y location from the top left corner.

A GDrawElement will only be drawn to the texture if attached to a view;

## GDrawElement should just be part of GView!! Unecessary complexity!!

