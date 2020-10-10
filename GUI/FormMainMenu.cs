using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using Tulpep.NotificationWindow;

namespace GUI
{
    public partial class FormMainMenu : Form, INotifyPropertyChanged
    {
        //Fields
        private Button currentBtn;
        private Panel leftBorderBtn;
        private String currentUC = "Home";
        private String file = ""; // "C:\\Users\\slash\\OneDrive\\Desktop\\PROGRAMMAZIONE DI SISTEMA\\PdS-Detection-System-master\\PDS_Detection_System\\x64\\Release\\PDS_Detection_System.exe"
        private String config = "";
        private int first = 0;
        private int old = 0;

        public event PropertyChangedEventHandler PropertyChanged;


        //Constructor
        public FormMainMenu()
        {
            InitializeComponent();
            leftBorderBtn = new Panel();
            leftBorderBtn.Size = new Size(7, 60);
            panelMenu.Controls.Add(leftBorderBtn);
            ActivateButton(btnSettings, RGBColors.color2);

            WpfControlLibrary.UserControlHome uc =
                new WpfControlLibrary.UserControlHome();

            // Assign the WPF UserControl to the ElementHost control's
            // Child property.
            uc.DataAvailable += new EventHandler(Child_DataAvailable);
            elementHostHome.Child = uc;

            //Form
            this.Text = string.Empty;
            this.ControlBox = false;
            this.DoubleBuffered = true;
            this.MaximizedBounds = Screen.FromHandle(this.Handle).WorkingArea;
        }
        //Structs
        private struct RGBColors
        {
            public static Color color1 = Color.FromArgb(172, 126, 241);
            public static Color color2 = Color.FromArgb(255, 70, 110);
            public static Color color3 = Color.FromArgb(253, 138, 114);
            public static Color color4 = Color.FromArgb(95, 77, 221);
            public static Color color5 = Color.FromArgb(249, 88, 155);
            public static Color color6 = Color.FromArgb(24, 161, 251);

        }

        //Methods
        private void ActivateButton(object senderBtn, Color color)
        {
            if (senderBtn != null)
            {
                DisableButton();
                //Button
                currentBtn = (Button)senderBtn;
                currentBtn.BackColor = Color.FromArgb(51, 51, 90);
                currentBtn.ForeColor = color;
                //currentBtn.Padding = new Padding(20, 0, 0, 0);
                //Left border button
                leftBorderBtn.BackColor = color;
                leftBorderBtn.Location = new Point(0, currentBtn.Location.Y);
                leftBorderBtn.Visible = true;
                leftBorderBtn.BringToFront();

            }
        }

        private void DisableButton()
        {
            if (currentBtn != null)
            {
                currentBtn.BackColor = Color.FromArgb(51, 51, 76);
                currentBtn.Padding = new Padding(10, 0, 0, 0);
                currentBtn.ForeColor = Color.Gainsboro;
                currentBtn.TextAlign = ContentAlignment.MiddleLeft;
                currentBtn.TextImageRelation = TextImageRelation.ImageBeforeText;
                currentBtn.ImageAlign = ContentAlignment.MiddleLeft;
            }
        }

        private void btnSettings_Click_1(object sender, EventArgs e)
        {

            // Create the WPF UserControl.
            if(currentUC != "Home")
            {
                if(currentUC == "Console")
                {
                    MessageBoxButtons buttons = MessageBoxButtons.YesNo;
                    DialogResult result;

                    // Displays the MessageBox.
                    result = MessageBox.Show("Do You want to interrupt the Server execution?","Exit from Server Execution", buttons);
                    if (result == System.Windows.Forms.DialogResult.Yes)
                    {
                        ActivateButton(sender, RGBColors.color2);
                        // Closes the parent form.
                        lblFormTitle.Text = "ESP Settings";
                        WpfControlLibrary.UserControlHome uc =
                        new WpfControlLibrary.UserControlHome();

                        // Assign the WPF UserControl to the ElementHost control's
                        // Child property.
                        uc.DataAvailable += new EventHandler(Child_DataAvailable);
                        elementHostHome.Child = uc;
                        currentUC = "Home";
                    }
                }
                else
                {
                    ActivateButton(sender, RGBColors.color2);
                    lblFormTitle.Text = "ESP Settings";
                    WpfControlLibrary.UserControlHome uc =
                    new WpfControlLibrary.UserControlHome();

                    // Assign the WPF UserControl to the ElementHost control's
                    // Child property.
                    uc.DataAvailable += new EventHandler(Child_DataAvailable);
                    elementHostHome.Child = uc;
                    currentUC = "Home";
                }
            }   
        }

        /// <summary>
        /// The child has data available - get it.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Child_DataAvailable(object sender, EventArgs e)
        {
            WpfControlLibrary.UserControlHome child = sender as WpfControlLibrary.UserControlHome;
            if (child != null)
            {
                if (int.Parse(child.args.Split(' ')[1]) < 3)
                {
                    MessageBox.Show("Invalid Configuration: ESP number must be greater then 2");
                }
                else
                {

                    PopupNotifier popup = new PopupNotifier();
                    popup.TitleText = "ESP Configuration";
                    popup.ContentText = "ESP Configuration confirmed!";
                    popup.AnimationDuration = 5;
                    popup.BodyColor = System.Drawing.Color.FromArgb(51, 51, 76);
                    popup.ContentColor = System.Drawing.Color.White;
                    popup.BorderColor = System.Drawing.Color.FromArgb(199, 21, 133);
                    popup.TitleColor = System.Drawing.Color.FromArgb(199, 21, 133);
                    popup.Popup();

                    config = child.args;
                    file = child.filename;
                    old = 0;
                    
                }
            }
        }


        private void buttonConsole_Click_1(object sender, EventArgs e)
        {
            if(currentUC != "Console")
            {
                if(file != "" && config != "")
                {
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.DialogResult.Yes;
                    if (old == 0)
                    {
                        old = 1;
                    }
                    else
                    {
                        MessageBoxButtons buttons = MessageBoxButtons.YesNo;

                        // Displays the MessageBox.
                        result = MessageBox.Show("Do You want to use the old configuration?", "Old Configuration", buttons);
                    }

                    if(result == System.Windows.Forms.DialogResult.Yes)
                    {
                        ActivateButton(sender, RGBColors.color2);
                        lblFormTitle.Text = "ESP Server Console";
                        // Create the WPF UserControl.
                        WpfControlLibrary.UserControlConsole uc =
                            new WpfControlLibrary.UserControlConsole(config, file, first);

                        // Assign the WPF UserControl to the ElementHost control's
                        // Child property.

                        elementHostHome.Child = uc;
                        currentUC = "Console";
                        //first = 1;
                    }

                }
                else
                {
                    MessageBox.Show("Invalid Configuration or No Configuration found");
                }
            }
        }

        private void buttonAnalytics_Click_1(object sender, EventArgs e)
        {
            if (currentUC != "Analytics")
            {
                ActivateButton(sender, RGBColors.color2);
                lblFormTitle.Text = "MySql Console";
                // Create the WPF UserControl.
                WpfControlLibrary.UserControlSql uc =
                    new WpfControlLibrary.UserControlSql();

                // Assign the WPF UserControl to the ElementHost control's
                // Child property.

                elementHostHome.Child = uc;



                currentUC = "Analytics";
            }
        }


        //Drag Form
        [DllImport("user32.DLL", EntryPoint = "ReleaseCapture")]
        private extern static void ReleaseCapture();

        [DllImport("user32.DLL", EntryPoint = "SendMessage")]
        private extern static void SendMessage(System.IntPtr hWnd, int wMsg, int wParam, int lParam);

        //Close-Maximize-Minimize


        private void panelTitleBar_MouseDown_1(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(this.Handle, 0x112, 0xf012, 0);
        }

        private void FormMainMenu_Resize_1(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Maximized)
                FormBorderStyle = FormBorderStyle.None;
            else
                FormBorderStyle = FormBorderStyle.Sizable;
        }

        private void btnExit_Click_1(object sender, EventArgs e)
        {
            {
                Application.Exit();
            }
        }

        private void btnMaximize_Click_1(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Normal)
                WindowState = FormWindowState.Maximized;
            else
                WindowState = FormWindowState.Normal;
        }

        private void btnMinimize_Click_1(object sender, EventArgs e)
        {
            WindowState = FormWindowState.Minimized;
        }
    }
}
