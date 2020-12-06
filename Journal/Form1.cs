using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Journal
{
    public partial class Journal : Form
    {
        private int ticks = 0;

        public Journal()
        {
            InitializeComponent();
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {//Changes the active directory to whatever is inside the text box, then scans for the files within.

            FileList.Items.Clear();

            if (!Directory.Exists(textBox1.Text))
                throw new System.ArgumentException("The Directory given doesn't exist");
            else
                Console.WriteLine("Directory Exists and is Valid");

            string[] files = Directory.GetFiles(textBox1.Text);

            foreach (string file in files)
                FileList.Items.Add(file.Substring(textBox1.TextLength + 1));
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Journal_Load(object sender, EventArgs e)
        {
            //Load Direcotry
            if (!Directory.Exists(textBox1.Text))
                throw new System.ArgumentException("The Directory given doesn't exist");
            else
                Console.WriteLine("Directory Exists and is Valid");

            string[] files = Directory.GetFiles(textBox1.Text);
            
            foreach (string file in files)
                FileList.Items.Add(file.Substring(textBox1.TextLength + 1));

            //Update the time on the file name
            richTextBox2.Text = (DateTime.Now.DayOfWeek.ToString() + " " +
                DateTime.Now.ToShortTimeString() + " " +
                DateTime.Now.ToShortDateString()).Replace("/", "_").Replace(":", ".");
        }

        private void FileList_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void richTextBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string dir = textBox1.Text;
            Directory.SetCurrentDirectory(dir);

            StreamWriter write_pointer = new StreamWriter(richTextBox2.Text + ".txt");

            write_pointer.WriteLine(richTextBox1.Text);

            write_pointer.Close();

            button2_Click(sender, e);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            ticks++;

            Console.WriteLine(ticks);

            if (ticks == 5)
            {
                button1_Click(sender, e);
                ticks = 0;
                timer1.Stop();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
        }

        private void FileList_DoubleClick(object sender, EventArgs e)
        {
            string fn = FileList.SelectedItem.ToString();

            string dir = textBox1.Text;
            Directory.SetCurrentDirectory(dir);

            StreamReader read_pointer = new StreamReader(fn);

            richTextBox1.Text = "";
            richTextBox1.Text = read_pointer.ReadToEnd();

            read_pointer.Close();

        }
    }
}
