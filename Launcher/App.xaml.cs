using System;
using System.IO;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;

namespace Launcher
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

            if (e.Args != null && e.Args.Length > 0)
            {
                for (int i = 0; i < e.Args.Length; i++)
                {
                    Console.WriteLine(e.Args[i]);
                }
            }

            // just testing stuff. i won't acutally be ALWAYS showing the window in the final product
            var window = new MainWindow();
            window.ShowDialog();
        }
    }
}
