#include <jni.h>
#include <android/bitmap.h>
#include <rlottie.h>
#include <zlib.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

namespace {

struct LottieInfo {
    std::unique_ptr<rlottie::Animation> animation;
    size_t frameCount = 0;
    int32_t fps = 30;
};

static bool readFile(const char *path, std::string &out) {
    gzFile file = gzopen(path, "rb");
    if (file == nullptr) {
        return false;
    }
    char buffer[16 * 1024];
    int read;
    while ((read = gzread(file, buffer, sizeof(buffer))) > 0) {
        out.append(buffer, static_cast<size_t>(read));
    }
    const bool ok = read == 0;
    gzclose(file);
    return ok && !out.empty();
}

static std::unique_ptr<std::map<int32_t, int32_t>> readColorReplacements(JNIEnv *env, jintArray colorReplacement) {
    std::unique_ptr<std::map<int32_t, int32_t>> colors;
    if (colorReplacement == nullptr) {
        return colors;
    }
    jint *arr = env->GetIntArrayElements(colorReplacement, nullptr);
    if (arr != nullptr) {
        const jsize len = env->GetArrayLength(colorReplacement);
        colors.reset(new std::map<int32_t, int32_t>());
        for (int32_t a = 0; a < len / 2; a++) {
            (*colors)[arr[a * 2]] = arr[a * 2 + 1];
        }
        env->ReleaseIntArrayElements(colorReplacement, arr, 0);
    }
    return colors;
}

static std::string normalizePrefix(std::string prefix) {
    if (prefix.size() >= 3 && prefix.compare(prefix.size() - 3, 3, ".**") == 0) {
        prefix.resize(prefix.size() - 3);
    } else if (prefix.size() >= 2 && prefix.compare(prefix.size() - 2, 2, "**") == 0) {
        prefix.resize(prefix.size() - 2);
    }
    return prefix;
}

static void applyLayerColors(JNIEnv *env, rlottie::Animation *animation, jobjectArray layerNames, jintArray layerColors) {
    if (animation == nullptr || layerNames == nullptr || layerColors == nullptr) {
        return;
    }
    const jsize count = std::min(env->GetArrayLength(layerNames), env->GetArrayLength(layerColors));
    jint *values = env->GetIntArrayElements(layerColors, nullptr);
    if (values == nullptr) {
        return;
    }
    for (jsize i = 0; i < count; ++i) {
        auto name = static_cast<jstring>(env->GetObjectArrayElement(layerNames, i));
        if (name == nullptr) {
            continue;
        }
        const char *chars = env->GetStringUTFChars(name, nullptr);
        if (chars != nullptr) {
            const jint color = values[i];
            animation->setValue<rlottie::Property::Color>(
                    normalizePrefix(chars),
                    rlottie::Color(
                            ((color) & 0xff) / 255.0f,
                            ((color >> 8) & 0xff) / 255.0f,
                            ((color >> 16) & 0xff) / 255.0f));
            env->ReleaseStringUTFChars(name, chars);
        }
        env->DeleteLocalRef(name);
    }
    env->ReleaseIntArrayElements(layerColors, values, JNI_ABORT);
}

static rlottie::FitzModifier toFitzModifier(jint modifier) {
    switch (modifier) {
        case 12: return rlottie::FitzModifier::Type12;
        case 3: return rlottie::FitzModifier::Type3;
        case 4: return rlottie::FitzModifier::Type4;
        case 5: return rlottie::FitzModifier::Type5;
        case 6: return rlottie::FitzModifier::Type6;
        default: return rlottie::FitzModifier::None;
    }
}

static jlong finishCreate(JNIEnv *env, std::unique_ptr<rlottie::Animation> animation,
                          std::unique_ptr<std::map<int32_t, int32_t>> colors, jintArray data,
                          jobjectArray layerNames, jintArray layerColors) {
    if (animation == nullptr) {
        return 0;
    }
    colors.release();
    applyLayerColors(env, animation.get(), layerNames, layerColors);
    auto *info = new LottieInfo();
    info->animation = std::move(animation);
    info->frameCount = info->animation->totalFrame();
    info->fps = (int32_t) info->animation->frameRate();
    if (info->fps <= 0 || info->fps > 60 || info->frameCount > 600) {
        delete info;
        return 0;
    }
    jint *dataArr = env->GetIntArrayElements(data, nullptr);
    if (dataArr != nullptr) {
        dataArr[0] = (jint) info->frameCount;
        dataArr[1] = (jint) info->fps;
        dataArr[2] = 0;
        env->ReleaseIntArrayElements(data, dataArr, 0);
    }
    return (jlong) (intptr_t) info;
}

} // namespace

extern "C" {

JNIEXPORT jlong JNICALL Java_org_telegram_ui_Components_RLottieNative_nCreate(
        JNIEnv *env, jclass clazz, jstring src, jstring json, jint w, jint h, jintArray data,
        jboolean precache, jintArray colorReplacement, jboolean limitFps, jint fitzModifier,
        jobjectArray layerNames, jintArray layerColors) {
    std::unique_ptr<std::map<int32_t, int32_t>> colors = readColorReplacements(env, colorReplacement);

    std::string input;
    std::string key;
    if (json != nullptr) {
        const char *chars = env->GetStringUTFChars(json, nullptr);
        if (chars != nullptr) {
            input.assign(chars, static_cast<size_t>(env->GetStringUTFLength(json)));
            env->ReleaseStringUTFChars(json, chars);
        }
        if (src != nullptr) {
            const char *path = env->GetStringUTFChars(src, nullptr);
            if (path != nullptr) {
                key = path;
                env->ReleaseStringUTFChars(src, path);
            }
        }
    } else if (src != nullptr) {
        const char *path = env->GetStringUTFChars(src, nullptr);
        if (path != nullptr) {
            readFile(path, input);
            key = path;
            env->ReleaseStringUTFChars(src, path);
        }
    }
    if (input.empty()) {
        return 0;
    }
    if (key.empty()) {
        key = "animation";
    }

    std::unique_ptr<rlottie::Animation> animation = rlottie::Animation::loadFromData(
            std::move(input), key, colors.get(), toFitzModifier(fitzModifier));
    return finishCreate(env, std::move(animation), std::move(colors), data, layerNames, layerColors);
}

JNIEXPORT jlong JNICALL Java_org_telegram_ui_Components_RLottieNative_nCreateWithJson(
        JNIEnv *env, jclass clazz, jstring json, jstring name, jintArray data,
        jintArray colorReplacement, jobjectArray layerNames, jintArray layerColors) {
    if (json == nullptr) {
        return 0;
    }
    const char *chars = env->GetStringUTFChars(json, nullptr);
    if (chars == nullptr) {
        return 0;
    }
    std::string input(chars, static_cast<size_t>(env->GetStringUTFLength(json)));
    env->ReleaseStringUTFChars(json, chars);

    std::string key = "animation";
    if (name != nullptr) {
        const char *nameChars = env->GetStringUTFChars(name, nullptr);
        if (nameChars != nullptr) {
            key = nameChars;
            env->ReleaseStringUTFChars(name, nameChars);
        }
    }

    std::unique_ptr<std::map<int32_t, int32_t>> colors = readColorReplacements(env, colorReplacement);
    std::unique_ptr<rlottie::Animation> animation = rlottie::Animation::loadFromData(
            std::move(input), key, colors.get(), rlottie::FitzModifier::None);
    return finishCreate(env, std::move(animation), std::move(colors), data, layerNames, layerColors);
}

JNIEXPORT void JNICALL Java_org_telegram_ui_Components_RLottieNative_nDestroy(JNIEnv *, jclass, jlong ptr) {
    if (!ptr) {
        return;
    }
    delete (LottieInfo *) (intptr_t) ptr;
}

JNIEXPORT jint JNICALL Java_org_telegram_ui_Components_RLottieNative_nGetFrame(
        JNIEnv *env, jclass clazz, jlong ptr, jint frame, jobject bitmap, jboolean clear) {
    if (!ptr || bitmap == nullptr) {
        return 0;
    }
    auto *info = (LottieInfo *) (intptr_t) ptr;

    AndroidBitmapInfo bitmapInfo;
    if (__builtin_expect(AndroidBitmap_getInfo(env, bitmap, &bitmapInfo) != ANDROID_BITMAP_RESULT_SUCCESS, 0)) {
        return 0;
    }
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        bitmapInfo.format != ANDROID_BITMAP_FORMAT_A_8) {
        return 0;
    }

    void *pixels = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        return -5;
    }

    bool result = false;
    const size_t count = static_cast<size_t>(bitmapInfo.width) * bitmapInfo.height;

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_A_8) {
        std::vector<uint32_t> temporary(count);
        rlottie::Surface surface(temporary.data(),
                                 static_cast<size_t>(bitmapInfo.width),
                                 static_cast<size_t>(bitmapInfo.height),
                                 static_cast<size_t>(bitmapInfo.width) * sizeof(uint32_t));
        info->animation->renderSync((size_t) frame, surface, clear, &result);
        if (result) {
            auto *destination = static_cast<uint8_t *>(pixels);
            for (uint32_t y = 0; y < bitmapInfo.height; ++y) {
                const uint8_t *sourceRow = reinterpret_cast<const uint8_t *>(temporary.data()) +
                                           static_cast<size_t>(y) * bitmapInfo.width * sizeof(uint32_t);
                uint8_t *destinationRow = destination + static_cast<size_t>(y) * bitmapInfo.stride;
                for (uint32_t x = 0; x < bitmapInfo.width; ++x) {
                    destinationRow[x] = sourceRow[x * sizeof(uint32_t) + 3];
                }
            }
        }
    } else if (bitmapInfo.stride == bitmapInfo.width * sizeof(uint32_t)) {
        rlottie::Surface surface(static_cast<uint32_t *>(pixels),
                                 static_cast<size_t>(bitmapInfo.width),
                                 static_cast<size_t>(bitmapInfo.height),
                                 static_cast<size_t>(bitmapInfo.stride));
        info->animation->renderSync((size_t) frame, surface, clear, &result);
    } else {
        std::vector<uint32_t> temporary(count);
        rlottie::Surface surface(temporary.data(),
                                 static_cast<size_t>(bitmapInfo.width),
                                 static_cast<size_t>(bitmapInfo.height),
                                 static_cast<size_t>(bitmapInfo.width) * sizeof(uint32_t));
        info->animation->renderSync((size_t) frame, surface, clear, &result);
        if (result) {
            auto *destination = static_cast<uint8_t *>(pixels);
            for (uint32_t y = 0; y < bitmapInfo.height; ++y) {
                std::copy_n(reinterpret_cast<const uint8_t *>(temporary.data()) +
                                    static_cast<size_t>(y) * bitmapInfo.width * sizeof(uint32_t),
                            bitmapInfo.width * sizeof(uint32_t),
                            destination + static_cast<size_t>(y) * bitmapInfo.stride);
            }
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    if (!result) {
        return -5;
    }
    return frame;
}
}
