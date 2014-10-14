using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.IO;

namespace QILGPcs
{
    class Output
    {
        #region Attributes

        int nCycles;
        int nExperims;
        StreamReader[] sReader;

        #endregion

        #region Methods

        public Output(int nCycles, int nExperims)
        {
            this.nCycles = nCycles;
            this.nExperims = nExperims;
            sReader = new StreamReader[nExperims];
            for (int e = 0; e < nExperims; e++)
            {
                sReader[e] = File.OpenText("..\\..\\..\\..\\Outputs\\expout" +
                                            e.ToString() + ".csv");
            }
        }

        public void WriteOutputCSV()
        {
            StreamWriter sWriter;
            uint[] tmpCycles = new uint[nExperims];
            double fit;
            string[] tmpStrs; //cycle, trainig error, validating error
            char[] separator = {';'};
            List<double> fitsList;
            List<uint>[] cycles = new List<uint>[nExperims];
            List<double>[] data = new List<double>[nExperims];
            int nStreamEnds = 0;
            int nColsEnds = 0;
            int nDataColumnEnds = 0;
            bool[] streamsEnded = new bool[nExperims];
            bool[] dataColumnEnded = new bool[nExperims];
            bool[] writeCycles = new bool[nCycles];
            uint cycleIdx = 1;
            double[] tmpFits = new double[nExperims];
            int[] arrayIdxs = new int[nExperims];

            for (int i = 0; i < data.Length; i++)
            {
                cycles[i] = new List<uint>();
                data[i]  = new List<double>();
            }

            sWriter = File.CreateText("..\\..\\..\\..\\Outputs\\QLEPout.csv");

            // Read files into memory
            for (int e = 0; e < nExperims; e++)
            {
                while (!sReader[e].EndOfStream)
                {
                    tmpStrs = sReader[e].ReadLine().Split(separator);
                    cycles[e].Add(Convert.ToUInt32(tmpStrs[0]));
                    data[e].Add(Convert.ToDouble(tmpStrs[1], CultureInfo.InvariantCulture));
                    writeCycles[Convert.ToUInt32(tmpStrs[0])] = true; // Get a effective cycle no.
                }
            }
            foreach (StreamReader sR in sReader)
            {
                sR.Close();
            }
            
            sWriter.Write("0;");
            for (int e = 0; e < nExperims; e++)
            {
                tmpFits[e] = data[e][0];
                arrayIdxs[e] = 1;
                sWriter.Write(tmpFits[e].ToString(CultureInfo.InvariantCulture) + ';'); 
            }
            sWriter.WriteLine();

            while (nColsEnds < nExperims)
            {
                while (cycleIdx < nCycles && !writeCycles[cycleIdx])
                {
                    cycleIdx++;
                }
                for (int e = 0; e < nExperims; e++)
                {
                    for (arrayIdxs[e] = 1; arrayIdxs[e] < cycles[e].Count; arrayIdxs[e]++)
                    {
                        if (!dataColumnEnded[e])
                        {
                            if (cycles[e][arrayIdxs[e]] == cycleIdx)
                            {
                                tmpFits[e] = data[e][arrayIdxs[e]];
                                if (arrayIdxs[e] == cycles[e].Count - 1)
                                {
                                    dataColumnEnded[e] = true;
                                    nColsEnds++;
                                }
                                break;
                            }
                            else if (cycles[e][arrayIdxs[e]] > cycleIdx)
                            {
                                arrayIdxs[e] = 1;
                                break;
                            }
                        }
                    }
                }
                sWriter.Write(cycleIdx.ToString() + ';');
                for (int e = 0; e < nExperims; e++)
                {
                    sWriter.Write(tmpFits[e].ToString(CultureInfo.InvariantCulture) + ';');
                }
                sWriter.WriteLine();
            }

            #region old
            //while (nStreamEnds < nExperims)
            //{
            //    for (int e = 0; e < nExperims; e++)
            //    {
            //        if (!sReader[e].EndOfStream)
            //        {
            //            tmpStrs = sReader[e].ReadLine().Split(separator); //STOPPED HERE!!!
            //            writeCycles[Convert.ToUInt32(tmpStrs[0])] = true;
            //            fit = Convert.ToDouble(tmpStrs[1], CultureInfo.InvariantCulture);
            //            fitsList = data[e];
            //            for (int i = 0; i < Convert.ToUInt32(tmpStrs[0]) - fitsList.Count; i++)
            //            {
            //                fitsList.Add(fitsList[fitsList.Count - 1]);
            //            }
            //            fitsList.Add(fit);
            //        }
            //        else
            //        {
            //            if (!streamsEnded[e])
            //            {
            //                nStreamEnds++;
            //                streamsEnded[e] = true;
            //            }
            //        }
            //    }
            //}


            //// Write to output file
            //for (int c = 0; nDataColumnEnds < nExperims; c++)
            //{
            //    while (cycleIdx < nCycles && !writeCycles[cycleIdx])
            //    {
            //        cycleIdx++;
            //    }

            //    sWriter.Write(cycleIdx.ToString() + ';');
                
            //    for (int e = 0; e < nExperims; e++)
            //    {
            //        if (c < data[e].Count)
            //        {
            //            fit = data[e][c];
            //        }
            //        else
            //        {
            //            fit = data[e][data[e].Count - 1];
            //            if (!dataColumnEnded[e])
            //            {
            //                nDataColumnEnds++;
            //                dataColumnEnded[e] = true;
            //            }
            //        }
            //        sWriter.Write(fit.ToString(CultureInfo.InvariantCulture) + ';');
            //    }
            //    sWriter.WriteLine();
            //    if (cycleIdx < nCycles)
            //    {
            //        cycleIdx++;
            //    }
            //}
            #endregion

            sWriter.Close();
        }

        private uint Max(ref uint[] vector)
        {
            uint max = 0;

            foreach (uint ui in vector)
            {
                if (ui > max)
                {
                    max = ui;
                }
            }

            return max;
        }

        #endregion
    }
}
