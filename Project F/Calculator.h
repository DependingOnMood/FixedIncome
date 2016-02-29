//
//  Calculator.h
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#ifndef Calculator_h
#define Calculator_h

//#include <stdio.h>

class Bond_calculator_interface
{
private:
    
public:
    int _frequency;
    double _coupon;
    double _rate;
    
    int _maturity;
    int _n;
    
    double _price;
    double _dv01;
    
    virtual double calculate_bond_price(void) = 0;
    virtual double calculate_bond_dv01(void) = 0;
    virtual double calculate_bond_dv01_shift(double shift) = 0;
    virtual double calculate_bond_price_shift(double shift) = 0;
//    virtual double calculate_bond_price_shift_up() = 0;
};

class Zero_coupon_calculator: public Bond_calculator_interface
{
private:
    
public:
    
    Zero_coupon_calculator() {};
    ~Zero_coupon_calculator() {};
    Zero_coupon_calculator(int frequency, double rate, int periodes);
    
    double calculate_bond_price(double rate);
    double calculate_bond_price();
    
    double calculate_bond_price_shift(double shift);
//    double calculate_bond_price_shift_up();
    
    double calculate_bond_dv01();
    double calculate_bond_dv01_shift(double shift);
};

class Coupon_bearing_calculator: public Bond_calculator_interface
{
private:
    
public:
    double _coupon;
    
    Coupon_bearing_calculator() {};
    ~Coupon_bearing_calculator() {};
    
    Coupon_bearing_calculator(int frequency, double couponRate, double rate, int periodes);
    
    double calculate_bond_price(double rate);
    double calculate_bond_price();
    
    double calculate_bond_price_shift(double shift);
    
    double calculate_bond_dv01();
    double calculate_bond_dv01_shift(double shift);
};

#endif /* Calculator_h */
