cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "-std=c++14 -DANDROID -g")
set(CMAKE_C_FLAGS "-w -std=c11 -DANDROID -D_LARGEFILE_SOURCE=1 -g -Wno-error=implicit-function-declaration")
set(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden -flto=full")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden -fvisibility-inlines-hidden -flto=full")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto=full -Wl,--gc-sections -Wl,--icf=safe -Wl,-Bsymbolic")

if (${ANDROID_ABI} STREQUAL "armeabi-v7a" OR ${ANDROID_ABI} STREQUAL "arm64-v8a")
    enable_language(ASM)
else()
    enable_language(ASM_NASM)
endif()

set(CAN_COMPILE_SME FALSE CACHE INTERNAL "Disable libyuv SME" FORCE)
add_subdirectory(
        third_party/libyuv
        ${CMAKE_CURRENT_BINARY_DIR}/libyuv
        EXCLUDE_FROM_ALL)

add_library(avutil STATIC IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/ffmpeg/build/${ANDROID_ABI}/lib/libavutil.a)

add_library(avformat STATIC IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/ffmpeg/build/${ANDROID_ABI}/lib/libavformat.a)

add_library(avcodec STATIC IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/ffmpeg/build/${ANDROID_ABI}/lib/libavcodec.a)

add_library(swresample STATIC IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/ffmpeg/build/${ANDROID_ABI}/lib/libswresample.a)

add_library(swscale STATIC IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/ffmpeg/build/${ANDROID_ABI}/lib/libswscale.a)

add_library(crypto STATIC IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/libressl/build/${ANDROID_ABI}/crypto/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/libressl/build/${ANDROID_ABI}/ssl/libssl.a)

add_library(libvpx STATIC IMPORTED)
set_target_properties(libvpx PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/libvpx/build/${ANDROID_ABI}/lib/libvpx.a)

add_library(libdav1d STATIC IMPORTED)
set_target_properties(libdav1d PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/dav1d/build/${ANDROID_ABI}/lib/libdav1d.a)

add_library(tde2e STATIC IMPORTED)
set_target_properties(tde2e PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/tde2e/${ANDROID_ABI}/libtde2e.a)

add_library(tdutils STATIC IMPORTED)
set_target_properties(tdutils PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/tde2e/${ANDROID_ABI}/libtdutils.a)

#tgnet
add_library(tgnet STATIC
        tgnet/ApiScheme.cpp
        tgnet/BuffersStorage.cpp
        tgnet/ByteArray.cpp
        tgnet/ByteStream.cpp
        tgnet/Connection.cpp
        tgnet/ConnectionSession.cpp
        tgnet/ConnectionsManager.cpp
        tgnet/ConnectionSocket.cpp
        tgnet/Datacenter.cpp
        tgnet/EventObject.cpp
        tgnet/FileLog.cpp
        tgnet/MTProtoScheme.cpp
        tgnet/NativeByteBuffer.cpp
        tgnet/Request.cpp
        tgnet/Timer.cpp
        tgnet/TLObject.cpp
        tgnet/ProxyCheckInfo.cpp
        tgnet/Handshake.cpp
        tgnet/Config.cpp)
target_compile_options(tgnet PUBLIC
        -Wall -frtti -finline-functions -ffast-math -Oz)
set_target_properties(tgnet PROPERTIES
        ANDROID_ARM_MODE arm)
target_compile_definitions(tgnet PUBLIC
        HAVE_PTHREAD)
target_include_directories(tgnet PUBLIC
        libressl/include/)
target_link_libraries(tgnet
        crypto)

#rlottie
add_library(rlottie STATIC
        rlottie/src/lottie/lottieanimation.cpp
        rlottie/src/lottie/lottieitem.cpp
        rlottie/src/lottie/lottiekeypath.cpp
        rlottie/src/lottie/lottieloader.cpp
        rlottie/src/lottie/lottiemodel.cpp
        rlottie/src/lottie/lottieparser.cpp
        rlottie/src/lottie/lottieproxymodel.cpp
        rlottie/src/vector/freetype/v_ft_math.cpp
        rlottie/src/vector/freetype/v_ft_raster.cpp
        rlottie/src/vector/freetype/v_ft_stroker.cpp
        rlottie/src/vector/pixman/vregion.cpp
        rlottie/src/vector/stb/stb_image.cpp
        rlottie/src/vector/vbezier.cpp
        rlottie/src/vector/vbitmap.cpp
        rlottie/src/vector/vbrush.cpp
        rlottie/src/vector/vcompositionfunctions.cpp
        rlottie/src/vector/vdasher.cpp
        rlottie/src/vector/vdebug.cpp
        rlottie/src/vector/vdrawable.cpp
        rlottie/src/vector/vdrawhelper.cpp
        rlottie/src/vector/vdrawhelper_neon.cpp
        rlottie/src/vector/velapsedtimer.cpp
        rlottie/src/vector/vimageloader.cpp
        rlottie/src/vector/vinterpolator.cpp
        rlottie/src/vector/vmatrix.cpp
        rlottie/src/vector/vpainter.cpp
        rlottie/src/vector/vpath.cpp
        rlottie/src/vector/vpathmesure.cpp
        rlottie/src/vector/vraster.cpp
        rlottie/src/vector/vrect.cpp
        rlottie/src/vector/vrle.cpp)
target_compile_options(rlottie PUBLIC
        -Wall -fno-rtti -finline-functions -ffast-math -Oz -fno-unwind-tables -fno-asynchronous-unwind-tables -Wnon-virtual-dtor -Woverloaded-virtual -Wno-unused-parameter -fvisibility=hidden)
set_target_properties(rlottie PROPERTIES
        ANDROID_ARM_MODE arm)
target_compile_definitions(rlottie PUBLIC
        HAVE_PTHREAD NDEBUG)
target_include_directories(rlottie PUBLIC
        ./
        rlottie/inc
        rlottie/src/vector/
        rlottie/src/vector/pixman
        rlottie/src/vector/freetype
        rlottie/src/vector/stb)

if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    target_compile_definitions(rlottie PUBLIC
            USE_ARM_NEON)
    target_sources(rlottie PRIVATE
            rlottie/src/vector/pixman/pixman-arm-neon-asm.S)
elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
    target_compile_definitions(rlottie PUBLIC
            USE_ARM_NEON __ARM64_NEON__)
    target_sources(rlottie PRIVATE
            rlottie/src/vector/pixman/pixman-arma64-neon-asm.S)
endif()

#sqlite
add_library(sqlite STATIC
        sqlite/sqlite3.c)
target_compile_options(sqlite PUBLIC
        -fno-strict-aliasing -Oz)
set_target_properties(sqlite PROPERTIES
        ANDROID_ARM_MODE arm)
target_compile_definitions(sqlite PUBLIC
        NULL=0 SOCKLEN_T=socklen_t LOCALE_NOT_USED ANDROID_NDK DISABLE_IMPORTGL AVOID_TABLES ANDROID_TILE_BASED_DECODE HAVE_STRCHRNUL=0 ANDROID_ARMV6_IDCT)

#breakpad
add_library(breakpad STATIC
        third_party/breakpad/src/client/linux/crash_generation/crash_generation_client.cc
        third_party/breakpad/src/client/linux/handler/exception_handler.cc
        third_party/breakpad/src/client/linux/handler/minidump_descriptor.cc
        third_party/breakpad/src/client/linux/log/log.cc
        third_party/breakpad/src/client/linux/dump_writer_common/thread_info.cc
        third_party/breakpad/src/client/linux/dump_writer_common/seccomp_unwinder.cc
        third_party/breakpad/src/client/linux/dump_writer_common/ucontext_reader.cc
        third_party/breakpad/src/client/linux/microdump_writer/microdump_writer.cc
        third_party/breakpad/src/client/linux/minidump_writer/linux_dumper.cc
        third_party/breakpad/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
        third_party/breakpad/src/client/linux/minidump_writer/minidump_writer.cc
        third_party/breakpad/src/client/minidump_file_writer.cc
        third_party/breakpad/src/common/android/breakpad_getcontext.S
        third_party/breakpad/src/common/convert_UTF.c
        third_party/breakpad/src/common/md5.cc
        third_party/breakpad/src/common/string_conversion.cc
        third_party/breakpad/src/common/linux/elfutils.cc
        third_party/breakpad/src/common/linux/file_id.cc
        third_party/breakpad/src/common/linux/guid_creator.cc
        third_party/breakpad/src/common/linux/linux_libc_support.cc
        third_party/breakpad/src/common/linux/memory_mapped_file.cc
        third_party/breakpad/src/common/linux/safe_readlink.cc)
set_target_properties(breakpad PROPERTIES ANDROID_ARM_MODE arm)
set_property(SOURCE third_party/breakpad/src/common/android/breakpad_getcontext.S PROPERTY LANGUAGE C)
target_include_directories(breakpad PUBLIC
        third_party/breakpad/src/common/android/include
        third_party/breakpad/src)


#voip
include(${CMAKE_HOME_DIRECTORY}/voip/CMakeLists.txt)

#libopus
add_library(libopus STATIC
        third_party/xiph/opus/src/opus.c
        third_party/xiph/opus/src/opus_decoder.c
        third_party/xiph/opus/src/opus_encoder.c
        third_party/xiph/opus/src/opus_multistream.c
        third_party/xiph/opus/src/opus_multistream_encoder.c
        third_party/xiph/opus/src/opus_multistream_decoder.c
        third_party/xiph/opus/src/repacketizer.c
        third_party/xiph/opus/src/analysis.c
        third_party/xiph/opus/src/mlp.c
        third_party/xiph/opus/src/mlp_data.c
        third_party/xiph/opus/src/opus_projection_encoder.c
        third_party/xiph/opus/src/opus_projection_decoder.c
        third_party/xiph/opus/src/mapping_matrix.c
        third_party/xiph/opus/src/extensions.c

        third_party/xiph/opus/silk/CNG.c
        third_party/xiph/opus/silk/code_signs.c
        third_party/xiph/opus/silk/init_decoder.c
        third_party/xiph/opus/silk/decode_core.c
        third_party/xiph/opus/silk/decode_frame.c
        third_party/xiph/opus/silk/decode_parameters.c
        third_party/xiph/opus/silk/decode_indices.c
        third_party/xiph/opus/silk/decode_pulses.c
        third_party/xiph/opus/silk/decoder_set_fs.c
        third_party/xiph/opus/silk/dec_API.c
        third_party/xiph/opus/silk/enc_API.c
        third_party/xiph/opus/silk/encode_indices.c
        third_party/xiph/opus/silk/encode_pulses.c
        third_party/xiph/opus/silk/gain_quant.c
        third_party/xiph/opus/silk/interpolate.c
        third_party/xiph/opus/silk/LP_variable_cutoff.c
        third_party/xiph/opus/silk/NLSF_decode.c
        third_party/xiph/opus/silk/NSQ.c
        third_party/xiph/opus/silk/NSQ_del_dec.c
        third_party/xiph/opus/silk/PLC.c
        third_party/xiph/opus/silk/shell_coder.c
        third_party/xiph/opus/silk/tables_gain.c
        third_party/xiph/opus/silk/tables_LTP.c
        third_party/xiph/opus/silk/tables_NLSF_CB_NB_MB.c
        third_party/xiph/opus/silk/tables_NLSF_CB_WB.c
        third_party/xiph/opus/silk/tables_other.c
        third_party/xiph/opus/silk/tables_pitch_lag.c
        third_party/xiph/opus/silk/tables_pulses_per_block.c
        third_party/xiph/opus/silk/VAD.c
        third_party/xiph/opus/silk/control_audio_bandwidth.c
        third_party/xiph/opus/silk/quant_LTP_gains.c
        third_party/xiph/opus/silk/VQ_WMat_EC.c
        third_party/xiph/opus/silk/HP_variable_cutoff.c
        third_party/xiph/opus/silk/NLSF_encode.c
        third_party/xiph/opus/silk/NLSF_VQ.c
        third_party/xiph/opus/silk/NLSF_unpack.c
        third_party/xiph/opus/silk/NLSF_del_dec_quant.c
        third_party/xiph/opus/silk/process_NLSFs.c
        third_party/xiph/opus/silk/stereo_LR_to_MS.c
        third_party/xiph/opus/silk/stereo_MS_to_LR.c
        third_party/xiph/opus/silk/check_control_input.c
        third_party/xiph/opus/silk/control_SNR.c
        third_party/xiph/opus/silk/init_encoder.c
        third_party/xiph/opus/silk/control_codec.c
        third_party/xiph/opus/silk/A2NLSF.c
        third_party/xiph/opus/silk/ana_filt_bank_1.c
        third_party/xiph/opus/silk/biquad_alt.c
        third_party/xiph/opus/silk/bwexpander_32.c
        third_party/xiph/opus/silk/bwexpander.c
        third_party/xiph/opus/silk/debug.c
        third_party/xiph/opus/silk/decode_pitch.c
        third_party/xiph/opus/silk/inner_prod_aligned.c
        third_party/xiph/opus/silk/lin2log.c
        third_party/xiph/opus/silk/log2lin.c
        third_party/xiph/opus/silk/LPC_analysis_filter.c
        third_party/xiph/opus/silk/LPC_inv_pred_gain.c
        third_party/xiph/opus/silk/table_LSF_cos.c
        third_party/xiph/opus/silk/NLSF2A.c
        third_party/xiph/opus/silk/NLSF_stabilize.c
        third_party/xiph/opus/silk/NLSF_VQ_weights_laroia.c
        third_party/xiph/opus/silk/pitch_est_tables.c
        third_party/xiph/opus/silk/resampler.c
        third_party/xiph/opus/silk/resampler_down2_3.c
        third_party/xiph/opus/silk/resampler_down2.c
        third_party/xiph/opus/silk/resampler_private_AR2.c
        third_party/xiph/opus/silk/resampler_private_down_FIR.c
        third_party/xiph/opus/silk/resampler_private_IIR_FIR.c
        third_party/xiph/opus/silk/resampler_private_up2_HQ.c
        third_party/xiph/opus/silk/resampler_rom.c
        third_party/xiph/opus/silk/sigm_Q15.c
        third_party/xiph/opus/silk/sort.c
        third_party/xiph/opus/silk/sum_sqr_shift.c
        third_party/xiph/opus/silk/stereo_decode_pred.c
        third_party/xiph/opus/silk/stereo_encode_pred.c
        third_party/xiph/opus/silk/stereo_find_predictor.c
        third_party/xiph/opus/silk/stereo_quant_pred.c
        third_party/xiph/opus/silk/LPC_fit.c

        third_party/xiph/opus/silk/fixed/LTP_analysis_filter_FIX.c
        third_party/xiph/opus/silk/fixed/LTP_scale_ctrl_FIX.c
        third_party/xiph/opus/silk/fixed/corrMatrix_FIX.c
        third_party/xiph/opus/silk/fixed/encode_frame_FIX.c
        third_party/xiph/opus/silk/fixed/find_LPC_FIX.c
        third_party/xiph/opus/silk/fixed/find_LTP_FIX.c
        third_party/xiph/opus/silk/fixed/find_pitch_lags_FIX.c
        third_party/xiph/opus/silk/fixed/find_pred_coefs_FIX.c
        third_party/xiph/opus/silk/fixed/noise_shape_analysis_FIX.c
        third_party/xiph/opus/silk/fixed/process_gains_FIX.c
        third_party/xiph/opus/silk/fixed/regularize_correlations_FIX.c
        third_party/xiph/opus/silk/fixed/residual_energy16_FIX.c
        third_party/xiph/opus/silk/fixed/residual_energy_FIX.c
        third_party/xiph/opus/silk/fixed/warped_autocorrelation_FIX.c
        third_party/xiph/opus/silk/fixed/apply_sine_window_FIX.c
        third_party/xiph/opus/silk/fixed/autocorr_FIX.c
        third_party/xiph/opus/silk/fixed/burg_modified_FIX.c
        third_party/xiph/opus/silk/fixed/k2a_FIX.c
        third_party/xiph/opus/silk/fixed/k2a_Q16_FIX.c
        third_party/xiph/opus/silk/fixed/pitch_analysis_core_FIX.c
        third_party/xiph/opus/silk/fixed/vector_ops_FIX.c
        third_party/xiph/opus/silk/fixed/schur64_FIX.c
        third_party/xiph/opus/silk/fixed/schur_FIX.c

        third_party/xiph/opus/celt/bands.c
        third_party/xiph/opus/celt/celt.c
        third_party/xiph/opus/celt/celt_encoder.c
        third_party/xiph/opus/celt/celt_decoder.c
        third_party/xiph/opus/celt/cwrs.c
        third_party/xiph/opus/celt/entcode.c
        third_party/xiph/opus/celt/entdec.c
        third_party/xiph/opus/celt/entenc.c
        third_party/xiph/opus/celt/kiss_fft.c
        third_party/xiph/opus/celt/laplace.c
        third_party/xiph/opus/celt/mathops.c
        third_party/xiph/opus/celt/mdct.c
        third_party/xiph/opus/celt/modes.c
        third_party/xiph/opus/celt/pitch.c
        third_party/xiph/opus/celt/celt_lpc.c
        third_party/xiph/opus/celt/quant_bands.c
        third_party/xiph/opus/celt/rate.c
        third_party/xiph/opus/celt/vq.c
        third_party/xiph/opus/celt/arm/armcpu.c
        third_party/xiph/opus/celt/arm/arm_celt_map.c
)
target_compile_options(libopus PUBLIC
        -ffast-math -Oz -funroll-loops -fno-strict-aliasing -fno-math-errno)
set_target_properties(libopus PROPERTIES
        OUTPUT_NAME opus
        ANDROID_ARM_MODE arm)
target_compile_definitions(libopus PUBLIC
        OPUS_BUILD FIXED_POINT USE_ALLOCA restrict= __EMX__ LOCALE_NOT_USED HAVE_LRINT HAVE_LRINTF)
if (${ANDROID_ABI} STREQUAL "armeabi-v7a" OR ${ANDROID_ABI} STREQUAL "arm64-v8a")
    target_compile_definitions(libopus PUBLIC
            OPUS_HAVE_RTCD OPUS_ARM_ASM)
    target_sources(libopus PRIVATE
            third_party/xiph/opus/celt/arm/celt_neon_intr.c
            third_party/xiph/opus/celt/arm/pitch_neon_intr.c
            third_party/xiph/opus/silk/arm/NSQ_neon.c
            third_party/xiph/opus/silk/arm/arm_silk_map.c
            third_party/xiph/opus/silk/arm/LPC_inv_pred_gain_neon_intr.c
            third_party/xiph/opus/silk/arm/NSQ_del_dec_neon_intr.c
            third_party/xiph/opus/silk/arm/biquad_alt_neon_intr.c
            third_party/xiph/opus/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c
            )
endif()
target_include_directories(libopus PUBLIC
        third_party/xiph/opus/include
        third_party/xiph/opus/silk
        third_party/xiph/opus/silk/fixed
        third_party/xiph/opus/celt
        third_party/xiph/opus)

set(NATIVE_LIB "tmessages.49")

#tmessages
add_library(${NATIVE_LIB} SHARED
        jni.c
        audio.c
        webm_encoder.c
        image.cpp
        intro/IntroRenderer.c
        SqliteWrapper.cpp
        utilities.cpp
        gifvideo.cpp
        lottie.cpp
        TgNetWrapper.cpp
        exoplayer/ffmpeg_jni.cc
        fast-edge.cpp
        genann.c
        secureid_ocr.cpp
        tde2e/bridge.cpp
)

target_compile_options(${NATIVE_LIB} PUBLIC
        -ffast-math -Oz -funroll-loops -ffast-math -fno-strict-aliasing -fno-math-errno)

target_compile_definitions(${NATIVE_LIB} PUBLIC
        BSD=1 NULL=0 SOCKLEN_T=socklen_t ANDROID_NDK DISABLE_IMPORTGL AVOID_TABLES ANDROID_TILE_BASED_DECODE __STDC_CONSTANT_MACROS ANDROID_ARMV6_IDCT)

if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set_target_properties(${NATIVE_LIB} PROPERTIES
            ANDROID_ARM_NEON FALSE
            ANDROID_ARM_MODE arm)
    target_compile_definitions(${NATIVE_LIB} PUBLIC
            ANDROID_ARM_NEON=false)
else()
    set_target_properties(${NATIVE_LIB} PROPERTIES
            ANDROID_ARM_NEON FALSE
            ANDROID_ARM_MODE arm)
endif()

if (${ANDROID_ABI} STREQUAL "armeabi-v7a" OR ${ANDROID_ABI} STREQUAL "arm64-v8a")
    target_compile_definitions(${NATIVE_LIB} PUBLIC
            LIBYUV_NEON)
endif()

target_sources(${NATIVE_LIB} PRIVATE

        third_party/xiph/opusfile/src/info.c
        third_party/xiph/opusfile/src/internal.c
        third_party/xiph/opusfile/src/opusfile.c
        third_party/xiph/opusfile/src/stream.c
        third_party/xiph/ogg/src/bitwise.c
        third_party/xiph/ogg/src/framing.c)

target_include_directories(${NATIVE_LIB} PUBLIC
        third_party/xiph/opus/include
        third_party/xiph/opus/silk
        third_party/xiph/opus/silk/fixed
        third_party/xiph/opus/celt
        third_party/xiph/opus
        third_party/xiph/opusfile/include
        third_party/xiph/ogg/include
        third_party/libyuv/include
        libressl/include
        ffmpeg/build/${ANDROID_ABI}/include
        ffmpeg/libavformat
        libvpx/build/${ANDROID_ABI}/include
        emoji
        exoplayer/include
        intro
        tgcalls/
        webrtc/
        tde2e/include
)

target_link_options(${NATIVE_LIB} PRIVATE "-Wl,-z,max-page-size=16384")
target_link_options(${NATIVE_LIB} PRIVATE "-Wl,-z,common-page-size=16384")
target_link_options(${NATIVE_LIB} PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports.map")
target_link_libraries(${NATIVE_LIB}
        -Wl,--whole-archive rnnoise openh264 voipandroid -Wl,--no-whole-archive
        tgvoip
        tgcalls
        tgcalls_tp
        tgnet
        rlottie
        sqlite
        swscale
        avformat
        avcodec
        swresample
        libvpx
        libdav1d
        libopus
        tde2e
        tdutils
        yuv
        avutil
        ssl
        crypto
        jnigraphics
        log
        z
        GLESv2
        EGL
        android
        OpenSLES
        cpufeatures
        breakpad)

#if (${ANDROID_ABI} STREQUAL "x86" OR ${ANDROID_ABI} STREQUAL "x86_64")
#    target_link_libraries(${NATIVE_LIB}
#            -Wl,--whole-archive vpxasm -Wl,--no-whole-archive
#            c)
#endif()

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()
