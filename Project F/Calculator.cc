//
//  Calculator.cc
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#include <math.h>
#include "iostream"
#include "Calculator.h"

using namespace std;

Zero_coupon_calculator::Zero_coupon_calculator(int frequency, double rate, int n) {
    _frequency = frequency;
    _rate = rate;
    _n = n;
    
    _maturity = 100;
    _coupon = 0.0;
}

double Zero_coupon_calculator::calculate_bond_price() {
    double rate = _rate * 0.01 / _frequency;
    _price = _maturity / pow((1 + rate), _n);
    return _price;
}

double Zero_coupon_calculator::calculate_bond_price(double rate) {
    return _maturity / pow((1 + rate), _n);
}

double Zero_coupon_calculator::calculate_bond_price_shift(double shift) {
    //cout << " in calculate_bond_price_shift: " << shift <<endl;
    double shift_rate = (_rate * 0.01 *(1+ shift)) / _frequency;
    return _maturity / pow((1 + shift_rate), _n);
}

double Zero_coupon_calculator::calculate_bond_dv01() {
    double dv01_up = (_rate * 0.01 + 0.0001) / _frequency;
    double dv01_down = (_rate * 0.01 - 0.0001) / _frequency;
    _dv01 = (calculate_bond_price(dv01_down) - calculate_bond_price(dv01_up)) / 2;
    return _dv01;
}

double Zero_coupon_calculator::calculate_bond_dv01_shift(double shift) {
    double dv01_up = (_rate * 0.01 *(1 + shift) + 0.0001) / _frequency;
    double dv01_down = (_rate * 0.01 *(1 + shift) - 0.0001) / _frequency;
    _dv01 = (calculate_bond_price(dv01_down) - calculate_bond_price(dv01_up)) / 2;
    return _dv01;
}


Coupon_bearing_calculator::Coupon_bearing_calculator(int frequency, double coupon, double rate, int n) {
    _frequency = frequency;
    _coupon = coupon;
    _rate = rate;
    _n = n;
    
    _maturity = 100;
    _coupon = coupon / frequency;
}

double Coupon_bearing_calculator::calculate_bond_price() {
    double rate = _rate * 0.01 / _frequency;
    _price = (_coupon * (1 - 1 / pow((1 + rate), _n)) / rate) + (_maturity / pow((1 + rate), _n));
    return _price;
}

double Coupon_bearing_calculator::calculate_bond_price(double rate) {
    return (_coupon * (1 - 1 / pow((1 + rate), _n)) / rate) + (_maturity / pow((1 + rate), _n));
}

double Coupon_bearing_calculator::calculate_bond_price_shift(double shift) {
    //cout << " in calculate_bond_price_shift: " << shift <<endl;
    double shift_rate = (_rate * 0.01 *(1 + shift)) / _frequency;
    return (_coupon * (1 - 1 / pow((1+shift_rate), _n)) / shift_rate) + (_maturity / pow((1+shift_rate), _n));
}

double Coupon_bearing_calculator::calculate_bond_dv01() {
    double dv01_up = (_rate * 0.01 + 0.0001) / _frequency;
    double dv01_down = (_rate * 0.01 - 0.0001) / _frequency;
    _dv01 = (calculate_bond_price(dv01_down) - calculate_bond_price(dv01_up)) / 2;
    return _dv01;
}

double Coupon_bearing_calculator::calculate_bond_dv01_shift(double shift) {
    double dv01_up = (_rate * 0.01 *(1 + shift) + 0.0001) / _frequency;
    double dv01_down = (_rate * 0.01 *(1 + shift) - 0.0001) / _frequency;
    _dv01 = (calculate_bond_price(dv01_down) - calculate_bond_price(dv01_up)) / 2;
    return _dv01;
}
