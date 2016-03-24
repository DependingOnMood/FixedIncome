//
//  Instrument.cc
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#include <cmath>

#include "Instrument.h"
#include "SBB_ratings.h"
#include "Calculator.h"
#include "iostream"


using namespace std;

Instrument::Instrument(const char* tradingbook, const char* yieldcurve) {
    //cout<< "11111" << endl;

    _yieldcurve = new YieldCurve(yieldcurve);
    
    SBB_bond_ratings ratings;
    
    SBB_instrument_input_file file(tradingbook);
    
    SBB_instrument_fields *fields_array = file.records(_instrument_size);

    _bond_array = new Bond[_instrument_size];
    
    _instrument_risk = 0;
    _instrument_position = 0;
    _instrument_LGD = 0;
    _instrument_largest_long_position = 0;
    _instrument_largest_short_position = 0;
    _instrument_most_risk = 0;
    
    _instrument_market_value = 0;
    
    bool flag = true;
    
    //cout << "_instrument_size: " << _instrument_size << endl;
    for (int i = 0; i < _instrument_size; i++) {
        
        //cout<< &ratings  << " "<< _yieldcurve << " "<< &fields_array[i] << endl;
        
        _bond_array[i].set_bond_state(&fields_array[i], _yieldcurve, &ratings);
        
        double current_bond_risk = _bond_array[i].get_bond_risk();
        _instrument_risk += current_bond_risk;
        
        if (abs(current_bond_risk) > abs(_instrument_most_risk)) {
            _instrument_most_risk = current_bond_risk;
        }
        
        
        int current_bond_amount = _bond_array[i].get_bond_amount();
        _instrument_position += current_bond_amount;
        
        _instrument_LGD += _bond_array[i].get_bond_LGD();
        
        if (current_bond_amount > _instrument_largest_long_position) {
            _instrument_largest_long_position = current_bond_amount;
        }
        if (current_bond_amount < _instrument_largest_short_position) {
            _instrument_largest_short_position = current_bond_amount;
        }
        
        _instrument_market_value += _bond_array[i].get_bond_market_value();
        
        int term = _bond_array[i].get_bond_term();
        
        //cout << "term: " << term << endl;
        
        if (term > 0 && term <= 2) {
            bucket_0_2_years.push_back(i);
        } else if (term > 2 && term <= 5){
            bucket_2_5_years.push_back(i);
        } else if (term > 5 && term <= 10){
            bucket_5_10_years.push_back(i);
        } else if (term > 10) {
            bucket_10_30_years.push_back(i);
        }
        
        //for VaR
        
        //cout << "_bond_array[i].get_bond_pnl().size(): " << _bond_array[i].get_bond_pnl().size() << endl;
        
        if (flag == true){
            for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
                _instrument_pnl.push_back(0.0);
            }
            
            for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
                _instrument_yield_pnl.push_back(0.0);
            }
            
            for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
                _instrument_spread_pnl.push_back(0.0);
            }
            
            flag = false;
            
        }
        //cout << "_instrument_pnl.size(): " << _instrument_pnl.size() << endl;
        
        for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
            _instrument_pnl[j] = _instrument_pnl[j] + _bond_array[i].get_bond_pnl()[j];
        }
        
        //cout << (_bond_array[i].get_bond_rate_type() == "YIELD") << endl;
        if (_bond_array[i].get_bond_rate_type() == "YIELD"){
            
            //cout<< "_instrument_yield_pnl.size()"<< _instrument_yield_pnl.size() << endl;
            
            for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
                _instrument_yield_pnl[j] = _instrument_yield_pnl[j] + _bond_array[i].get_bond_pnl()[j];
            }
            
        }else{
            
            //cout<< "_instrument_spread_pnl.size()"<< _instrument_spread_pnl.size() << endl;
            
            for (int j = 0; j < _bond_array[i].get_bond_pnl().size(); j++){
                _instrument_spread_pnl[j] = _instrument_spread_pnl[j] + _bond_array[i].get_bond_pnl()[j];
            }
        }
    }
    
    sort(_instrument_pnl.begin(), _instrument_pnl.end());
    sort(_instrument_yield_pnl.begin(), _instrument_yield_pnl.end());
    sort(_instrument_spread_pnl.begin(), _instrument_spread_pnl.end());
    
//    for (int i = 0; i < _instrument_spread_pnl.size(); i++){
//        cout << _instrument_spread_pnl[i] << endl;
//    }
    
    conf_interval = 99;
    
    pnl_vector_index = trunc(_instrument_size - (conf_interval/100 * _instrument_size));
    
    _instrument_var = _instrument_pnl[pnl_vector_index]/1000;
    _instrument_yield_var = _instrument_yield_pnl[pnl_vector_index]/1000;
    _instrument_spread_var = _instrument_spread_pnl[pnl_vector_index]/1000;
    
//    cout << "pnl_vector_index: " << pnl_vector_index << endl;
  
    file.free_records();

}

Instrument::~Instrument() {
    free_bond_array();
}

double Instrument::get_instrument_position() {
    return _instrument_position;
}

int Instrument::get_instrument_size() {
    return _instrument_size;
}

double Instrument::get_instrument_risk() {
    return _instrument_risk;
}

double Instrument::get_instrument_LGD() {
    return _instrument_LGD;
}

int Instrument::get_instrument_largest_long_position(){
    return _instrument_largest_long_position;
}

int Instrument::get_instrument_largest_short_position(){
    return _instrument_largest_short_position;
}

double Instrument::get_instrument_most_risk(){
    return _instrument_most_risk;
}

vector<int> Instrument::get_bucket_0_2_years() {
    return bucket_0_2_years;
}

vector<int> Instrument::get_bucket_2_5_years() {
    return bucket_2_5_years;
}

vector<int> Instrument::get_bucket_5_10_years() {
    return bucket_5_10_years;
}

vector<int> Instrument::get_bucket_10_30_years() {
    return bucket_10_30_years;
}

Bond* Instrument::get_bond_array() {
    return _bond_array;
}

void Instrument::free_bond_array() {
    delete[] _bond_array;
}

//get_instrument_market_value
//----------------------
double Instrument::get_instrument_market_value(){
    return _instrument_market_value;
}

double Instrument::get_instrument_market_value_2_year(){
    double bucket_market_value = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        bucket_market_value += _bond_array[bucket_0_2_years[i]].get_bond_market_value();
    }
    
    return bucket_market_value;
}

double Instrument::get_instrument_market_value_5_year(){
    double bucket_market_value = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        bucket_market_value += _bond_array[bucket_2_5_years[i]].get_bond_market_value();
    }
    
    return bucket_market_value;
}

double Instrument::get_instrument_market_value_10_year(){
    double bucket_market_value = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        bucket_market_value += _bond_array[bucket_5_10_years[i]].get_bond_market_value();
    }
    
    return bucket_market_value;
}

double Instrument::get_instrument_market_value_30_year(){
    double bucket_market_value = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        bucket_market_value += _bond_array[bucket_10_30_years[i]].get_bond_market_value();
    }
    
    return bucket_market_value;
}

//get_instrument_risk
//----------------------
double Instrument::get_instrument_risk_2_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        bucket_risk += _bond_array[bucket_0_2_years[i]].get_bond_risk();
    }
    
    return bucket_risk;
}

double Instrument::get_instrument_risk_5_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        bucket_risk += _bond_array[bucket_2_5_years[i]].get_bond_risk();
    }
    
    return bucket_risk;
}

double Instrument::get_instrument_risk_10_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        bucket_risk += _bond_array[bucket_5_10_years[i]].get_bond_risk();
    }
    
    return bucket_risk;
}

double Instrument::get_instrument_risk_30_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        bucket_risk += _bond_array[bucket_10_30_years[i]].get_bond_risk();
    }
    
    return bucket_risk;
}

//get_2_year_hedge
//----------------------
double Instrument::get_2_year_hedge_amount_2_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        bucket_risk += _bond_array[bucket_0_2_years[i]].get_bond_risk();
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - bucket_risk / _yieldcurve->calculate_2_year_dv01();
}

double Instrument::get_2_year_hedge_amount_5_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        bucket_risk += _bond_array[bucket_2_5_years[i]].get_bond_risk();
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - bucket_risk / _yieldcurve->calculate_2_year_dv01();
}

double Instrument::get_2_year_hedge_amount_10_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        bucket_risk += _bond_array[bucket_5_10_years[i]].get_bond_risk();
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - bucket_risk / _yieldcurve->calculate_2_year_dv01();
}

double Instrument::get_2_year_hedge_amount_30_year() {
    double bucket_risk = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        bucket_risk += _bond_array[bucket_10_30_years[i]].get_bond_risk();
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - bucket_risk / _yieldcurve->calculate_2_year_dv01();
}

//get_shift_instrument_market_value
//----------------------
double Instrument::get_shift_instrument_market_value(double shift) {
    double shifted_total_market_value = 0.0;

    for (int i=0; i<_instrument_size; i++) {
        shifted_total_market_value += _bond_array[i].get_bond_price_shift(shift)/100 * _bond_array[i].get_bond_amount();
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_total_market_value;
}

double Instrument::get_shift_instrument_market_value_2_year(double shift) {
    double shifted_market_value = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        shifted_market_value += _bond_array[bucket_0_2_years[i]].get_bond_price_shift(shift)/100 * _bond_array[bucket_0_2_years[i]].get_bond_amount();
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_market_value;
}

double Instrument::get_shift_instrument_market_value_5_year(double shift) {
    double shifted_market_value = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        shifted_market_value += _bond_array[bucket_2_5_years[i]].get_bond_price_shift(shift)/100 * _bond_array[bucket_2_5_years[i]].get_bond_amount();
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_market_value;
}

double Instrument::get_shift_instrument_market_value_10_year(double shift) {
    double shifted_market_value = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        shifted_market_value += _bond_array[bucket_5_10_years[i]].get_bond_price_shift(shift)/100 * _bond_array[bucket_5_10_years[i]].get_bond_amount();
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_market_value;
}

double Instrument::get_shift_instrument_market_value_30_year(double shift) {
    double shifted_market_value = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        shifted_market_value += _bond_array[bucket_10_30_years[i]].get_bond_price_shift(shift)/100 * _bond_array[bucket_10_30_years[i]].get_bond_amount();
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_market_value;
}

//get_shift_instrument_risk
//----------------------
double Instrument::get_shift_instrument_risk_2_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        shifted_risk += _bond_array[bucket_0_2_years[i]].get_bond_risk() * (1 + shift);
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_risk;
}

double Instrument::get_shift_instrument_risk_5_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        shifted_risk += _bond_array[bucket_2_5_years[i]].get_bond_risk() * (1 + shift);
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_risk;
}

double Instrument::get_shift_instrument_risk_10_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        shifted_risk += _bond_array[bucket_5_10_years[i]].get_bond_risk() * (1 + shift);
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_risk;
}

double Instrument::get_shift_instrument_risk_30_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        shifted_risk += _bond_array[bucket_10_30_years[i]].get_bond_risk() * (1 + shift);
        //cout << "total_market_value: " << total_market_value << endl;
    }
    return shifted_risk;
}

//get_shift_2_year_hedge
//----------------------
double Instrument::get_shift_2_year_hedge_amount_2_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_0_2_years.size(); i++) {
        shifted_risk += _bond_array[bucket_0_2_years[i]].get_bond_risk() * (1 + shift);
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - shifted_risk / _yieldcurve->calculate_2_year_dv01_shift(shift);

}

double Instrument::get_shift_2_year_hedge_amount_5_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_2_5_years.size(); i++) {
        shifted_risk += _bond_array[bucket_2_5_years[i]].get_bond_risk() * (1 + shift);
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - shifted_risk / _yieldcurve->calculate_2_year_dv01_shift(shift);
}

double Instrument::get_shift_2_year_hedge_amount_10_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_5_10_years.size(); i++) {
        shifted_risk += _bond_array[bucket_5_10_years[i]].get_bond_risk() * (1 + shift);
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - shifted_risk / _yieldcurve->calculate_2_year_dv01_shift(shift);
}

double Instrument::get_shift_2_year_hedge_amount_30_year(double shift){
    double shifted_risk = 0.0;
    
    for (int i=0; i<bucket_10_30_years.size(); i++) {
        shifted_risk += _bond_array[bucket_10_30_years[i]].get_bond_risk() * (1 + shift);
    }
    
    //return - bucket_risk / _yieldcurve->calculate_2_year_dv01() * 100;
    
    //grader said off by 100
    return - shifted_risk / _yieldcurve->calculate_2_year_dv01_shift(shift);
}


//get_yield_rate
//----------------------
double Instrument::get_yield_rate_2_year(){
    return _yieldcurve -> find_2_year_yield_rate();
}

double Instrument::get_yield_rate_5_year(){
    return _yieldcurve -> find_5_year_yield_rate();
}

double Instrument::get_yield_rate_10_year(){
    return _yieldcurve -> find_10_year_yield_rate();
}

double Instrument::get_yield_rate_30_year(){
    return _yieldcurve -> find_30_year_yield_rate();
}

//get_instrument_var
//----------------------
double Instrument::get_instrument_var(){
    return _instrument_var;
}

double Instrument::get_instrument_yield_var(){
    return _instrument_yield_var;
}

double Instrument::get_instrument_spread_var(){
    return _instrument_spread_var;
}

//get_instrument_pnl_dist
//----------------------
vector<int> Instrument::get_instrument_pnl_dist(){
    double interval = (_instrument_pnl[_instrument_pnl.size() - 1] - _instrument_pnl[0])/8;
    
    vector<int> _instrument_pnl_dist;
    for (int i = 0; i < 8; i++) {
        _instrument_pnl_dist.push_back(0);
    }
    
    for (int i = 0; i < _instrument_pnl.size(); i++) {
        if ((_instrument_pnl[i] >= _instrument_pnl[0]) && (_instrument_pnl[i] <= _instrument_pnl[0]+ interval)){
            _instrument_pnl_dist[0] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + interval) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*2))){
            _instrument_pnl_dist[1] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*2)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*3))){
            _instrument_pnl_dist[2] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*3)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*4))){
            _instrument_pnl_dist[3] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*4)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*5))){
            _instrument_pnl_dist[4] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*5)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*6))){
            _instrument_pnl_dist[5] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*6)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*7))){
            _instrument_pnl_dist[6] ++;
        }
        else if ((_instrument_pnl[i] >= _instrument_pnl[0] + (interval*7)) && (_instrument_pnl[i] <= _instrument_pnl[0] + (interval*8))){
            _instrument_pnl_dist[7] ++;
        }
    }
    
    return _instrument_pnl_dist;
}