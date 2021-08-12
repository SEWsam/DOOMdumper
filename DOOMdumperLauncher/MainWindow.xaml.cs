using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;

namespace DOOMdumperLauncher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string LatestVersion { get; set; }
        public string DumpedVersion { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            LatestVersion = Launcher.latest_version.ToString();
            DumpedVersion = Launcher.dumped_version.ToString();
            // this is supposed to be bad but my implementation is minimal so i dont care rn
            DataContext = this;  
        }

        private void Launch_click(object sender, RoutedEventArgs e)
        {
            Launcher.StartDEternal(Launcher.doom_args);
            Close();
            System.Threading.Thread.Sleep(5000);
            return;
        }

        private void Update_click(object sender, RoutedEventArgs e)
        {
            string message = "Are you sure you want to uninstall DOOM Eternal in order to update it?";
            string title = "DOOM Eternal Update";
            MessageBoxButton buttons = MessageBoxButton.YesNo;
            MessageBoxImage icon = MessageBoxImage.Warning;
            if (MessageBox.Show(message, title, buttons, icon) == MessageBoxResult.Yes)
            {
                Launcher.UninstallDEternal();
                MessageBox.Show("doom eternal should probably de-register when you exit. still figuring out how to handle deleting the installation.");
            }
        }
    }
}
