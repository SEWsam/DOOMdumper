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

        /// <summary>
        /// Start a DOOMEternalx64vk.exe process, 
        /// assuming the exe is in the same dir as the launcher exe
        /// </summary>
        public static void StartDEternal(string args="")
        {
            string exepath = Process.GetCurrentProcess().MainModule.FileName;
            string exedir = Path.GetDirectoryName(exepath);

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
