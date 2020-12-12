using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace CEA_2020.IDE
{
    public partial class CEA : Form
    {
        private string fileName = String.Empty;
        private const string compilerPath = "CEA-2020\\CEA-2020.exe";

        public CEA()
        {
            InitializeComponent();
            saveFileDialog1.Filter = "Текстовые файлы (.txt)|.txt";
            this.KeyPreview = true;
            this.KeyDown += new KeyEventHandler(Form_KeyDown); // создаю ивент чтобы отслеживать гор. клавиши
        }

        void Form_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.S)
            {
                if (fileName != String.Empty)
                {
                    File.WriteAllText(fileName, richTextBox1.Text);
                    sourceCodePath.Text = fileName;
                    this.Text = $"CEA-2020.IDE | {fileName}";
                }
                else сохранитьКакToolStripMenuItem_Click(sender, e);
            }
            if (e.Control && e.KeyCode == Keys.O) открытьToolStripMenuItem_Click(sender, e);
            if (e.Control && e.KeyCode == Keys.N) создатьToolStripMenuItem_Click(sender, e);
            if (e.KeyCode == Keys.F7) запускToolStripMenuItem_Click(sender, e);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // clear
        }

        private void сохранитьКакToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.Cancel) return; // если закрываем

            string file = saveFileDialog1.FileName; // путь к файлу
            fileName = file;
            File.WriteAllText(file, richTextBox1.Text);
        }

        private void открытьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.Cancel) return;

            string file = openFileDialog1.FileName;
            string code = File.ReadAllText(file);
            richTextBox1.Text = code;
            this.Text = $"CEA-2020.IDE | {file}";
            sourceCodePath.Text = file;
            fileName = file; // глобальная переменная файла, где мы сейчас работаем
            MessageBox.Show($"Загружен код из файла {file}");
        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }

        private void запускToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (sourceCodePath.Text == String.Empty)
            {
                DialogResult a = MessageBox.Show("Вы должны изменить настройки проекта для запуска!", "Внимание!");
                settingsForm.Visible = true;
                return;
            }

            // запуск;
            string command = $"/C {compilerPath} -in:{sourceCodePath.Text}";
            if (outFilePath.Text != String.Empty) command += $" -out:{outFilePath.Text}\\CEAasm.asm";
            else command += @" -out:\CEAasm.asm";
            if(LogFilePath.Text != String.Empty) command += $" -log:{LogFilePath.Text}\\log.txt";
            if (lex.Checked) command += " -lex";
            if (id.Checked) command += " -id";
            if (tree.Checked) command += " -tree";
            System.Diagnostics.Process.Start("cmd.exe", command);
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.Cancel) return;
            LogFilePath.Text = folderBrowserDialog1.SelectedPath;
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void настройкиToolStripMenuItem_Click(object sender, EventArgs e)
        {
            settingsForm.Visible = true; // включаю настройки
        }

        private void saveAndCloseSettings_Click(object sender, EventArgs e)
        {
            // save all and exit
            // now only exit, save later
            settingsForm.Visible = false;
        }

        private void создатьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            fileName = String.Empty;
            richTextBox1.Text = String.Empty;
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void changeSourceCodePath_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.Cancel) return;
            sourceCodePath.Text = openFileDialog1.FileName;
        }

        private void changeOutFilePath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.Cancel) return;
            outFilePath.Text = folderBrowserDialog1.SelectedPath;
        }

        private void сохранитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void пепсиToolStripMenuItem_Click(object sender, EventArgs e)
        {
            menuStrip1.BackColor = Color.MediumSlateBlue;
            settingsForm.BackColor = Color.MediumSlateBlue;

            textBox6.BackColor = Color.MediumSlateBlue;
            textBox6.ForeColor = Color.White;
            textBox7.BackColor = Color.MediumSlateBlue;
            textBox7.ForeColor = Color.White;
            textBox9.BackColor = Color.MediumSlateBlue;
            textBox9.ForeColor = Color.White;
            textBox1.BackColor = Color.MediumSlateBlue;
            textBox1.ForeColor = Color.White;
            textBox3.BackColor = Color.MediumSlateBlue;
            textBox3.ForeColor = Color.White;
            textBox5.BackColor = Color.MediumSlateBlue;
            textBox5.ForeColor = Color.White;
            textBox2.BackColor = Color.MediumSlateBlue;
            textBox2.ForeColor = Color.White;

            changeLogFilePath.ForeColor = Color.White;
            changeOutFilePath.ForeColor = Color.White;
            changeSourceCodePath.ForeColor = Color.White;
            saveAndCloseSettings.ForeColor = Color.White;
        }

        private void фантаToolStripMenuItem_Click(object sender, EventArgs e)
        {
            menuStrip1.BackColor = Color.DarkOrange;
            settingsForm.BackColor = Color.DarkOrange;

            textBox6.BackColor = Color.DarkOrange;
            textBox6.ForeColor = Color.Black;
            textBox7.BackColor = Color.DarkOrange;
            textBox7.ForeColor = Color.Black;
            textBox9.BackColor = Color.DarkOrange;
            textBox9.ForeColor = Color.Black;
            textBox1.BackColor = Color.DarkOrange;
            textBox1.ForeColor = Color.Black;
            textBox3.BackColor = Color.DarkOrange;
            textBox3.ForeColor = Color.Black;
            textBox5.BackColor = Color.DarkOrange;
            textBox5.ForeColor = Color.Black;
            textBox2.BackColor = Color.DarkOrange;
            textBox2.ForeColor = Color.Black;

            changeLogFilePath.ForeColor = Color.Black;
            changeOutFilePath.ForeColor = Color.Black;
            changeSourceCodePath.ForeColor = Color.Black;
            saveAndCloseSettings.ForeColor = Color.Black;
        }

        private void светлаяToolStripMenuItem_Click(object sender, EventArgs e)
        {
            menuStrip1.BackColor = Color.White;
            settingsForm.BackColor = Color.White;

            textBox6.BackColor = Color.White;
            textBox6.ForeColor = Color.Black;
            textBox7.BackColor = Color.White;
            textBox7.ForeColor = Color.Black;
            textBox9.BackColor = Color.White;
            textBox9.ForeColor = Color.Black;
            textBox1.BackColor = Color.White;
            textBox1.ForeColor = Color.Black;
            textBox3.BackColor = Color.White;
            textBox3.ForeColor = Color.Black;
            textBox5.BackColor = Color.White;
            textBox5.ForeColor = Color.Black;
            textBox2.BackColor = Color.White;
            textBox2.ForeColor = Color.Black;

            changeLogFilePath.ForeColor = Color.Black;
            changeOutFilePath.ForeColor = Color.Black;
            changeSourceCodePath.ForeColor = Color.Black;
            saveAndCloseSettings.ForeColor = Color.Black;
        }

        private void textBox6_TextChanged_1(object sender, EventArgs e)
        {
            
        }

        private void textBox2_TextChanged_1(object sender, EventArgs e)
        {

        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox9_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void цветШрифтаToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.Cancel) return;
            richTextBox1.ForeColor = colorDialog1.Color;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void lex_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void id_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void tree_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
