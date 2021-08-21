using System;
using System.IO;
using System.Diagnostics;
using System.Configuration;
using System.Net;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Management.Deployment;
using Windows.Foundation;

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

        static string exepath = Process.GetCurrentProcess().MainModule.FileName;
        public static string exedir = Path.GetDirectoryName(exepath);

        /// <summary>
        /// Start a DOOMEternalx64vk.exe process, 
        /// assuming the exe is in the same dir as the launcher exe
        /// </summary>
        public static void StartDEternal(string args="")
        {
            string path = Path.Combine(exedir, "DOOMEternalx64vk.exe");
            Process.Start(path, args);
        }

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

        public static void UninstallDEternal()
        {
            /* todo: idea -- requirements for game deletion mode:
             * first launcher arg must be --uninstall (`-l "--uninstall"`) 
             * second arg must be --path, with argument after (`-l "--uninstall --path 'E:\Games\DOOMEternal'"`)
             * Deletion will not proceed if DOOMEternalx64vk.exe is present and if `exedir` matches input path.
             * 
             * Two failure outcomes of the above requirements:
             * 1. If ANY args are passed along with any requirements not met, nothing will happen.
             * 2. If no args are passed, along with any other requirements not met, we will create 
             *    a temp copy of this EXE, move it into null with the DELAY_UNTIL_REBOOT flag, then spawn a
             *    proc of our temp EXE (with the required args) and exit our orig proc. Now, from the
             *    temp EXE, we will delete the provided directory, de-register DOOM Eternal's UWP Package, and exit.
             */
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
        }
    }
}
