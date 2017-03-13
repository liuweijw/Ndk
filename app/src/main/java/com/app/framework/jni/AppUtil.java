package com.app.framework.jni;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

/**
 * Created by LW on 2017/3/13.
 */

public class AppUtil {

    /**
     * 获取签名信息
     * @param context
     * @return
     */
    public static String getAppSignInfo(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(
                    context.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signs = packageInfo.signatures;
            Signature sign = signs[0];
            System.out.println(sign.toCharsString());
            return sign.toCharsString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

}
