package com.app.framework.jni;

/**
 * Created by LW on 2017/3/3.
 */

public class AppNativeUtil {

    static {
        //appnativeutil这个参数对应着c的文件名，以及后面的配置名以及so的库名称
        System.loadLibrary("appnativeutil");
    }

    //c/c++中要对应实现的方法，必须声明native

    /**
     * 上下文context
     *
     * @param context
     * @return
     */
    public native String buildReqKey(Object context);

}
