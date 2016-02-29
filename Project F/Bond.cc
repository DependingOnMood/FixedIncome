//
//  Bond.cc
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#include "Bond.h"
#include "SBB_date.h"
#include "Yield_curve.h"
#include "Calculator.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void Bond::set_bond_state(SBB_instrument_fields* bond_record, YieldCurve* yield_curve, SBB_bond_ratings* ratings) {
    
    _bond_settlement_date = bond_record->SettlementDate();
    _bond_maturity_date = bond_record->MaturityDate();
    
    SBB_date from_dt;
    from_dt.set_from_yyyymmdd(_bond_settlement_date);
    
    SBB_date to_dt;
    to_dt.set_from_yyyymmdd(_bond_maturity_date);
    
    int semiannual_period_count = SBB_periods::get_number_of_periods_semiannual(from_dt, to_dt);
    _bond_term = semiannual_period_count >> 1;
    
    _bond_frequency = bond_record->Frequency();
    
    if (_bond_frequency == 2) {
        _bond_n = semiannual_period_count;
    } else {
        _bond_n = (semiannual_period_count * _bond_frequency) >> 1;
    }
    
    
    string securityID(bond_record->SecurityID());
    _bond_id = securityID;
    
    string Ticker(bond_record->Ticker());
    _bond_ticker = Ticker;
    
    _bond_coupon = bond_record->Coupon();
    
    _bond_rate_type = bond_record->ValType();
    
    if (_bond_rate_type == "YIELD"){
        _bond_original_rate = bond_record->Yield();
        _bond_rate = bond_record->Yield();

    }else{
        _bond_original_rate = bond_record->Spread();

        _bond_rate = bond_record->Spread()/100 + yield_curve->find_yield_rate(_bond_term);
    }

    if (_bond_coupon == 0)
    {
        _bond_type = "Zero Coupon";
        Zero_coupon_calculator calculator(_bond_frequency, _bond_rate, _bond_n);
        bond_calculator_ptr = &calculator;
    } else {
        _bond_type = "Coupon Bearing";
        Coupon_bearing_calculator calculator(_bond_frequency, _bond_coupon, _bond_rate, _bond_n);
        bond_calculator_ptr = &calculator;
    }
    
    _bond_quality = bond_record->Quality();
    
    _bond_amount = bond_record->Amount();
    _bond_price = bond_calculator_ptr->calculate_bond_price();
    _bond_dv01 = bond_calculator_ptr->calculate_bond_dv01();
    _bond_risk = _bond_dv01 * _bond_amount/100;
    _bond_LGD = ratings->LGD_given_SnP_Fitch(_bond_quality) * _bond_amount;
    
    _bond_market_value =  _bond_price/100*_bond_amount;
    
    //cout << "ccccc"<<endl;
    
    //For VaR
    read_historical_rate(_bond_id);
    
    if (_bond_rate_type == "YIELD"){
        //cout << "_bond_historical_size: " << _bond_historical.size() <<endl;
        for (int i = 1; i < _bond_historical.size(); i++){
            _bond_BP_change.push_back((_bond_historical[i].rate - _bond_historical[i-1].rate)*100);
        }
    }else{
        
        //cout << "yield_curve->get_yield_T()" << yield_curve->get_yield_T() <<endl;
        read_historical_T(yield_curve->get_yield_T());
        
        for (int i = 1; i < _bond_historical.size(); i++){
            double real_bond_historical_this = _bond_historical[i].rate/100 + _bond_sy_historical[i].rate;
            double real_bond_historical_last = _bond_historical[i-1].rate/100 + _bond_sy_historical[i-1].rate;
            _bond_BP_change.push_back((real_bond_historical_this - real_bond_historical_last)*100);
        }
        //cout << "ccccc22222"<<endl;
    }
    
    //cout << "_bond_BP_change.size(): " <<_bond_BP_change.size() <<endl;
    //cout << "bbbb"<<endl;

    for (int i = 0; i <_bond_BP_change.size(); i++){
        
        double _bond_new_rate = _bond_rate + _bond_BP_change[i]/100;
        
        Bond_calculator_interface* new_bond_calculator_ptr;
        
        if (_bond_coupon == 0)
        {
            Zero_coupon_calculator new_calculator(_bond_frequency, _bond_new_rate, _bond_n);
            new_bond_calculator_ptr = &new_calculator;
        } else {
            Coupon_bearing_calculator new_calculator(_bond_frequency, _bond_coupon, _bond_new_rate, _bond_n);
            new_bond_calculator_ptr = &new_calculator;
        }
        
        double _new_price = new_bond_calculator_ptr->calculate_bond_price();
        
        
        _bond_price_change.push_back(_new_price - _bond_price);
    }
    
    //cout << "_bond_price_change.size(): " <<_bond_price_change.size() <<endl;
    
    for (int i = 0; i <_bond_price_change.size(); i++){
        _bond_pnl.push_back((_bond_price_change[i] * _bond_amount)/100);
    }
    
//    cout << "dv01: " << _bond_dv01 <<endl;
//    
//    for (int i = 0; i <_bond_pnl.size(); i++){
//        cout << "_bond_pnl: " << _bond_pnl[i]<<endl;
//    }
//    
    //cout << "kkkkk"<<endl;
    

}

string Bond::get_bond_id() {
    return _bond_id;
}

string Bond::get_bond_ticker(){
    return _bond_ticker;
}

int Bond::get_bond_settlement_date(){
    return _bond_settlement_date;
}

int Bond::get_bond_maturity_date(){
    return _bond_maturity_date;
}

int Bond::get_bond_n(){
    return _bond_n;
}

double Bond::get_bond_coupon(){
    return _bond_coupon;
}

int Bond::get_bond_frequency(){
    return _bond_frequency;
}

double Bond::get_bond_rate(){
    return _bond_rate;
}

double Bond::get_bond_original_rate(){
    return _bond_original_rate;
}

string Bond::get_bond_type() {
    return _bond_type;
}

int Bond::get_bond_amount() {
    return _bond_amount;
};

string Bond::get_bond_rate_type(){
    return _bond_rate_type;
}

string Bond::get_bond_quality(){
    return _bond_quality;
}

double Bond::get_bond_price() {
    return _bond_price;
}

double Bond::get_bond_dv01() {
    return _bond_dv01;
}

double Bond::get_bond_risk() {
    return _bond_risk;
}

double Bond::get_bond_LGD() {
    return _bond_LGD;
}

double Bond::get_bond_market_value() {
    return _bond_market_value;
}

double Bond::get_bond_price_shift(double shift) {
    if (_bond_coupon == 0)
    {
        _bond_type = "Zero Coupon";
        Zero_coupon_calculator calculator(_bond_frequency, _bond_rate, _bond_n);
        bond_calculator_ptr = &calculator;
    } else {
        _bond_type = "Coupon Bearing";
        Coupon_bearing_calculator calculator(_bond_frequency, _bond_coupon, _bond_rate, _bond_n);
        bond_calculator_ptr = &calculator;
    }
    return bond_calculator_ptr->calculate_bond_price_shift(shift);
}

int Bond::get_bond_term() {
    return _bond_term;
}

void Bond::read_historical_rate(string bond_id){
    int date;
    string type;
    double rate;
    string bench;

    ifstream stream;
    stream.open(("./var/" + bond_id + ".txt").c_str());
    string line;
    while (getline(stream, line)){
        if (line[0] == '#') continue;
        istringstream tokens(line);
        tokens >> date >> type >> rate >> bench;
        date_historical my_date_historical;
        my_date_historical.date = date;
        my_date_historical.type = type;
        my_date_historical.rate = rate;
        my_date_historical.bench = bench;
        _bond_historical.push_back(my_date_historical);
    }
    stream.close();

}

void Bond::read_historical_T(string T_num){
    int date;
    string type;
    double rate;
    string bench;
    
    ifstream stream;
    stream.open(("./var/" + T_num +  ".txt").c_str());
    string line;
    while (getline(stream, line)){
        if (line[0] == '#') continue;
        istringstream tokens(line);
        tokens >> date >> type >> rate >> bench;
        date_historical my_date_historical;
        my_date_historical.date = date;
        my_date_historical.type = type;
        my_date_historical.rate = rate;
        my_date_historical.bench = bench;
        _bond_sy_historical.push_back(my_date_historical);
    }
    stream.close();
    
}

vector<double> Bond::get_bond_pnl(){
    return _bond_pnl;
}