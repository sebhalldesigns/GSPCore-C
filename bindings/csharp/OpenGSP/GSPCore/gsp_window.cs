using System.Runtime.InteropServices;

namespace OpenGSP.GSPCore.Native;

using gwindow_t = UIntPtr;
using gstring_t = string;


public static class gsp_window {

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void gsp_window_poll_events();

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool gsp_window_is_window_valid(gwindow_t window);

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr gsp_window_create_window();

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void gsp_window_set_title(gwindow_t window, gstring_t title);

}
