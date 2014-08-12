#ifndef __CC_WebView_INL_H_
#define __CC_WebView_INL_H_

#include "WebView.h"
#include "CCGLView.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

namespace cocos2d {
namespace plugin {
WebView::WebView() : impl(new WebViewImpl(this)) {
}

WebView::~WebView() {
    delete impl;
}

WebView *WebView::create() {
    auto pRet = new WebView();
    if (pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    return nullptr;
}

void WebView::setJavascriptInterfaceScheme(const std::string &scheme) {
    impl->setJavascriptInterfaceScheme(scheme);
}

void WebView::loadData(const cocos2d::Data &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    impl->loadData(data, MIMEType, encoding, baseURL);
}

void WebView::loadHTMLString(const std::string &string, const std::string &baseURL) {
    impl->loadHTMLString(string, baseURL);
}

void WebView::loadUrl(const std::string &url) {
    impl->loadUrl(url);
}

void WebView::loadFile(const std::string &fileName) {
    impl->loadFile(fileName);
}

void WebView::stopLoading() {
    impl->stopLoading();
}

void WebView::reload() {
    impl->reload();
}

bool WebView::canGoBack() {
    return impl->canGoBack();
}

bool WebView::canGoForward() {
    return impl->canGoForward();
}

void WebView::goBack() {
    impl->goBack();
}

void WebView::goForward() {
    impl->goForward();
}

void WebView::evaluateJS(const std::string &js) {
    impl->evaluateJS(js);
}

void WebView::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) {
    cocos2d::ui::Widget::draw(renderer, transform, flags);
    impl->draw(renderer, transform, flags);
}

void WebView::setVisible(bool visible) {
    Node::setVisible(visible);
    impl->setVisible(visible);
}
} // namespace cocos2d
} // namespace plugin

#endif
