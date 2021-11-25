using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace Interfaz
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://thingspeak.com/channels/1546302");
        }

        private void button_obtener_peso_Click(object sender, EventArgs e)
        {
            
        }

        private void button_obtener_peso_MouseCaptureChanged(object sender, EventArgs e)
        {

             
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            CheckForIllegalCrossThreadCalls = false;
            
            button_open.Enabled = true;
            button_obtener_etiqueta.Enabled = false;
            button_obtener_peso.Enabled = false;
            button_close.Enabled = false;
            button_agregar.Enabled = false;
            button_mirar.Enabled = true;
            button_ayuda.Enabled = true;
            button_salir.Enabled = true;
            comboBox_baud.Text = "9600";

            string[] portList = SerialPort.GetPortNames();
            comboBox_port.Items.AddRange(portList);
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen) { serialPort1.Close(); }
        }

        private void button_open_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = comboBox_port.Text;
                serialPort1.BaudRate = Convert.ToInt32(comboBox_baud.Text);
                serialPort1.Open();
                
                button_open.Enabled = false;
                button_close.Enabled = true;
                button_obtener_peso.Enabled = true;
                button_agregar.Enabled = true;
                comboBox_port.Enabled = false;
                comboBox_baud.Enabled = false;
            }

            catch(Exception error) { MessageBox.Show(error.Message); }
        }

        private void button_close_Click(object sender, EventArgs e)
        {
            try
            {
                
                serialPort1.Close();

                button_open.Enabled = true;
                button_close.Enabled = false;
                button_obtener_peso.Enabled = false;
                button_agregar.Enabled = false;
                comboBox_port.Enabled = true;
                comboBox_baud.Enabled = true;
            }
            catch (Exception error) { MessageBox.Show(error.Message); }
        }

        private void button_salir_Click(object sender, EventArgs e)
        {
            MessageBox.Show("1.Ilona Giovanna Pava González.\n" +
                "2.Santiago Pacheco Sánchez.\n" +
                "3.Laura Daniela Cabarcas Tovar.\n");
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            
            while(serialPort1.IsOpen && serialPort1.BytesToRead > 0)
            {
                int i = 0;
                string serialData = serialPort1.ReadLine();
                bool result = int.TryParse(serialData, out i);
                
                if(result == false){
                    if (serialData.Substring(0, 2) == "Az") { pictureBox_producto.Image = Properties.Resources.azucar; }
                    if (serialData.Substring(0, 2) == "Ar") { pictureBox_producto.Image = Properties.Resources.arroz; }
                    if (serialData.Substring(0, 2) == "Ha") { pictureBox_producto.Image = Properties.Resources.harina; }
                    textBox_rfid.Text = serialData;

                  
                }
                
                if(result == true) {
                    int valor = Convert.ToInt32(serialData);
                    if (valor >= circularProgressBar1.Minimum && valor <= circularProgressBar1.Maximum)
                    {
                        circularProgressBar1.Text = serialData;
                        circularProgressBar1.Value = valor;
                    }
                }
                
                
            }

                }

        private void circularProgressBar1_Click(object sender, EventArgs e)
        {

        }

        private void button_obtener_peso_Click_1(object sender, EventArgs e)
        {   
            try {
                serialPort1.WriteLine("#On");
                button_obtener_etiqueta.Enabled = true;
                
            }
            catch (Exception error) { MessageBox.Show(error.Message); }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine("#On1");
                
            }
            catch (Exception error) { MessageBox.Show(error.Message); }
        }

        private void button_agregar_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine("#On2");
                button_obtener_etiqueta.Enabled = false;

            }
            catch (Exception error) { MessageBox.Show(error.Message); }
        }

        private void button_ayuda_Click(object sender, EventArgs e)
        {
            MessageBox.Show("1.El arduino debe estar conectado.\n" +
                "2.Seleccionar el puerto COM donde este la direccion del Arduino y click en abrir.\n" +
                "3.Esperar 5 segundos paa que el Arduino se conecte a la red.\n" + 
                "4.Ubicar en la bascula el objeto que se desea pesar.\n" +
                "5.Click en obtener peso y etiqueta para identificar el objeto.\n" + 
                "6.La opcion de agregar al inventario envia a un servidor los datos del objeto pesado.\n" + 
                "7.El servidor puede recibir datos cada 15 segundos.\n" + 
                "8.Mirar el inventario redirecciona a la web donde se alojan los datos.");
        }

       
    }
        }
    

