/*
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003-2009, 2005    (see file CONTACT for details)
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


#ifndef __POLYNOM_HH__
#define __POLYNOM_HH__


#include <vector>

#include "Base/Debug/Debug.hh"
#include "Base/Debug/Error.hh"


namespace BIAS{
  /**@class Polynom
     @ingroup g_math
     @brief class for Polynoms of arbitary order

     class Polynom represents a Polynom of arbitary order, where
     ith element is the coefficient of the ith exponent

     @author Marcel Lilienthal,June 2005; Christian Beder, August 2007
  */
  class Polynom : public std::vector<double>,public Debug
  {
  public:
    Polynom(double d=0.0):std::vector<double>(1,0.0){
      SetOrder(0);
      (*this)[0] = d;
    };

    Polynom(const Polynom& toCopy){
      operator=(toCopy);
    };

   // ~Polynom(){
    //  std::cout << this << std::endl;
      //if((*this) == NULL)
       // std::cout << "what" << std::endl;
   // }

    /**
       sets the order of polynom to i
       new generated coefficients are 0

       @author Marcel Lilienthal
     */
    inline void SetOrder(unsigned int i){
      order_=i;
      this->resize(i+1,0.0);
    };


    /**
       removes zero leading coefficients
    */
    void Prune() {
      int deg=GetOrder();
      while ((*this)[deg]==0) {
        deg--;
        if (deg==-1) {
          SetOrder(0);
          return;
        }
      }
      std::vector<double> tmp(deg+1);
      for (int i=0; i<=deg; i++) {
        tmp[i] = (*this)[i];
      }
      SetOrder(deg);
      for (int i=0; i<=deg; i++) {
        (*this)[i] = tmp[i];
      }
    }

    double Evaluate(double const x) const {
      double res=0;
      double xx=1;
      for (unsigned int i=0; i<=order_; i++) {
        res += (*this)[i]*xx;
        xx *= x;
      }
      return res;
    }

    inline unsigned int GetOrder() const { return order_; };

    //operators
    inline Polynom& operator=(const Polynom& a);
    inline Polynom& operator=(const double& d);
    inline Polynom& operator+=(const Polynom& a);
    inline Polynom& operator+=(const double& d);
    inline Polynom& operator-=(const Polynom& a);
    inline Polynom& operator-=(const double& d);
    inline Polynom& operator*=(const Polynom& a);
    inline Polynom& operator*=(const double& d);
    inline Polynom& operator/=(const Polynom& a);
    inline Polynom& operator/=(const double& d);
    inline bool operator==(const Polynom& a) const;
    inline bool operator!=(const Polynom& a) const;

  protected:
    unsigned int order_;
  };//class

  inline Polynom& Polynom::operator=(const Polynom& a){
    if (this != &a){
      SetOrder(a.order_);
      for(unsigned int i=0;i<=order_; i++){
        (*this)[i]=a[i];
      }
    }
    return *this;
  }

  inline Polynom& Polynom::operator=(const double& d){
    SetOrder(0);
    (*this)[0]=d;
    return *this;
  }

  inline Polynom& Polynom::operator+=(const Polynom& a){
    if ( a.order_>order_ )
      SetOrder(a.order_);
    for(unsigned int i=0; i<=a.order_; i++){
      (*this)[i]+=a[i];
    }
    return *this;
  }

  inline Polynom& Polynom::operator+=(const double& d){
    (*this)[0]+=d;
    return *this;
  }

  inline Polynom& Polynom::operator-=(const Polynom& a){
    if ( a.order_>order_ )
      SetOrder(a.order_);
    for(unsigned int i=0;i<=a.order_;i++){
      (*this)[i]-=a[i];
    }
    return *this;
  }

  inline Polynom& Polynom::operator-=(const double& d){
    (*this)[0]-=d;
    return *this;
  }

  inline Polynom& Polynom::operator*=(const Polynom& a){
    std::vector<double>  c(order_ + a.order_ + 1, 0.0);
    for(unsigned int i=0; i<=order_; i++) {
      for(unsigned int j=0; j<=a.order_; j++) {
        c[i+j] += (*this)[i] * a[j];
      }
    }
    SetOrder((unsigned int)c.size()-1);
    for(unsigned int i=0;i<c.size(); i++){
      (*this)[i]=c[i];
    }

    return *this;
  }

  inline Polynom& Polynom::operator*=(const double& d){
    for(unsigned int i=0;i<=order_;i++){
      (*this)[i]*=d;
    }
    return *this;
  }

  inline Polynom& Polynom::operator/=(const Polynom& a){
    BIASERR("Polynom division not yet implemented!");
    BIASABORT;
    return *this;
  }

  inline Polynom& Polynom::operator/=(const double& d){
    for(unsigned int i=0;i<=order_;i++){
      (*this)[i]/=d;
    }
    return *this;
  }

  inline bool Polynom::operator==(const Polynom& a) const{
    bool res = (this == &a);
    if(!res){
      res = order_ == a.order_;
      if(res){
        for(unsigned int i=0; i<=order_; i++){
          res = (*this)[i]==a[i];
          if(!res)
            return res;
        }
      }
    }
    return res;
  }

  inline bool Polynom::operator!=(const Polynom& a) const{
    return !operator==(a);
  }

  inline Polynom operator+(const Polynom& a, const Polynom& b){
    Polynom c=a;
    c+=b;
    return c;
  }

  inline Polynom operator-(const Polynom& a, const Polynom& b){
    Polynom c=a;
    c-=b;
    return c;
  }

  inline Polynom operator*(const Polynom& a, const Polynom& b){
    Polynom c=a;
    c*=b;
    return c;
  }

  inline Polynom operator*(const double& d, const Polynom& a){
    Polynom c=a;
    c*=d;
    return c;
  }

  inline Polynom operator*(const Polynom& a, const double& d){
    Polynom c=a;
    c*=d;
    return c;
  }

  inline Polynom operator/(const Polynom& a, const double& d){
    Polynom c=a;
    c/=d;
    return c;
  }

  inline Polynom operator/(const Polynom& a, const Polynom& b){
    Polynom c=a;
    c/=b;
    return c;
  }

  inline std::istream& operator>>(std::istream& s, BIAS::Polynom &p){
    BIASERR(">> operator not yet implemented for Polynom");
    BIASABORT;
    return s;
  }

  inline std::ostream& operator<<(std::ostream& s, const BIAS::Polynom &p){
    int order = (int)p.GetOrder();
    for (int i=order; i>=0; i--) {
      if (i==order)
        s << p[i];
      else {
        if (p[i]<0)
          s << " - " << -p[i];
        else
          s << " + " << p[i];
      }
      if (i>1)
        s << "x^"<< i;
      else if (i==1)
        s << "x";
    }
    return s;
  }

  inline bool operator<(const BIAS::Polynom& p1, const BIAS::Polynom &p2){
    unsigned int o1=p1.GetOrder();
    unsigned int o2=p2.GetOrder();
    if (o1==o2)
      return p1[o1]<p2[o1];
    else
      return o1<o2;
  }

  inline bool operator>(const BIAS::Polynom& p1, const BIAS::Polynom &p2){
    unsigned int o1=p1.GetOrder();
    unsigned int o2=p2.GetOrder();
    if (o1==o2)
      return p1[o1]>p2[o1];
    else
      return o1>o2;
  }

}//namespace



#endif //__POLYNOM_HH__
