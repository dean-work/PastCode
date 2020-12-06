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
    public partial class Form2 : Form
    {
        public Form2(ListView data)
        {
            InitializeComponent();

            //Get enum.
            var data_enum = data.Items.GetEnumerator();

            //Plot Chart
            for (int i = 0; i < data.Items.Count; i++) {
                data_enum.MoveNext();
               
                Console.Write(data_enum.Current.ToString());

                //chart1.Series["number"].Points.Add();
               // chart1.Show();
            }
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }
    }
}
