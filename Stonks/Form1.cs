using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Web;
using System.Text.Json;
using System.Web.Helpers;



namespace Stonks
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string APIpath = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol="
                    + textBox2.Text
                    + "&interval=5min&apikey=NZ10UCTX41ZQPWTF";

            //Create an iterative return iterator for the http request
            WebRequest requestObjectGetter = WebRequest.Create(APIpath);

            //What method to use in this request
            requestObjectGetter.Method = "GET";

            //Zero off the response
            HttpWebResponse responseObject = null;

            //Iterate through the responses and concatenate into a string
            string strResult = null;
            responseObject = (HttpWebResponse)requestObjectGetter.GetResponse();

            using (Stream stream = responseObject.GetResponseStream())
            {
                StreamReader sr = new StreamReader(stream);
                strResult = sr.ReadToEnd(); //Reads all to one string
                sr.Close();
            }

            //Write to URL textbox
            textBox1.Text = APIpath;

            //Interpret and save JSON data to an object
            dynamic StockData = Json.Decode(strResult);


            //Read through and give stock name
            label3.Text = StockData["Meta Data"]["2. Symbol"];

            //Data Handling Section *********************************************************************************
            //Note that while the read in dynamic object is of type, DynamicJsonObject, the dictionaries within it are classified as Objects. (System.Object)

            chart1.Series.Clear();
            chart1.Series.Add("Open");
            chart1.Series.Add("Close");

            //Loop over every element in the Dictionary

            int tmp = 0;
            int loop_limit_iterator = 0; 
            int loop_limit = (int)numericUpDown1.Maximum;

            if (radioButton1.Checked)
                loop_limit = (int)numericUpDown1.Value;

            foreach (var entry in StockData["Time Series (5min)"])
            {
                if (loop_limit_iterator++ >= loop_limit)
                    break;

                //How the fuck do you dereference the contents of a DynamicJsonObject  
                chart1.Series["Open"].Points.AddXY(tmp, Convert.ToDouble(entry.Value["1. open"]));
                chart1.Series["Close"].Points.AddXY(tmp--, Convert.ToDouble(entry.Value["4. close"]));
            }

            //Format the Chart
            chart1.Series["Open"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.Series["Close"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

            //Min Y value
            double miny = chart1.Series["Open"].Points.FindMaxByValue().YValues[0];

            chart1.ChartAreas[0].AxisY.Minimum = miny*0.98;
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
                button1_Click(sender, e);
        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                button1_Click(sender, e);
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void radioButton1_Click(object sender, EventArgs e)
        {
            radioButton1.Checked = !radioButton1.Checked;
        }
    }
}
