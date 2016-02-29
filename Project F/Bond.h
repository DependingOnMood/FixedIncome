//
//  Bond.h
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright © 2015 Dongbo Xiao. All rights reserved.
//

#ifndef Bond_h
#define Bond_h

#include "SBB_io.h"
#include "Yield_curve.h"
#include "SBB_ratings.h"
#include "Calculator.h"
#include <string>
#include <vector>
using namespace std;

#define RATETYPE_YIELD "YIELD"
#define RATETYPE_SPREAD "SPREAD"

class Bond
{
private:
    
    string _bond_id;
    string _bond_ticker;
    int _bond_settlement_date;
    int _bond_maturity_date;
    
    int _bond_term;
    
    Bond_calculator_interface* bond_calculator_ptr;
    
    int _bond_n;
    double _bond_coupon;
    int _bond_frequency;
    
    double _bond_rate;
    double _bond_original_rate;
    string _bond_type;
    string _bond_rate_type;
    
    const char* _bond_quality;
    
    double _bond_price;
    
    double _bond_dv01;
    double _bond_risk;
    
    double _bond_LGD;
    
    int _bond_amount;
    
    double _bond_market_value;
    
    struct date_historical{
        int date;
        string type;
        double rate;
        string bench;
    };
    
    vector<date_historical> _bond_historical;
    vector<date_historical> _bond_sy_historical;
    vector<double> _bond_BP_change;
    vector<double> _bond_price_change;
    vector<double> _bond_pnl;
    
public:
    Bond() {};
    ~Bond() {};
    
    void set_bond_state(SBB_instrument_fields* bond_record, YieldCurve* yield_curve, SBB_bond_ratings* ratings);
    
    string get_bond_id();
    string get_bond_ticker();
    int get_bond_settlement_date();
    int get_bond_maturity_date();
    
    int get_bond_term();
    
    int get_bond_n();
    double get_bond_coupon();
    int get_bond_frequency();
    double get_bond_rate();
    double get_bond_original_rate();
    string get_bond_type();
    int get_bond_amount();
    string get_bond_rate_type();
    string get_bond_quality();
    
    double get_bond_price();
    
    double get_bond_dv01();
    double get_bond_risk();
    
    double get_bond_LGD();
    
    double get_bond_market_value();
    
    double get_bond_price_shift(double shift);
    
    void read_historical_rate(string bond_id);
    void read_historical_T(string T_num);
    
    vector<double> get_bond_pnl();

};



#endif /* Bond_h */
