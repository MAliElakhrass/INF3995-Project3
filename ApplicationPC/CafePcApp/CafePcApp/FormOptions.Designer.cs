namespace CafePcApp
{
    partial class FormOptions
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripComboBox1 = new System.Windows.Forms.ToolStripComboBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPage_logs = new System.Windows.Forms.TabPage();
            this.textBox_logs = new System.Windows.Forms.TextBox();
            this.button_update_logs = new System.Windows.Forms.Button();
            this.tabPage_statistiques = new System.Windows.Forms.TabPage();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.button_update_stats = new System.Windows.Forms.Button();
            this.label_nb_chansons = new System.Windows.Forms.Label();
            this.label_nb_users = new System.Windows.Forms.Label();
            this.label_nb_removed = new System.Windows.Forms.Label();
            this.label_temps = new System.Windows.Forms.Label();
            this.label_nb_songs_val = new System.Windows.Forms.Label();
            this.label_nb_users_val = new System.Windows.Forms.Label();
            this.label_nb_removed_val = new System.Windows.Forms.Label();
            this.label_temps_val = new System.Windows.Forms.Label();
            this.button_logout = new System.Windows.Forms.Button();
            this.button_change_pwd = new System.Windows.Forms.Button();
            this.tabPage_sup_account = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_username_sup = new System.Windows.Forms.TextBox();
            this.textBox_pwd_sup = new System.Windows.Forms.TextBox();
            this.button_create_sup_account = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_delete_sup = new System.Windows.Forms.TextBox();
            this.button_delete_sup = new System.Windows.Forms.Button();
            this.contextMenuStrip1.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabPage_logs.SuspendLayout();
            this.tabPage_statistiques.SuspendLayout();
            this.tabPage_sup_account.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.toolStripComboBox1});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(182, 53);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(181, 22);
            this.toolStripMenuItem1.Text = "toolStripMenuItem1";
            // 
            // toolStripComboBox1
            // 
            this.toolStripComboBox1.Name = "toolStripComboBox1";
            this.toolStripComboBox1.Size = new System.Drawing.Size(121, 23);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabPage_logs);
            this.tabControl.Controls.Add(this.tabPage_statistiques);
            this.tabControl.Controls.Add(this.tabPage_sup_account);
            this.tabControl.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControl.Location = new System.Drawing.Point(12, 12);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(776, 426);
            this.tabControl.TabIndex = 3;
            // 
            // tabPage_logs
            // 
            this.tabPage_logs.Controls.Add(this.textBox_logs);
            this.tabPage_logs.Controls.Add(this.button_update_logs);
            this.tabPage_logs.Location = new System.Drawing.Point(4, 27);
            this.tabPage_logs.Name = "tabPage_logs";
            this.tabPage_logs.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_logs.Size = new System.Drawing.Size(768, 395);
            this.tabPage_logs.TabIndex = 0;
            this.tabPage_logs.Text = "Logs";
            this.tabPage_logs.UseVisualStyleBackColor = true;
            // 
            // textBox_logs
            // 
            this.textBox_logs.Location = new System.Drawing.Point(6, 51);
            this.textBox_logs.Multiline = true;
            this.textBox_logs.Name = "textBox_logs";
            this.textBox_logs.ReadOnly = true;
            this.textBox_logs.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_logs.Size = new System.Drawing.Size(756, 338);
            this.textBox_logs.TabIndex = 1;
            // 
            // button_update_logs
            // 
            this.button_update_logs.Location = new System.Drawing.Point(315, 6);
            this.button_update_logs.Name = "button_update_logs";
            this.button_update_logs.Size = new System.Drawing.Size(107, 39);
            this.button_update_logs.TabIndex = 0;
            this.button_update_logs.Text = "Update Logs";
            this.button_update_logs.UseVisualStyleBackColor = true;
            this.button_update_logs.Click += new System.EventHandler(this.button_update_logs_Click);
            // 
            // tabPage_statistiques
            // 
            this.tabPage_statistiques.Controls.Add(this.label_temps_val);
            this.tabPage_statistiques.Controls.Add(this.label_nb_removed_val);
            this.tabPage_statistiques.Controls.Add(this.label_nb_users_val);
            this.tabPage_statistiques.Controls.Add(this.label_nb_songs_val);
            this.tabPage_statistiques.Controls.Add(this.label_temps);
            this.tabPage_statistiques.Controls.Add(this.label_nb_removed);
            this.tabPage_statistiques.Controls.Add(this.label_nb_users);
            this.tabPage_statistiques.Controls.Add(this.label_nb_chansons);
            this.tabPage_statistiques.Controls.Add(this.button_update_stats);
            this.tabPage_statistiques.Location = new System.Drawing.Point(4, 27);
            this.tabPage_statistiques.Name = "tabPage_statistiques";
            this.tabPage_statistiques.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_statistiques.Size = new System.Drawing.Size(768, 395);
            this.tabPage_statistiques.TabIndex = 1;
            this.tabPage_statistiques.Text = "Statistics";
            this.tabPage_statistiques.UseVisualStyleBackColor = true;
            // 
            // button_update_stats
            // 
            this.button_update_stats.Location = new System.Drawing.Point(315, 6);
            this.button_update_stats.Name = "button_update_stats";
            this.button_update_stats.Size = new System.Drawing.Size(107, 39);
            this.button_update_stats.TabIndex = 1;
            this.button_update_stats.Text = "Update Stats";
            this.button_update_stats.UseVisualStyleBackColor = true;
            this.button_update_stats.Click += new System.EventHandler(this.button_update_stats_Click);
            // 
            // label_nb_chansons
            // 
            this.label_nb_chansons.AutoSize = true;
            this.label_nb_chansons.Location = new System.Drawing.Point(16, 73);
            this.label_nb_chansons.Name = "label_nb_chansons";
            this.label_nb_chansons.Size = new System.Drawing.Size(134, 18);
            this.label_nb_chansons.TabIndex = 2;
            this.label_nb_chansons.Text = "Number of songs: ";
            // 
            // label_nb_users
            // 
            this.label_nb_users.AutoSize = true;
            this.label_nb_users.Location = new System.Drawing.Point(16, 107);
            this.label_nb_users.Name = "label_nb_users";
            this.label_nb_users.Size = new System.Drawing.Size(130, 18);
            this.label_nb_users.TabIndex = 3;
            this.label_nb_users.Text = "Number of users: ";
            // 
            // label_nb_removed
            // 
            this.label_nb_removed.AutoSize = true;
            this.label_nb_removed.Location = new System.Drawing.Point(16, 142);
            this.label_nb_removed.Name = "label_nb_removed";
            this.label_nb_removed.Size = new System.Drawing.Size(199, 18);
            this.label_nb_removed.TabIndex = 4;
            this.label_nb_removed.Text = "Number of songs removed: ";
            // 
            // label_temps
            // 
            this.label_temps.AutoSize = true;
            this.label_temps.Location = new System.Drawing.Point(16, 177);
            this.label_temps.Name = "label_temps";
            this.label_temps.Size = new System.Drawing.Size(147, 18);
            this.label_temps.TabIndex = 5;
            this.label_temps.Text = "Average song time: ";
            // 
            // label_nb_songs_val
            // 
            this.label_nb_songs_val.AutoSize = true;
            this.label_nb_songs_val.Location = new System.Drawing.Point(225, 73);
            this.label_nb_songs_val.Name = "label_nb_songs_val";
            this.label_nb_songs_val.Size = new System.Drawing.Size(0, 18);
            this.label_nb_songs_val.TabIndex = 6;
            // 
            // label_nb_users_val
            // 
            this.label_nb_users_val.AutoSize = true;
            this.label_nb_users_val.Location = new System.Drawing.Point(225, 107);
            this.label_nb_users_val.Name = "label_nb_users_val";
            this.label_nb_users_val.Size = new System.Drawing.Size(0, 18);
            this.label_nb_users_val.TabIndex = 7;
            // 
            // label_nb_removed_val
            // 
            this.label_nb_removed_val.AutoSize = true;
            this.label_nb_removed_val.Location = new System.Drawing.Point(225, 142);
            this.label_nb_removed_val.Name = "label_nb_removed_val";
            this.label_nb_removed_val.Size = new System.Drawing.Size(0, 18);
            this.label_nb_removed_val.TabIndex = 8;
            // 
            // label_temps_val
            // 
            this.label_temps_val.AutoSize = true;
            this.label_temps_val.Location = new System.Drawing.Point(225, 177);
            this.label_temps_val.Name = "label_temps_val";
            this.label_temps_val.Size = new System.Drawing.Size(0, 18);
            this.label_temps_val.TabIndex = 9;
            // 
            // button_logout
            // 
            this.button_logout.Location = new System.Drawing.Point(713, 12);
            this.button_logout.Name = "button_logout";
            this.button_logout.Size = new System.Drawing.Size(75, 23);
            this.button_logout.TabIndex = 4;
            this.button_logout.Text = "Logout";
            this.button_logout.UseVisualStyleBackColor = true;
            this.button_logout.Click += new System.EventHandler(this.button_logout_Click);
            // 
            // button_change_pwd
            // 
            this.button_change_pwd.Location = new System.Drawing.Point(600, 12);
            this.button_change_pwd.Name = "button_change_pwd";
            this.button_change_pwd.Size = new System.Drawing.Size(107, 23);
            this.button_change_pwd.TabIndex = 5;
            this.button_change_pwd.Text = "Change password";
            this.button_change_pwd.UseVisualStyleBackColor = true;
            this.button_change_pwd.Click += new System.EventHandler(this.button_change_pwd_Click);
            // 
            // tabPage_sup_account
            // 
            this.tabPage_sup_account.Controls.Add(this.button_delete_sup);
            this.tabPage_sup_account.Controls.Add(this.textBox_delete_sup);
            this.tabPage_sup_account.Controls.Add(this.label3);
            this.tabPage_sup_account.Controls.Add(this.button_create_sup_account);
            this.tabPage_sup_account.Controls.Add(this.textBox_pwd_sup);
            this.tabPage_sup_account.Controls.Add(this.textBox_username_sup);
            this.tabPage_sup_account.Controls.Add(this.label2);
            this.tabPage_sup_account.Controls.Add(this.label1);
            this.tabPage_sup_account.Location = new System.Drawing.Point(4, 27);
            this.tabPage_sup_account.Name = "tabPage_sup_account";
            this.tabPage_sup_account.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_sup_account.Size = new System.Drawing.Size(768, 395);
            this.tabPage_sup_account.TabIndex = 2;
            this.tabPage_sup_account.Text = "Supervisor Account";
            this.tabPage_sup_account.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 18);
            this.label1.TabIndex = 0;
            this.label1.Text = "Username:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 18);
            this.label2.TabIndex = 1;
            this.label2.Text = "Password:";
            // 
            // textBox_username_sup
            // 
            this.textBox_username_sup.Location = new System.Drawing.Point(97, 4);
            this.textBox_username_sup.Name = "textBox_username_sup";
            this.textBox_username_sup.Size = new System.Drawing.Size(178, 26);
            this.textBox_username_sup.TabIndex = 2;
            // 
            // textBox_pwd_sup
            // 
            this.textBox_pwd_sup.Location = new System.Drawing.Point(97, 37);
            this.textBox_pwd_sup.Name = "textBox_pwd_sup";
            this.textBox_pwd_sup.Size = new System.Drawing.Size(178, 26);
            this.textBox_pwd_sup.TabIndex = 3;
            // 
            // button_create_sup_account
            // 
            this.button_create_sup_account.Location = new System.Drawing.Point(97, 70);
            this.button_create_sup_account.Name = "button_create_sup_account";
            this.button_create_sup_account.Size = new System.Drawing.Size(178, 32);
            this.button_create_sup_account.TabIndex = 4;
            this.button_create_sup_account.Text = "Create";
            this.button_create_sup_account.UseVisualStyleBackColor = true;
            this.button_create_sup_account.Click += new System.EventHandler(this.button_create_sup_account_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 135);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(84, 18);
            this.label3.TabIndex = 5;
            this.label3.Text = "Username:";
            // 
            // textBox_delete_sup
            // 
            this.textBox_delete_sup.Location = new System.Drawing.Point(97, 132);
            this.textBox_delete_sup.Name = "textBox_delete_sup";
            this.textBox_delete_sup.Size = new System.Drawing.Size(178, 26);
            this.textBox_delete_sup.TabIndex = 6;
            // 
            // button_delete_sup
            // 
            this.button_delete_sup.Location = new System.Drawing.Point(282, 132);
            this.button_delete_sup.Name = "button_delete_sup";
            this.button_delete_sup.Size = new System.Drawing.Size(116, 26);
            this.button_delete_sup.TabIndex = 7;
            this.button_delete_sup.Text = "Delete";
            this.button_delete_sup.UseVisualStyleBackColor = true;
            this.button_delete_sup.Click += new System.EventHandler(this.button_delete_sup_Click);
            // 
            // FormOptions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::CafePcApp.Properties.Resources.yellow_cup;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.button_change_pwd);
            this.Controls.Add(this.button_logout);
            this.Controls.Add(this.tabControl);
            this.Name = "FormOptions";
            this.Text = "Option";
            this.contextMenuStrip1.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.tabPage_logs.ResumeLayout(false);
            this.tabPage_logs.PerformLayout();
            this.tabPage_statistiques.ResumeLayout(false);
            this.tabPage_statistiques.PerformLayout();
            this.tabPage_sup_account.ResumeLayout(false);
            this.tabPage_sup_account.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBox1;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPage_logs;
        private System.Windows.Forms.TabPage tabPage_statistiques;
        private System.Windows.Forms.TextBox textBox_logs;
        private System.Windows.Forms.Button button_update_logs;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button button_update_stats;
        private System.Windows.Forms.Label label_nb_chansons;
        private System.Windows.Forms.Label label_temps_val;
        private System.Windows.Forms.Label label_nb_removed_val;
        private System.Windows.Forms.Label label_nb_users_val;
        private System.Windows.Forms.Label label_nb_songs_val;
        private System.Windows.Forms.Label label_temps;
        private System.Windows.Forms.Label label_nb_removed;
        private System.Windows.Forms.Label label_nb_users;
        private System.Windows.Forms.Button button_logout;
        private System.Windows.Forms.Button button_change_pwd;
        private System.Windows.Forms.TabPage tabPage_sup_account;
        private System.Windows.Forms.Button button_create_sup_account;
        private System.Windows.Forms.TextBox textBox_pwd_sup;
        private System.Windows.Forms.TextBox textBox_username_sup;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_delete_sup;
        private System.Windows.Forms.TextBox textBox_delete_sup;
        private System.Windows.Forms.Label label3;
    }
}