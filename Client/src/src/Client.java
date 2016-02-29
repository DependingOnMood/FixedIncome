/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package src;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Dongbo
 */
public class Client {
    
    //static String res1;
    //static String res2;
    //static String res3;
    
    /**
     * @param args the command line arguments
     */
    
    
    
    
    
    public static void main(String[] args) {
       
//        RiskByMaturityFrame test = new RiskByMaturityFrame();
//        test.setVisible(true);
    Socket socket = new Socket();
    try{
    socket = new Socket("Dongbos-MacBook-Pro.local",0x1234);
    openpage mainPage = new openpage(socket);
    mainPage.setVisible(true);
//    DataOutputStream out= new DataOutputStream(socket.getOutputStream());
//    BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//    out.write("get_change()".getBytes());
//    System.out.println(in.readLine());
//    res1 = in.readLine();
//    out.write("get_risk_with_original()".getBytes());
//    res2 = in.readLine();
  


//    out.write("get_pnl_dist()".getBytes());
//    res3 = in.readLine();
//    //System.out.println(res);
//    socket.close();
    
    
    
    } catch(Exception e){
        e.printStackTrace();
    }
        
        
        
        
    }
    
}
