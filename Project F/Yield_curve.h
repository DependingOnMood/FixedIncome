//
//  Yield_curve.h
//  Homework 5
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#ifndef Yield_curve_h
#define Yield_curve_h

//#include <stdio.h>
#include "SBB_io.h"
#include <vector>
#include <string>

using namespace std;

class Yield {
private:
    string _yield_T;
    int _yield_term;
    double _yield_rate;
    double _yield_coupon;
    
public:
    Yield() {};
    ~Yield() {};
    
    void set_yield_state(SBB_instrument_fields* yield_record);
    
    string get_yield_T();
    int get_yield_term();
    double get_yield_rate();
    double get_yield_coupon();
    
};

class YieldCurve {
private:
    Yield* _yield_curve_array;
    int _yield_curve_size;
    string yield_T;
    vector<double> spread_added;
    
public:
    YieldCurve(const char* yieldcurve);
    YieldCurve();
    ~YieldCurve();
    
    string get_yield_T();
    
    double find_yield_rate(int term);
    double find_yield_coupon(int term);
    double calculate_2_year_dv01();
    
    double calculate_2_year_dv01_shift(double shift);
    
    void read_spread_added();
    
    int get_yield_curve_size();
    Yield* get_yield_curve_array();
    
    double find_2_year_yield_rate();
    double find_5_year_yield_rate();
    double find_10_year_yield_rate();
    double find_30_year_yield_rate();
};

#endif /* Yield_curve_h */
