package com.zhangshirong.miaomiao;

import android.app.Activity;
import android.net.Uri;
import android.support.v7.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Typeface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Parcelable;
import android.support.v4.app.Fragment;
import android.support.v4.widget.SwipeRefreshLayout;
import android.util.Base64;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import com.squareup.picasso.MemoryPolicy;
import com.squareup.picasso.Picasso;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.PublicKey;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
/**
 * Created by Jarvis on 2016/8/18.
 */
public class Common {
    /*
    public static boolean isSpecialTheme(String name){
        if(name.length()>8)return (name.substring(0,8).equals("special_"));
        return false;
    }
    public static void callPhone(Context context,String phone) {
        Intent intent = new Intent(Intent.ACTION_DIAL, Uri.parse("tel:"+phone));
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }
    public static int getColorByTheme(GongGong app,String name){

        String nameT=name;
        if(!app.localSetting.isLogin())app.theme="";
        if(!app.theme.isEmpty()){
            nameT+="_"+app.theme;
        }
        return app.getResources().getIdentifier(nameT,"color",app.getPackageName());
    }
    public static int getDrawableByTheme(GongGong app,String name){
        String nameT=name;
        String theme=app.localSetting.readData(R.string.SETTING_THEME);
        if(!theme.isEmpty()){
            nameT+="_"+theme;
        }
        return app.getResources().getIdentifier(nameT,"drawable",app.getPackageName());
    }
    public static Typeface getIconfontTypeface(Context context){
        return Typeface.createFromAsset(context.getApplicationContext().getAssets(), "fonts/iconfont.ttf");
    }

    public static String getMonthDate(int next,String format){
        String re="";
        Calendar cd = Calendar.getInstance();
        cd.setTimeInMillis(System.currentTimeMillis());
        int year=cd.get(Calendar.YEAR);
        int month=cd.get(Calendar.MONTH)+1+next;
        if(month>12){
            month-=12;
            year++;
        }
        re=year+"-"+month+"-1 00:00:00";
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-M-d HH:mm:ss");
        SimpleDateFormat sdfT=new SimpleDateFormat(format);
        try {
            Date date=sdf.parse(re);
            re=sdfT.format(date);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return re;
    }
    public static String getCardId(String id,boolean isWid){
        String nid=id;
        if(nid.length()==19){
            nid=nid.substring(0,4)+" **** **** ***"+nid.substring(nid.length()-4,nid.length()-3)+" "+nid.substring(nid.length()-3);
        }
        if(isWid){
            String nnid="";
            for(int a=0;a<nid.length();a++){
                String temp= String.valueOf(nid.charAt(a))+" ";
                nnid+=temp;
            }
            nnid.replace("  ","   ");
            nid=nnid;
        }
        return nid;
    }

    public static void cleanNoticeList(Context context){
        GongGong app= (GongGong) context.getApplicationContext();
        NotificationManager notiManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        String[] list=app.localSetting.getNoticeList();
        for(int a=0;a<list.length;a++){
            try{
                int temp=Integer.parseInt(list[a]);
                notiManager.cancel(temp);
            }
            catch (Exception e){}
        }
        app.localSetting.clearNoticeList();
    }
    public static void sendNotice(final Context context, final String title, final String content, final String url){
        new Thread(new Runnable() {
            @Override
            public void run() {
                GongGong app= (GongGong) context.getApplicationContext();
                NotificationManager manager=(NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
                String key=title+"=>"+content;
                if(app.notificationsNotice.containsKey(key)){
                    manager.cancel(app.notificationsNotice.get(key));
                }
                if(app.localSetting.getNoticeOnce()){
                    if(app.localSetting.inOnce(key)){
                        return;
                    }
                }
                app.localSetting.setOnce(key);
                Notification.Builder builder=new Notification.Builder(context);
                builder.setContentTitle(title);
                builder.setContentText(content);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    builder.setColor(0xf897aa);
                    builder.setSmallIcon(R.drawable.logo_white);
                }
                else{
                    builder.setSmallIcon(R.mipmap.ic_launcher);
                }
                builder.setTicker(title);
                builder.setDefaults(Notification.DEFAULT_VIBRATE|Notification.DEFAULT_LIGHTS);
                builder.setWhen(System.currentTimeMillis());
                Intent notificationIntent =new Intent(context, Web.class);
                Bundle bundle=new Bundle();
                bundle.putString("url",url);
                notificationIntent.putExtras(bundle);
                PendingIntent contentItent = PendingIntent.getActivity(context, 0, notificationIntent, 0);
                builder.setContentIntent(contentItent);
                Notification nf=builder.getNotification();
                nf.flags=Notification.FLAG_AUTO_CANCEL;
                int id=(int) System.currentTimeMillis()+(int)Math.random()*1000;
                app.notificationsNotice.put(key,id);
                app.localSetting.setNoticeList(id);
                synchronized (context){
                    try {
                        manager.notify(id,nf);
                        context.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();


    }
    public static String decrypt(String key, String str) throws Exception {
        byte[] text64 =Base64.decode(str,Base64.DEFAULT);
        SecretKeySpec newKey = new SecretKeySpec(key.getBytes(), "AES");
        Cipher cipher = Cipher.getInstance("AES/ECB/ZeroBytePadding");
        cipher.init(Cipher.DECRYPT_MODE, newKey);
        byte[] re=cipher.doFinal(text64);
        return new String(re);
    }
    public static void sendNotice(final Context context, final String title, final String content, final Class c){
        new Thread(new Runnable() {
            @Override
            public void run() {
                GongGong app= (GongGong) context.getApplicationContext();
                NotificationManager manager=(NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
                String key=title+"=>"+content;
                if(app.notificationsNotice.containsKey(key)){
                    manager.cancel(app.notificationsNotice.get(key));
                }
                if(app.localSetting.getNoticeOnce()){
                    if(app.localSetting.inOnce(key)){
                        return;
                    }
                }
                app.localSetting.setOnce(key);
                Notification.Builder builder=new Notification.Builder(context);
                builder.setContentTitle(title);
                builder.setContentText(content);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    builder.setColor(0xf897aa);
                    builder.setSmallIcon(R.drawable.logo_white);
                }
                else{
                    builder.setSmallIcon(R.mipmap.ic_launcher);
                }
                builder.setTicker(title);
                builder.setDefaults(Notification.DEFAULT_VIBRATE|Notification.DEFAULT_LIGHTS);
                builder.setWhen(System.currentTimeMillis());
                Intent notificationIntent =new Intent(context, c);
                PendingIntent contentItent = PendingIntent.getActivity(context, 0, notificationIntent, 0);
                builder.setContentIntent(contentItent);
                Notification nf=builder.getNotification();
                nf.flags=Notification.FLAG_AUTO_CANCEL;
                int id=(int) System.currentTimeMillis()+(int)Math.random()*1000;
                app.notificationsNotice.put(key,id);
                app.localSetting.setNoticeList(id);
                synchronized (context){
                    try {
                        manager.notify(id,nf);
                        context.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();


    }
    public static Fragment getFragmentByClass(Class source){
        Fragment f=null;
        try {
            source.getDeclaredConstructor().setAccessible(true);
            f=(Fragment)source.newInstance();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return f;
    }
    public static String toDurationTime(int mics){
        if(mics<=0)return "00:00";
        int second=(int)((float)mics/1000);
        int minutes= (int) ((float)second/60);
        second=second%60;
        String min=String.valueOf(minutes);
        String sec=String.valueOf(second);
        if(min.length()<2)min="0"+min;
        if(sec.length()<2)sec="0"+sec;
        return min+":"+sec;

    }
    public static String getTermString(int id){
        String re="大";
        int m= (int) Math.floor(id/ 2);
        int n=id%2;
        switch (m+n){
            case 1:
                re+="一";
                break;
            case 2:
                re+="二";
                break;
            case 3:
                re+="三";
                break;
            case 4:
                re+="四";
                break;
            case 5:
                re+="五";
                break;
            case 6:
                re+="六";
                break;
            case 7:
                re+="七";
                break;
            case 8:
                re+="八";
                break;
        }
        if(n>0){
            re+="上";
        }
        else{
            re+="下";
        }
        re+="学期";
        return re;
    }
    public static int getDays_MD(String startDay){
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-M-d");
        int re=0;
        try {
            Calendar now=Calendar.getInstance();
            Date cd = sdf.parse(now.get(Calendar.YEAR)+"-"+startDay);
            re= (int) ((System.currentTimeMillis()-cd.getTime())/1000/60/60/24);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return re;
    }
    public static int getMinutes_MD(String startTime_){
        String startTime=startTime_;
        if(startTime.length()<5){
            startTime="0"+startTime;
        }
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-M-d HH:mm");
        int re=0;
        try {
            Calendar now=Calendar.getInstance();
            Date cd = sdf.parse(now.get(Calendar.YEAR)+"-"+(now.get(Calendar.MONTH)+1)+"-"+now.get(Calendar.DAY_OF_MONTH)+" "+startTime);
            re= (int) ((System.currentTimeMillis()-cd.getTime())/1000/60);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return re;
    }
    public static int getDays(String startDay){
        return getDays(startDay,"yyyy-MM-dd");
    }
    public static int getDays(String startDay,String format){
        SimpleDateFormat sdf = new SimpleDateFormat(format);
        int re=0;
        try {
            Date cd = sdf.parse(startDay);
            Calendar cld = Calendar.getInstance();
            cld.setTimeInMillis(System.currentTimeMillis());
            SimpleDateFormat sdf_ = new SimpleDateFormat("yyyy-M-d");
            Date cd_ =sdf_.parse(cld.get(Calendar.YEAR)+"-"+(cld.get(Calendar.MONTH)+1)+"-"+cld.get(Calendar.DAY_OF_MONTH));
            re= (int) ((cd_.getTime()-cd.getTime())/1000/60/60/24);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return re;
    }
    public static void setClipboard(Context context,String data){
        ClipboardManager clip = (ClipboardManager)context.getSystemService(Context.CLIPBOARD_SERVICE);
        clip.setPrimaryClip(ClipData.newPlainText("text",data));
    }
    public static Date getDayByAddTimestamp(String start,long timestamp){
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Date now=new Date();
        try {
            Date cd = sdf.parse(start);
            now.setTime(cd.getTime()+timestamp);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return now;
    }
    public static String getCurrentMonthStart(){
        return getCurrentMonthStart(System.currentTimeMillis());
    }
    public static String getCurrentMonthEnd(){
        return getCurrentMonthEnd(System.currentTimeMillis());
    }
    public static String getCurrentMonthStart(long time){
        SimpleDateFormat sdfy = new SimpleDateFormat("yyyyMM");
        Date now=new Date(time);
        return sdfy.format(now)+"01";
    }
    public static String getCurrentMonthEnd(long time){
        String re="";
        SimpleDateFormat sdfy = new SimpleDateFormat("yyyy");
        SimpleDateFormat sdfm = new SimpleDateFormat("M");
        Date now=new Date(time);
        int tempY=Integer.parseInt(sdfy.format(now));
        int tempM=Integer.parseInt(sdfm.format(now));
        if(tempM+1>12){
            tempY++;
            tempM=1;
        }
        else{
            tempM++;
        }
        String month=String.valueOf(tempM);
        if(month.length()<2)month="0"+month;
        String nextmonth=String.valueOf(tempY)+month+"01";
        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
        try {
            Date date=sdf.parse(nextmonth);
            date.setTime(date.getTime()-1000);
            sdf = new SimpleDateFormat("yyyyMMdd");
            re=sdf.format(date);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return re;
    }
    public static void openUrl(Activity activity,String url){
        Intent mainIntent = new Intent(activity,Web.class);
        Bundle bundle=new Bundle();
        bundle.putString("url",url);
        mainIntent.putExtras(bundle);
        //mainIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_SINGLE_TOP);
        activity.startActivity(mainIntent);
    }
    public static String getColorString(int color){
        return String.format("#%06X", 0xFFFFFF & color);
    }
    public static void openUrlByColor(Activity activity,String url,String color){
        Intent mainIntent = new Intent(activity,Web.class);
        Bundle bundle=new Bundle();
        bundle.putString("url",url);
        bundle.putString("color",color);
        mainIntent.putExtras(bundle);
        //mainIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_SINGLE_TOP);
        activity.startActivity(mainIntent);
    }
    public static void openUrl(Activity activity,String url,String activityName){
        Intent mainIntent = new Intent(activity,Web.class);
        Bundle bundle=new Bundle();
        bundle.putString("url",url);
        bundle.putString("nextActivity",activityName);
        mainIntent.putExtras(bundle);
        activity.startActivity(mainIntent);
    }
    public static void share(Activity activity,String str){
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);
        sendIntent.putExtra(Intent.EXTRA_TEXT, str);
        sendIntent.putExtra("exit_on_sent", true);
        sendIntent.setType("text/plain");
        activity.startActivity(Intent.createChooser(sendIntent, "选择分享到的应用"));
        //activity.startActivityForResult(sendIntent, 1);
    }
    public static int getTodayWeek(){
        Date now=new Date(System.currentTimeMillis());
        Calendar cd=Calendar.getInstance();
        int week=(cd.get(cd.DAY_OF_WEEK)-1+7)%7;
        if(week==0)week+=7;
        return week;
    }
    public static String getSectionTime(int section){
        return getSectionTime(section,false);
    }
    public static String getSectionTime(int section,boolean end){
        final int TIME_MORNING=8*60;
        final int TIME_COURSE=45;
        final int TIME_REST_SHORT=10;
        final int TIME_REST_LONG=30;
        final int TIME_AFTERNOON;
        final int TIME_NIGHT;
        final String TIME_SUMMER="5-1";
        final String TIME_WINTER="10-1";
        int a=getDays_MD(TIME_SUMMER);
        int b=getDays_MD(TIME_WINTER);
        if(a<0 || b>0){
            TIME_AFTERNOON=14*60;
            TIME_NIGHT=19*60;
        }
        else{
            TIME_AFTERNOON=14*60+30;
            TIME_NIGHT=19*60+30;
        }
        int time;
        if(section<5){
            time=TIME_MORNING;
            int c=section;
            int d=(c-1)/2;
            time+=(TIME_COURSE+TIME_REST_SHORT)*(c-1)+d*(TIME_REST_LONG-TIME_REST_SHORT);
        }
        else if(section<9){
            time=TIME_AFTERNOON;
            int c=section-4;
            int d=(c-1)/2;
            time+=(TIME_COURSE+TIME_REST_SHORT)*(c-1)+d*(TIME_REST_LONG-TIME_REST_SHORT);
        }
        else{
            time=TIME_NIGHT;
            int c=section-8;
            int d=(c-1)/2;
            time+=(TIME_COURSE+TIME_REST_SHORT)*(c-1);
        }
        if (end)time+=45;
        String h=String.valueOf(time/60);
        String m=String.valueOf(time%60);
        if(m.length()<2)m="0"+m;
        return h+":"+m;
    }
    public static void active(Context context,Class className){
        Intent mainIntent = new Intent(context,className);
        context.startActivity(mainIntent);
    }
    public static void activeAndClear(Context context,Class className){
        Intent mainIntent = new Intent(context,className);
        mainIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK |Intent.FLAG_ACTIVITY_CLEAR_TOP);
        context.startActivity(mainIntent);
    }
    public static void activeNewProcess(Context context,Class className){
        Intent mainIntent = new Intent(context,className);
        mainIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_SINGLE_TOP);
        context.startActivity(mainIntent);
    }
    public static void active(Fragment fragment, Class className, int requestCode){
        Intent mainIntent = new Intent(fragment.getContext(),className);
        fragment.startActivityForResult(mainIntent,requestCode);
    }
    public static void active(Activity activity, Class className, int requestCode){
        Intent mainIntent = new Intent(activity,className);
        activity.startActivityForResult(mainIntent,requestCode);
    }
    public static String getNameOfDataId(int dataRid){
        String re="";
        switch(dataRid){
            case R.string.DATA_COURSE:
                re="课表";
                break;
            case R.string.DATA_GRADE:
                re="查成绩";
                break;
            case R.string.DATA_ECARD_INFO:
            case R.string.DATA_ECARD_BILLING:
                re="一卡通";
                break;
            case R.string.DATA_USERINFO:
                re="教务/信息门户";
                break;
            case R.string.DATA_LIBRARY:
                re="图书馆";
                break;
        }
        return re;
    }
    public static void swipeRefresh(final SwipeRefreshLayout swipe){
        if(!swipe.isRefreshing()){
            swipe.post(new Runnable() {
                @Override
                public void run() {
                    swipe.setRefreshing(false);
                    swipe.setRefreshing(true);
                }
            });
        }

    }
    public static void swipeRefreshStop(final SwipeRefreshLayout swipe){
        swipe.post(new Runnable() {
            @Override
            public void run() {
                swipe.setRefreshing(false);
            }
        });
    }
    public static String getVersion(Context context){
        PackageManager packageManager = context.getPackageManager();
        // getPackageName()是你当前类的包名，0代表是获取版本信息
        PackageInfo packInfo = null;
        String version="";
        try {
            packInfo = packageManager.getPackageInfo(context.getPackageName(),0);
            version = packInfo.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return version;

    }

    public static boolean isUpdate(String local,String update){
        if(local.isEmpty())return true;
        String[] acl=local.split("\\.");
        String[] acu=update.split("\\.");
        boolean is=false;
        boolean same=false;
        int a;
        for(a=0;a<acl.length && a<acu.length;a++){
            same=false;
            if(acu[a].indexOf("_")>-1){
                acu[a]=acu[a].substring(0,acu[a].indexOf("_"));
            }
            if(acl[a].indexOf("_")>-1){
                acl[a]=acl[a].substring(0,acl[a].indexOf("_"));
            }

            try{
                int m=Integer.parseInt(acu[a]);
                int n=Integer.parseInt(acl[a]);
                if(m>n){
                    is=true;
                    break;
                }
                else if(m==n){
                    same=true;
                }
                else{
                    is=false;
                    break;
                }
            }
            catch (Exception e){
                //跳过
            }
        }
        if(acu.length>acl.length && same)is=true;
        return is;
    }
    public static boolean inFilter(String filter,String sid){
        return sid.matches(filter);
    }
    public static void setWindowStatusBarColor(Activity activity, String color) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                Window window = activity.getWindow();
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(Color.parseColor(color));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static void setWindowStatusBarColor(Activity activity, int colorResId) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                Window window = activity.getWindow();
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(activity.getResources().getColor(colorResId));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static String week2chinese(int week){
        String re="";
        switch (week){
            case 1:
                re="一";
                break;
            case 2:
                re="二";
                break;
            case 3:
                re="三";
                break;
            case 4:
                re="四";
                break;
            case 5:
                re="五";
                break;
            case 6:
                re="六";
                break;
            case 7:
                re="日";
                break;

        }
        return re;
    }
    public static int getStatusBarHeight(Context context) {
        int result = 0;
        int resourceId = context.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = context.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }*/
    //透明状态栏
    //getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
    //取消
    //getWindow().clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
    //透明导航栏
    //getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
    public static Bitmap doBlur(Bitmap sentBitmap, int radius, boolean canReuseInBitmap) {

        // Stack Blur v1.0 from
        // http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
        //
        // Java Author: Mario Klingemann <mario at quasimondo.com>
        // http://incubator.quasimondo.com
        // created Feburary 29, 2004
        // Android port : Yahel Bouaziz <yahel at kayenko.com>
        // http://www.kayenko.com
        // ported april 5th, 2012

        // This is a compromise between Gaussian Blur and Box blur
        // It creates much better looking blurs than Box Blur, but is
        // 7x faster than my Gaussian Blur implementation.
        //
        // I called it Stack Blur because this describes best how this
        // filter works internally: it creates a kind of moving stack
        // of colors whilst scanning through the image. Thereby it
        // just has to add one new block of color to the right side
        // of the stack and remove the leftmost color. The remaining
        // colors on the topmost layer of the stack are either added on
        // or reduced by one, depending on if they are on the right or
        // on the left side of the stack.
        //
        // If you are using this algorithm in your code please add
        // the following line:
        //
        // Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>

        Bitmap bitmap;
        if (canReuseInBitmap) {
            bitmap = sentBitmap;
        } else {
            bitmap = sentBitmap.copy(sentBitmap.getConfig(), true);
        }

        if (radius < 1) {
            return (null);
        }

        int w = bitmap.getWidth();
        int h = bitmap.getHeight();

        int[] pix = new int[w * h];
        bitmap.getPixels(pix, 0, w, 0, 0, w, h);

        int wm = w - 1;
        int hm = h - 1;
        int wh = w * h;
        int div = radius + radius + 1;

        int r[] = new int[wh];
        int g[] = new int[wh];
        int b[] = new int[wh];
        int rsum, gsum, bsum, x, y, i, p, yp, yi, yw;
        int vmin[] = new int[Math.max(w, h)];

        int divsum = (div + 1) >> 1;
        divsum *= divsum;
        int dv[] = new int[256 * divsum];
        for (i = 0; i < 256 * divsum; i++) {
            dv[i] = (i / divsum);
        }

        yw = yi = 0;

        int[][] stack = new int[div][3];
        int stackpointer;
        int stackstart;
        int[] sir;
        int rbs;
        int r1 = radius + 1;
        int routsum, goutsum, boutsum;
        int rinsum, ginsum, binsum;

        for (y = 0; y < h; y++) {
            rinsum = ginsum = binsum = routsum = goutsum = boutsum = rsum = gsum = bsum = 0;
            for (i = -radius; i <= radius; i++) {
                p = pix[yi + Math.min(wm, Math.max(i, 0))];
                sir = stack[i + radius];
                sir[0] = (p & 0xff0000) >> 16;
                sir[1] = (p & 0x00ff00) >> 8;
                sir[2] = (p & 0x0000ff);
                rbs = r1 - Math.abs(i);
                rsum += sir[0] * rbs;
                gsum += sir[1] * rbs;
                bsum += sir[2] * rbs;
                if (i > 0) {
                    rinsum += sir[0];
                    ginsum += sir[1];
                    binsum += sir[2];
                } else {
                    routsum += sir[0];
                    goutsum += sir[1];
                    boutsum += sir[2];
                }
            }
            stackpointer = radius;

            for (x = 0; x < w; x++) {

                r[yi] = dv[rsum];
                g[yi] = dv[gsum];
                b[yi] = dv[bsum];

                rsum -= routsum;
                gsum -= goutsum;
                bsum -= boutsum;

                stackstart = stackpointer - radius + div;
                sir = stack[stackstart % div];

                routsum -= sir[0];
                goutsum -= sir[1];
                boutsum -= sir[2];

                if (y == 0) {
                    vmin[x] = Math.min(x + radius + 1, wm);
                }
                p = pix[yw + vmin[x]];

                sir[0] = (p & 0xff0000) >> 16;
                sir[1] = (p & 0x00ff00) >> 8;
                sir[2] = (p & 0x0000ff);

                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];

                rsum += rinsum;
                gsum += ginsum;
                bsum += binsum;

                stackpointer = (stackpointer + 1) % div;
                sir = stack[(stackpointer) % div];

                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];

                rinsum -= sir[0];
                ginsum -= sir[1];
                binsum -= sir[2];

                yi++;
            }
            yw += w;
        }
        for (x = 0; x < w; x++) {
            rinsum = ginsum = binsum = routsum = goutsum = boutsum = rsum = gsum = bsum = 0;
            yp = -radius * w;
            for (i = -radius; i <= radius; i++) {
                yi = Math.max(0, yp) + x;

                sir = stack[i + radius];

                sir[0] = r[yi];
                sir[1] = g[yi];
                sir[2] = b[yi];

                rbs = r1 - Math.abs(i);

                rsum += r[yi] * rbs;
                gsum += g[yi] * rbs;
                bsum += b[yi] * rbs;

                if (i > 0) {
                    rinsum += sir[0];
                    ginsum += sir[1];
                    binsum += sir[2];
                } else {
                    routsum += sir[0];
                    goutsum += sir[1];
                    boutsum += sir[2];
                }

                if (i < hm) {
                    yp += w;
                }
            }
            yi = x;
            stackpointer = radius;
            for (y = 0; y < h; y++) {
                // Preserve alpha channel: ( 0xff000000 & pix[yi] )
                pix[yi] = (0xff000000 & pix[yi]) | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];

                rsum -= routsum;
                gsum -= goutsum;
                bsum -= boutsum;

                stackstart = stackpointer - radius + div;
                sir = stack[stackstart % div];

                routsum -= sir[0];
                goutsum -= sir[1];
                boutsum -= sir[2];

                if (x == 0) {
                    vmin[y] = Math.min(y + r1, hm) * w;
                }
                p = x + vmin[y];

                sir[0] = r[p];
                sir[1] = g[p];
                sir[2] = b[p];

                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];

                rsum += rinsum;
                gsum += ginsum;
                bsum += binsum;

                stackpointer = (stackpointer + 1) % div;
                sir = stack[stackpointer];

                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];

                rinsum -= sir[0];
                ginsum -= sir[1];
                binsum -= sir[2];

                yi += w;
            }
        }

        bitmap.setPixels(pix, 0, w, 0, 0, w, h);

        return (bitmap);
    }
    //获取内置SD卡路径
    public static String getInnerSDCardPath() {
        return Environment.getExternalStorageDirectory().getAbsolutePath();
    }

    //获取外置SD卡路径
    public static List<String> getExtSDCardPath()
    {
        List<String> lResult = new ArrayList<String>();
        try {
            Runtime rt = Runtime.getRuntime();
            Process proc = rt.exec("mount");
            InputStream is = proc.getInputStream();
            InputStreamReader isr = new InputStreamReader(is);
            BufferedReader br = new BufferedReader(isr);
            String line;
            while ((line = br.readLine()) != null) {

                if (line.contains("extSdCard") || line.contains("sdcard"))
                {
                    String [] arr = line.split(" ");
                    String path = arr[1];
                    File file = new File(path);
                    if (file.isDirectory())
                    {
                        lResult.add(path);
                    }
                }
            }
            isr.close();
        } catch (Exception e) {
        }
        return lResult;
    }

    public static String dataSpeed(long size){
        long size_=size;
        float i= (float)size_/1024;
        if(i<1){
            return String.valueOf(size_)+"B/s";
        }
        else{
            size_= (long) i;
            i= i/1024;
            if(i<1){
                return String.valueOf(size_)+"KB/s";
            }
            else{
                size_= (long) i;
                i= i/1024;
                if(i<1){
                    return String.valueOf(size_)+"MB/s";
                }
                else{
                    size_= (long) i;
                    return String.valueOf(size_)+"GB/s";
                }
            }
        }
    }
    /*
    public static String dataSize(long size){
        long size_=size;
        float i= (float)size_/1024;
        if(i<1){
            return String.valueOf(size_)+"B";
        }
        else{
            size_= (long) i;
            i= i/1024;
            if(i<1){
                return String.valueOf(size_)+"KB";
            }
            else{
                size_= (long) i;
                i= i/1024;
                if(i<1){
                    return String.valueOf(size_)+"MB";
                }
                else{
                    size_= (long) i;
                    return String.valueOf(size_)+"GB";
                }
            }
        }
    }
    public static void loadHeaderImage(String url, String sex, ImageView view){
        if(url.isEmpty()){
            if(sex.equals("男")){
                Picasso.with(view.getContext()).load(R.drawable.header).fit().centerCrop().config(Bitmap.Config.RGB_565).memoryPolicy(MemoryPolicy.NO_STORE).into(view);
            }
            else{
                Picasso.with(view.getContext()).load(R.drawable.header2).fit().centerCrop().config(Bitmap.Config.RGB_565).into(view);
            }
        }
        else{
            Picasso.with(view.getContext()).load(url).error(R.drawable.user_none).fit().centerCrop().config(Bitmap.Config.RGB_565).into(view);
        }
    }
    public static void selectDownloadDialog(Context context, final GongGong app, final Runnable select) {
        List<String> ext=getExtSDCardPath();
        String rootSd=getInnerSDCardPath();
        String curr=app.localSetting.getRadioDownloadPath();
        int id=0;
        final String items[]=new String[1+ext.size()];
        items[0]=rootSd;
        for(int a=0;a<ext.size();a++){
            items[1+a]=ext.get(a);
            if(items[1+a].equals(curr)){
                id=a+1;
            }
        }
        AlertDialog.Builder builder=new AlertDialog.Builder(context);  //先得到构造器
        builder.setTitle("选择下载目录"); //设置标题
        builder.setSingleChoiceItems(items,id,new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                app.localSetting.setRadioDownloadPath(items[which]);
                if(select!=null)select.run();
                dialog.dismiss();

            }
        });
        builder.create().show();
    }

    public static boolean underWIFI(Context context){
        ConnectivityManager connectivityManager= (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo=connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        if(networkInfo!=null){
            return networkInfo.isConnected();
        }
        return false;
    }
    public static boolean underNetConnect(Context context){
        ConnectivityManager connectivityManager= (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo=connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        NetworkInfo networkInfoM=connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        NetworkInfo networkInfoMD=connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE_DUN);
        boolean re=false;
        if(networkInfo!=null ){
            re=re || networkInfo.isConnected();
        }
        if(networkInfoM!=null){
            re=re || networkInfoM.isConnected();
        }
        if(networkInfoMD!=null){
            re=re || networkInfoMD.isConnected();
        }
        return re;
    }

    public static String encodeUrl(String url){
        String encode="";
        Pattern p= Pattern.compile("[\u4e00-\u9fa5]");

        for(int a=0;a<url.length();a++){
            String temp=url.substring(a,a+1);
            Matcher m=p.matcher(temp);
            if(m.matches()){
                try {
                    encode += URLEncoder.encode(temp,"utf-8");
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }
            else{
                encode+=temp;
            }
        }
        return encode;
    }

    public static void createShortCut(Context context,String title,Class className,int iconResource){
        //创建快捷方式的Intent
        Intent shortcutintent = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
        //不允许重复创建
        shortcutintent.putExtra("duplicate", false);
        //需要现实的名称
        shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_NAME, title);
        //快捷图片
        Parcelable icon = Intent.ShortcutIconResource.fromContext(context, iconResource);
        shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);
        //点击快捷图片，运行的程序主入口
        Intent intent = new Intent(context , className);
        shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, intent);
        //发送广播。OK
        context.sendBroadcast(shortcutintent);
    }
    public static String getCourseId(JSONObject json,int week){
        return getCourseId(json,week,true);
    }
    public static String getCourseId(JSONObject json,int week,boolean hasSection){
        String data="";
        try {
            data= week+"|"+json.getString("course")+"|"+json.getString("location")+"|"+json.getString("teacher");
            if(hasSection)data+="|"+json.getString("section_start")+"|"+json.getString("section_end");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return data;
    }
    public static void refreshCourseAppWidget(Context context){
        //更新widget
        Bundle bundle=new Bundle();
        bundle.putInt("id",-1);
        bundle.putInt("pos",-1);
        Intent intent=new Intent(Config.BROADCAST_EVENT_APPWIDGET_COURSE);
        intent.putExtras(bundle);
        context.sendBroadcast(intent);
    }
    public static void refreshTimerAppWidget(Context context){
        //更新widget
        Intent intent=new Intent(Config.BROADCAST_EVENT_APPWIDGET_TIMER);
        context.sendBroadcast(intent);
    }
    */

}