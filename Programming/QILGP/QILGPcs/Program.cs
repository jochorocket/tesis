using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.IO;

namespace QILGPcs
{
    class Program
    {
        static void Main(string[] args)
        {   
            double bestFitness;
            //Config config = new Config(@"..\..\..\QILGPConfig.xml");
            Config config = new Config("QILGPConfig.xml");
            DateTime startTime, endTime;
            StreamWriter writer;

            startTime = DateTime.Now;

            bestFitness = Wrapper.RunQILGP();
            endTime = DateTime.Now;

            Console.WriteLine("   Start: " + startTime);
            Console.WriteLine("  Finish: " + endTime + '\n');
            Console.WriteLine("Duration: " + endTime.Subtract(startTime) + '\n');

            Console.WriteLine("QILGP's return = " + bestFitness + '\n');

            writer = new StreamWriter("experimsOut.txt", true);
            writer.WriteLine();
            writer.WriteLine("Duration = " + endTime.Subtract(startTime));
            writer.Close();

            Console.WriteLine("Press 'esc' to exit");
            Console.Beep(880, 100);
            Console.Beep(55,  100);
            Console.Beep(880, 100);
            Console.Beep(55,  100);
            Console.Beep(880, 100);
            while (Console.ReadKey(true).Key != ConsoleKey.Escape);
        }
    }
}
