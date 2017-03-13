//
// Created by LW on 2017/3/13.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

static jclass contextClass;
static jclass signatureClass;
static jclass packageNameClass;
static jclass packageInfoClass;

/**
    之前生成好的签名字符串
*/
const char* RELEASE_SIGN = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3136303632303039303130315a170d3436303631333039303130315a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100a871201812b65f15236ca658b6ba8f790482d248bcdbc5316df35dc097140094c7f3a6850f8068acb4d74857a5b9887df0a4539f53eeaf6d836d23d23a8c80d7c16e05956ca0a0a7068795280d2884b6d1bf620b77369dab5396671cd67b0f1f8d7c3a439152132d11294b1aa4ca7c5d4271ccea673088ca80a13b7883b8ca8f0203010001300d06092a864886f70d0101050500038181001aa814a9a5275072f0efb2990fc9c622770267db04f56a1ede5d0153b7f214a05e346432267d22615f904d3706e9dc6a447d0cecf771b243f9d23ba5add54600752b1c7e6dd4623aa43fed1ad573a7dfa5d3b5950796d43a906ca51ef76868a3d4e79b7ad06074e6e5b433f84ac9aaec8c0817114d6803d8e02663a009f354e9";

/*
    根据context对象,获取签名字符串
*/
const char* getSignString(JNIEnv *env,jobject contextObject) {
    jmethodID getPackageManagerId = (env)->GetMethodID(contextClass, "getPackageManager","()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (env)->GetMethodID(contextClass, "getPackageName","()Ljava/lang/String;");
    jmethodID signToStringId = (env)->GetMethodID(signatureClass, "toCharsString","()Ljava/lang/String;");
    jmethodID getPackageInfoId = (env)->GetMethodID(packageNameClass, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageManagerObject =  (env)->CallObjectMethod(contextObject, getPackageManagerId);
    jstring packNameString =  (jstring)(env)->CallObjectMethod(contextObject, getPackageNameId);
    jobject packageInfoObject = (env)->CallObjectMethod(packageManagerObject, getPackageInfoId,packNameString, 64);
    jfieldID signaturefieldID =(env)->GetFieldID(packageInfoClass,"signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray)(env)->GetObjectField(packageInfoObject, signaturefieldID);
    jobject signatureObject =  (env)->GetObjectArrayElement(signatureArray,0);
    return (env)->GetStringUTFChars((jstring)(env)->CallObjectMethod(signatureObject, signToStringId),0);
}

jstring Java_com_app_framework_jni_AppNativeUtil_buildReqKey(JNIEnv *env,jobject thiz,jobject contextObject) {
    const char* signStrng =  getSignString(env,contextObject);
    //签名一致  返回合法的 api key，否则返回错误
    if(strcmp(signStrng,RELEASE_SIGN)==0) {
        return (env)->NewStringUTF("签名key");
    }else {
        return (env)->NewStringUTF("签名不正确");
    }
}

/**
    利用OnLoad钩子,初始化需要用到的Class类.
*/
JNIEXPORT jint JNICALL JNI_OnLoad (JavaVM* vm,void* reserved){

    JNIEnv* env = NULL;
    jint result=-1;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        return result;

    contextClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/Context"));
    signatureClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/Signature"));
    packageNameClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageManager"));
    packageInfoClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageInfo"));

    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif