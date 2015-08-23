
const int NCHIP = 8;
const char *filetypes[] = { "All files",     "*",
                            "DAT  files",    "*.dat",
                            "ROOT files",    "*.root",
                            0,               0 };

const int NFILE = 200;
TH1F *fhit[NFILE]    ;//= new TH1F("fhit","Integral hit of all channels", 64, 0, 64);
TH1F *fdecode[NFILE] ;//= new TH1F("fdecode","decoded hit", 166, 0, 166);
TH1F *fdecode2[NFILE];// = new TH1F("fdecode2","decoded hit by 2 hits", 166, 0, 166);
TH1F *fdecode3[NFILE];// = new TH1F("fdecode3","decoded hit by 3 hits", 166, 0, 166);
TH1F *fdecode4[NFILE];// = new TH1F("fdecode4","decoded hit by larger than 4 hits", 166, 0, 166);
TCanvas *c1[NFILE];
TCanvas c2, c3;
//TCanvas c4, c5;
TH1F *mean, *sigma, *nentry, *spatial;
TGraphErrors *sig;

void Sorted(int n, int *r);
int Decode(int x, int y);
int ReadFile(const char *sigfile, TH1F *fHit, TH1F *fDecode, TH1F *fDecode2, TH1F *fDecode3, TH1F *fDecode4);

void Analysis1x15_map_Filelist()
{
  gStyle->SetOptStat(0);
  //static TString dir(".");
  //TGFileInfo fi;
  //fi.fFileTypes = filetypes;
  //fi.fIniDir    = StrDup(dir);
  //printf("fIniDir = %s\n", fi.fIniDir);
  //new TGFileDialog(gClient->GetRoot(), NULL, kFDOpen, &fi);
  //printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
  //dir = fi.fIniDir;

  //ifstream fl(fi.fFilename);
  ifstream fl("filelist");
  TFile *rfile = new TFile("filelist.root");

  
  int nfile = 0;
  if(rfile->IsOpen()==0) {
      char line[100];
      fl>>line;
      TString filepath(line);
      nfile = 0;

      while (!fl.eof()) {
        fl>>line;
        TString filename = TString(line);
        cout<<"-->Reading "<<filename<<endl;
        fhit[nfile] = new TH1F(Form("fhit%d",nfile), Form("hitmap for %s",filename.Data()), 21, 0, 21);
        fdecode[nfile] = new TH1F(Form("fdecode%d",nfile), Form("decoded hit for %s",filename.Data()), 166, 0, 166);
        fdecode2[nfile]= new TH1F(Form("fdecode2_%d",nfile), Form("decoded hit by 2 hits for %s",filename.Data()), 166, 0, 166);
        fdecode3[nfile]= new TH1F(Form("fdecode3_%d",nfile), Form("decoded hit by 3 hits for %s",filename.Data()), 166, 0, 166);
        fdecode4[nfile]= new TH1F(Form("fdecode4_%d",nfile), Form("decoded hit by 4 hits for %s",filename.Data()), 166, 0, 166);

        if(ReadFile(filename.Data(), fhit[nfile], fdecode[nfile], fdecode2[nfile], fdecode3[nfile], fdecode4[nfile])==-1) {nfile++; continue;}
        if(nfile>=NFILE) break;
        c1[nfile] = new TCanvas(Form("c1%d",nfile),"Canvas",600,480);
        c1[nfile]->Divide(2,2);
        c1[nfile]->cd(1);
        fdecode[nfile]->Fit("gaus");
        c1[nfile]->cd(2);
        fdecode2[nfile]->Draw();
        c1[nfile]->cd(3);
        fdecode3[nfile]->Draw();
        c1[nfile]->cd(4);
        fdecode4[nfile]->Draw();
        //if(nfile==0)  f[nfile]->Fit("gaus");
        //else {
        //     double _m = f[nfile-1]->GetFunction("gaus")->GetParameter(1);
        //     double _s = f[nfile-1]->GetFunction("gaus")->GetParameter(2);
        //     f[nfile]->Fit("gaus","","",_m-5*_s, _m+5*_s);
        //}
        nfile++;
      }
      rfile = new TFile("filelist.root","RECREATE");
      for(int i=0;i<nfile;i++) {
         fhit[i]->Write();
         fdecode[i]->Write();
         fdecode2[i]->Write();
         fdecode3[i]->Write();
         fdecode4[i]->Write();
      }
  } else {
      for(int i=0;i<NFILE;i++) {
         fhit[i]     = (TH1F *) rfile->Get(Form("fhit%d",i));
         if(fhit[i]==NULL) {nfile=i; break;}
         fdecode[i]  = (TH1F *) rfile->Get(Form("fdecode%d",i));
         fdecode2[i] = (TH1F *) rfile->Get(Form("fdecode2_%d",i));
         fdecode3[i] = (TH1F *) rfile->Get(Form("fdecode3_%d",i));
         fdecode4[i] = (TH1F *) rfile->Get(Form("fdecode4_%d",i));
      }
  }
  
  mean = new TH1F("mean", "mean", nfile, 0, nfile);
  sigma= new TH1F("sigma","sigma",nfile, 0, nfile);
  nentry=new TH1F("nentry","nentry",nfile,0,nfile);
  spatial=new TH1F("spatial","spatial",30,-0.5,0.5);
  sig   =new TGraphErrors(nfile);

  double m[1000];

  for(int i=0;i<nfile;i++) {
     nentry->Fill(i,          fdecode[i]->GetFunction("gaus")->GetParameter(0));
     mean  ->Fill(i,          fdecode[i]->GetFunction("gaus")->GetParameter(1));
     sigma ->Fill(i,          fdecode[i]->GetFunction("gaus")->GetParameter(2));
     sig->SetPoint(i, i,      fdecode[i]->GetFunction("gaus")->GetParameter(2));
     sig->SetPointError(i, 0, fdecode[i]->GetFunction("gaus")->GetParError(2));
     m[i] = fdecode[i]->GetFunction("gaus")->GetParameter(1);
  }

  for(int i=1;i<nfile;i++) {cout<<(m[i]-m[0])*0.3-i*0.04<<endl; spatial->Fill((m[i]-m[0])*0.3-i*0.04);}

  double error[nfile];
  for(int i=0;i<nfile;i++) error[i]=0;

  c2.cd();
  c2.SetGridx();
  c2.SetGridy();
  mean->SetMarkerStyle(8);
  mean->SetMarkerSize(1);
  mean->SetError(error);
  mean->GetXaxis()->SetTitle("x");
  mean->Draw("pc");

  c3.cd();
  c3.SetGridx();
  c3.SetGridy();
  sigma->SetMarkerStyle(8);
  sigma->SetMarkerSize(1);
  sigma->SetError(error);
  sigma->GetXaxis()->SetTitle("x");
  sigma->Draw("pc");

  //c4.cd();
  //c4.SetGridx();
  //c4.SetGridy();
  //sig->SetMarkerStyle(8);
  //sig->SetMarkerSize(1);
  //sig->GetXaxis()->SetTitle("x");
  //sig->Draw("p");
  //
  //c5.cd();
  //c5.SetGridx();
  //c5.SetGridy();
  //spatial->SetMarkerStyle(8);
  //spatial->SetMarkerSize(1);
  //spatial->GetXaxis()->SetTitle("x");
  //spatial->Draw("p");
}


int ReadFile(const char *sigfile, TH1F *fHit, TH1F *fDecode, TH1F *fDecode2, TH1F *fDecode3, TH1F *fDecode4)
{
  ifstream ifsig(sigfile);
  if(!ifsig.is_open()) {cout<<"==>Failed to open "<<sigfile<<endl; return -1;}
  cout<<"==>Reading..."<<endl;

  const int N = 8;
  unsigned short dat[N];
  int index = 0;

  //TH1F *ftmp = new TH1F("ftmp","ftmp",64,0,64);

  while(!ifsig.eof())
  {
    index++;
    ifsig.read((char*)dat, sizeof (short)*N);

    if(dat[0]!=0x8899 || dat[1]!=0xaabb) {cout<<"==>Error! Event head is wrong!"<<endl; return -1;}
    int chipID   = (dat[2]&0x0F00)>>8;
    int tstamp   = ((dat[2]&0x00FF)<<16) | dat[3];
    if(chipID>=NCHIP || chipID<0) {cout<<"==>Error! chipID is out of range!"<<endl; return -1;}
    if(tstamp==0xFFFFFE) continue;

    int hittmp[64];
    for(int i=0;i<64;i++) hittmp[i]=0;
    for(int i=0;i<16;i++) {
	 if( (dat[4]&(1<<i))!=0 ) {hittmp[i+48]++;}
         if( (dat[5]&(1<<i))!=0 ) {hittmp[i+32]++;}
         if( (dat[6]&(1<<i))!=0 ) {hittmp[i+16]++;}
         if( (dat[7]&(1<<i))!=0 ) {hittmp[i]++;}
    }
    if(chipID>0) continue;

    int nhit=0;
    int hitted[64];
    for(int i=0;i<64;i++) {
	if(hittmp[i]) { fHit->Fill(chipID*64+i); hitted[nhit++]=i+1;} //ftmp->SetBinContent(i, 1); 
        //else            ftmp->SetBinContent(i, 0);
    }

    if(nhit<2 || nhit>=4) continue;
    //------------10 hits---------------
    //if(nhit<2 || nhit>=10) continue;

    //for(int i=0;i<64;i++) {if(hittmp[i])  cout<<i+1<<" "; }
    //cout<<"hitted."<<endl;

    Sorted(nhit, hitted);

    double dID = -1;

    if(nhit==2) dID = Decode(hitted[0], hitted[1]);
 
    if(nhit==3) {
       if(hitted[2]>=10) {
	    if(hitted[1]-hitted[0]==1) {
                    double dID1 = Decode(hitted[0], hitted[2]);
                    double dID2 = Decode(hitted[1], hitted[2]);
                    dID  = (dID1+dID2)/2.;
            } else if(hitted[2]-hitted[1]==1) {
                if(hitted[0]==1 && hitted[2]%2==1) {
                    double dID1 = Decode(hitted[0], hitted[2]);
                    double dID2 = Decode(hitted[1], hitted[2]);
                    dID  = (dID1+dID2)/2.;
                } else {
                    double dID1 = Decode(hitted[0], hitted[2]);
                    double dID2 = Decode(hitted[0], hitted[1]);
                    dID  = (dID1+dID2)/2.;
                } 
           } else {
                    dID = -1;
                    //cout<<"-- decode for triple hit is wrong"<<endl;
	   }
        }
    }

    if(nhit>=4) {
        int increasement=0;
	int low=-1, up=-1;
        for(int i=0;i<nhit-1;i++) {
	    if(hitted[i+1]-hitted[i]!=1) {increasement++; low=hitted[i]; up=hitted[i+1];}
	}

	if(increasement==0) {
	    if(hitted[0]!=1 && hitted[nhit-1]%2==1) {
	       double dID1 = Decode(hitted[nhit-2], hitted[nhit-1]);
	       double dID2 = Decode(hitted[nhit-1], hitted[0]);
	       dID = (dID1+dID2)/2.;
	    } else {
	       dID = -1;
	       //cout<<"-- decode for "<<nhit<<" hit is wrong"<<endl;
	    }
	} else if(increasement==1) {
	    if(hitted[0]==1) {
	        if(hitted[nhit-1]%2==1) {
		    double dID1 = Decode(hitted[low], hitted[nhit-1]);
		    double dID2 = Decode(hitted[nhit-3], hitted[up]);
		    dID = (dID1+dID2)/2.;
		} else {
		    double dID1 = Decode(hitted[low], hitted[nhit-1]);
		    double dID2 = Decode(hitted[nhit-2], hitted[up]);
		    dID = (dID1+dID2)/2.;
		}
	    } else {
	        if(hitted[nhit-1]%2==1) {
		    double dID1 = Decode(hitted[nhit-3], hitted[nhit-1]);
		    double dID2 = Decode(hitted[nhit-1], hitted[0]);
		    dID = (dID1+dID2)/2.;
		} else {
	           dID = -1;
	           //cout<<"-- decode for "<<nhit<<" hit is wrong"<<endl;
		}
	    }
	} else dID=-1;
    }

    //cout<<"->decoded to be "<<dID<<endl<<endl;
    fDecode->Fill(dID);
    if(nhit==2) fDecode2->Fill(dID);
    if(nhit==3) fDecode3->Fill(dID);
    if(nhit>=4) fDecode4->Fill(dID);

    //for(int i=0;i<N;i++) cout<<hex<<dat[i]<<" ";
    //cout<<endl<<endl;
    //ftmp->Draw();
    //c1.Update();
    //getchar();
  }

  return 0;
}



void Sorted(int n, int *r) 
{
    int low = -1;
    int high= n-1;
    int tmp, j;

    while(low < high) {
        for(j=low; j<high; ++j)
	    if(r[j]>r[j+1]) {
	        tmp = r[j]; r[j]=r[j+1];r[j+1]=tmp;
	    }
	--high;
	for(j=high;j>low;--j)
	   if(r[j]<r[j-1]) {
	        tmp = r[j]; r[j]=r[j-1];r[j-1]=tmp;
	   }
	++low;   
    }
}


int Decode(int x, int y)
{
    double decode = -1;
    if(y%2==0) { 
      if(x%2==0) {
         decode = (y-1)*(y-2)/2+2*x-2-36 + 0.5;
      }
      else {
         decode = (y-1)*(y-2)/2+2*x-1-36 + 0.5;
      } 
    } 
    else { //y is odd
      if(fabs(y-x)==1) {
          decode = (y-2)*(y-3)/2+2*x-2-36 + 0.5;
      }
      else if(x%2==0) {
          decode = (y-2)*(y-3)/2+2*x-1-36 + 0.5;
      } 
      else { 
          if(x==1) decode = y*(y-1)/2-36 + 0.5;
          else     decode = (y-2)*(y-3)/2+2*x-2-36 + 0.5;
      }
    }
    return decode;
}
