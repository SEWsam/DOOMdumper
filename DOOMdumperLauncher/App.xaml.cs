using System;
using System.IO;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;

namespace DOOMdumperLauncher
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        // this doesn't do much different than the default setup. just placeholder code really
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            bool waitpls = false;
            while (waitpls) { continue; }

            bool skip_updates = bool.Parse(ConfigurationManager.AppSettings.Get("SkipUpdates"));
            if (e.Args != null && e.Args.Length > 0)
            {
                for (int i = 0; i < e.Args.Length; i++)
                {
                    if(e.Args[i].Equals("--skip-updates"))
                    {
                        skip_updates = true;
                    }
                }
            }
            // todo: exception handling if config is invalid
            bool update_available = false;
            if (!skip_updates)
            {
                Launcher.latest_version = Launcher.GetLatestVersion();
                update_available = (Launcher.latest_version > Launcher.dumped_version);
            }
            if (update_available)
            {
                var window = new MainWindow();
                window.ShowDialog();
            }
            else
            {
                Launcher.StartDEternal();
                Shutdown(0);
                return;
            }
        }
    }
}
