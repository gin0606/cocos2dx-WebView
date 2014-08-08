//
// Created by gin0606 on 2014/07/30.
//

#ifndef __cocos2d_plugin_WebViewImpl_IOS_H_
#define __cocos2d_plugin_WebViewImpl_IOS_H_

#include <iosfwd>

@class UIWebViewWrapper;

namespace cocos2d {
class Renderer;
class Mat4;
namespace plugin {
class WebView;

class WebViewImpl {
public:
    WebViewImpl(cocos2d::plugin::WebView *webView);

    virtual ~WebViewImpl();

    void setJavascriptInterfaceScheme(const std::string &scheme);

    void loadUrl(const std::string &url);

    void loadFile(const std::string &fileName);

    bool canGoBack();

    bool canGoForward();

    void goBack();

    void goForward();

    void evaluateJS(const std::string &js);

    virtual void draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags);

    virtual void setVisible(bool visible);

private:
    UIWebViewWrapper *uiWebViewWrapper;
    WebView *webView;
};

} // namespace cocos2d
} // namespace plugin

#endif //__cocos2d_plugin_WebViewImpl_IOS_H_
