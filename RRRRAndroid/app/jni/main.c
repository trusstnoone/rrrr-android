#include "ru_smarttransport_citytransport_rrrrandroid_R4.h"
#include "rrrr/config.h"
#include "rrrr/router.h"
#include "rrrr/api.h"
#include "rrrr/set.h"
#include "rrrr/plan_render_otp.h"

#include "rrrr/router_result.h"
#include "rrrr/router_request.h"
#include "rrrr/plan_render_text.h"
#include "rrrr/tdata.h"

#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_TAG "smart"
#define OUTPUT_LEN 32768

static tdata_t tdata;
static router_t router;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    /* Initialise the structures to zero, so we can trust NULL pointers */
    memset (&router, 0, sizeof(router_t));
    memset (&tdata,  0, sizeof(tdata_t));

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
    char *nativeString = (*env)->GetStringUTFChars(env, filePath, JNI_FALSE);
    bool result;

    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", nativeString);

    result = tdata_load(&tdata, nativeString);
    (*env)->ReleaseStringUTFChars(env, filePath, nativeString);

    if (result) {
        /* The hashgrid is "only" required if you want to do lat/lon searches
         * or use it together with the geocoder. In general SQLite with FTS might
         * be a better solution on Android.
         *
         * result &= tdata_hashgrid_setup(&tdata);
         */
        result &= router_setup (&router, &tdata);
    }

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

//#if 0
//JNIEXPORT jint JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_getStopIndexbyId (JNIEnv *env, jobject thisObj)
//{
//    jclass cls = (*env)->FindClass(env, "ru/smarttransport/citytransport/models/Test");
//    jmethodID methodID = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;)V");
//    jobject a=(*env)->NewObject(env,cls, methodID, 5, (*env)->NewStringUTF(env, "my test string"));
//    return a;
//}
//#endif

JNIEXPORT jint JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_getStopIndexbyId  (JNIEnv *env, jobject thisObj, jstring stopId)
{
    const char *nativeStopId = (*env)->GetStringUTFChars(env, stopId, JNI_FALSE);
    uint32_t result = tdata_stop_pointidx_by_stop_point_id(&tdata, nativeStopId, 0);
    (*env)->ReleaseStringUTFChars(env, stopId, nativeStopId);
    return (jint) result;
}

JNIEXPORT jstring JNICALL Java_ru_smarttransport_citytransport_rrrrandroid_R4_planRoute(JNIEnv * env, jobject this, jint from, jint to, jboolean arrive_by, jlong epoch)
{
    router_request_t req;
    plan_t plan;
    bool find;
    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "begin init");
    router_request_initialize (&req);

    req.from_stop_point = from;
    req.to_stop_point = to;
    req.arrive_by = arrive_by;
    router_request_from_epoch (&req, &tdata, epoch);

    if (req.time_rounded && ! (req.arrive_by)) {
        req.time++;
    }
    req.time_rounded = false;

    if (! tdata_hashgrid_setup (&tdata)) {
            return (*env)->NewStringUTF(env, "");
    }

    if (req.arrive_by) {
        req.time_cutoff = 0;
    } else {
        req.time_cutoff = UNREACHED;
    }

    memset (&plan, 0, sizeof(plan_t));
    router_result_init_plan(&plan);
         __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "begin route");
    if ( router_route_full_reversal(&router, &req, &plan) ) {
        __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "begin output");
        jstring result;
        char result_buf[OUTPUT_LEN];

        plan.req.time = req.time;
        router_result_sort (&plan);
        __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "result sort");
        /* what you probably want later here is result pruning
         * remove the results from the plan which really are not
         * an improvement at all, because they are marginally
         * faster and take much extra effort such as an extra
         * transfer.
         */
       __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "before output");
        plan_render_otp(&plan, &tdata, result_buf, OUTPUT_LEN);
        __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", "after output");

        return (*env)->NewStringUTF(env, result_buf);
    }

    return (*env)->NewStringUTF(env, "");
}
