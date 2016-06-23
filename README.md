# This repository is DEPRECATED.
This WebView merged into cocos2d-x3.3!

Please use [cocos2d/cocos2d-x](https://github.com/cocos2d/cocos2d-x)

# Require
* cocos2d-x ver3.0(or greater
* android-10(or greater

# How to setup

## common
* create libs directory under target project and clone this repository.

```sh
cd /path/to/my-project
mkdir libs && cd libs
git clone git@github.com:gin0606/cocos2dx-WebView.git
```

## iOS
* Add `cocos2dx-WebView/common` and `cocos2dx-WebView/ios` to your project.

## Android
* Update project.properties file.

```sh
cd /path/to/my-project/libs/cocos2dx-WebView/android/java
android update project -p . -t [android API level]
android update project -p . -l /path/to/my-project/cocos2d/cocos/platform/android/java
cd /path/to/my-project/proj.android
android update project -p . -l /path/to/my-project/libs/cocos2dx-WebView/android/java
```

* Update Android.mk file.

```sh
open /path/to/my-project/proj.android/jni/Android.mk
```

```diff
-# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
+LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
+LOCAL_WHOLE_STATIC_LIBRARIES += cocos_webview_plugin_static
-# $(call import-module,editor-support/cocostudio)
+$(call import-module,editor-support/cocostudio)
+$(call import-module,../../libs/cocos2dx-WebView/android)
```

* Edit Activity class to handle Cocos2dxWebViewHelper instance.

```java
public class AppActivity extends Cocos2dxActivity {
    private Cocos2dxWebViewHelper mWebViewHelper = null;
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(mWebViewHelper == null){
            mWebViewHelper = new Cocos2dxWebViewHelper(mFrameLayout);
        }
    }
}
```

# How to use.
```cpp
#include "WebView.h"

auto webView = cocos2d::plugin::WebView::create();
webView->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
webView->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
webView->loadUrl("http://www.google.co.jp");
this->addChild(webView);

// if set js scheme. call `onJsCallback` when load `hoge-scheme://foo`.
webView->setJavascriptInterfaceScheme("hoge-scheme");
```

