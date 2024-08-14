using System.Runtime.InteropServices;

namespace OpenGSP.GSPCore.Native;

using glist_t = UIntPtr;

public static class gsp_list {

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern glist_t gsp_list_create_list();

    [DllImport("GSPCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void gsp_list_destroy_list(glist_t list);

}
