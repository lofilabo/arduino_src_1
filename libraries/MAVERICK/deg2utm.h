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
#ifndef deg2utm_h
#define deg2utm_h

#include "Arduino.h"
#include "deg2utm.h"
#include <math.h>

void deg2utm(double la,double lo,double* UTMNorthing, double* UTMEasting);
void dist_bearing(double N0,double E0,double N1,double E1,double* dist, double* bearing);
#endif

