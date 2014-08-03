//
// Created by gin0606 on 2014/07/30.
//

#ifndef __cocos2d_plugin_WebViewImpl_android_H_
#define __cocos2d_plugin_WebViewImpl_android_H_

#include <iosfwd>

namespace cocos2d {
class Renderer;
class Mat4;

namespace plugin {
class WebView;
}
}

namespace cocos2d {
namespace plugin {

class WebViewImpl {
public:
    WebViewImpl(cocos2d::plugin::WebView *webView);

    virtual ~WebViewImpl();

    void loadUrl(const std::string &url);

    void loadFile(const std::string &fileName);

    void evaluateJS(const std::string &js);

    virtual void draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags);

    virtual void setVisible(bool visible);

    static bool shouldStartLoading(const int viewTag, const std::string &url);
    static void didFinishLoading(const int viewTag, const std::string &url);
    static void didFailLoading(const int viewTag, const std::string &url);
    static void onJsCallback(const int viewTag, const std::string &message);

private:
    int viewTag;
    WebView *webView;
};

} // namespace cocos2d
} // namespace plugin

#endif //__cocos2d_plugin_WebViewImpl_android_H_
