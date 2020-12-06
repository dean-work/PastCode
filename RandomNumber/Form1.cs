using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RandomNumber
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Random rnd = new Random();

            //Get the size of the number in digits
            int digits = (int)numericUpDown1.Value;

            //Ensure it's not more than 9 digits
            if (digits > 9)
                throw new System.ArgumentException("digit input cannot be above 9!");

            //Loop the process for number of iterations
            for (int i = 0; i < numericUpDown2.Value; i++)
            {
                //Setup
                textBox1.Clear();

                //Output the number
                int generated_number = rnd.Next(0, (int)Math.Abs(Math.Pow(10, digits)));
                textBox1.AppendText($"{generated_number}");

                //Output number to the log 
                ListViewItem tmp = new ListViewItem();
                tmp.Text = generated_number.ToString();
                listView1.Items.Add(tmp);


            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {
           
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            Form2 graph_form = new Form2(listView1);
            graph_form.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var filename = textBox2.Text;
            var fp = FILE
        }
    }
}
