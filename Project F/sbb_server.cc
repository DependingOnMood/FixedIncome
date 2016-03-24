//
//  sbb_server.cc
//  Project F
//
//  Created by Dongbo Xiao on 10/20/15.
//  Copyright (c) 2015 Dongbo Xiao. All rights reserved.
//

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

#include "sbb_socket.h"
#include "SBB_util.h"
#include "Instrument.h"

const char* filename_tradingbook_opening = "tradingbook_opening.txt";
const char* filename_tradingbook_closing = "tradingbook_closing.txt";
const char* filename_yieldcurve = "yieldcurve.txt";
const char* filename_spreadadded = "spreadadded.txt";

//#include <iostream>

//========================================
//get_change_info
//========================================
string get_change_info(){
    
    //timer start
    SBB_util sbb;
    sbb.start_clock();
    
    struct info{
        string name;
        int amount;
        double risk;
        double LGD;
    };
    
    //for closing
    //--------------------------------
    Instrument* instrument_close = new Instrument(filename_tradingbook_closing, filename_yieldcurve);
    Bond* bond_array_close = instrument_close->get_bond_array();
    
    vector<string> quality_table;
    quality_table.empty();
    
    vector<info> ticker_group_close;
    vector<info> rating_group_close;
    ticker_group_close.empty();
    rating_group_close.empty();
    
    for (int i = 0; i < instrument_close->get_instrument_size(); i++){
        string bond_ticker_close = bond_array_close[i].get_bond_ticker();
        string bond_rating_close = bond_array_close[i].get_bond_quality();
        quality_table.push_back(bond_rating_close);
        int bond_amount_close = bond_array_close[i].get_bond_amount();
        double bond_risk_close = bond_array_close[i].get_bond_risk();
        double bond_LGD_close = bond_array_close[i].get_bond_LGD();
        
        //ticker group
        bool found_ticker = false;
        
        for (int j = 0; j < ticker_group_close.size(); j++){
            if (ticker_group_close[j].name == bond_ticker_close){
                found_ticker = true;
                ticker_group_close[j].amount = ticker_group_close[j].amount + bond_amount_close;
                ticker_group_close[j].risk = ticker_group_close[j].risk + bond_risk_close;
                ticker_group_close[j].LGD = ticker_group_close[j].LGD + bond_LGD_close;
            }
        }
        
        if (!found_ticker){
            info temp_info;
            temp_info.name = bond_ticker_close;
            temp_info.amount = bond_amount_close;
            temp_info.risk = bond_risk_close;
            temp_info.LGD = bond_LGD_close;
            
            ticker_group_close.push_back(temp_info);
        }
        
        //rating group
        bool found_rating = false;
        
        for (int j = 0; j < rating_group_close.size(); j++){
            //cout << rating_group_close[j].name << " " << bond_rating_close << endl;
            if (rating_group_close[j].name == bond_rating_close){
                found_rating = true;
                rating_group_close[j].amount = rating_group_close[j].amount + bond_amount_close;
                rating_group_close[j].risk = rating_group_close[j].risk + bond_risk_close;
                rating_group_close[j].LGD = rating_group_close[j].LGD + bond_LGD_close;
            }
        }
        
        if (!found_rating){
            info temp_info;
            temp_info.name = bond_rating_close;
            temp_info.amount = bond_amount_close;
            temp_info.risk = bond_risk_close;
            temp_info.LGD = bond_LGD_close;
            
            rating_group_close.push_back(temp_info);
        }
        
    }
    
    double spread_var_close = instrument_close->get_instrument_spread_var();
    double yield_var_close = instrument_close->get_instrument_yield_var();
    double total_var_close = instrument_close->get_instrument_var();

    
    //for opening
    //--------------------------------
    Instrument* instrument_open = new Instrument(filename_tradingbook_opening, filename_yieldcurve);
    
    Bond* bond_array_open = instrument_open->get_bond_array();
    
    vector<info> ticker_group_open;
    vector<info> rating_group_open;
    ticker_group_open.empty();
    rating_group_open.empty();
    
    for (int i = 0; i < instrument_open->get_instrument_size(); i++){
        string bond_ticker_open = bond_array_open[i].get_bond_ticker();
        string bond_rating_open = quality_table[i];
        int bond_amount_open = bond_array_open[i].get_bond_amount();
        double bond_risk_open = bond_array_open[i].get_bond_risk();
        double bond_LGD_open = bond_array_open[i].get_bond_LGD();
        
        //cout << bond_array_open[i].get_bond_quality() << endl;;
        
        //ticker group
        bool found_ticker = false;
        
        for (int j = 0; j < ticker_group_open.size(); j++){
            if (ticker_group_open[j].name == bond_ticker_open){
                found_ticker = true;
                ticker_group_open[j].amount = ticker_group_open[j].amount + bond_amount_open;
                ticker_group_open[j].risk = ticker_group_open[j].risk + bond_risk_open;
                ticker_group_open[j].LGD = ticker_group_open[j].LGD + bond_LGD_open;
            }
        }
        
        if (!found_ticker){
            info temp_info;
            temp_info.name = bond_ticker_open;
            temp_info.amount = bond_amount_open;
            temp_info.risk = bond_risk_open;
            temp_info.LGD = bond_LGD_open;
            
            ticker_group_open.push_back(temp_info);
        }
        
        //rating group
        bool found_rating = false;
        
        for (int j = 0; j < rating_group_open.size(); j++){
            //cout << rating_group_open[j].name << " " << bond_rating_open << endl;
            if (rating_group_open[j].name == bond_rating_open){
                found_rating = true;
                rating_group_open[j].amount = rating_group_open[j].amount + bond_amount_open;
                rating_group_open[j].risk = rating_group_open[j].risk + bond_risk_open;
                rating_group_open[j].LGD = rating_group_open[j].LGD + bond_LGD_open;
            }
        }
        
        if (!found_rating){
            info temp_info;
            temp_info.name = bond_rating_open;
            temp_info.amount = bond_amount_open;
            temp_info.risk = bond_risk_open;
            temp_info.LGD = bond_LGD_open;
            
            rating_group_open.push_back(temp_info);
        }
        
    }
    
    double spread_var_open = instrument_open->get_instrument_spread_var();
    double yield_var_open = instrument_open->get_instrument_yield_var();
    double total_var_open = instrument_open->get_instrument_var();

    
    //for change
    //--------------------------------
    vector<info> ticker_group_change;
    vector<info> rating_group_change;
    ticker_group_change.empty();
    rating_group_change.empty();
    
    //cout << "ticker_group_close.size(): " << ticker_group_close.size() << endl;
    
    for (int i = 0; i < ticker_group_close.size(); i++){
        info temp_info;
        temp_info.name = ticker_group_close[i].name;
        temp_info.amount = ticker_group_close[i].amount -  ticker_group_open[i].amount;
        temp_info.risk = ticker_group_close[i].risk -  ticker_group_open[i].risk;
        temp_info.LGD = ticker_group_close[i].LGD -  ticker_group_open[i].LGD;
        
        ticker_group_change.push_back(temp_info);

    }
    
    
    for (int i = 0; i < rating_group_close.size(); i++){
        info temp_info;
        temp_info.name = rating_group_close[i].name;
        temp_info.amount = rating_group_close[i].amount -  rating_group_open[i].amount;
        temp_info.risk = rating_group_close[i].risk -  rating_group_open[i].risk;
        temp_info.LGD = rating_group_close[i].LGD -  rating_group_open[i].LGD;
        rating_group_change.push_back(temp_info);
        
    }
    
    cout << "eeeee" << endl;
    
    double spread_var_change = spread_var_close -spread_var_open;
    double yield_var_change = yield_var_close - yield_var_open;
    double total_var_change = total_var_close - total_var_open;
    
    //timer end
    double real_time, user_time, system_time;
    sbb.end_clock(real_time, user_time, system_time);
    sbb.end_clock();
    
    //string builder
    //--------------------------------
    
    //ticker string
    string ticker_name_string = "";
    string open_amount_ticker_string = "";
    string open_risk_ticker_string = "";
    string open_LGD_ticker_string = "";
    string close_amount_ticker_string = "";
    string close_risk_ticker_string = "";
    string close_LGD_ticker_string = "";
    string change_amount_ticker_string = "";
    string change_risk_ticker_string = "";
    string change_LGD_ticker_string = "";
    for (int i = 0; i < ticker_group_change.size(); i++){
        ticker_name_string += ticker_group_open[i].name + " ";
        open_amount_ticker_string += std::to_string(ticker_group_open[i].amount) + " ";
        open_risk_ticker_string += std::to_string(ticker_group_open[i].risk) + " ";
        open_LGD_ticker_string += std::to_string(ticker_group_open[i].LGD) + " ";
        close_amount_ticker_string += std::to_string(ticker_group_close[i].amount) + " ";
        close_risk_ticker_string += std::to_string(ticker_group_close[i].risk) + " ";
        close_LGD_ticker_string += std::to_string(ticker_group_close[i].LGD) + " ";
        change_amount_ticker_string += std::to_string(ticker_group_change[i].amount) + " ";
        change_risk_ticker_string += std::to_string(ticker_group_change[i].risk) + " ";
        change_LGD_ticker_string += std::to_string(ticker_group_change[i].LGD) + " ";
    }
    
    string ticker_string = "";
    ticker_string = ticker_name_string + "\n" +
                    open_amount_ticker_string +"\n" +
                    open_risk_ticker_string + "\n" +
                    open_LGD_ticker_string + "\n" +
                    close_amount_ticker_string + "\n" +
                    close_risk_ticker_string + "\n" +
                    close_LGD_ticker_string + "\n" +
                    change_amount_ticker_string + "\n" +
                    change_risk_ticker_string + "\n" +
                    change_LGD_ticker_string + "\n";
    
    //rating string
    
    string rating_name_string = "";
    string open_amount_rating_string = "";
    string open_risk_rating_string = "";
    string open_LGD_rating_string = "";
    string close_amount_rating_string = "";
    string close_risk_rating_string = "";
    string close_LGD_rating_string = "";
    string change_amount_rating_string = "";
    string change_risk_rating_string = "";
    string change_LGD_rating_string = "";
    for (int i = 0; i < rating_group_change.size(); i++){
        rating_name_string += rating_group_open[i].name + " ";
        open_amount_rating_string += std::to_string(rating_group_open[i].amount) + " ";
        open_risk_rating_string += std::to_string(rating_group_open[i].risk) + " ";
        open_LGD_rating_string += std::to_string(rating_group_open[i].LGD) + " ";
        close_amount_rating_string += std::to_string(rating_group_close[i].amount) + " ";
        close_risk_rating_string += std::to_string(rating_group_close[i].risk) + " ";
        close_LGD_rating_string += std::to_string(rating_group_close[i].LGD) + " ";
        change_amount_rating_string += std::to_string(rating_group_change[i].amount) + " ";
        change_risk_rating_string += std::to_string(rating_group_change[i].risk) + " ";
        change_LGD_rating_string += std::to_string(rating_group_change[i].LGD) + " ";
    }
    
    string rating_string = "";
    rating_string = rating_name_string + "\n" +
                    open_amount_rating_string +"\n" +
                    open_risk_rating_string + "\n" +
                    open_LGD_rating_string + "\n" +
                    close_amount_rating_string + "\n" +
                    close_risk_rating_string + "\n" +
                    close_LGD_rating_string + "\n" +
                    change_amount_rating_string + "\n" +
                    change_risk_rating_string + "\n" +
                    change_LGD_rating_string + "\n";
    
    //var string
    string var_string = "";
    
    var_string += (string("VaR-credit(spread) ") + "VaR-rate(yield) " + "VaR-total\n");
    
    var_string += std::to_string(spread_var_open) + " " + std::to_string(yield_var_open)  + " " + std::to_string(total_var_open) + "\n";
    
    var_string += std::to_string(spread_var_close) + " " + std::to_string(yield_var_close)  + " " + std::to_string(total_var_close) + "\n";
    
    var_string += std::to_string(spread_var_change) + " " + std::to_string(yield_var_change)  + " " + std::to_string(total_var_change) + "\n";
    
    //time string
    string time_string = "";
    time_string = std::to_string(real_time) + "\n" + std::to_string(user_time)  + "\n" + std::to_string(system_time) + "\n";
    
    //final string
    string final_string = "";
    final_string = ticker_string + rating_string + var_string + time_string;
    
    return final_string;
}



//========================================
//get_risk_info
//========================================
string get_risk_info(double spread_2_year, double spread_5_year, double spread_10_year, double spread_30_year){
    
    //timer start
    SBB_util sbb;
    sbb.start_clock();
    
    //build a new file for spread added
    FILE* spreadadded_File = fopen (filename_spreadadded,"w");
    if (!spreadadded_File){
        fprintf(stderr,"ERROR couldn't open: %s\n", filename_spreadadded);
        throw std::runtime_error("file open failure");
    }
    
    fprintf(spreadadded_File,"%.2f %.2f %.2f %.2f\n",
            spread_30_year, spread_2_year, spread_5_year, spread_10_year);
    
    if (fclose(spreadadded_File)) {
        fprintf(stderr,"flose failed on file %s errno: %d\n", filename_spreadadded, errno);
    }
    
    Instrument* instrument_close = new Instrument(filename_tradingbook_closing, filename_yieldcurve);
    
    double risk_2_year = instrument_close -> get_instrument_risk_2_year();
    double risk_5_year = instrument_close -> get_instrument_risk_5_year();
    double risk_10_year = instrument_close -> get_instrument_risk_10_year();
    double risk_30_year = instrument_close -> get_instrument_risk_30_year();
    
    double market_value_2_year = instrument_close -> get_instrument_market_value_2_year();
    double market_value_5_year = instrument_close -> get_instrument_market_value_5_year();
    double market_value_10_year = instrument_close -> get_instrument_market_value_10_year();
    double market_value_30_year = instrument_close -> get_instrument_market_value_30_year();
    
    double hedge_2_year = instrument_close -> get_2_year_hedge_amount_2_year();
    double hedge_5_year = instrument_close -> get_2_year_hedge_amount_5_year();
    double hedge_10_year = instrument_close -> get_2_year_hedge_amount_10_year();
    double hedge_30_year = instrument_close -> get_2_year_hedge_amount_30_year();
    
    double yield_2_year = instrument_close ->get_yield_rate_2_year();
    double yield_5_year = instrument_close ->get_yield_rate_5_year();
    double yield_10_year = instrument_close ->get_yield_rate_10_year();
    double yield_30_year = instrument_close ->get_yield_rate_30_year();
    
    //timer end
    double real_time, user_time, system_time;
    sbb.end_clock(real_time, user_time, system_time);
    sbb.end_clock();
    
    //time string
    string time_string = "";
    time_string = std::to_string(real_time) + " " + std::to_string(user_time)  + " " + std::to_string(system_time) + "\n";
    
    //final string
    string final_string = "";
    final_string += std::to_string(risk_2_year) + " " + std::to_string(market_value_2_year)  + " " +
                    std::to_string(risk_5_year) + " " + std::to_string(market_value_5_year)  + " " +
                    std::to_string(risk_10_year) + " " + std::to_string(market_value_10_year)  + " " +
                    std::to_string(risk_30_year) + " " + std::to_string(market_value_30_year)  + " ";
    final_string += std::to_string(hedge_2_year) + " " + std::to_string(hedge_5_year)  + " " +
                    std::to_string(hedge_10_year) + " " + std::to_string(hedge_30_year)  + " ";
    final_string += std::to_string(yield_2_year) + " " + std::to_string(yield_5_year)  + " " +
                    std::to_string(yield_10_year) + " " + std::to_string(yield_30_year)  + " ";
    final_string += time_string;
    
    return final_string;
}

//========================================
//get_risk_info_with_shift
//========================================
string get_risk_info_with_shift(double shift, double spread_2_year, double spread_5_year, double spread_10_year, double spread_30_year){
    
    //timer start
    SBB_util sbb;
    sbb.start_clock();
    
    //build a new file for spread added
    FILE* spreadadded_File = fopen (filename_spreadadded,"w");
    if (!spreadadded_File){
        fprintf(stderr,"ERROR couldn't open: %s\n", filename_spreadadded);
        throw std::runtime_error("file open failure");
    }
    
    fprintf(spreadadded_File,"%.2f %.2f %.2f %.2f\n",
            spread_30_year, spread_2_year, spread_5_year, spread_10_year);
    
    if (fclose(spreadadded_File)) {
        fprintf(stderr,"flose failed on file %s errno: %d\n", filename_spreadadded, errno);
    }
    
    Instrument* instrument_close = new Instrument(filename_tradingbook_closing, filename_yieldcurve);
    
    double risk_2_year = instrument_close -> get_shift_instrument_risk_2_year(shift);
    double risk_5_year = instrument_close -> get_shift_instrument_risk_5_year(shift);
    double risk_10_year = instrument_close -> get_shift_instrument_risk_10_year(shift);
    double risk_30_year = instrument_close -> get_shift_instrument_risk_30_year(shift);
    
    double market_value_2_year = instrument_close -> get_shift_instrument_market_value_2_year(shift);
    double market_value_5_year = instrument_close -> get_shift_instrument_market_value_5_year(shift);
    double market_value_10_year = instrument_close -> get_shift_instrument_market_value_10_year(shift);
    double market_value_30_year = instrument_close -> get_shift_instrument_market_value_30_year(shift);
    
    double hedge_2_year = instrument_close -> get_shift_2_year_hedge_amount_2_year(shift);
    double hedge_5_year = instrument_close -> get_shift_2_year_hedge_amount_5_year(shift);
    double hedge_10_year = instrument_close -> get_shift_2_year_hedge_amount_10_year(shift);
    double hedge_30_year = instrument_close -> get_shift_2_year_hedge_amount_30_year(shift);
    
    double yield_2_year = instrument_close ->get_yield_rate_2_year();
    double yield_5_year = instrument_close ->get_yield_rate_5_year();
    double yield_10_year = instrument_close ->get_yield_rate_10_year();
    double yield_30_year = instrument_close ->get_yield_rate_30_year();
    
    
    //timer end
    double real_time, user_time, system_time;
    sbb.end_clock(real_time, user_time, system_time);
    sbb.end_clock();
    
    //time string
    string time_string = "";
    time_string = std::to_string(real_time) + " " + std::to_string(user_time)  + " " + std::to_string(system_time) + "\n";
    
    //final string
    string final_string = "";
    final_string += std::to_string(risk_2_year) + " " + std::to_string(market_value_2_year)  + " " +
    std::to_string(risk_5_year) + " " + std::to_string(market_value_5_year)  + " " +
    std::to_string(risk_10_year) + " " + std::to_string(market_value_10_year)  + " " +
    std::to_string(risk_30_year) + " " + std::to_string(market_value_30_year)  + " ";
    final_string += std::to_string(hedge_2_year) + " " + std::to_string(hedge_5_year)  + " " +
    std::to_string(hedge_10_year) + " " + std::to_string(hedge_30_year)  + " ";
    final_string += std::to_string(yield_2_year) + " " + std::to_string(yield_5_year)  + " " +
    std::to_string(yield_10_year) + " " + std::to_string(yield_30_year)  + " ";
    final_string += time_string;
    
    return final_string;
}



string get_pnl_dist_info(){
    
    Instrument* instrument_close = new Instrument(filename_tradingbook_closing, filename_yieldcurve);
    
    vector<int> instrument_pnl_dist = instrument_close -> get_instrument_pnl_dist();
    
    string final_string = "";
    
    for (int i = 0; i < instrument_pnl_dist.size(); i++){
        final_string += " " + std::to_string(instrument_pnl_dist[i]);
    }
    
    final_string += "\n";
    
    return final_string;
}

int main() {
    //may need to add while true here to keep server alive
    
    /*
     * get an internet domain socket
     */
    int sd;
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    /*
     * set up the socket structure
     */
    struct sockaddr_in	sock_addr;
    
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    
#ifdef SBB_ANY
    /* set to INADDR_ANY if want server to be open to any client on any machine */
    sock_addr.sin_addr.s_addr = INADDR_ANY;
#else
    char	hostname[128];
    /*
     *  we'll default to this host and call a section 3 func to get this host
     */
    if( gethostname(hostname,sizeof(hostname)) ){
        fprintf(stderr," SBB gethostname(...) failed errno: %d\n", errno);
        exit(1);
    }
    printf("SBB gethostname() local hostname: \"%s\"\n", hostname);
    
    /*
     * set up socket structure for our host machine
     */
    struct hostent *hp;
    
    if ((hp = gethostbyname(hostname)) == 0) {
        fprintf(stderr,"SBB gethostbyname(...) failed errno: %d exiting...\n", errno);
        exit(1);
    }
    sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
#endif
    sock_addr.sin_port = htons(PORT);
    
    /*
     * bind the socket to the port number
     */
    if (::bind(sd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
        perror("bind");
        exit(1);
    }
    
    /*
     * advertise we are available on this socket/port
     */
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }
    
    /*
     * wait for a client to connect
     */
    struct sockaddr_in	sock_addr_from_client;
    socklen_t addrlen = sizeof(sock_addr_from_client);
    int sd_current;
    if ((sd_current = accept(sd, (struct sockaddr *)  &sock_addr_from_client, &addrlen)) == -1) {
        fprintf(stderr,"SBB accept(...) failed errno: %d  exiting...\n", errno);
        exit(1);
    }
    printf("SBB client ip address: %s port: %x\n",
           inet_ntoa(sock_addr_from_client.sin_addr),
           PORT);
    
    //		ntohs(sock_addr_from_client.sin_port));
    
    /*
     * block on socket waiting for client message
     */
    
    int ret = 1;//server close if ret ==0, other value make it open
    
    char	msg[MSGSIZE];
    fprintf(stderr," SBB: sizeof msg: %lu\n", sizeof(msg));
    
    while ( (ret = recv(sd_current, msg, sizeof(msg), 0) > 0)) {
        printf("SBB server received msg: [%s] from client\n", msg);
        
        string recived_msg(msg);
        
        string final_result = "";
        
        //should respond here
        if (recived_msg.substr(0,12) == "get_change()"){
            final_result = get_change_info();
            std::cout << "result:\n" << final_result << endl;
            
        } else if (recived_msg.substr(0,14) == "get_pnl_dist()"){
            final_result = get_pnl_dist_info();
            std::cout << "result:\n" << final_result << endl;
            
        } else if (recived_msg.substr(0,9) == "get_risk(") {
            size_t start_pos = recived_msg.find("(");
            size_t end_pos = recived_msg.find(")");
            string value_string = recived_msg.substr(start_pos+1, end_pos - start_pos+1);
            
            std::vector<double> value_vect;
            std::istringstream buf(value_string);
            for(std::string token; getline(buf, token, ','); )
            value_vect.push_back(stod(token));
            
            final_result = get_risk_info(value_vect[0], value_vect[1], value_vect[2], value_vect[3]);
            std::cout << "result:\n" << final_result << endl;
        } else if (recived_msg.substr(0,20) == "get_risk_with_shift("){
            
            size_t start_pos = recived_msg.find("(");
            size_t end_pos = recived_msg.find(")");
            string value_string = recived_msg.substr(start_pos+1, end_pos - start_pos+1);
            
            std::vector<double> value_vect;
            std::istringstream buf(value_string);
            for(std::string token; getline(buf, token, ','); )
                value_vect.push_back(stod(token));
            
            final_result = get_risk_info_with_shift(value_vect[0], value_vect[1], value_vect[2], value_vect[3], value_vect[4]);
            std::cout << "result:\n" << final_result << endl;
        }
        
        strcpy(msg,final_result.c_str());
        
        
        /*
         * ack back to the client
         */
        
        if (send(sd_current, msg, strlen(msg), 0) == -1) {
            fprintf(stderr,"SBB send(...) failed errno: %d exiting...\n", errno);
            exit(1);
        }

    }
    
    if( 0 == ret ) {
        printf("SBB ciient exited...\n");
        /* For TCP sockets
         * the return value 0 means the peer has closed its half side of the connection
         */
    }
    else if( -1 == ret ) {
        fprintf(stderr,"SBB recv(...) returned failed - errno: %d exiting...\n", errno);
        exit(1);
    }
    
    close(sd_current);
    close(sd);
    
    //string final_result = get_change_info();
    //string final_result = get_risk_info(0, 0, 0, 0);
    //string final_result = get_risk_info_with_shift(0.5,2,3,4,5);
    //string final_result = get_pnl_dist_info();
    
    //cout <<  final_result << endl;
}
