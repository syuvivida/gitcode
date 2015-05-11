{

  // Calculate the Chi-square of alpha ratio 
  Double_t DifSqr = 0.;
  Double_t ErrSqr = 0.;
  Double_t ChiSqr = 0.;

  for(Int_t i = 1; i <= nvarBins; i++){

    Double_t DifSqr = pow((f_alphaFit_const->Eval(varBins[i-1]) - h_alpha->GetBinContent(i)), 2);
    Double_t ErrSqr = pow(h_alpha->GetBinError(i), 2);

    ChiSqr += DifSqr/ErrSqr;

  }

  cout << "\n------- Chi-Square = " << ChiSqr << " --------" << endl;
  cout << "------ Degree of freedom = " << nvarBins << " -------" << endl;
  cout << "--- Probability of fit = " << TMath::Prob(ChiSqr, nvarBins) << " ---\n" << endl;

  // This probability is the probability of getting Chi2 larger or equal to (chiSqr) when the ndf is (nvarBins).
  // Only when the error is Gaussian distributed (error on the ratio covers 68% of the distribution) will this
  // probability make sense. 

}
