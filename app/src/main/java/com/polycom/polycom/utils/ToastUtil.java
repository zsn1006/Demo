package com.polycom.polycom.utils;

import android.content.Context;
import android.widget.Toast;

/**
 * 单一的toast
 * @author zsn
 *
 */
public class ToastUtil{
	//第一次的时间
	private static long formerTime=0;
	//第二次的时间
	private static long lasterTime=0;
	private static String oldMsg=null;
	public static Toast toast=null;
	public static void showToast(Context context,String str){
		if(toast==null){
			toast= Toast.makeText(context, str, Toast.LENGTH_SHORT);
			toast.show();
			formerTime=	System.currentTimeMillis();
		}else{
			lasterTime= System.currentTimeMillis();
			if(str.equals(oldMsg)){
				//当是同一文字的时候
				if((lasterTime-formerTime)> Toast.LENGTH_SHORT){
					toast.show();
				}
			}else{
				//当其他文字的时候
				oldMsg=str;
				toast.setText(oldMsg);
				toast.show();
			}
		}
		formerTime=lasterTime;
	}
	
	public static void showToast(Context context,int resId){
		showToast(context, context.getString(resId));
	}
}