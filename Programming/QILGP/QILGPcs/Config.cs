using System;
using System.IO;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.Xml;


namespace QILGPcs
{
    class Config
    {
        #region Attributes

        int nCycles;
        int nExperims;
        byte nFuncs;
        XmlDocument xmlDoc;

        #endregion

        #region Methods
        public Config(string fileName)
        {
            xmlDoc = new XmlDocument();
            xmlDoc.Load(fileName);
            Setup();
        }

        private void Setup()
        {
            LoadQIndivSetup();
            LoadIndivSetup();
            LoadInputCases();
            LoadEvolutionSetup();

            //Wrapper.CallBack callBack = CallBackMethod;
            //Wrapper.SetCallBack(callBack);

        }

        //public void CallBackMethod(double[] a)
        //{
        //    for (int i = 0; i < 1; i++)
        //    {
        //        Console.WriteLine(a[0]);
        //    }
        //}

        private void LoadQIndivSetup()
        {            
            int qIndivLen;
            double qNopProb;
            bool typedTerms;
            bool effObserving;
            byte mCodeDim = 0;
            byte termsDim = 0;
            string termType;
            byte[] termsCardins;
            byte[] termIdxs; //termTypeIdxs[funcTk] -> termQTkIdx
            byte[] destReg0;
            byte[,] mCodesLens;
            byte[, ,] mCodeTable;

            // Auxiliary vars
            
            char[] separator = { ' ' };
            string mCodeStr;
            string[] mCodeByteStrs;
            XmlNode node;
            XmlNodeList functionNodes;
            XmlNodeList mCodeNodes;

            node = xmlDoc.SelectSingleNode("descendant::qIndividual");
            qIndivLen = Convert.ToInt16(node.Attributes.GetNamedItem("length").Value);
            qNopProb = Convert.ToDouble(node.Attributes.GetNamedItem("qNopProb").Value, CultureInfo.InvariantCulture);
            typedTerms = Convert.ToBoolean(node.Attributes.GetNamedItem("typedTerms").Value);
            effObserving = Convert.ToBoolean(node.Attributes.GetNamedItem("effObserving").Value);
            functionNodes = xmlDoc.SelectNodes("descendant::function");
            nFuncs = (byte)functionNodes.Count;
            termsCardins = new byte[nFuncs];
            termIdxs = new byte[nFuncs];
            destReg0 = new byte[nFuncs];
            for (int i = 0; i < nFuncs; i++)
            {
                termType = functionNodes[i].Attributes.GetNamedItem("terminalType").Value;
                switch (termType)
                {
                    case "memory": 
                        termIdxs[i] = 1;
                        destReg0[i] = 1;
                        break;
                    case "register":
                        termIdxs[i] = 2;
                        destReg0[i] = (byte)(Convert.ToBoolean(functionNodes[i].
                                Attributes.GetNamedItem("destReg0").Value) ? 1 : 0);
                        break;
                    default:
                        termIdxs[i] = 0;
                        destReg0[i] = (byte)(Convert.ToBoolean(functionNodes[i].
                                Attributes.GetNamedItem("destReg0").Value) ? 1 : 0);
                        break;
                }

                mCodeNodes = functionNodes[i].SelectNodes("mCode");
                termsCardins[i] = (byte)mCodeNodes.Count;
                if (termsCardins[i] > termsDim)
                    termsDim = termsCardins[i];
            }
            mCodesLens = new byte[nFuncs, termsDim];
            for (int i = 0; i < nFuncs; i++)
            {
                mCodeNodes = functionNodes[i].SelectNodes("mCode");
                for (int j = 0; j < mCodeNodes.Count; j++)
                {
                    mCodeStr = mCodeNodes[j].Attributes.GetNamedItem("value").Value;
                    mCodeByteStrs = mCodeStr.Split(separator);
                    mCodesLens[i, j] = (byte)mCodeByteStrs.Length;
                    if (mCodesLens[i, j] > mCodeDim)
                        mCodeDim = mCodesLens[i, j];
                }
            }
            mCodeTable = new byte[nFuncs, termsDim, mCodeDim];
            for (int i = 0; i < nFuncs; i++)
            {
                mCodeNodes = functionNodes[i].SelectNodes("mCode");
                for (int j = 0; j < mCodeNodes.Count; j++)
                {
                    mCodeStr = mCodeNodes[j].Attributes.GetNamedItem("value").Value;
                    mCodeByteStrs = mCodeStr.Split(separator);
                    for (int k = 0; k < mCodesLens[i, j]; k++)
                    {
                        mCodeTable[i, j, k] = Byte.Parse(mCodeByteStrs[k], NumberStyles.HexNumber);
                    }
                }
            }
            //if (!typedTerms) termIdxs = null;
            Wrapper.SetQIndiv(qIndivLen,
                              qNopProb,
                              nFuncs,
                              termsDim,
                              termsCardins,
                              termIdxs,
                              destReg0,
                              (byte)(typedTerms ? 1 : 0),
                              (byte)(effObserving ? 1 : 0),
                              mCodesLens,
                              mCodeDim,
                              mCodeTable);
        }

        private void LoadIndivSetup()
        {
            byte[] header;
            byte[] footer;
            
            char[] separator = {'\t', ' '};
            string mCodeStr;
            string[] mCodeByteStrs;
            XmlNode node;

            node = xmlDoc.SelectSingleNode("descendant::individual/header");
            mCodeStr = node.Attributes.GetNamedItem("mCode").Value;
            mCodeByteStrs = mCodeStr.Split(separator);
            header = new byte[mCodeByteStrs.Length];
            for (int i = 0; i < header.Length; i++)
            {
                header[i] = Byte.Parse(mCodeByteStrs[i], NumberStyles.HexNumber);
            }

            node = xmlDoc.SelectSingleNode("descendant::individual/footer");
            mCodeStr = node.Attributes.GetNamedItem("mCode").Value;
            mCodeByteStrs = mCodeStr.Split(separator);
            footer = new byte[mCodeByteStrs.Length];
            for (int i = 0; i < footer.Length; i++)
            {
                footer[i] = Byte.Parse(mCodeByteStrs[i], NumberStyles.HexNumber);
            }

            Wrapper.SetIndiv(header, 
                             (byte)header.Length, 
                             footer, 
                             (byte)footer.Length);
        }

        private void LoadInputCases()
        {
            bool classifProb;
            int dataRowDim;
            double[,] trainingData;
            double[,] validatingData;
            double[,] testingData;
            string fileName;
            XmlNode node;
            char[] separator = {'\t', ' ', '\n'};
            string[] constsStrs;
            double[] consts;

            node = xmlDoc.SelectSingleNode("descendant::inputData");
            classifProb = Convert.ToBoolean(node.Attributes.GetNamedItem("classifProblem").Value);
            fileName = node.Attributes.GetNamedItem("trainingFile").Value;
            trainingData = ReadCasesFile(fileName, out dataRowDim);

            fileName = node.Attributes.GetNamedItem("validatingFile").Value;
            if (fileName == "")
            {
                validatingData = null;
            }
            else
            {
                validatingData = ReadCasesFile(fileName, out dataRowDim);
            }

            fileName = node.Attributes.GetNamedItem("testingFile").Value;
            if (fileName == "")
            {
                testingData = null;
            }
            else
            {
                testingData = ReadCasesFile(fileName, out dataRowDim);
            }

            constsStrs = node.Attributes.GetNamedItem("constants").Value.Split(separator);
            consts = new double[constsStrs.Length];
            for (int i = 0; i < consts.Length; i++)
            {
                consts[i] = Convert.ToDouble(constsStrs[i], CultureInfo.InvariantCulture);
            }

            Wrapper.SetInputCases((byte)(classifProb ? 1 : 0), 
                                  trainingData, validatingData, testingData,
                                  trainingData.GetLength(0),
                                  (validatingData != null) ? validatingData.GetLength(0) : 0,
                                  (testingData != null) ? testingData.GetLength(0) : 0,
                                  dataRowDim, consts, consts.Length);
        }

        private void LoadEvolutionSetup()
        {
            int popSize, nDemes, nCycles, nGenerations, generationsFreq, nExperims;
            double lGateStep;
            XmlNode node;

            node = xmlDoc.SelectSingleNode("descendant::settings/lGateStep");
            lGateStep = Convert.ToDouble(node.Attributes.GetNamedItem("value").Value, 
                                        CultureInfo.InvariantCulture);
            
            node = xmlDoc.SelectSingleNode("descendant::settings/popSize");
            popSize = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);
            node = xmlDoc.SelectSingleNode("descendant::settings/numDemes");
            nDemes = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);
            node = xmlDoc.SelectSingleNode("descendant::settings/numCycles");
            nCycles = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);
            node = xmlDoc.SelectSingleNode("descendant::settings/numGenerations");
            nGenerations = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);
            node = xmlDoc.SelectSingleNode("descendant::settings/generationsFreq");
            generationsFreq = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);
            node = xmlDoc.SelectSingleNode("descendant::settings/numExperiments");
            nExperims = Convert.ToInt32(node.Attributes.GetNamedItem("value").Value);

            this.nCycles = nCycles;
            this.nExperims = nExperims;

            Wrapper.SetEvolution(lGateStep, popSize, nDemes, nCycles, nGenerations, generationsFreq, nExperims);
        }

        private double[,] ReadCasesFile(string fileName, out int dataRowDim)
        {
            string line;
            string[] fileData;
            double[,] inputData;
            StreamReader reader;
            char[] separator = { '\t', ' ', '\n' };
            
            reader = new StreamReader(fileName);
            line = reader.ReadLine();
            dataRowDim = (int)(line.Length / 2) + 1;
            fileData = reader.ReadToEnd().Split(separator);
            inputData = new double[fileData.Length / dataRowDim,
                                      dataRowDim];
            reader.BaseStream.Position = 0;
            line = reader.ReadLine(); // Skip 1st data file row (header row)
            for (int i = 0; i < inputData.GetLength(0); i++)
            {
                line = reader.ReadLine();
                fileData = line.Split(separator);
                for (int j = 0; j < inputData.GetLength(1); j++)
                {
                    inputData[i, j] = Convert.ToDouble(fileData[j], CultureInfo.InvariantCulture);
                }
            }
            reader.Close();

            return inputData;
        }

        #endregion

        #region Properties

        public int NCycles
        {
            get { return nCycles; }
        }

        public int NExperims
        {
            get { return nExperims; }
            set { nExperims = value; }
        }

        public byte NFuncs
        {
            get { return nFuncs; }
        }

        #endregion
    }
}
