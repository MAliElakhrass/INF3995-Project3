using System;
using System.Windows.Forms;
using static CafePcApp.UtilJson;
using static CafePcApp.Utils;

namespace CafePcApp
{
    public partial class FormOptions : Form
    {
        public Service service = new Service();
        public Logs logs;
        private String password;

        public FormOptions(string password)
        {
            InitializeComponent();

            this.password = password;

            timer1.Interval = 10000;
            timer1.Enabled = true;
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Start();
        }

        private void button_update_logs_Click(object sender, EventArgs e)
        {
            updateLogs();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            updateLogs();
        }

        private void updateLogs()
        {
            // Create json with the last log received
            String jsonDernier = UtilJson.createJsonLastLog(logs);

            Console.WriteLine(jsonDernier);

            // Server's response
            ResponseServer response = service.logsAdmin(jsonDernier);

            switch (response.status)
            {
                case 200:
                    logs = UtilJson.parseJsonLogs(response.message);
                    displayLogs();
                    break;
                case 400:
                case 401:
                    this.textBox_logs.Text = response.message;
                    break;
                default:
                    break;
            }
        }

        private void displayLogs()
        {
            // Display the logs
            string logsDisplay = this.textBox_logs.Text;
            for (int i = 0; i < logs.log.Count; i++)
            {
                logsDisplay += logs.log[i].info + "\r\n";
            }
            this.textBox_logs.Text = logsDisplay;
        }

        private void button_update_stats_Click(object sender, EventArgs e)
        {
            updateStats();
        }

        private void updateStats()
        {
            ResponseServer response = service.getStatistics();

            switch (response.status)
            {
                case 200:
                    Statistiques stats = UtilJson.parseJsonsStatistics(response.message);
                    displayStats(stats);
                    break;
                case 401:
                    this.label_nb_songs_val.Text = response.message;
                    this.label_nb_users_val.Text = response.message;
                    this.label_nb_removed_val.Text = response.message;
                    this.label_temps_val.Text = response.message;
                    break;
                default:
                    break;
            }
        }

        private void displayStats(Statistiques stats)
        {
            this.label_nb_songs_val.Text = stats.chansons.ToString();
            this.label_nb_users_val.Text = stats.utilisateurs.ToString();
            this.label_nb_removed_val.Text = stats.elemines.ToString();
            this.label_temps_val.Text = stats.temps;
        }

        private void button_logout_Click(object sender, EventArgs e)
        {
            ResponseServer response = service.logout();

            switch (response.status)
            {
                case 200:
                    this.Close();
                    FormLogin formLogin = new FormLogin();
                    formLogin.Show();
                    break;
                case 401:
                default:
                    break;
            }
        }

        private void button_change_pwd_Click(object sender, EventArgs e)
        {
            string newPwd = Microsoft.VisualBasic.Interaction.InputBox("", "New Password", "New password");

            ResponseServer response = service.changePassword(UtilJson.createJsonPassword(new Password(password, newPwd)));

            switch (response.status)
            {
                case 200:
                    this.password = newPwd;
                    break;
                case 400:
                case 401:
                    MessageBox.Show(response.message);
                    break;
                default:
                    break;
            }
        }

        private void button_create_sup_account_Click(object sender, EventArgs e)
        {
            string usernameSup = this.textBox_username_sup.Text;
            string passwordSup = this.textBox_pwd_sup.Text;

            ResponseServer response = service.createSupAccount(
                UtilJson.createJsonSupervisorUser(new SupervisorAccount(usernameSup, passwordSup)));

            switch (response.status)
            {
                case 200:
                case 400:
                case 403:
                default:
                    MessageBox.Show(response.message);
                    break;
            }

        }

        private void button_delete_sup_Click(object sender, EventArgs e)
        {
            string userDelete = this.textBox_delete_sup.Text;

            ResponseServer response = service.deleteSupAccount(
                UtilJson.createJsonSupervisorDelete(new SupervisorAccount.Delete(userDelete)));

            switch (response.status)
            {
                case 200:
                case 400:
                case 403:
                default:
                    MessageBox.Show(response.message);
                    break;
            }
        }
    }
}
