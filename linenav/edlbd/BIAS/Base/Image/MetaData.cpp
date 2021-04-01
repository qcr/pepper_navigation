/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003-2009    (see file CONTACT for details)
  Multimediale Systeme der Informationsverarbeitung
  Institut fuer Informatik
  Christian-Albrechts-Universitaet Kiel


BIAS is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

BIAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with BIAS; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <iostream>
#include "MetaData.hh"
#include <Base/Debug/Error.hh>
#include <cstring>

using namespace BIAS;
using namespace std;

// #define DEBUG_METADATA

////////////////////////////////////////////////////////////////////////
// AppData
///////////////////////////////////////////////////////////////////////

AppData::AppData() 
{
  data = NULL;
  Clear();
}

AppData::~AppData() 
{ 
  if (data != NULL) { delete[] data; data = NULL; } 
}

AppData::AppData(const AppData& app) 
{
  data = NULL;
  (*this) = app;
}

AppData& AppData::operator=(const AppData& app)
{
  tag = app.tag;
  length = app.length;
  stag = app.stag;
  sdata = app.sdata;
  if (data != NULL) delete[] data;
  if (length>0) {
    data = new char[length];
    memcpy(data, app.data, length);
  }
  else data = NULL;
  return (*this);
}

void AppData::Clear()
{
  tag=MD_Invalid;
  if (data) delete[] data; data=NULL;
  data=NULL; 
  length=0;
  stag=sdata="";
}

int AppData::CheckFormat()
{
  //cerr <<"checking meta data format for "<<stag<<" sdata\n";
  if (tag==MD_USE_ASCII){
    string::size_type pos;
    string::size_type last=sdata.length()-1;
 
    //cerr << "checking sdata\n";
    
    // check if sdata contains only \n followed by '#'
    pos=0;
    while ((pos=sdata.find('\n', (pos==0)?0:pos+1))!=string::npos){
      if (pos==last || sdata[pos+1]!='#'){
        BIASERR("there should be no CR without an immediatly following '#' in "
                <<"ascii meta data "<<sdata);
        return -1;
      }
    };
    // check if sdata only contains '#' with a leading '\n'
    pos=0;
    while ((pos=sdata.find('#', (pos==0)?0:pos+1))!=string::npos){
      if (pos==0 || sdata[pos-1]!='\n'){
        BIASERR("there should be no '#' without a leading CR in "
                <<"ascii meta data\nthe first '#' is added automatically "
                <<stag<<endl<<sdata<<" "<<pos);
        return -2;
      }
    }
  
    //cerr << "checking stag\n";
    last=stag.length()-1;
    if (last<3){
      BIASERR("stag too short, stag should look like this: "
              <<"'#[a tag]': "<<last+1<<" "<<stag);
      return -3;
    }
    if (stag[0]!='#'){
      BIASERR("wrong stag format, stag should start with '#[' "<<stag);
      return -4;
    }
    if (stag[1]!='['){
      BIASERR("wrong stag format, stag should start with '#[' "<<stag);
      return -5;
    }
    if (stag[last]!=']'){
      BIASERR("wrong stag format, stag should end with ']' "<<stag);
      return -6;
    }
  } else {
    if (stag!="" || sdata!=""){
      BIASERR("stag or sdata is set but tag is not MD_USE_ASCII");
      return -7;
    }
    if (length<0){
      BIASERR("invalid length "<<length);
      return -8;
    }
  }
  //cerr << "CheckFormat finished\n";
  return 0;
}

ostream& BIAS::operator<<(ostream& os, const AppData::TAppData& ta)
{
  switch(ta){
  case AppData::MD_PMatrix: 
    os << "MD_PMatrix ("<<(int)AppData::MD_PMatrix<<")"; break;
  case AppData::MD_Orientation: 
    os << "MD_Orientation ("<<(int)AppData::MD_Orientation<<")"; break;
  case AppData::MD_TimeStamp:
    os << "MD_TimeStamp ("<<(int)AppData::MD_TimeStamp<<")"; break;
  case AppData::MD_PTU_Data: 
    os << "MD_PTU_Data ("<<(int)AppData::MD_PTU_Data<<")"; break;
  case AppData::MD_Inertial_Sensor: 
    os << "MD_Inertial_Sensor ("<<(int)AppData::MD_Inertial_Sensor<<")"; break;
  case AppData::MD_ASCII_DATA: 
    os << "MD_ASCII_DATA ("<<(int)AppData::MD_ASCII_DATA<<")"; break;
  case AppData::MD_RMatrix: 
    os << "MD_RMatrix ("<<(int)AppData::MD_RMatrix<<")"; break;
  case AppData::MD_KMatrix: 
    os << "MD_KMatrix ("<<(int)AppData::MD_KMatrix<<")"; break;
  case AppData::MD_CVector: 
    os << "MD_CVector ("<<(int)AppData::MD_CVector<<")"; break;
  case AppData::MD_HomgPoint2D: 
    os << "MD_HomgPoint2D ("<<(int)AppData::MD_HomgPoint2D<<")"; break;
  case AppData::MD_Invalid: 
    os << "MD_Invalid ("<<(int)AppData::MD_Invalid<<")"; break;
  case AppData::MD_UUID: 
    os << "MD_UUID ("<<(int)AppData::MD_UUID<<")"; break;
  case AppData::MD_USE_ASCII: 
    os << "MD_USE_ASCII ("<<(int)AppData::MD_USE_ASCII<<")"; break;
  case AppData::MD_Focal_Length: 
    os << "MD_Focal_Length ("<<(int)AppData::MD_Focal_Length<<")"; break;
  default: os <<"unknown TAppData.tag: "<<(int)ta; break;
  }
  return os;
}

ostream& BIAS::operator<<(ostream& os, const AppData& ad)
{
  if (ad.tag!=AppData::MD_USE_ASCII){
#ifdef DEBUG_METADATA
    cerr << "writing binary: "<<ad.tag<<" length "<<ad.length
         << "   at pos: "<<(int)os.tellp()<<endl;
#endif
    os.write((char*)&ad.tag, sizeof(AppData::TAppData));
    os.write((char*)&ad.length, sizeof(int));
    os.write(ad.data, ad.length);
  } else {
#ifdef DEBUG_METADATA
    cerr << "writing ascii: "<< ad.stag << "\n# "<<ad.sdata;
    cerr<<"read binary AppData, type: "<<ad.tag<<" length: "<<ad.length<<endl;
    cout << "length "<<ad.sdata.length();
    if (ad.sdata.length()>0)
      cout <<"  first "<<ad.sdata.c_str()[0];
    cout <<endl;
      cout << "tag: '"<< ad.stag <<"'\ndata : '"<< ad.sdata<<"'\n";
#endif
    if (ad.sdata.length()>0 && ad.sdata.c_str()[0]=='#'){
      os << ad.stag << "\n"<<ad.sdata;
    } else {
      os << ad.stag << "\n# "<<ad.sdata;
#ifdef DEBUG_METADATA
      cout << "added #\n";
#endif
    }
  }
  return os;
}

#define MD_LINE_LENGTH 4096

istream& BIAS::operator>>(istream& is, AppData& ad)
{
  char id[2]={' ',' '};
  is.read(id, 2*sizeof(char));
  if (is.fail() && is.eof()){
    return is;
  }
  is.putback(id[1]);// Since OpenSuse 12.2 'putback' seems to unset the eof bit
  is.putback(id[0]);// even when 0 bytes were read
  if (id[0]=='#' && id[1]=='[') { // ascii data
    ad.tag=AppData::MD_USE_ASCII;
    // read stag
    char c;
    char line[MD_LINE_LENGTH];
    do {
      is.get(c);
      ad.stag+=c;
    } while (c!=']');
    is.getline(line, MD_LINE_LENGTH);
    string checkString(line);
    if(checkString.size() == MD_LINE_LENGTH) {
      BIASERR("you supposedly ran out of line length!");
      BIASABORT;
    }
    //cout << "line: "<<line<<endl;
    ad.sdata+=line;
    ad.sdata+="\n";
    // read sdata
    do {
      if (is.peek()!='#') break;
      is.read(line, 2);
      if (line[0]=='#' && line[1]=='[') {
        is.putback(line[1]);
        is.putback(line[0]);
        break; 
      } 
      is.putback(line[1]);
      is.putback(line[0]);
      is.getline(line, MD_LINE_LENGTH);
      //cout << "line: "<<line<<endl;
      ad.sdata+=line;
      ad.sdata+="\n";
    } while (is);
    if (ad.stag!="#[PTU Data]" && ad.stag!="#[Inertial Sensor]"){
      //cerr << " now removing whitespace from "<<ad.stag<<endl;
      // now remove leading '#', CRs, and whitespace
      while (ad.sdata.length()>0 && (ad.sdata[0]=='#' || ad.sdata[0]==' ' ||
                                     ad.sdata[0]=='\n'))
        ad.sdata.erase(0, 1);
    } else {
      while (ad.sdata.length()>0 && (ad.sdata[0]==' ' || ad.sdata[0]=='\n'))
        ad.sdata.erase(0, 1);
    }
    // remove appended CRs
    while (ad.sdata[ad.sdata.length()-1]=='\n')
      ad.sdata.erase(ad.sdata.length()-1, 1);
#ifdef DEBUG_METADATA
    cerr << "read ascii AppData : stag: "<<ad.stag<<"\t:"<<ad.sdata<<endl;
#endif
  } else { // binary data
#ifdef BIAS_DEBUG
    if (is.peek()=='#'){
      BIASERR("this image contains probably MetaData in old format"
              <<"\n use '/usr/net/bin/convasciidata' to convert it");
#if __GNU_C__  < 3
      is.setstate(ios::badbit | ios::failbit);
#else
      is.setstate(ios_base::badbit | ios_base::failbit);
#endif
      return is;
    }
#endif
#ifdef DEBUG_METADATA
    cerr<<"read binary AppData at pos: "<<(int)is.tellg()<<endl;
#endif
    ad.stag="";
    ad.sdata="";
    is.read((char*)&ad.tag, sizeof(ad.tag));
    if (!is) return is;
    
    is.read((char*)&ad.length, sizeof(ad.length));
    if (!is) return is;
    
#ifdef DEBUG_METADATA
    cerr<<"read binary AppData, type: "<<ad.tag<<" length: "<<ad.length<<endl;
#endif
    char *data = new char[ad.length];
    is.read(data, ad.length);
    ad.data = data;
  }
  return is;
}

////////////////////////////////////////////////////////////////////////
// MetaData
///////////////////////////////////////////////////////////////////////

MetaData::MetaData() //: Debug()
  : vector<AppData>()
{}

MetaData::~MetaData()
{}

void MetaData::Add(enum AppData::TAppData ID, unsigned int length, char *data)
{
  AppData ad;
  if (Find(ID, ad)>=0)
    Delete(ID);
  ad.Clear();
  ad.tag=ID;
  ad.length=length;
  ad.data=new char[ad.length];
  memcpy(ad.data, data, ad.length);
  push_back(ad);
}

void MetaData::Add(AppData& ad)
{
  AppData ad2;
  if (ad.tag==AppData::MD_USE_ASCII){
    if (Find(ad.stag, ad2)>=0)
      Delete(ad.stag);
#ifdef BIAS_DEBUG
    ad.CheckFormat();
#endif
    push_back(ad);
  } else {
    if (Find(ad.tag, ad2)>=0)
      Delete(ad.tag);
    push_back(ad);
  }
}

void MetaData::Add(const string &tag, const string& data)
{
  AppData ad;
  if (Find(tag, ad)>=0)
    Delete(tag);
  ad.Clear();
  ad.tag=AppData::MD_USE_ASCII;
  ad.stag=tag;
  ad.sdata=data;
#ifdef BIAS_DEBUG
  ad.CheckFormat();
#endif
  push_back(ad);
}


int MetaData::Find(const AppData::TAppData tag, AppData &data) const
{
  MetaData::const_iterator it;
  int i;
  bool ascii_present=false;
  for (i=0, it=begin(); it!=end(); it++, i++){
    if ((*it).tag == AppData::MD_USE_ASCII) ascii_present=true;
    if ((*it).tag == tag) {
      data = (*it);
      return i;
    }
  }
  // not found yet
  data.Clear();
  if (ascii_present)
    return -2;
  else 
    return -1;
  //return -5; // just to please the compiler. NO, unreachable... (jw) 
}

int MetaData::Find(const string &tag, AppData &data) const
{   
  MetaData::const_iterator it;
  int i;
  bool binary_present=false;
  for (i=0, it=begin(); it!=end(); it++, i++){
    if ((*it).tag != AppData::MD_USE_ASCII && 
        (*it).tag != AppData::MD_Invalid) binary_present=true;
    if ((*it).stag == tag) {
      data = (*it);
      // cerr << "tag: "<< data.tag << endl;
      return i;
    }
  }
  // not found yet
  data.Clear();
  if (binary_present)
    return -2;
  else 
    return -1;
  //return -5; // just to please the compiler. NO unreachable
}


int MetaData::Find(const AppData::TAppData tag, const string &stag, 
                   AppData &data) const
{
  int res;
  if ((res=Find(tag, data))==-2)
    res=Find(stag, data);
  return res;
}

void MetaData::Delete(enum AppData::TAppData tag)
{
  MetaData::iterator it;
  for (it=begin(); it!=end(); it++){
    if ((*it).tag == tag) {
      erase(it);
      break;
    }
  }
}

void MetaData::Delete(const std::string &tag)
{
  MetaData::iterator it;
  for (it=begin(); it!=end(); it++){
    if ((*it).stag == tag) {
      erase(it);
      break;
    }
  }
}

ostream &MetaData::WriteBinary(ostream& os) const
{
  MetaData::const_iterator it;
  for (it=begin(); it!=end(); it++){
    if ((*it).tag!=AppData::MD_USE_ASCII)
      os << (*it);
  }
  return os;
}
    
ostream &MetaData::WriteAscii(ostream& os) const
{
  MetaData::const_iterator it;
  for (it=begin(); it!=end(); it++){
    if ((*it).tag==AppData::MD_USE_ASCII)
      os << (*it) << endl;
  }
  return os;
}


void MetaData::Dump(ostream& os)
{
  for (unsigned int i=0; i<size(); i++)
    switch ((*this)[i].tag) {
    case AppData::MD_PMatrix:{
      os <<"PMatrix ("<<(*this)[i].length<<"): ";
      double *d = (double*)(*this)[i].data;
      for (int i=0; i<12; i++) os <<"  "<<d[i];
      os <<endl;
      break;}
    case AppData::MD_Projection:
      os<< "MD_Projection("<<(*this)[i].length<<"): ";
	os <<(*this)[i].data;
      os<<endl;
      break;
    case AppData::MD_Orientation:
      os << "MD_Orientation("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_TimeStamp:
      os << "MD_TimeStamp("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_PTU_Data:
      os << "MD_PTU_Data("<<(*this)[i].length<<"):  ";
      os <<endl;
      break;
    case AppData::MD_Inertial_Sensor:
      os << "MD_Inertial_Sensor("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_ASCII_DATA:
      os << "MD_ASCII_DATA: "<<(*this)[i].data<<endl;
      break;
    case AppData::MD_RMatrix:
      os << "MD_RMatrix("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_KMatrix:
      os << "MD_KMatrix("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_CVector:
      os << "MD_CVector("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_HomgPoint2D:{
      os << "MD_HomgPoint2D: [";
      double *d = (double*)(*this)[i].data;
      for (int i=0; i<3; i++) os <<" "<<d[i];
      os << "]\n";
      break;}
    case AppData::MD_Invalid:
      os << "MD_Invalid("<<(*this)[i].length<<"): ";
      os <<endl;
      break;
    case AppData::MD_UUID: 
      os <<"MD_UUID("<<(*this)[i].length<<"): "<< (char*)(*this)[i].data<<endl;
      break;
    case AppData::MD_USE_ASCII:
      os << (*this)[i].stag << " - " << (*this)[i].sdata << endl;
      break;
    case AppData::MD_Focal_Length:
      os << "MD_Focal_Length("<<(*this)[i].length<<"): " << " - " 
         <<*((double*)(*this)[i].data)<<endl;
      break;
    default:
      BIASERR("unknown tag: "<<(int)(*this)[i].tag);
      break;
    }

}


istream& BIAS::operator>>(istream& is, MetaData& md)
{  
#ifdef DEBUG_METADATA
  cerr << "A reading binary MetaDatas at pos: "<<is.tellg() << endl;
#endif
  AppData ad;
  bool binary=true;
  if (is.peek()=='#') binary=false;
#ifdef DEBUG_METADATA
  cerr << "B reading binary MetaDatas at pos: "<<is.tellg() << endl;
#endif
  if (binary){
#ifdef DEBUG_METADATA
    cerr << "reading binary MetaDatas" << endl;
#endif
    while(is){
      ad.Clear();
      is >> ad;
      if (is)
        md.push_back(ad);
    }
  } else {
#ifdef DEBUG_METADATA
    cerr << "reading ascii MetaDatas" << endl;
#endif
    char id[2];
    char line[MD_LINE_LENGTH];
    while(is && is.peek()=='#'){
      is.read(id, 2*sizeof(char));
      is.putback(id[1]);
      is.putback(id[0]);
      if (id[0]=='#' && id[1]=='[') { // ascii data
        ad.Clear();
        is >> ad;
        if (is)
          md.push_back(ad);
        //cerr << "meta datum : "<<ad.stag<<endl;
      } else {
        is.getline(line, MD_LINE_LENGTH);
#ifdef BIAS_DEBUG
#ifdef BIAS_EXTRA_WARN
        BIASERR("skipping non-meta data image comments: "<<line);
#endif
#endif
      }
    }
  }
  return is;
}

ostream& BIAS::operator<<(ostream& os, const MetaData& md)
{  
  if (!os) {
    BIASERR("MetaData::AppendToOStream(): ostream not writable!");
    return os;
  }
  MetaData::const_iterator it;
  for (it=md.begin(); it!=md.end(); it++){
    os << (*it);
  }
  return os;
}


