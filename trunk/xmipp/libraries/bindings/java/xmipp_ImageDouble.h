/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xmipp_ImageDouble */

#ifndef _Included_xmipp_ImageDouble
#define _Included_xmipp_ImageDouble
#ifdef __cplusplus
extern "C" {
#endif
#undef xmipp_ImageDouble_FIRST_IMAGE
#define xmipp_ImageDouble_FIRST_IMAGE 1L
#undef xmipp_ImageDouble_FIRST_SLICE
#define xmipp_ImageDouble_FIRST_SLICE 1L
#undef xmipp_ImageDouble_ALL_IMAGES
#define xmipp_ImageDouble_ALL_IMAGES 0L
#undef xmipp_ImageDouble_ALL_SLICES
#define xmipp_ImageDouble_ALL_SLICES 0L
#undef xmipp_ImageDouble_MID_SLICE
#define xmipp_ImageDouble_MID_SLICE -1L
/*
 * Class:     xmipp_ImageDouble
 * Method:    storeIds
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_storeIds
  (JNIEnv *, jclass);

/*
 * Class:     xmipp_ImageDouble
 * Method:    create
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_create
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_destroy
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    read_image
 * Signature: (Ljava/lang/String;ZJ)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_read_1image
  (JNIEnv *, jobject, jstring, jboolean, jlong);

/*
 * Class:     xmipp_ImageDouble
 * Method:    read_preview
 * Signature: (Ljava/lang/String;IIIJ)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_read_1preview
  (JNIEnv *, jobject, jstring, jint, jint, jint, jlong);

/*
 * Class:     xmipp_ImageDouble
 * Method:    readApplyGeo
 * Signature: (Ljava/lang/String;Lxmipp/MetaData;JII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_readApplyGeo
  (JNIEnv *, jobject, jstring, jobject, jlong, jint, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    setData
 * Signature: (IIII[D)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_setData
  (JNIEnv *, jobject, jint, jint, jint, jint, jdoubleArray);

/*
 * Class:     xmipp_ImageDouble
 * Method:    write
 * Signature: (Ljava/lang/String;IZII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_write
  (JNIEnv *, jobject, jstring, jint, jboolean, jint, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getData
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_xmipp_ImageDouble_getData__
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getData
 * Signature: (JI)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_xmipp_ImageDouble_getData__JI
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    convertPSD
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_convertPSD
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getXsize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xmipp_ImageDouble_getXsize
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getYsize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xmipp_ImageDouble_getYsize
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getZsize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xmipp_ImageDouble_getZsize
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    getNsize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_xmipp_ImageDouble_getNsize
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    setXmippOrigin
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_setXmippOrigin
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    printShape
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_printShape
  (JNIEnv *, jobject);

/*
 * Class:     xmipp_ImageDouble
 * Method:    fastEstimateEnhancedPSD
 * Signature: (Ljava/lang/String;DII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_fastEstimateEnhancedPSD
  (JNIEnv *, jobject, jstring, jdouble, jint, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    bandPassFilter
 * Signature: (Ljava/lang/String;DDDII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_bandPassFilter
  (JNIEnv *, jobject, jstring, jdouble, jdouble, jdouble, jint, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    gaussianFilter
 * Signature: (Ljava/lang/String;DII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_gaussianFilter
  (JNIEnv *, jobject, jstring, jdouble, jint, jint);

/*
 * Class:     xmipp_ImageDouble
 * Method:    badPixelsFilter
 * Signature: (Ljava/lang/String;DII)V
 */
JNIEXPORT void JNICALL Java_xmipp_ImageDouble_badPixelsFilter
  (JNIEnv *, jobject, jstring, jdouble, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
