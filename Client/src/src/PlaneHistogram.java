/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package src;
//package com.image.histogram; 
import static com.sun.org.apache.xalan.internal.xsltc.compiler.util.Type.String;
import java.awt.Color;  
import java.awt.Font;  
import java.awt.FontMetrics;  
import java.awt.Graphics;  
import java.awt.image.BufferedImage;  
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;  
import java.io.IOException;  
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.ArrayList;
  
import javax.imageio.ImageIO; 
import static src.DailyChangeFrame.dailychange_serverinput;
/**
 *
 * @author apple
 */ 
   
  
/** 
 * 柱状图 
 * 
 *  
 */  
public class PlaneHistogram { 
    Socket socket;
    String str3 ;
    DataOutputStream out;
    BufferedReader in; 
    
    //parse the server
    static String var_serverinput;
    static String[] var_seg;
    private final int histogramWidth = 15;// 柱形图的宽度  
    private final int histogramPitch = 10;// 柱形图的间距  
    private float scaling = 1f;// 缩放的比例  
    private int maxStrWidth = 0; // 字符串需要的最大宽度  
  
    /** 
     * <pre> 
     *   参数b[i]和str[i]必须对应 
     * </pre> 
     * 
     * @param g 
     * @param title 
     * @param v 
     * @param str 
     * @param color 
     *            可以为空 
     */ 
        public BufferedImage paintPlaneHistogram(String title, int[] v,  
            String[] str, Color[] color) {  
        int width = str.length * histogramWidth+str.length*histogramPitch+50;  
        int height = 255;  
        scaling = calculateScale(v, height);//计算缩放比例  
          
        BufferedImage bufferImage = new BufferedImage(width, height,  
                BufferedImage.TYPE_INT_RGB);  
        Graphics g = bufferImage.getGraphics();  
        g.setColor(Color.WHITE);  
        g.fillRect(0, 0, width, height);  
        FontMetrics metrics = null;  
  
        g.setFont(new Font(null, Font.BOLD, 18));  
        g.setColor(Color.RED);  
  
        g.drawString(title, (bufferImage.getWidth() - g.getFontMetrics()  
                .stringWidth(title)) >> 1, 30);// 画标题  
  
        g.setFont(new Font(null, Font.PLAIN, 12));  
  
        metrics = g.getFontMetrics();  
  
        g.setColor(Color.BLACK);  
  
        g.drawLine(10, 0, 10, height - 15); // 画Y坐标  
        g.drawLine(10, height - 15, width, height - 15);// 画X坐标  
          
        int j = 0;  
        int colorCount=color.length;  
          
        for (int i = 0; i < v.length; ++i) {  
  
            if (color != null){  
                g.setColor(color[j]);// 设置前景色  
                if(j+1<colorCount){  
                    j++;  
                }else{  
                    j=0;  
                }  
            }else{  
                g.setColor(Color.RED);  
            }  
  
            int x = 20 + i  
                    * (histogramPitch + histogramWidth + (maxStrWidth >> 1));// 计算出X坐标  
            int y = height - 16 - (int) (v[i] * scaling); // 计算出Y坐标  
  
            // 画占的比例  
            g.drawString(v[i] + "", x  
                    - ((metrics.stringWidth(v[i] + "") - histogramWidth) >> 1),  
                    y);  
  
            // 画平面的柱状图  
            g.drawRect(x, y, histogramWidth, (int) (v[i] * scaling));  
            g.fillRect(x, y, histogramWidth, (int) (v[i] * scaling));  
  
            // 画每一项表示的东西  
            g.drawString(str[i], x  
                    - ((metrics.stringWidth(str[i]) - histogramWidth) >> 1),  
                    height - 2);  
        }  
  
        return bufferImage;  
    }  
      
    /** 
     * 计算缩放比例 
     * @param v 
     * @param h 图片的高度 
     * @return 
     */  
    public float calculateScale(int[] v , int h){  
        float scale = 1f;  
        int max = Integer.MIN_VALUE;  
        for(int i=0 , len=v.length ; i < len ;++i){  
            if(v[i]>h && v[i]>max){  
                max=v[i];  
            }  
        }  
        if(max > h){  
            scale=((int)(h*1.0f/max*1000))*1.0f/1000;  
        }  
        return scale;  
    }  
      
    public PlaneHistogram(Socket socket) throws IOException  {  
        this.socket = socket; 
    out= new DataOutputStream(socket.getOutputStream());
    in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    out.write("get_pnl_dist()".getBytes());
    str3 = in.readLine().trim();
     System.out.println(str3);
       //str1 = "FM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM GX CB SM\nFM\nCB\nSM\n";
     //   str3 = "1 2 3 4 5 6 7 8";
        var_serverinput = str3;
        var_seg = var_serverinput.split(" ");
        //PlaneHistogram planeHistogram = new PlaneHistogram();  
       
     
        int[] int_var = new int[8];
        
       
        for (int i =0;i<var_seg.length;i++){
          int_var[i] = Integer.parseInt(var_seg[i]);  
        };
//        PlaneHistogram planeHistogram = new PlaneHistogram(socket);
        
        BufferedImage image = this.paintPlaneHistogram("pnl",  
                        new int[]{int_var[0],int_var[1],int_var[2],int_var[3],int_var[4],int_var[5],int_var[6],int_var[7]}, 
                        new String[]{"1","2","3","4","5","6","7","8"} ,  
                        new Color[] {Color.RED, Color.GREEN, Color.BLACK, Color.BLUE });  
        File output = new File("src/333.jpg");  
        try {  
            ImageIO.write(image, "jpg", output);  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
    }  
  
}       
    
    
   
    
    
    
    
    
    
    
    
    
    
    
    
    
//    public BufferedImage paintPlaneHistogram(String title, int[] v,  
//            String[] str, Color[] color) {  
//        int width = str.length * histogramWidth+str.length*histogramPitch+50;  
//        int height = 255;  
//        scaling = calculateScale(v, height);//计算缩放比例  
//          
//        BufferedImage bufferImage = new BufferedImage(width, height,  
//                BufferedImage.TYPE_INT_RGB);  
//        Graphics g = bufferImage.getGraphics();  
//        g.setColor(Color.WHITE);  
//        g.fillRect(0, 0, width, height);  
//        FontMetrics metrics = null;  
//  
//        g.setFont(new Font(null, Font.BOLD, 18));  
//        g.setColor(Color.RED);  
//  
//        g.drawString(title, (bufferImage.getWidth() - g.getFontMetrics()  
//                .stringWidth(title)) >> 1, 30);// 画标题  
//  
//        g.setFont(new Font(null, Font.PLAIN, 12));  
//  
//        metrics = g.getFontMetrics();  
//  
//        g.setColor(Color.BLACK);  
//  
//        g.drawLine(10, 0, 10, height - 15); // 画Y坐标  
//        g.drawLine(10, height - 15, width, height - 15);// 画X坐标  
//          
//        int j = 0;  
//        int colorCount=color.length;  
//          
//        for (int i = 0; i < v.length; ++i) {  
//  
//            if (color != null){  
//                g.setColor(color[j]);// 设置前景色  
//                if(j+1<colorCount){  
//                    j++;  
//                }else{  
//                    j=0;  
//                }  
//            }else{  
//                g.setColor(Color.RED);  
//            }  
//  
//            int x = 20 + i  
//                    * (histogramPitch + histogramWidth + (maxStrWidth >> 1));// 计算出X坐标  
//            int y = height - 16 - (int) (v[i] * scaling); // 计算出Y坐标  
//  
//            // 画占的比例  
//            g.drawString(v[i] + "", x  
//                    - ((metrics.stringWidth(v[i] + "") - histogramWidth) >> 1),  
//                    y);  
//  
//            // 画平面的柱状图  
//            g.drawRect(x, y, histogramWidth, (int) (v[i] * scaling));  
//            g.fillRect(x, y, histogramWidth, (int) (v[i] * scaling));  
//  
//            // 画每一项表示的东西  
//            g.drawString(str[i], x  
//                    - ((metrics.stringWidth(str[i]) - histogramWidth) >> 1),  
//                    height - 2);  
//        }  
//  
//        return bufferImage;  
//    }  
//      
//    /** 
//     * 计算缩放比例 
//     * @param v 
//     * @param h 图片的高度 
//     * @return 
//     */  
//    public float calculateScale(int[] v , int h){  
//        float scale = 1f;  
//        int max = Integer.MIN_VALUE;  
//        for(int i=0 , len=v.length ; i < len ;++i){  
//            if(v[i]>h && v[i]>max){  
//                max=v[i];  
//            }  
//        }  
//        if(max > h){  
//            scale=((int)(h*1.0f/max*1000))*1.0f/1000;  
//        }  
//        return scale;  
//    }  
//      
//    public static void main(String[] args)  {  
//        PlaneHistogram planeHistogram = new PlaneHistogram();  
//        var_serverinput = "11 11 12 13 14 15 16 14";
//        var_seg = var_serverinput.split(" ");
//        int[] int_var = new int[8];
//       
//        for (int i =0;i<var_seg.length;i++){
//          int_var[i] = Integer.parseInt(var_seg[i]);  
//        };
//        
//        BufferedImage image = planeHistogram.paintPlaneHistogram("pnl",  
//                        new int[]{int_var[0],int_var[1],int_var[2],int_var[3],int_var[4],int_var[5],int_var[6],int_var[7]}, 
//                        new String[]{"1","2","3","4","5","6","7","8"} ,  
//                        new Color[] {Color.RED, Color.GREEN, Color.BLACK, Color.BLUE });  
//        File output = new File("src/333.jpg");  
//        try {  
//            ImageIO.write(image, "jpg", output);  
//        } catch (IOException e) {  
//            e.printStackTrace();  
//        }  
//    }  
//  
//}    