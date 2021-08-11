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
#if DEBUG
            bool waiting = true;
            while (waiting) { continue; }
#endif
            bool skip_updates = bool.Parse(ConfigurationManager.AppSettings.Get("SkipUpdates"));

            var launcher_args = new List<string>();
            var doom_args = new List<string>();

            // arg parsing
            if (e.Args != null && e.Args.Length > 0)
            {
                // this is for the command line
                bool l_parse = true;
                for (int i = 0; i < e.Args.Length; i++)
                {
                    if (e.Args[i].Equals("-l") && l_parse)
                    {
                        launcher_args = e.Args[i + 1].Split(' ').ToList();
                        i++;
                        l_parse = false;
                        continue;
                    }
                    else
                    {
                        doom_args.Add(e.Args[i]);
                    }
                }
                
                // the arg(s) after the '-l' option
                for (int i = 0; i < launcher_args.Count; i++)
                {
                    if (launcher_args[i].Equals("--skip-updates"))
                    {
                        skip_updates = true;
                    }
                }

            }
            Launcher.doom_args = string.Join(" ", doom_args.ToArray());
            
            

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
                Launcher.StartDEternal(Launcher.doom_args);
                Shutdown(0);
                return;
            }
        }
    }
}
