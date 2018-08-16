using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace SudokuInterface
{
    public static class SudokuLibApi
    {
        public static int RESPONSE_SIZE = 810;

        [DllImport(@"SudokuAPI.dll", EntryPoint = "Initialize", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Initialize(byte[] b);

        [DllImport(@"SudokuAPI.dll", EntryPoint = "Reset", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Reset();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetCurrentBoard", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetCurrentBoard();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetBoardValue", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetBoardValue();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetActiveSolvers", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetActiveSolvers();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "Solve", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Solve();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetIsSolved", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetIsSolved();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "EnableGuessing", CallingConvention = CallingConvention.Cdecl)]
        public static extern void EnableGuessing();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "DisableGuessing", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DisableGuessing();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetNumGuesses", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetNumGuesses();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetBestScore", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetBestScore();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetNumInvalids", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetNumInvalids();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetNumSurrenders", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetNumSurrenders();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "Shutdown", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Shutdown();

        [DllImport(@"SudokuAPI.dll", EntryPoint = "GetBoardData", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetBoardData();
    }
}
