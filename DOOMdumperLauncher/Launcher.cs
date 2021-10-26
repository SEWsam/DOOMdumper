using System;
using System.Threading;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Net;
using Windows.Foundation;
using Windows.Management.Deployment;
using Microsoft.Win32.SafeHandles;

namespace DOOMdumperLauncher
{
    /// <summary>
    /// Launcher class which houses methods and variables used by the launcher
    /// </summary>
    public static class Launcher
    {
        public static string doom_args;
        public static Version latest_version;
        public static Version dumped_version = new Version(ConfigurationManager.AppSettings.Get("GameVersion"));

        public static string exe_path = Process.GetCurrentProcess().MainModule.FileName;
        public static string exe_dir = Path.GetDirectoryName(exe_path);
        public static string game_exe_path = Path.Combine(exe_dir, "DOOMEternalx64vk.exe");

        /// <summary>
        /// Start a DOOMEternalx64vk.exe process, 
        /// assuming the exe is in the same dir as the launcher exe
        /// </summary>
        public static void StartDEternal(string args="")
        {
            string path = Path.Combine(exe_dir, "DOOMEternalx64vk.exe");
            Process.Start(path, args);
        }

        /// <summary>
        /// Get's the latest DOOM Eternal version as a string,
        /// which is then converted to a Version object
        /// </summary>
        /// <returns>Version Object with the obtained version string.</returns>
        public static Version GetLatestVersion()
        {
            // todo: this url should be in app config
            string url = "https://sewsam.github.io/deternal_version";
            string version_string;
            using (var webclient = new WebClient())
            {
                version_string = webclient.DownloadString(url);
            }

            var version = new Version(version_string);
            return version;
        }

        public static void InitUninstaller()
        {
            string uninstaller_dir = Path.Combine(Path.GetTempPath(), "DOOMdumperLauncher");
            string uninstaller_path = Path.Combine(uninstaller_dir, "Launcher.exe");
            Directory.CreateDirectory(uninstaller_dir);

            File.Copy(exe_path, uninstaller_path);

            // TODO: It would be nice to remove the temp exe, one day maybe

            string args = "--uninstall --path " + exe_dir;
            Process.Start(uninstaller_path, args);
            Thread.Sleep(200);
            return;
        }

        public static int UninstallDEternal(string path)
        {
            if (exe_dir == path || File.Exists(game_exe_path))
            {
                return 1;
            }

            Directory.Delete(path, true);

            PackageManager packageManager = new PackageManager();
            var deploymentOperation = packageManager.RemovePackageAsync("BethesdaSoftworks.DOOMEternal-PC_1.0.10.0_x64__3275kfvn8vcwc");

            // Check the status of the operation
            if (deploymentOperation.Status == AsyncStatus.Error)
            {
                DeploymentResult deploymentResult = deploymentOperation.GetResults();
                Console.WriteLine("Error code: {0}", deploymentOperation.ErrorCode);
                Console.WriteLine("Error text: {0}", deploymentResult.ErrorText);
            }
            else if (deploymentOperation.Status == AsyncStatus.Canceled)
            {
                Console.WriteLine("Removal canceled");
            }
            else if (deploymentOperation.Status == AsyncStatus.Completed)
            {
                Console.WriteLine("Removal succeeded");
            }
            else
            {
                Console.WriteLine("Removal status unknown");
            }

            return 0;
        }

        /* 
         * This is useful stuff -- commenting out for now
         * 
        /// <summary>
        /// Stuff for interacting with C / Unmanaged Win32 API code
        /// </summary>
        public static class Win32
        {
            [Flags]
            public enum GenericAccessFlags : uint
            {
                GENERIC_READ     =  0x80000000,
                GENERIC_WRITE    =  0x40000000,
                GENERIC_EXECUTE  =  0x20000000,
                GENERIC_ALL      =  0x10000000
            }

            [Flags]
            public enum FileShareFlags : uint
            {
                FILE_SHARE_READ    =  0x00000001,
                FILE_SHARE_WRITE   =  0x00000002,
                FILE_SHARE_DELETE  =  0x00000004
            }

            [Flags]
            public enum FileFlags : uint
            {
                FILE_FLAG_WRITE_THROUGH        =  0x80000000,
                FILE_FLAG_OVERLAPPED           =  0x40000000,
                FILE_FLAG_NO_BUFFERING         =  0x20000000,
                FILE_FLAG_RANDOM_ACCESS        =  0x10000000,
                FILE_FLAG_SEQUENTIAL_SCAN      =  0x08000000,
                FILE_FLAG_DELETE_ON_CLOSE      =  0x04000000,
                FILE_FLAG_BACKUP_SEMANTICS     =  0x02000000,
                FILE_FLAG_POSIX_SEMANTICS      =  0x01000000,
                FILE_FLAG_SESSION_AWARE        =  0x00800000,
                FILE_FLAG_OPEN_REPARSE_POINT   =  0x00200000,
                FILE_FLAG_OPEN_NO_RECALL       =  0x00100000,
                FILE_FLAG_FIRST_PIPE_INSTANCE  =  0x00080000
            }


            [System.Runtime.InteropServices.DllImport("kernel32.dll", EntryPoint = "CreateFileA")]
            public static extern SafeFileHandle CreateFileA(string lpFileName, uint dwDesiredAccess, uint dwShareMode,
                IntPtr lpSecurityAttributes, uint dwCreationDisposition, uint dwFlagsAndAttributes, IntPtr hTemplateFile);

            
        }


            //var uninstaller_handle = Win32.CreateFileA(
            //    uninstaller_path, 
            //    (uint)Win32.GenericAccessFlags.GENERIC_READ,
            //    (uint)Win32.FileShareFlags.FILE_SHARE_READ | (uint)Win32.FileShareFlags.FILE_SHARE_DELETE, 
            //    IntPtr.Zero, 
            //    3, 
            //    (uint)Win32.FileFlags.FILE_FLAG_DELETE_ON_CLOSE, 
            //    IntPtr.Zero);
        */
    }
}
