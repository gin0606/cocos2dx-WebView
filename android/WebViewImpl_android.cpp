//
// Created by gin0606 on 2014/07/30.
//

#include "WebViewImpl_android.h"
#include "WebView.h"
#include "org_cocos2dx_lib_Cocos2dxWebViewHelper.h"
#include "jni/JniHelper.h"
#include "CCGLView.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"
#include <unordered_map>

#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxWebViewHelper"

static int createWebViewJNI() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createWebView", "()I")) {
        jint viewTag = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return viewTag;
    }
    return -1;
}

static void removeWebViewJNI(int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "removeWebView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

static void setWebViewRectJNI(int index,int left,int top,int width,int height) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setWebViewRect", "(IIIII)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, left, top, width, height);
        t.env->DeleteLocalRef(t.classID);
    }
}

static void loadUrlJNI(int index, std::string url) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadUrl", "(ILjava/lang/String;)V")) {
        jstring jUrl = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jUrl);
        t.env->DeleteLocalRef(t.classID);
    }
}

static void loadFileJNI(int index, std::string filePath) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadFile", "(ILjava/lang/String;)V")) {
        jstring jFilePath = t.env->NewStringUTF(filePath.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jFilePath);
        t.env->DeleteLocalRef(t.classID);
    }
}

static void evaluateJSJNI(int index, std::string js) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "evaluateJS", "(ILjava/lang/String;)V")) {
        jstring jjs = t.env->NewStringUTF(js.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jjs);
        t.env->DeleteLocalRef(t.classID);
    }
}

static void setWebViewVisibleJNI(int index, bool visible) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setVisible", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, visible);
        t.env->DeleteLocalRef(t.classID);
    }
}

namespace {
std::string getUrlStringByFileName(const std::string &fileName) {
    const std::string basePath("file:///android_asset/");
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    const std::string assetsPath("assets/");

    std::string urlString;
    if (fullPath.find(assetsPath) != std::string::npos) {
        urlString = fullPath.replace(fullPath.find_first_of(assetsPath), assetsPath.length(), basePath);
    } else {
        urlString = fullPath;
    }

    return urlString;
}
}

namespace cocos2d {
namespace plugin {
static std::unordered_map<int, cocos2d::plugin::WebViewImpl*> s_WebViewImpls;

WebViewImpl::WebViewImpl(WebView *webView) : viewTag(-1), webView(webView) {
    viewTag = createWebViewJNI();
    s_WebViewImpls[viewTag] = this;
}

WebViewImpl::~WebViewImpl() {
    removeWebViewJNI(viewTag);
    s_WebViewImpls.erase(viewTag);
}

void WebViewImpl::loadUrl(const std::string &url) {
    loadUrlJNI(viewTag, url);
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = getUrlStringByFileName(fileName);
    loadFileJNI(viewTag, fullPath);
}

void WebViewImpl::evaluateJS(const std::string &js) {
    evaluateJSJNI(viewTag, js);
}

bool WebViewImpl::shouldStartLoading(const int viewTag, const std::string &url) {
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->webView;
        if (webView->shouldStartLoading) {
            return webView->shouldStartLoading(webView, url);
        }
    }
    return true;
}

void WebViewImpl::didFinishLoading(const int viewTag, const std::string &url){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->webView;
        if (webView->didFinishLoading) {
            webView->didFinishLoading(webView, url);
        }
    }
}

void WebViewImpl::didFailLoading(const int viewTag, const std::string &url){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->webView;
        if (webView->didFailLoading) {
            webView->didFailLoading(webView, url);
        }
    }
}

void WebViewImpl::onJsCallback(const int viewTag, const std::string &message){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->webView;
        if (webView->onJsCallback) {
            webView->onJsCallback(webView, message);
        }
    }
}

void WebViewImpl::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) {
    if (flags & cocos2d::Node::FLAGS_TRANSFORM_DIRTY) {
        auto directorInstance = cocos2d::Director::getInstance();
        auto glView = directorInstance->getOpenGLView();
        auto frameSize = glView->getFrameSize();

        auto winSize = directorInstance->getWinSize();

        auto leftBottom = this->webView->convertToWorldSpace(cocos2d::Point::ZERO);
        auto rightTop = this->webView->convertToWorldSpace(cocos2d::Point(this->webView->getContentSize().width,this->webView->getContentSize().height));

        auto uiLeft = frameSize.width / 2 + (leftBottom.x - winSize.width / 2 ) * glView->getScaleX();
        auto uiTop = frameSize.height /2 - (rightTop.y - winSize.height / 2) * glView->getScaleY();

        setWebViewRectJNI(viewTag,uiLeft,uiTop,
                        (rightTop.x - leftBottom.x) * glView->getScaleX(),
                        (rightTop.y - leftBottom.y) * glView->getScaleY());
    }
}

void WebViewImpl::setVisible(bool visible) {
    setWebViewVisibleJNI(viewTag, visible);
}
} // namespace cocos2d
} // namespace plugin
