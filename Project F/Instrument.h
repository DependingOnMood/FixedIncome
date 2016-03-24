//
//  Instrument.h
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#ifndef Instrument_h
#define Instrument_h

#include "Bond.h"
#include "Yield_curve.h"
#include <vector>

class Instrument {
private:
    YieldCurve* _yieldcurve;
    Bond *_bond_array;
    int _instrument_size;
    double _instrument_risk;
    double _instrument_position;
    double _instrument_LGD;
    
    int _instrument_largest_long_position;
    int _instrument_largest_short_position;
    double _instrument_most_risk;
    
    double _instrument_market_value;
    
    vector<int> bucket_0_2_years;
    vector<int> bucket_2_5_years;
    vector<int> bucket_5_10_years;
    vector<int> bucket_10_30_years;
    
    double conf_interval;
    int pnl_vector_index;
    
    vector<double> _instrument_pnl;
    vector<double> _instrument_spread_pnl;
    vector<double> _instrument_yield_pnl;
    
    double _instrument_var;
    double _instrument_spread_var;
    double _instrument_yield_var;
    
    
public:
    Instrument(const char* tradingbook, const char* yieldcurve);
    Instrument();
    ~Instrument();
    
    int get_instrument_size();
    double get_instrument_position();
    double get_instrument_risk();
    double get_instrument_LGD();
    
    vector<int> get_bucket_0_2_years();
    vector<int> get_bucket_2_5_years();
    vector<int> get_bucket_5_10_years();
    vector<int> get_bucket_10_30_years();
    
    int get_instrument_largest_long_position();
    int get_instrument_largest_short_position();
    double get_instrument_most_risk();
    
    double get_instrument_market_value();
    double get_instrument_market_value_2_year();
    double get_instrument_market_value_5_year();
    double get_instrument_market_value_10_year();
    double get_instrument_market_value_30_year();
    
    double get_instrument_risk_2_year();
    double get_instrument_risk_5_year();
    double get_instrument_risk_10_year();
    double get_instrument_risk_30_year();
    
    //double get_2_year_hedge_amount();
    double get_2_year_hedge_amount_2_year();
    double get_2_year_hedge_amount_5_year();
    double get_2_year_hedge_amount_10_year();
    double get_2_year_hedge_amount_30_year();
    
    double get_shift_instrument_market_value(double shift);
    double get_shift_instrument_market_value_2_year(double shift);
    double get_shift_instrument_market_value_5_year(double shift);
    double get_shift_instrument_market_value_10_year(double shift);
    double get_shift_instrument_market_value_30_year(double shift);
    
    double get_shift_instrument_risk_2_year(double shift);
    double get_shift_instrument_risk_5_year(double shift);
    double get_shift_instrument_risk_10_year(double shift);
    double get_shift_instrument_risk_30_year(double shift);
    
    double get_shift_2_year_hedge_amount_2_year(double shift);
    double get_shift_2_year_hedge_amount_5_year(double shift);
    double get_shift_2_year_hedge_amount_10_year(double shift);
    double get_shift_2_year_hedge_amount_30_year(double shift);
    
    double get_yield_rate_2_year();
    double get_yield_rate_5_year();
    double get_yield_rate_10_year();
    double get_yield_rate_30_year();
    
    Bond* get_bond_array();
    
    void free_bond_array();
    
    double get_instrument_var();
    double get_instrument_yield_var();
    double get_instrument_spread_var();
    vector<int> get_instrument_pnl_dist();
};

#endif /* Instrument_h */
