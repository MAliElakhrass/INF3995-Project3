using System;
using System.Drawing;
using System.Windows.Forms;
using static CafePcApp.UtilJson;
using static CafePcApp.Utils;

namespace CafePcApp
{
    public partial class FormLogin : Form
    {
        private Service service;

        public FormLogin()
        {
            InitializeComponent();
            service = new Service();
        }

        #region UI_EVENT_HANDLER
        private void button_login_Click(object sender, EventArgs e)
        {
            // Read user's input
            string username = textbox_username.Text;
            string password = textbox_pwd.Text;

            if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
            {
                // Create json
                Login login = new Login(username, password);
                string jsonLogin = createJsonLogin(login);
                ResponseServer response = service.loginAdmin(jsonLogin);
                this.label_status_message_login.Visible = true;
                switch (response.status)
                {
                    case 200:
                        this.label_status_message_login.BackColor = Color.Green;
                        this.label_status_message_login.Text = response.message;
                        this.Hide();
                        FormOptions optionForm = new FormOptions(password);
                        optionForm.Show();
                        break;
                    case 400:
                    case 403:
                        this.label_status_message_login.BackColor = Color.Red;
                        this.label_status_message_login.Text = response.message;
                        break;
                    default:
                        break;
                }
            }
        }
        #endregion
        
    }
}
