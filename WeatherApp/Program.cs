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
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
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

            Console.WriteLine("Output begings");
            Console.WriteLine((double)weatherForecast.main.temp - 273.15);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
