namespace CEA_2020.IDE
{
    partial class CEA
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CEA));
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.проектToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.создатьToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.открытьToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.сохранитьКакToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.сохранитьToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.проектToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.настройкиToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.запускToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.темаToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.пепсиToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.фантаToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.светлаяToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.kToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.цветШрифтаToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.settingsForm = new System.Windows.Forms.Panel();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.saveAndCloseSettings = new System.Windows.Forms.Button();
            this.textBox9 = new System.Windows.Forms.TextBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.changeOutFilePath = new System.Windows.Forms.Button();
            this.outFilePath = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.changeLogFilePath = new System.Windows.Forms.Button();
            this.LogFilePath = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.changeSourceCodePath = new System.Windows.Forms.Button();
            this.sourceCodePath = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.directorySearcher1 = new System.DirectoryServices.DirectorySearcher();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.lex = new System.Windows.Forms.CheckBox();
            this.id = new System.Windows.Forms.CheckBox();
            this.tree = new System.Windows.Forms.CheckBox();
            this.menuStrip1.SuspendLayout();
            this.settingsForm.SuspendLayout();
            this.SuspendLayout();
            // 
            // richTextBox1
            // 
            this.richTextBox1.AutoWordSelection = true;
            this.richTextBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(32)))), ((int)(((byte)(32)))));
            this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBox1.ForeColor = System.Drawing.Color.Gainsboro;
            this.richTextBox1.Location = new System.Drawing.Point(0, 24);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(10, 20, 10, 10);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.richTextBox1.Size = new System.Drawing.Size(720, 537);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "\n";
            this.richTextBox1.TextChanged += new System.EventHandler(this.richTextBox1_TextChanged);
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.menuStrip1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.проектToolStripMenuItem,
            this.проектToolStripMenuItem1,
            this.запускToolStripMenuItem,
            this.темаToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(720, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // проектToolStripMenuItem
            // 
            this.проектToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.создатьToolStripMenuItem,
            this.открытьToolStripMenuItem,
            this.сохранитьКакToolStripMenuItem,
            this.сохранитьToolStripMenuItem});
            this.проектToolStripMenuItem.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.проектToolStripMenuItem.Name = "проектToolStripMenuItem";
            this.проектToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.проектToolStripMenuItem.Text = "Файл";
            // 
            // создатьToolStripMenuItem
            // 
            this.создатьToolStripMenuItem.Name = "создатьToolStripMenuItem";
            this.создатьToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.создатьToolStripMenuItem.Text = "Новый файл (Ctrl+N)";
            this.создатьToolStripMenuItem.Click += new System.EventHandler(this.создатьToolStripMenuItem_Click);
            // 
            // открытьToolStripMenuItem
            // 
            this.открытьToolStripMenuItem.Name = "открытьToolStripMenuItem";
            this.открытьToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.открытьToolStripMenuItem.Text = "Открыть (Ctrl+O)";
            this.открытьToolStripMenuItem.Click += new System.EventHandler(this.открытьToolStripMenuItem_Click);
            // 
            // сохранитьКакToolStripMenuItem
            // 
            this.сохранитьКакToolStripMenuItem.Name = "сохранитьКакToolStripMenuItem";
            this.сохранитьКакToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.сохранитьКакToolStripMenuItem.Text = "Сохранить как";
            this.сохранитьКакToolStripMenuItem.Click += new System.EventHandler(this.сохранитьКакToolStripMenuItem_Click);
            // 
            // сохранитьToolStripMenuItem
            // 
            this.сохранитьToolStripMenuItem.Name = "сохранитьToolStripMenuItem";
            this.сохранитьToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.сохранитьToolStripMenuItem.Text = "Сохранить (Ctrl+C)";
            this.сохранитьToolStripMenuItem.Click += new System.EventHandler(this.сохранитьToolStripMenuItem_Click);
            // 
            // проектToolStripMenuItem1
            // 
            this.проектToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.настройкиToolStripMenuItem});
            this.проектToolStripMenuItem1.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.проектToolStripMenuItem1.ForeColor = System.Drawing.SystemColors.Desktop;
            this.проектToolStripMenuItem1.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.проектToolStripMenuItem1.Name = "проектToolStripMenuItem1";
            this.проектToolStripMenuItem1.Size = new System.Drawing.Size(59, 20);
            this.проектToolStripMenuItem1.Text = "Проект";
            // 
            // настройкиToolStripMenuItem
            // 
            this.настройкиToolStripMenuItem.Image = global::CEA_2020.IDE.Properties.Resources._1494258023_bugladybugspringinsectautumn_84344;
            this.настройкиToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.White;
            this.настройкиToolStripMenuItem.Name = "настройкиToolStripMenuItem";
            this.настройкиToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.настройкиToolStripMenuItem.Text = "Настройки";
            this.настройкиToolStripMenuItem.Click += new System.EventHandler(this.настройкиToolStripMenuItem_Click);
            // 
            // запускToolStripMenuItem
            // 
            this.запускToolStripMenuItem.Name = "запускToolStripMenuItem";
            this.запускToolStripMenuItem.Size = new System.Drawing.Size(80, 20);
            this.запускToolStripMenuItem.Text = "Запуск (F7)";
            this.запускToolStripMenuItem.Click += new System.EventHandler(this.запускToolStripMenuItem_Click);
            // 
            // темаToolStripMenuItem
            // 
            this.темаToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.пепсиToolStripMenuItem,
            this.фантаToolStripMenuItem,
            this.светлаяToolStripMenuItem,
            this.kToolStripMenuItem,
            this.цветШрифтаToolStripMenuItem});
            this.темаToolStripMenuItem.Name = "темаToolStripMenuItem";
            this.темаToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.темаToolStripMenuItem.Text = "Тема";
            // 
            // пепсиToolStripMenuItem
            // 
            this.пепсиToolStripMenuItem.Name = "пепсиToolStripMenuItem";
            this.пепсиToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.пепсиToolStripMenuItem.Text = "Пепси";
            this.пепсиToolStripMenuItem.Click += new System.EventHandler(this.пепсиToolStripMenuItem_Click);
            // 
            // фантаToolStripMenuItem
            // 
            this.фантаToolStripMenuItem.Name = "фантаToolStripMenuItem";
            this.фантаToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.фантаToolStripMenuItem.Text = "Фанта";
            this.фантаToolStripMenuItem.Click += new System.EventHandler(this.фантаToolStripMenuItem_Click);
            // 
            // светлаяToolStripMenuItem
            // 
            this.светлаяToolStripMenuItem.Name = "светлаяToolStripMenuItem";
            this.светлаяToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.светлаяToolStripMenuItem.Text = "Светлая";
            this.светлаяToolStripMenuItem.Click += new System.EventHandler(this.светлаяToolStripMenuItem_Click);
            // 
            // kToolStripMenuItem
            // 
            this.kToolStripMenuItem.Name = "kToolStripMenuItem";
            this.kToolStripMenuItem.Size = new System.Drawing.Size(145, 6);
            // 
            // цветШрифтаToolStripMenuItem
            // 
            this.цветШрифтаToolStripMenuItem.Name = "цветШрифтаToolStripMenuItem";
            this.цветШрифтаToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.цветШрифтаToolStripMenuItem.Text = "Цвет шрифта";
            this.цветШрифтаToolStripMenuItem.Click += new System.EventHandler(this.цветШрифтаToolStripMenuItem_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.saveFileDialog1_FileOk);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // folderBrowserDialog1
            // 
            this.folderBrowserDialog1.HelpRequest += new System.EventHandler(this.folderBrowserDialog1_HelpRequest);
            // 
            // settingsForm
            // 
            this.settingsForm.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.settingsForm.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.settingsForm.Controls.Add(this.tree);
            this.settingsForm.Controls.Add(this.id);
            this.settingsForm.Controls.Add(this.lex);
            this.settingsForm.Controls.Add(this.textBox2);
            this.settingsForm.Controls.Add(this.saveAndCloseSettings);
            this.settingsForm.Controls.Add(this.textBox9);
            this.settingsForm.Controls.Add(this.textBox6);
            this.settingsForm.Controls.Add(this.textBox7);
            this.settingsForm.Controls.Add(this.changeOutFilePath);
            this.settingsForm.Controls.Add(this.outFilePath);
            this.settingsForm.Controls.Add(this.textBox5);
            this.settingsForm.Controls.Add(this.changeLogFilePath);
            this.settingsForm.Controls.Add(this.LogFilePath);
            this.settingsForm.Controls.Add(this.textBox3);
            this.settingsForm.Controls.Add(this.changeSourceCodePath);
            this.settingsForm.Controls.Add(this.sourceCodePath);
            this.settingsForm.Controls.Add(this.textBox1);
            this.settingsForm.Location = new System.Drawing.Point(109, 67);
            this.settingsForm.Name = "settingsForm";
            this.settingsForm.Size = new System.Drawing.Size(477, 289);
            this.settingsForm.TabIndex = 2;
            this.settingsForm.Visible = false;
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox2.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.textBox2.Location = new System.Drawing.Point(44, 46);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.textBox2.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.textBox2.Size = new System.Drawing.Size(400, 13);
            this.textBox2.TabIndex = 21;
            this.textBox2.Text = "https://github.com/zheniacherniavsky/CEA-2020";
            this.textBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged_1);
            // 
            // saveAndCloseSettings
            // 
            this.saveAndCloseSettings.FlatAppearance.BorderSize = 0;
            this.saveAndCloseSettings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.saveAndCloseSettings.ForeColor = System.Drawing.SystemColors.Info;
            this.saveAndCloseSettings.Location = new System.Drawing.Point(317, 247);
            this.saveAndCloseSettings.Name = "saveAndCloseSettings";
            this.saveAndCloseSettings.Size = new System.Drawing.Size(146, 20);
            this.saveAndCloseSettings.TabIndex = 20;
            this.saveAndCloseSettings.Text = "Сохранить и закрыть";
            this.saveAndCloseSettings.UseVisualStyleBackColor = true;
            this.saveAndCloseSettings.Click += new System.EventHandler(this.saveAndCloseSettings_Click);
            // 
            // textBox9
            // 
            this.textBox9.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox9.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox9.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox9.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox9.Location = new System.Drawing.Point(16, 79);
            this.textBox9.Name = "textBox9";
            this.textBox9.ReadOnly = true;
            this.textBox9.Size = new System.Drawing.Size(200, 13);
            this.textBox9.TabIndex = 19;
            this.textBox9.Text = "Настройки аргументов";
            this.textBox9.TextChanged += new System.EventHandler(this.textBox9_TextChanged);
            // 
            // textBox6
            // 
            this.textBox6.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox6.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox6.Location = new System.Drawing.Point(15, 8);
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(100, 13);
            this.textBox6.TabIndex = 18;
            this.textBox6.Text = "Компилятор";
            this.textBox6.TextChanged += new System.EventHandler(this.textBox6_TextChanged_1);
            // 
            // textBox7
            // 
            this.textBox7.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox7.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox7.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox7.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.textBox7.Location = new System.Drawing.Point(44, 27);
            this.textBox7.Name = "textBox7";
            this.textBox7.ReadOnly = true;
            this.textBox7.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.textBox7.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.textBox7.Size = new System.Drawing.Size(400, 13);
            this.textBox7.TabIndex = 12;
            this.textBox7.Text = "Используется компилятор CEA-2020";
            this.textBox7.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBox7.TextChanged += new System.EventHandler(this.textBox7_TextChanged);
            // 
            // changeOutFilePath
            // 
            this.changeOutFilePath.FlatAppearance.BorderSize = 0;
            this.changeOutFilePath.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.changeOutFilePath.ForeColor = System.Drawing.SystemColors.Info;
            this.changeOutFilePath.Location = new System.Drawing.Point(389, 211);
            this.changeOutFilePath.Name = "changeOutFilePath";
            this.changeOutFilePath.Size = new System.Drawing.Size(75, 20);
            this.changeOutFilePath.TabIndex = 11;
            this.changeOutFilePath.Text = "Изменить";
            this.changeOutFilePath.UseVisualStyleBackColor = true;
            this.changeOutFilePath.Click += new System.EventHandler(this.changeOutFilePath_Click);
            // 
            // outFilePath
            // 
            this.outFilePath.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.outFilePath.Location = new System.Drawing.Point(15, 211);
            this.outFilePath.Name = "outFilePath";
            this.outFilePath.ReadOnly = true;
            this.outFilePath.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.outFilePath.Size = new System.Drawing.Size(367, 20);
            this.outFilePath.TabIndex = 10;
            // 
            // textBox5
            // 
            this.textBox5.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox5.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox5.Location = new System.Drawing.Point(339, 191);
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.textBox5.Size = new System.Drawing.Size(125, 13);
            this.textBox5.TabIndex = 9;
            this.textBox5.Text = "Путь к файлу отладки";
            this.textBox5.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // changeLogFilePath
            // 
            this.changeLogFilePath.FlatAppearance.BorderSize = 0;
            this.changeLogFilePath.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.changeLogFilePath.ForeColor = System.Drawing.SystemColors.Info;
            this.changeLogFilePath.Location = new System.Drawing.Point(389, 161);
            this.changeLogFilePath.Name = "changeLogFilePath";
            this.changeLogFilePath.Size = new System.Drawing.Size(75, 20);
            this.changeLogFilePath.TabIndex = 8;
            this.changeLogFilePath.Text = "Изменить";
            this.changeLogFilePath.UseVisualStyleBackColor = true;
            this.changeLogFilePath.Click += new System.EventHandler(this.button1_Click);
            // 
            // LogFilePath
            // 
            this.LogFilePath.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LogFilePath.Location = new System.Drawing.Point(15, 161);
            this.LogFilePath.Name = "LogFilePath";
            this.LogFilePath.ReadOnly = true;
            this.LogFilePath.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.LogFilePath.Size = new System.Drawing.Size(367, 20);
            this.LogFilePath.TabIndex = 7;
            this.LogFilePath.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox3.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox3.Location = new System.Drawing.Point(339, 141);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.textBox3.Size = new System.Drawing.Size(125, 13);
            this.textBox3.TabIndex = 3;
            this.textBox3.Text = "Путь к файлу протокола";
            this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // changeSourceCodePath
            // 
            this.changeSourceCodePath.FlatAppearance.BorderSize = 0;
            this.changeSourceCodePath.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.changeSourceCodePath.ForeColor = System.Drawing.SystemColors.Info;
            this.changeSourceCodePath.Location = new System.Drawing.Point(389, 112);
            this.changeSourceCodePath.Name = "changeSourceCodePath";
            this.changeSourceCodePath.Size = new System.Drawing.Size(75, 20);
            this.changeSourceCodePath.TabIndex = 2;
            this.changeSourceCodePath.Text = "Изменить";
            this.changeSourceCodePath.UseVisualStyleBackColor = true;
            this.changeSourceCodePath.Click += new System.EventHandler(this.changeSourceCodePath_Click);
            // 
            // sourceCodePath
            // 
            this.sourceCodePath.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.sourceCodePath.Location = new System.Drawing.Point(15, 112);
            this.sourceCodePath.Name = "sourceCodePath";
            this.sourceCodePath.ReadOnly = true;
            this.sourceCodePath.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.sourceCodePath.Size = new System.Drawing.Size(367, 20);
            this.sourceCodePath.TabIndex = 1;
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.Color.MediumSlateBlue;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox1.ForeColor = System.Drawing.SystemColors.Info;
            this.textBox1.Location = new System.Drawing.Point(339, 93);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.textBox1.Size = new System.Drawing.Size(125, 13);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "Путь к исходному коду";
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // directorySearcher1
            // 
            this.directorySearcher1.ClientTimeout = System.TimeSpan.Parse("-00:00:01");
            this.directorySearcher1.ServerPageTimeLimit = System.TimeSpan.Parse("-00:00:01");
            this.directorySearcher1.ServerTimeLimit = System.TimeSpan.Parse("-00:00:01");
            // 
            // lex
            // 
            this.lex.AutoSize = true;
            this.lex.Location = new System.Drawing.Point(16, 249);
            this.lex.Name = "lex";
            this.lex.Size = new System.Drawing.Size(39, 17);
            this.lex.TabIndex = 22;
            this.lex.Text = "lex";
            this.lex.UseVisualStyleBackColor = true;
            this.lex.CheckedChanged += new System.EventHandler(this.lex_CheckedChanged);
            // 
            // id
            // 
            this.id.AutoSize = true;
            this.id.Location = new System.Drawing.Point(61, 249);
            this.id.Name = "id";
            this.id.Size = new System.Drawing.Size(34, 17);
            this.id.TabIndex = 23;
            this.id.Text = "id";
            this.id.UseVisualStyleBackColor = true;
            this.id.CheckedChanged += new System.EventHandler(this.id_CheckedChanged);
            // 
            // tree
            // 
            this.tree.AutoSize = true;
            this.tree.Location = new System.Drawing.Point(101, 249);
            this.tree.Name = "tree";
            this.tree.Size = new System.Drawing.Size(44, 17);
            this.tree.TabIndex = 24;
            this.tree.Text = "tree";
            this.tree.UseVisualStyleBackColor = true;
            this.tree.CheckedChanged += new System.EventHandler(this.tree_CheckedChanged);
            // 
            // CEA
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.LightSteelBlue;
            this.ClientSize = new System.Drawing.Size(720, 561);
            this.Controls.Add(this.settingsForm);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "CEA";
            this.Opacity = 0.97D;
            this.Text = "CEA-2020.IDE";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.settingsForm.ResumeLayout(false);
            this.settingsForm.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem проектToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem создатьToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem открытьToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem сохранитьКакToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem проектToolStripMenuItem1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.ToolStripMenuItem запускToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem настройкиToolStripMenuItem;
        private System.Windows.Forms.Panel settingsForm;
        private System.Windows.Forms.Button changeSourceCodePath;
        private System.Windows.Forms.TextBox sourceCodePath;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button changeLogFilePath;
        private System.Windows.Forms.TextBox LogFilePath;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.Button changeOutFilePath;
        private System.Windows.Forms.TextBox outFilePath;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox9;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Button saveAndCloseSettings;
        private System.DirectoryServices.DirectorySearcher directorySearcher1;
        private System.Windows.Forms.ToolStripMenuItem сохранитьToolStripMenuItem;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.ToolStripMenuItem темаToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem пепсиToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem фантаToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem светлаяToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator kToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem цветШрифтаToolStripMenuItem;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.CheckBox tree;
        private System.Windows.Forms.CheckBox id;
        private System.Windows.Forms.CheckBox lex;
    }
}

