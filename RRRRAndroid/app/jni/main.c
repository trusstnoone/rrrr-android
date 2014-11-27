#include "ru_smarttransport_citytransport_rrrrandroid_R4.h"
#include "rrrr/router.h"
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>

#define DEBUG_TAG "smart"

static tdata_t tdata;
static char *stops = NULL;
static size_t stops_len = 0;
static jstring RRRR_INPUT_FILE_JAVA;


jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
//    if(tdata_load(RRRR_INPUT_FILE, &tdata) == false) {
//        __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Error load timetable");
//    }
//    else
//    {
//     __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Succeess load timetable,yahooo");
//    }

    return JNI_VERSION_1_6;
}

jint JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
    tdata_close(&tdata);
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

    const bool result = true;

   tdata_load( nativeString,&tdata);
   //ansi    tdata_load(&tdata, nativeString);
//    if( (result == false) ) {
//         __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Error load timetable");
//    }
//    else
//    {
//      __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Succeess load timetable,yahooo");
//    }

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

       uint32_t result = tdata_stopidx_by_stop_id(&tdata, nativeStopId,0);

       (*env)->ReleaseStringUTFChars(env, stopId, nativeStopId);

       return (jint) result;
   }

JNIEXPORT jstring JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_planRoute(JNIEnv * env, jobject this, jint from, jint to, jboolean arriveby,jlong epoch)
   {
       char result_buf[8000] = "test";
       char *iso_datetime = NULL;
       router_request_t req;
       router_request_initialize(&req);

//       _android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG,"initialize req");

       req.from=from;
       req.to=to;
   //    req.walk_speed = 1.5;//0.5 - 2.5
       req.arrive_by=arriveby;
      // req.walk_slack = 500;
       router_request_from_epoch (&req, &tdata, epoch); // from struct_tm instead?
     //  req.mode = m_bus;
       router_t router;

       router_setup(&router, &tdata);

       router_route(&router, &req);

       uint32_t n_reversals = req.arrive_by ? 1 : 2;
       for (uint32_t i = 0; i < n_reversals; ++i) {
               __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG,"hz iteration");
           router_request_reverse (&router, &req); // handle case where route is not reversed
           router_route (&router, &req);
       }
       int result_length = router_result_dump(&router, &req, result_buf, 8000);

      __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG,"%i",result_length);

       jstring result = (*env)->NewStringUTF(env, result_buf);
       router_teardown(&router);
       return result;
   }

