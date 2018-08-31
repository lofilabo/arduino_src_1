/*
  deg2utm.h - Library for converting lat long to UTM coordinates WGS84.
  Modified by Samuel Wane, Harper Adams University, UK February 2016.
  Released into the public domain.
  Modified from:
  Rafael Palacios MATLAB function 'deg2utm.m'

% Some code has been extracted from UTM.m function by Gabriel Ruiz Martinez.
%
% Inputs:
%    Lat: Latitude vector.   Degrees.  +ddd.ddddd  WGS84
%    Lon: Longitude vector.  Degrees.  +ddd.ddddd  WGS84
%

*/

#include "Arduino.h"
#include "deg2utm.h"
#include <math.h>
#define POW pow
#define SIN sin
#define COS cos
#define pi M_PI

void deg2utm(double la,double lo,double* UTMNorthing, double* UTMEasting)
{


  double  sa = 6378137.000000 ; 
  double sb = 6356752.314245;
         
   
   double  e2 = sqrt( ( ( sa *sa ) - ( sb *sb ) )  ) / sb;
   double e2cuadrada = pow(e2 , 2);
   double c;
    c = (pow(sa,2))/sb; //6399593.625758674;
  
  
   double lat = la * ( pi / 180 );
   double lon = lo * ( pi / 180 );

   double Huso = int( ( lo / 6 ) + 31);
   
   double S = ( ( Huso * 6 ) - 183 );
   double deltaS = lon - ( S * ( pi / 180 ) );  //0.009961257266247
   //deltaS=deltaS*100000;
   
   double a = cos(lat) * sin(deltaS);
   
   double epsilon = 0.5 * log( ( 1 +  a) / ( 1 - a ) );

   double nu = atan( tan(lat) / cos(deltaS) ) - lat;
   //double v = sqrt( c / ( ( 1 + ( e2cuadrada * ( cos(lat) *cos(lat)) ) ) ) ) * 0.9996;
   double v;
   v =  ( 1 + ( e2cuadrada *  cos(lat)*cos(lat)  ) ) ; 
   v=sqrt(v);
   v=c/v;
   v=v*0.9996;

   double  ta = ( e2cuadrada / 2 ) * epsilon*epsilon * cos(lat)*cos(lat) ;
   
   double a1 = sin( 2 * lat );
   double a2 = a1 * ( pow(cos(lat), 2));
   double j2 = lat + ( a1 / 2 );
   double j4 = ( ( 3 * j2 ) + a2 ) / 4;
   double j6 = ( ( 5 * j4 ) + ( a2 * ( pow(cos(lat)  , 2)) )) / 3;   
   double alfa = ( 0.75 ) * e2cuadrada;
   double beta = ( 1.66666666666667 ) * alfa*alfa;
   double gama = ( 1.2962962962963 ) * alfa*alfa*alfa;//pow(alfa , 3);
   double aaa= ( lat - alfa * j2 + beta * j4 - gama * j6 );
   //aaa=0.914172303604986;
   double Bm = 0.9996 * c*aaa; 
   double xx = epsilon * v * ( 1 + ( ta / 3 ) ) + 500000;
   double yy = nu * v * ( 1 + ta ) + Bm;

   if (yy<0)
       yy=9999999+yy;

  *UTMNorthing=yy;
  *UTMEasting=xx;
   
}

void dist_bearing(double N0,double E0,double N1,double E1,double* dist, double* bearing)
{//N0,E0 is start pos, returns bearing to N1,E1 
  *dist=sqrt(((N1-N0)*(N1-N0))+((E1-E0)*(E1-E0)));
  *bearing=atan2((E1-E0),(N1-N0));
  *bearing=*bearing*57.29578;
  if (*bearing<0) *bearing=360+*bearing;
}

