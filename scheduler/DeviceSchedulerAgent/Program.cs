using System;
using System.Collections.Generic;
using System.Text;
using DeviceHandler;
using System.Data;
using System.Windows.Forms;

namespace DeviceSchedulerAgent
{
    class Program
    {

        static void Main(string[] args)
        {
            //If no arguments are supplied then exit.
            if (args.Length == 0) 
            {
                MessageBox.Show("Inga argument har angivits!");
                return; 
            }

            try
            {

                //Makesure that we have all data loaded. 
                DataStorage data = LoadData();

                //Get the JobID.
                int jobID = Convert.ToInt32(args[0]);

                DataStorage.JobRow job = data.Job.FindByID(jobID);

                if (job != null)
                {
                    DeviceHandler.Handler handler = new Handler();

                    //Load devices.
                    handler.Load();

                    //Go through each action and execute it.
                    foreach (DataStorage.JobActionsRow action in job.GetJobActionsRows())
                    {
                        Device item = handler.Find(action.DeviceID);

						if (item != null)
						{
							if (action.TurnOn)
								handler.TurnOnWithDelay(item);
							else
								handler.TurnOffWithDelay(item);
						}
						else
						{
							MessageBox.Show("Device could not be found. Please make sure that TelldusService is running and no device scheduled for turn on/off has been removed.", "Telldus scheduler");
						}
                    }

                }
                else
                {
					MessageBox.Show("No scheduled jobs could be found.", "Telldus scheduler");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

        private static DataStorage LoadData()
        {

            string path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            path = System.IO.Path.Combine(path, "Jobs.dat");

            DataStorage data = new DataStorage();

            if (System.IO.File.Exists(path))
            {
                data.ReadXml(path, XmlReadMode.ReadSchema);
            }

            return data;
        }
    }
}
