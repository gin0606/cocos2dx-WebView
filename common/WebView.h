//
// Created by gin0606 on 2014/07/29.
//

#ifndef __Cocos2d_Plugin_WebView_H_
#define __Cocos2d_Plugin_WebView_H_

#include "ui/UIWidget.h"
#include "base/CCData.h"

namespace cocos2d {
namespace plugin {
class WebViewImpl;

class WebView : public cocos2d::ui::Widget {
public:
    static WebView *create();

    WebView();
    virtual ~WebView();

    std::function<bool(WebView *sender, std::string url)> shouldStartLoading;
    std::function<void(WebView *sender, std::string url)> didFinishLoading;
    std::function<void(WebView *sender, std::string url)> didFailLoading;

    /**
    * Set javascript interface scheme.
    * See `onJsCallback`.
    */
    void setJavascriptInterfaceScheme(const std::string &scheme);

    /**
    * This callback called when load URL that start with javascript interface scheme.
    */
    std::function<void(WebView *sender, std::string message)> onJsCallback;

    /**
    * Sets the main page contents, MIME type, content encoding, and base URL.
    * @param data The content for the main page.
    * @param MIMEType The MIME type of the data.
    * @param encoding the encoding of the data.
    * @param baseURL The base URL for the content.
    */
    void loadData(const cocos2d::Data &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL);

    /**
    * Sets the main page content and base URL.
    * @param string The content for the main page.
    * @param baseURL The base URL for the content.
    */
    void loadHTMLString(const std::string &string, const std::string &baseURL);

    void loadUrl(const std::string &url);
    void loadFile(const std::string &fileName);

    /**
    * Stops the current load.
    */
    void stopLoading();

    /**
    * Reloads the current URL.
    */
    void reload();

    bool canGoBack();

    bool canGoForward();

    void goBack();

    void goForward();

    void evaluateJS(const std::string &js);

    virtual void draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) override;

    virtual void setVisible(bool visible) override;

private:
    cocos2d::plugin::WebViewImpl *impl;
};
} // namespace cocos2d
} // namespace plugin

#endif //__Cocos2d_Plugin_WebView_H_
