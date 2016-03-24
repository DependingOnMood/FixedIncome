//
//  Yield_curve.cc
//  Homework 5
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#include "Yield_curve.h"
#include "SBB_date.h"
#include "Calculator.h"
#include <stdlib.h>
#include <limits.h>
#include <sstream>
#include <fstream>
#include <string>

void Yield::set_yield_state(SBB_instrument_fields* yield_record) {
    
    SBB_date from_dt;
    from_dt.set_from_yyyymmdd(yield_record->SettlementDate());
    
    SBB_date to_dt;
    to_dt.set_from_yyyymmdd(yield_record->MaturityDate());
    
    _yield_term = SBB_periods::get_number_of_periods_annual(from_dt, to_dt);
    
    _yield_T = yield_record->SecurityID();
    
    _yield_rate = yield_record->Yield();
    
    _yield_coupon = yield_record->Coupon();

}

string Yield::get_yield_T() {
    return _yield_T;
}

int Yield::get_yield_term() {
    return _yield_term;
}

double Yield::get_yield_rate() {
    return _yield_rate;
}

double Yield::get_yield_coupon() {
    return _yield_coupon;
}

YieldCurve::YieldCurve(const char* yieldcurve) {
    
    SBB_instrument_input_file file(yieldcurve);
    SBB_instrument_fields *fields_array = file.records(_yield_curve_size);
    
    _yield_curve_array = new Yield[_yield_curve_size];
    
    for (int i = 0; i < _yield_curve_size; i++) {
        _yield_curve_array[i].set_yield_state(&fields_array[i]);
    }
    
    file.free_records();
}

void YieldCurve::read_spread_added(){
    double spread_added_2_year;
    double spread_added_5_year;
    double spread_added_10_year;
    double spread_added_30_year;
    
    ifstream stream;
    stream.open("spreadadded.txt");
    string line;
    while (getline(stream, line)){
        istringstream tokens(line);
        tokens >> spread_added_30_year >> spread_added_2_year >> spread_added_5_year >> spread_added_10_year;

        spread_added.push_back(spread_added_30_year);
        spread_added.push_back(spread_added_2_year);
        spread_added.push_back(spread_added_5_year);
        spread_added.push_back(spread_added_10_year);
    }
    stream.close();
}

double YieldCurve::find_yield_rate(int term) {

    read_spread_added();
    
    double rate = 0.0;
    int min_distance = INT_MAX;
    int min_term = INT_MAX;
    
    for (int i = 0; i < _yield_curve_size; i++) {
        int compare_term = _yield_curve_array[i].get_yield_term();
        int distance = abs(compare_term - term);
        
        if (min_distance > distance) {
            min_distance = distance;
            min_term = compare_term;
            rate = _yield_curve_array[i].get_yield_rate() + spread_added[i]/100;
            yield_T = _yield_curve_array[i].get_yield_T();
        } else if (min_distance == distance) {
            if (min_term > compare_term) {
                min_term = compare_term;
                rate = _yield_curve_array[i].get_yield_rate() + spread_added[i]/100;
                yield_T = _yield_curve_array[i].get_yield_T();
            }
        }
    }

    return rate;
}

string YieldCurve::get_yield_T(){
    return yield_T;
}

double YieldCurve::find_2_year_yield_rate(){
    return _yield_curve_array[1].get_yield_rate();
}

double YieldCurve::find_5_year_yield_rate(){
    return _yield_curve_array[2].get_yield_rate();
}

double YieldCurve::find_10_year_yield_rate(){
    return _yield_curve_array[3].get_yield_rate();
}

double YieldCurve::find_30_year_yield_rate(){
    return _yield_curve_array[0].get_yield_rate();
}

double YieldCurve::find_yield_coupon(int term) {
    
    double rate = 0.0;
    int min_distance = INT_MAX;
    int min_term = INT_MAX;
    
    for (int i = 0; i < _yield_curve_size; i++) {
        int compare_term = _yield_curve_array[i].get_yield_term();
        int distance = abs(compare_term - term);
        
        if (min_distance > distance) {
            min_distance = distance;
            min_term = compare_term;
            rate = _yield_curve_array[i].get_yield_coupon();
        } else if (min_distance == distance) {
            if (min_term > compare_term) {
                min_term = compare_term;
                rate = _yield_curve_array[i].get_yield_coupon();
            }
        }
    }
    return rate;
}

double YieldCurve::calculate_2_year_dv01() {
    double rate_for_2_year = find_yield_rate(2);
    double coupon_for_2_year = find_yield_coupon(2);
    Coupon_bearing_calculator *bearing = new Coupon_bearing_calculator(2, coupon_for_2_year, rate_for_2_year, 4);
    return bearing->calculate_bond_dv01();
}

double YieldCurve::calculate_2_year_dv01_shift(double shift) {
    double rate_for_2_year = find_yield_rate(2);
    double coupon_for_2_year = find_yield_coupon(2);
    Coupon_bearing_calculator *bearing = new Coupon_bearing_calculator(2, coupon_for_2_year, rate_for_2_year, 4);
    return bearing->calculate_bond_dv01_shift(shift);
}

YieldCurve::~YieldCurve() {
    delete[] _yield_curve_array;
};

int YieldCurve::get_yield_curve_size() {
    return _yield_curve_size;
}

Yield* YieldCurve::get_yield_curve_array() {
    return _yield_curve_array;
}

