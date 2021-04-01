
#include <cmath>
#include <time.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include "LineDescriptor.hh"
#include "PairwiseLineMatching.hh"
#include <iostream>


using namespace std;
void usage(int argc, char** argv){
	cout<<"Usage: "<<argv[0]<<"lines1"<<"lines2"<<"match"<<endl;
}



int main(int argc, char** argv)
{
	int ret = -1;
	if(argc<4){
		usage(argc,argv);
		return ret;
    }

	ScaleLines linesInLeft;
	
	std::ifstream ifile(argv[1]);
	
	while(ifile) {
		std::string str;
		std::getline(ifile,str,'\n');
		if(str.length()==0) continue;

		double x0,y0,x1,y1,len,grad,sal,npix;
		std::vector<float> descriptor;
		

		std::stringstream in(str);
	     
		in>>x0>>y0>>x1>>y1>>len>>grad>>sal>>npix;

		


		descriptor.clear();
		//char tmp[1000];
	//	sscanf(str.c_str(), 
		//	"%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%u%*[^0-9-+.eE]%[^\t]", &x0,&y0,&x1,&y1,&len,&grad,&sal,&npix,tmp);
		    //  cout<<x0<<" "<<y0<<" "<<x1<<" "<<y1<<" "<<len<<" "<<grad<<" "<<sal<<" "<<npix<<" ";
		  int ct = 0;
			while(in) {
			float xx;
			in>>xx;
			descriptor.push_back(xx);
		//	std::cout<<ct++<<" "<<xx<<std::endl;
			ct++;
			if(ct==72)
				break;
		}  

        //   for(int kkk=0;kkk<descriptor.size();kkk++)
          //  int kkk = 1;
              //  std::cout<<kkk+1<<" "<<descriptor.at(kkk)<<"\t";

		//std::cout<<std::endl;
	
			OctaveSingleLine singleLine;
            singleLine.numOfPixels  = (unsigned int)(npix);
			singleLine.startPointX = x0;
			singleLine.startPointY = y0;
			singleLine.endPointX = x1;
			singleLine.endPointY = y1;
			singleLine.lineLength = len;
			singleLine.direction = grad;
			singleLine.ePointInOctaveX = x1;
	        singleLine.ePointInOctaveY = y1;
		    singleLine.sPointInOctaveX = x0;
		    singleLine.sPointInOctaveY = y0;
		    singleLine.salience = sal;
		    singleLine.octaveCount = 0;
			singleLine.descriptor = descriptor;
	        LinesVec lv;
	        lv.push_back(singleLine);
		    linesInLeft.push_back(lv);
			//std::cout<<std::endl<<descriptor.size()<<std::endl;
		}
	
	ifile.close();

	
	ScaleLines   linesInRight;


	std::ifstream ifile2(argv[2]);
	
	while(ifile2) {
		std::string str;
		std::getline(ifile2,str,'\n');
		if(str.length()==0) continue;

		double x0,y0,x1,y1,len,grad,sal,npix;
		std::vector<float> descriptor;
	//	unsigned int npix;
		std::stringstream in(str);
	     descriptor.clear();
		in>>x0>>y0>>x1>>y1>>len>>grad>>sal>>npix;
		//sscanf(str.c_str(), 
		//	"%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%lf%*[^0-9-+.eE]%u", &x0,&y0,&x1,&y1,&len,&grad,&sal,&npix);
	//	cout<<x0<<" "<<y0<<" "<<x1<<" "<<y1<<" "<<len<<" "<<grad<<" "<<sal<<" "<<npix<<endl;
		 int ct = 0;
			while(in) {
			float xx;
			in>>xx;
			descriptor.push_back(xx);
		//	std::cout<<ct++<<" "<<xx<<std::endl;
			ct++;
			if(ct==72)
				break;
		}  

			//for(int kkk=0;kkk<descriptor.size();kkk++)
			//	std::cout<<kkk+1<<" "<<descriptor.at(kkk)<<std::endl;

			//std::cout<<std::endl;
	
			OctaveSingleLine singleLine;
            singleLine.numOfPixels = (unsigned int)(npix);
			singleLine.startPointX = x0;
			singleLine.startPointY = y0;
			singleLine.endPointX = x1;
			singleLine.endPointY = y1;
			singleLine.lineLength = len;
			singleLine.direction = grad;
			singleLine.ePointInOctaveX = x1;
	        singleLine.ePointInOctaveY = y1;
		    singleLine.sPointInOctaveX = x0;
		    singleLine.sPointInOctaveY = y0;
		    singleLine.salience = sal;
		    singleLine.octaveCount = 0;
			singleLine.descriptor = descriptor;
	        LinesVec lv;
	        lv.push_back(singleLine);
		    linesInRight.push_back(lv);
		//	std::cout<<std::endl<<descriptor.size()<<std::endl;
		}
	
	ifile2.close();
	//cout<<"readdone"<<endl;

	
	//LineDescriptor lineDesc;
	//lineDesc.findLineDesc(linesInLeft);
//	lineDesc.findLineDesc(linesInRight);
//	cout<<"ffff";

	


	//cout<<"readdone"<<endl;

	PairwiseLineMatching lineMatch;

	
	
	std::vector<unsigned int> matchResult;

	

	
	//cout<<"Matching............"<<endl;
	try{
		lineMatch.LineMatching(linesInLeft,linesInRight,matchResult);
	}
	catch(int exception)
	{
		 std::ofstream ofile(argv[3]);
		 ofile.close();
		 return -1;
	}
//	cout<<"Matching. done..........."<<endl;
		/*if(argc>3){
			std::ofstream ofile1(argv[3]);
			for (int i = 0;i < linesInLeft.size();i++)
				ofile1<<linesInLeft[i][0].startPointX<<" "<<linesInLeft[i][0].startPointY<<" "<<linesInLeft[i][0].endPointX<<" "<<linesInLeft[i][0].endPointY<<endl;
			ofile1.close();

			std::ofstream ofile2(argv[4]);

			for (int i = 0;i <linesInRight.size();i++)
				ofile2<<linesInRight[i][0].startPointX<<" "<<linesInRight[i][0].startPointY<<" "<<linesInRight[i][0].endPointX<<" "<<linesInRight[i][0].endPointY<<endl;
			ofile2.close();

	lineMatch.LineMatching(linesInLeft,linesInRight,matchResult);*/
	    std::ofstream ofile(argv[3]);
		for (int i = 0;i < matchResult.size()/2;i++)
		{
			ofile<<matchResult.at(2*i)<<" "<<matchResult.at(2*i + 1)<<endl;
		}
	
		ofile.close();
    }
