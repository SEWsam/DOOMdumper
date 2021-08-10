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

namespace Launcher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Launch_Click(object sender, RoutedEventArgs e)
        {
            string exepath = Process.GetCurrentProcess().MainModule.FileName;
            string exedir = System.IO.Path.GetDirectoryName(exepath);

            string path = System.IO.Path.Combine(exedir, "DOOMEternalx64vk.exe");
            string args = "";
            Process.Start(path, args);
        }
    }
}
