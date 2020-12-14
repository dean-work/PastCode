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

namespace WeatherApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string PathApi = "http://api.openweathermap.org/data/2.5/weather?q=Melbourne,AU&appid=54a42103dcc76331baeac1ff7656fe1b";
            WebRequest requestObjGet = WebRequest.Create(PathApi);
            requestObjGet.Method = "GET";
            HttpWebResponse responseObjeGet = null;
            responseObjeGet = (HttpWebResponse)requestObjGet.GetResponse();

            string strresulttest = null;
            using (Stream stream = responseObjeGet.GetResponseStream())
            {
                StreamReader sr = new StreamReader(stream);
                strresulttest = sr.ReadToEnd(); //Reads all to one string
                sr.Close();
            }

            dynamic weatherForecast = Json.Decode(strresulttest);

            //Change Stuff on the Form
            textBox1.Text = (string)weatherForecast.name;
            textBox2.Text = ((double)weatherForecast.main.temp - 273.15).ToString();
            textBox2.Text += " Celcius";
            textBox3.Text = ((double)weatherForecast.wind.speed).ToString();
            textBox3.Text += " Kmph";

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            string PathApi = "http://api.openweathermap.org/data/2.5/weather?q=Sydney,AU&appid=54a42103dcc76331baeac1ff7656fe1b";
            WebRequest requestObjGet = WebRequest.Create(PathApi);
            requestObjGet.Method = "GET";
            HttpWebResponse responseObjeGet = null;
            responseObjeGet = (HttpWebResponse)requestObjGet.GetResponse();

            string strresulttest = null;
            using (Stream stream = responseObjeGet.GetResponseStream())
            {
                StreamReader sr = new StreamReader(stream);
                strresulttest = sr.ReadToEnd(); //Reads all to one string
                sr.Close();
            }

            dynamic weatherForecast = Json.Decode(strresulttest);

            //Change Stuff on the Form
            textBox1.Text = (string)weatherForecast.name;
            textBox2.Text = ((double)weatherForecast.main.temp - 273.15).ToString();
            textBox2.Text += " Celcius";
            textBox3.Text = ((double)weatherForecast.wind.speed).ToString();
            textBox3.Text += " Kmph";

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
