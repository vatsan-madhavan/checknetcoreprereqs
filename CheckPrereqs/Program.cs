using PInvoke;
using System;

namespace AddDllDirectoryDetectCs
{
    class Program
    {
        static void Main(string[] args)
        {
            using (var hModule = Kernel32.LoadLibrary("Kernel32.dll"))
            {
                if (!hModule.IsInvalid)
                {
                    IntPtr hFarProc = Kernel32.GetProcAddress(hModule, "AddDllDirectory");
                    if (hFarProc != IntPtr.Zero)
                    {
                        Console.WriteLine("Either running on Win8+, or KB2533623 is installed");
                    }
                    else
                    {
                        Console.Write("Likely running on Win7 or older OS, and KB2533623 is not installed");
                    }
                }
            }

            using (var hModule = Kernel32.LoadLibraryEx("UCRTBASE.dll", IntPtr.Zero, Kernel32.LoadLibraryExFlags.LOAD_LIBRARY_SEARCH_SYSTEM32))
            {
                if (!hModule.IsInvalid)
                {
                    Console.WriteLine("UCRT is available - Either running on Win10+ or KB2999226 is installed");
                }
                else
                {
                    Console.WriteLine("UCRT is not available - Likely running on OS older than Win10 and KB2999226 is not installed");
                }
            }
        }
    }
}
