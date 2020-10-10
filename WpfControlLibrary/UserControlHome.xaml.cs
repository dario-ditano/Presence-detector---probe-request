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
using Tulpep.NotificationWindow;


namespace WpfControlLibrary
{

    public partial class UserControlHome : UserControl
    {
        protected Process TCPServer;
        //protected MySqlConnection DBconnection;

        public string filename = "C:\\Users\\slash\\OneDrive\\Desktop\\PROGRAMMAZIONE DI SISTEMA\\PdS-Detection-System-master\\PDS_Detection_System\\x64\\Release\\PDS_Detection_System.exe";
        public static Label espn_box;
        public static Label output_box;
        public String args = "";
        public String file = "";

        protected const int startEspNum = 1;
        protected int espNum;
        public int maxX = 0, minX = 0, maxY = 0, minY = 0;

        private const int ellipseSize = 13;
        public List<ESPH> ESPS;

        public UserControlHome()
        {
        
            InitializeComponent();
            ESPS = new List<ESPH>();
            esp_number.Content = startEspNum;
            espNum = startEspNum;
            espn_box = (Label)this.FindName("esp_number");
            output_box = (Label)this.FindName("info_picker");
            ESPS.Add(new ESPH(0, 0, 0, true));
            for (int i = startEspNum; i < 8; i++)
            {
                TextBox espBoxX = (TextBox)this.FindName("esp" + i + "x");
                espBoxX.Visibility = Visibility.Hidden;
                TextBox espBoxY = (TextBox)this.FindName("esp" + i + "y");
                espBoxY.Visibility = Visibility.Hidden;
                Label espBoxXL = (Label)this.FindName("esp" + i + "xl");
                espBoxXL.Visibility = Visibility.Hidden;
                Label espBoxYL = (Label)this.FindName("esp" + i + "yl");
                espBoxYL.Visibility = Visibility.Hidden;
                ESPS.Add(new ESPH(i, 0, 0, false));
            }
        }


        /// <summary>
        /// Event to indicate new data is available
        /// </summary>
        public event EventHandler DataAvailable;
        /// <summary>
        /// Called to signal to subscribers that new data is available
        /// </summary>
        /// <param name="e"></param>
        protected virtual void OnDataAvailable(EventArgs e)
        {
            EventHandler eh = DataAvailable;
            if (eh != null)
            {
                eh(this, e);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            int duplicate = 0;

            string num = espn_box.Content.ToString();
            string x0 = esp0x.Text, y0 = esp0y.Text;
            string x1 = esp1x.Text, y1 = esp1y.Text;
            string x2 = esp2x.Text, y2 = esp2y.Text;
            string x3 = esp3x.Text, y3 = esp3y.Text;
            string x4 = esp4x.Text, y4 = esp4y.Text;
            string x5 = esp5x.Text, y5 = esp5y.Text;
            string x6 = esp6x.Text, y6 = esp6y.Text;
            string x7 = esp7x.Text, y7 = esp7y.Text;

            String positions = x0 + y0 + " " + x1 + y1 + " " + x2 + y2 + " " + x3 + y3 + " " + x4 + y4 + " " + x5 + y5 + " " + x6 + y6 + " " + x7 + y7;
            List<String> p = new List<string>();

            for (int i = 0; i < int.Parse(num); i++)
            {
                String current = positions.Split(' ')[i];
                if (p.Contains(current))
                {
                    duplicate = 1;
                }
                else
                {
                    p.Add(current);
                }
            }

            if(duplicate == 0)
            {
                /*
             args for TCPServer.cpp main() function:
             0/1 -> if 1 the server should erase all the db
             num -> num of esp in the system
             coordinates of esps
             */
                args = "1 " + num + " " + x0 + " " + y0 + " " + x1 + " " + y1 + " " +
                                                            x2 + " " + y2 + " " + x3 + " " + y3 + " " +
                                                            x4 + " " + y4 + " " + x5 + " " + y5 + " " +
                                                            x6 + " " + y6 + " " + x7 + " " + y7;

                OnDataAvailable(null);
                //DataWindow dataW = new DataWindow(args, "C:\\Users\\slash\\OneDrive\\Desktop\\PROGRAMMAZIONE DI SISTEMA\\PdS-Detection-System-master\\PDS_Detection_System\\x64\\Release\\PDS_Detection_System.exe");
                // dataW.Show();
                //Application.Current.MainWindow.Close();
            }
            else
            {
                MessageBox.Show("Error: There are multiple ESP with the same position ");
            }


        }

        private void Exe_picker_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.Filter = "Executable File (.exe) | *.exe";
            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                //output_box.Content = "Succes: Server executable file selected.";
                PopupNotifier popup = new PopupNotifier();
                popup.TitleText = "Executable File";
                popup.ContentText = "Executable File (.exe) selected";
                popup.AnimationDuration = 5;

                popup.BodyColor = System.Drawing.Color.FromArgb(51, 51, 76);
                popup.ContentColor = System.Drawing.Color.White;
                popup.BorderColor = System.Drawing.Color.FromArgb(199, 21, 133);
                popup.TitleColor = System.Drawing.Color.FromArgb(199, 21, 133);
                popup.Popup();
                filename = dlg.FileName;

            }
        }

        private void Esp_increase_Click(object sender, RoutedEventArgs e)
        {
            if (espNum < 8)
            {
                TextBox espBoxX = (TextBox)this.FindName("esp" + espNum + "x");
                espBoxX.Visibility = Visibility.Visible;
                TextBox espBoxY = (TextBox)this.FindName("esp" + espNum + "y");
                espBoxY.Visibility = Visibility.Visible;
                Label espBoxXL = (Label)this.FindName("esp" + espNum + "xl");
                espBoxXL.Visibility = Visibility.Visible;
                Label espBoxYL = (Label)this.FindName("esp" + espNum + "yl");
                espBoxYL.Visibility = Visibility.Visible;
                espNum++;
                esp_number.Content = espNum;
            }
        }

        private void Esp_derease_Click(object sender, RoutedEventArgs e)
        {
            if (espNum > 1)
            {
                espNum--;
                esp_number.Content = espNum;
                TextBox espBoxX = (TextBox)this.FindName("esp" + espNum + "x");
                espBoxX.Visibility = Visibility.Hidden;
                TextBox espBoxY = (TextBox)this.FindName("esp" + espNum + "y");
                espBoxY.Visibility = Visibility.Hidden;
                Label espBoxXL = (Label)this.FindName("esp" + espNum + "xl");
                espBoxXL.Visibility = Visibility.Hidden;
                Label espBoxYL = (Label)this.FindName("esp" + espNum + "yl");
                espBoxYL.Visibility = Visibility.Hidden;
            }
        }


        private void Home_Update_Click(object sender, RoutedEventArgs e)
        {
            int t = 1;
            var x = new List<int>();
            var y = new List<int>();

            int x0 = Convert.ToInt32(esp0x.Text), y0 = Convert.ToInt32(esp0y.Text);
            x.Add(x0);
            y.Add(y0);

            Label espBoxX1 = (Label)this.FindName("esp" + 1 + "xl");
            bool v1 = espBoxX1.IsVisible;
            int x1 = Convert.ToInt32(esp1x.Text), y1 = Convert.ToInt32(esp1y.Text);
            if (v1)
            {
                x.Add(x1);
                y.Add(y1);
                t++;
            }

            Label espBoxX2 = (Label)this.FindName("esp" + 2 + "xl");
            bool v2 = espBoxX2.IsVisible;
            int x2 = Convert.ToInt32(esp2x.Text), y2 = Convert.ToInt32(esp2y.Text);
            if (v2)
            {
                x.Add(x2);
                y.Add(y2);
                t++;
            }

            Label espBoxX3 = (Label)this.FindName("esp" + 3 + "xl");
            bool v3 = espBoxX3.IsVisible;
            int x3 = Convert.ToInt32(esp3x.Text), y3 = Convert.ToInt32(esp3y.Text);
            if (v3)
            {
                x.Add(x3);
                y.Add(y3);
                t++;
            }

            Label espBoxX4 = (Label)this.FindName("esp" + 4 + "xl");
            bool v4 = espBoxX4.IsVisible;
            int x4 = Convert.ToInt32(esp4x.Text), y4 = Convert.ToInt32(esp4y.Text);
            if (v4)
            {
                x.Add(x4);
                y.Add(y4);
                t++;
            }

            Label espBoxX5 = (Label)this.FindName("esp" + 5 + "xl");
            bool v5 = espBoxX5.IsVisible;
            int x5 = Convert.ToInt32(esp5x.Text), y5 = Convert.ToInt32(esp5y.Text);
            if (v5)
            {
                x.Add(x5);
                y.Add(y5);
                t++;
            }

            Label espBoxX6 = (Label)this.FindName("esp" + 6 + "xl");
            bool v6 = espBoxX6.IsVisible;
            int x6 = Convert.ToInt32(esp6x.Text), y6 = Convert.ToInt32(esp6y.Text);
            if (v6)
            {
                x.Add(x6);
                y.Add(y6);
                t++;
            }

            Label espBoxX7 = (Label)this.FindName("esp" + 7 + "xl");
            bool v7 = espBoxX7.IsVisible;
            int x7 = Convert.ToInt32(esp7x.Text), y7 = Convert.ToInt32(esp7y.Text);
            if (v7)
            {
                x.Add(x7);
                y.Add(y7);
                t++;
            }

            maxX = x.Max();
            minX = x.Min();
            maxY = y.Max();
            minY = y.Min();

            var v = new List<bool>() { true, v1, v2, v3, v4, v5, v6, v7 };
            ESPS.Clear();

            for (int i = 0; i < t; i++)
            {
                ESPS.Add(new ESPH(i, x[i], y[i], v[i]));
            }


            ClearMap();
        }

        private void ClearMap()
        {
            HomeMapOfDevices.Children.Clear();
            foreach (ESPH curEsp in ESPS)
            {
                if (curEsp.H)
                {
                    DrawESP(curEsp);
                }
            }
        }

        private void DrawESP(ESPH curEsp)
        {
            // Create a red Ellipse.
            Ellipse myEllipse = new Ellipse();

            // Create a SolidColorBrush with a red color to fill the 
            // Ellipse with.
            SolidColorBrush mySolidColorBrush = new SolidColorBrush();

            // Describes the brush's color using RGB values. 
            // Each value has a range of 0-255.
            mySolidColorBrush.Color = Color.FromArgb(255, 66, 119, 244);
            myEllipse.Fill = mySolidColorBrush;
            myEllipse.StrokeThickness = 2;
            myEllipse.Stroke = Brushes.Gray;

            // Set the width and height of the Ellipse.
            myEllipse.Width = ellipseSize;
            myEllipse.Height = ellipseSize;

            // Add a ToolTip that shows the name of the Device and its position 
            ToolTip tt = new ToolTip();
            tt.Content = "ESP " + curEsp.Id + "\nX: " + curEsp.X + " - Y: " + curEsp.Y;
            myEllipse.ToolTip = tt;

            // Add the Ellipse to the Canvas.
            HomeMapOfDevices.Children.Add(myEllipse);

            Canvas.SetLeft(myEllipse, ScaleX(curEsp.X));
            Canvas.SetBottom(myEllipse, ScaleY(curEsp.Y));
        }

        private double ScaleX(double value)
        {
            double scaledValue;
            int width = Convert.ToInt32(HomeMapOfDevices.Width);

            if (maxX - minX != 0)
                scaledValue = (value * width / (maxX - minX)) - ellipseSize / 2;
            else
                scaledValue = width / 2;

            return scaledValue;
        }

        private double ScaleY(double value)
        {
            double scaledValue;
            int height = Convert.ToInt32(HomeMapOfDevices.Height);

            if (maxY - minY != 0)
                scaledValue = (value * height / (maxY - minY)) - ellipseSize / 2;
            else
                scaledValue = height / 2;

            return scaledValue;
        }

    }

    public class ESPH
    {
        private int x, y, id;
        private bool hid = false;
        public int Id
        {
            get { return id; }
        }
        public int X
        {
            get { return x; }
        }
        public int Y
        {
            get { return y; }
        }

        public bool H
        {
            get { return hid; }
        }


        public ESPH(int id, int x, int y, bool hid)
        {
            this.id = id;
            this.x = x;
            this.y = y;
            this.hid = hid;
        }
    }
}

