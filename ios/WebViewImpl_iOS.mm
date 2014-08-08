//
// Created by gin0606 on 2014/07/30.
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "WebViewImpl_iOS.h"
#import "UIWebViewWrapper.h"
#include "renderer/CCRenderer.h"
#include "WebView.h"
#include "CCDirector.h"
#include "CCGLView.h"
#include "CCEAGLView.h"
#include "platform/CCFileUtils.h"

namespace cocos2d {
namespace plugin {

WebViewImpl::WebViewImpl(WebView *webView)
        : uiWebViewWrapper([UIWebViewWrapper webViewWrapper]), webView(webView) {
    [uiWebViewWrapper retain];
    uiWebViewWrapper.shouldStartLoading = [this](std::string url) {
        if (this->webView->shouldStartLoading) {
            return this->webView->shouldStartLoading(this->webView, url);
        }
        return true;
    };
    uiWebViewWrapper.didFinishLoading = [this](std::string url) {
        if (this->webView->didFinishLoading) {
            this->webView->didFinishLoading(this->webView, url);
        }
    };
    uiWebViewWrapper.didFailLoading = [this](std::string url) {
        if (this->webView->didFailLoading) {
            this->webView->didFailLoading(this->webView, url);
        }
    };
    uiWebViewWrapper.onJsCallback = [this](std::string url) {
        if (this->webView->onJsCallback) {
            this->webView->onJsCallback(this->webView, url);
        }
    };
}

WebViewImpl::~WebViewImpl() {
    [uiWebViewWrapper release];
    uiWebViewWrapper = nullptr;
}

void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    [uiWebViewWrapper setJavascriptInterfaceScheme:scheme];
}

void WebViewImpl::loadUrl(const std::string &url) {
    [uiWebViewWrapper loadUrl:url];
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    [uiWebViewWrapper loadFile:fullPath];
}

void WebViewImpl::evaluateJS(const std::string &js) {
    [uiWebViewWrapper evaluateJS:js];
}

void WebViewImpl::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) {
    if (flags & cocos2d::Node::FLAGS_TRANSFORM_DIRTY) {
        auto direcrot = cocos2d::Director::getInstance();
        auto glView = direcrot->getOpenGLView();
        auto frameSize = glView->getFrameSize();
        auto scaleFactor = [static_cast<CCEAGLView *>(glView->getEAGLView()) contentScaleFactor];

        auto winSize = direcrot->getWinSize();

        auto leftBottom = this->webView->convertToWorldSpace(cocos2d::Vec2::ZERO);
        auto rightTop = this->webView->convertToWorldSpace(cocos2d::Vec2(this->webView->getContentSize().width, this->webView->getContentSize().height));

        auto x = (frameSize.width / 2 + (leftBottom.x - winSize.width / 2) * glView->getScaleX()) / scaleFactor;
        auto y = (frameSize.height / 2 - (rightTop.y - winSize.height / 2) * glView->getScaleY()) / scaleFactor;
        auto width = (rightTop.x - leftBottom.x) * glView->getScaleX() / scaleFactor;
        auto height = (rightTop.y - leftBottom.y) * glView->getScaleY() / scaleFactor;

        [uiWebViewWrapper setFrameWithX:x
                                      y:y
                                  width:width
                                 height:height];
    }
}

void WebViewImpl::setVisible(bool visible) {
    [uiWebViewWrapper setVisible:visible];
}
} // namespace cocos2d
} // namespace plugin

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS
