#include "ru_smarttransport_citytransport_rrrrandroid_R4.h"
#include "rrrr/router.h"
#include "rrrr/router_result.h"
#include "rrrr/router_request.h"
#include "rrrr/plan_render_otp.h"
#include "rrrr/plan_render_text.h"
#include "rrrr/tdata.h"
#include "rrrr/api.h"
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>

#define DEBUG_TAG "smart"
#define OUTPUT_LEN 16000

static tdata_t tdata;
static router_t router;
static hashgrid_t hg;



jint JNI_OnLoad(JavaVM* vm, void* reserved)
{

    return JNI_VERSION_1_6;
}

jint JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
   router_teardown (&router);
   tdata_close (&tdata);
    return JNI_VERSION_1_6;
}


JNIEXPORT jstring JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_getMessage
          (JNIEnv *env, jobject thisObj) {
   return (*env)->NewStringUTF(env, "Hello from native code!");
}

JNIEXPORT jboolean JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_initWithFile
  (JNIEnv *env, jobject thisObj, jstring filePath)
 {
     const char *nativeString = (*env)->GetStringUTFChars(env, filePath, JNI_FALSE);

    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", nativeString);

     bool result = true;

     result = tdata_load(&tdata, nativeString);
     hashgrid_setup(&hg, &tdata);
     router_setup(&router, &tdata, &hg);

    (*env)->ReleaseStringUTFChars(env, filePath, nativeString);

    return (jboolean)result;
 }

JNIEXPORT void JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_nativePipeSTDERRToLogcat
   (JNIEnv *env, jobject thisObj)
   {
    int pipes[2];
       pipe(pipes);
       dup2(pipes[1], STDERR_FILENO);
       FILE *inputFile = fdopen(pipes[0], "r");
       char readBuffer[256];
       while (1) {
           fgets(readBuffer, sizeof(readBuffer), inputFile);
           __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG,"%s", readBuffer);
       }
   }

//JNIEXPORT jint JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_getStopIndexbyId (JNIEnv *env, jobject thisObj)
//   {
//       jclass cls = (*env)->FindClass(env, "ru/smarttransport/citytransport/models/Test");
//       jmethodID methodID = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;)V");
//       jobject a=(*env)->NewObject(env,cls, methodID, 5, (*env)->NewStringUTF(env, "my test string"));
//       return a;
//   }

  JNIEXPORT jint JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_getStopIndexbyId  (JNIEnv *env, jobject thisObj, jstring stopId)
   {
       const char *nativeStopId = (*env)->GetStringUTFChars(env, stopId, JNI_FALSE);

       uint32_t result = tdata_stop_pointidx_by_stop_point_id(&tdata, nativeStopId, 0);

       (*env)->ReleaseStringUTFChars(env, stopId, nativeStopId);

       return (jint) result;
   }

JNIEXPORT jstring JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_planRoute(JNIEnv * env, jobject this, jint from, jint to, jboolean arriveby,jlong epoch)
   {
    router_request_t req;

    plan_t plan;
    plan.n_itineraries = 0;
    router_request_initialize (&req);

    req.from_stop_point=from;
    req.to_stop_point=to;
    req.arrive_by=arriveby;
    router_request_from_epoch (&req, &tdata, epoch);

    bool find =  router_route_full_reversal(&router, &req, &plan);

    plan.req = req;
    char result_buf[OUTPUT_LEN];
    plan_render_otp(&plan, &tdata, result_buf, OUTPUT_LEN);

    jstring result = (*env)->NewStringUTF(env, result_buf);
    return result;

   }

