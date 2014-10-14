using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace QILGPcs
{
    class Wrapper
    {
        //public delegate void CallBack(double[] a);

        [DllImport("qilgp.dll")]
        public static extern byte SetQIndiv(int qIndivLen,
                                            double qNopProb,
                                            byte nFuncs,
                                            byte termsDim,
                                            byte[] termsCardins,
                                            byte[] termIdxs,
                                            byte[] destReg0,
                                            byte typedTerms,
                                            byte effObserving,
                                            byte[,] mCodesLens,
                                            byte mCodeDim,
                                            byte[,,] mCodeTable);
        [DllImport("qilgp.dll")]
        public static extern byte SetIndiv(byte[] header,
                                           byte headerLen,
                                           byte[] footer,
                                           byte footerLen);
        [DllImport("qilgp.dll")]
        public static extern byte SetInputCases(byte classifProb, 
                                                double[,] trainigCases,
                                                double[,] validatingCases,
                                                double[,] testingCases,
                                                int nSamplesTrain,
                                                int nSamplesValid, 
                                                int nSamplesTest,
                                                int dataRowDim,
                                                double[] consts,
                                                int nConsts);

        [DllImport("qilgp.dll")]
        public static extern byte SetEvolution(double lGateStep,
                                               int popSize,
                                               int nDemes,
                                               int nCycles,
                                               int nGenerations, 
                                               int generationsFreq,
                                               int nExperiments);
        
        [DllImport("qilgp.dll")]
        public static extern double Run();

        [DllImport("qilgp.dll")]
        public static extern double RunQILGP();
    }
}
