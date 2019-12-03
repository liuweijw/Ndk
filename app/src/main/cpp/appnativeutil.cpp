//
// Created by LW on 2017/3/13.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static jclass contextClass;
static jclass signatureClass;
static jclass packageNameClass;
static jclass packageInfoClass;

/**
    之前生成好的签名字符串
*/
const char *RELEASE_SIGN = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3138303732363034303130305a170d3438303731383034303130305a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100e8a57b594c417c34b972076e9c36a4b63b1e4d0a1de471ec6f354cae2921085ad534f34358376da7bb0cf662ae54feb196915157b9b804593916067c64152968431e0a16e13356dac13bb1776b32513417fd93d5684c589fa8ac807a02477c11c69e409bdf852e4f20713cd625c38d05994bb3c536aeac56d04c754d624b6ba70203010001300d06092a864886f70d010105050003818100dc3b40b4d1a19b109fa1b9a06d3dfefbb72e674a61da780ae7b936ae427163db1287d8a9082455172b601a2640071f3b699e8a8e510be555348af55e8b647d812d5136224ea26ddff29aa358fa11fe3bff2619c5d268d35f650ffdf22244e33f863b34f26eada6931d6e73f4ee1f3fbf5b5e4dbd2a0dd929b66aee021894ad02";

/*
    根据context对象,获取签名字符串
*/
const char *getSignString(JNIEnv *env, jobject contextObject) {
    jmethodID getPackageManagerId = (env)->GetMethodID(contextClass, "getPackageManager",
                                                       "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (env)->GetMethodID(contextClass, "getPackageName",
                                                    "()Ljava/lang/String;");
    jmethodID signToStringId = (env)->GetMethodID(signatureClass, "toCharsString",
                                                  "()Ljava/lang/String;");
    jmethodID getPackageInfoId = (env)->GetMethodID(packageNameClass, "getPackageInfo",
                                                    "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageManagerObject = (env)->CallObjectMethod(contextObject, getPackageManagerId);
    jstring packNameString = (jstring) (env)->CallObjectMethod(contextObject, getPackageNameId);
    jobject packageInfoObject = (env)->CallObjectMethod(packageManagerObject, getPackageInfoId,
                                                        packNameString, 64);
    jfieldID signaturefieldID = (env)->GetFieldID(packageInfoClass, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray) (env)->GetObjectField(packageInfoObject,
                                                                       signaturefieldID);
    jobject signatureObject = (env)->GetObjectArrayElement(signatureArray, 0);
    return (env)->GetStringUTFChars(
            (jstring) (env)->CallObjectMethod(signatureObject, signToStringId), 0);
}

jstring Java_com_app_framework_jni_AppNativeUtil_buildReqKey(JNIEnv *env, jobject thiz,
                                                             jobject contextObject) {
    const char *signStrng = getSignString(env, contextObject);
    //签名一致  返回合法的 api key，否则返回错误
    if (strcmp(signStrng, RELEASE_SIGN) == 0) {
        return (env)->NewStringUTF("签名值正确显示");
    } else {
        return (env)->NewStringUTF("签名不正确");
    }
}

/**
    利用OnLoad钩子,初始化需要用到的Class类.
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK)
        return result;

    contextClass = (jclass) env->NewGlobalRef((env)->FindClass("android/content/Context"));
    signatureClass = (jclass) env->NewGlobalRef((env)->FindClass("android/content/pm/Signature"));
    packageNameClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/pm/PackageManager"));
    packageInfoClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/pm/PackageInfo"));

    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif