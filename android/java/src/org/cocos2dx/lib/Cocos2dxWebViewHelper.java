package org.cocos2dx.lib;

import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.util.SparseArray;
import android.view.View;
import android.widget.FrameLayout;

import junit.framework.Assert;

import java.lang.ref.WeakReference;

public class Cocos2dxWebViewHelper {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();
    private Cocos2dxActivity cocos2dxActivity;
    private FrameLayout layout;

    static private WebViewHandler handler;
    private static SparseArray<Cocos2dxWebView> webViews;
    private static int viewTag = 0;

    private static final int kWebViewTaskCreate = 0;
    private static final int kWebViewTaskRemove = 1;
    private static final int kWebViewTaskSetRect = 2;
    private static final int kWebViewTaskLoadURI = 3;
    private static final int kWebViewTaskEvaluateJS = 4;
    private static final int kWebViewTaskSetVisible = 5;
    private static final int kWebViewTaskSetJsScheme = 6;
    private static final int kWebViewTaskGoBack = 7;
    private static final int kWebViewTaskGoForward = 8;

    public Cocos2dxWebViewHelper(Cocos2dxActivity activity, FrameLayout layout) {
        this.cocos2dxActivity = activity;
        this.layout = layout;

        Cocos2dxWebViewHelper.webViews = new SparseArray<Cocos2dxWebView>();
        Cocos2dxWebViewHelper.handler = new WebViewHandler(Looper.myLooper(), this);
    }

    private static native boolean shouldStartLoading(int index, String message);

    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }

    private static native void didFinishLoading(int index, String message);

    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }

    private static native void didFailLoading(int index, String message);

    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }

    private static native void onJsCallback(int index, String message);

    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }

    static class WebViewHandler extends Handler {
        WeakReference<Cocos2dxWebViewHelper> helperReference;

        WebViewHandler(Looper looper, Cocos2dxWebViewHelper webViewHelper) {
            super(looper);
            this.helperReference = new WeakReference<Cocos2dxWebViewHelper>(webViewHelper);
        }

        @Override
        public void handleMessage(Message msg) {
            Cocos2dxWebViewHelper helper = this.helperReference.get();
            switch (msg.what) {
                case kWebViewTaskCreate:
                    helper._createWebView(msg.arg1);
                    break;
                case kWebViewTaskRemove:
                    helper._removeWebView(msg.arg1);
                    break;
                case kWebViewTaskSetRect:
                    Rect rect = (Rect) msg.obj;
                    helper._setWebViewRect(msg.arg1, rect.left, rect.top, rect.right, rect.bottom);
                    break;
                case kWebViewTaskLoadURI:
                    helper._loadUrl(msg.arg1, (String) msg.obj);
                    break;
                case kWebViewTaskEvaluateJS:
                    helper._evaluateJS(msg.arg1, (String) msg.obj);
                    break;
                case kWebViewTaskSetVisible:
                    helper._setVisible(msg.arg1, msg.arg2 != 0);
                    break;
                case kWebViewTaskSetJsScheme:
                    helper._setJavascriptInterfaceScheme(msg.arg1, (String) msg.obj);
                    break;
                case kWebViewTaskGoBack:
                    helper._goBack(msg.arg1);
                    break;
                case kWebViewTaskGoForward:
                    helper._goForward(msg.arg1);
                    break;
                default:
                    Assert.fail("unknown message");
                    break;
            }
            super.handleMessage(msg);
        }
    }

    public static int createWebView() {
        Message msg = new Message();
        msg.what = kWebViewTaskCreate;
        msg.arg1 = viewTag;
        handler.sendMessage(msg);
        return viewTag++;
    }

    private void _createWebView(int index) {
        Cocos2dxWebView webView = new Cocos2dxWebView(cocos2dxActivity, index);
        FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layout.addView(webView, lParams);

        webViews.put(index, webView);
    }

    public static void removeWebView(int index) {
        Message msg = new Message();
        msg.what = kWebViewTaskRemove;
        msg.arg1 = index;
        handler.sendMessage(msg);
    }

    private void _removeWebView(int index) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webViews.remove(index);
            layout.removeView(webView);
        }
    }

    public static void setVisible(int index, boolean visible) {
        Message msg = new Message();
        msg.what = kWebViewTaskSetVisible;
        msg.arg1 = index;
        msg.arg2 = visible ? 1 : 0;
        handler.sendMessage(msg);
    }

    private void _setVisible(int index, boolean visible) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            if (visible) {
                webView.setVisibility(View.VISIBLE);
            } else {
                webView.setVisibility(View.GONE);
            }
        }
    }

    public static void setWebViewRect(int index, int left, int top, int maxWidth, int maxHeight) {
        Message msg = new Message();
        msg.what = kWebViewTaskSetRect;
        msg.arg1 = index;
        msg.obj = new Rect(left, top, maxWidth, maxHeight);
        handler.sendMessage(msg);
    }

    private void _setWebViewRect(int index, int left, int top, int maxWidth, int maxHeight) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.setWebViewRect(left, top, maxWidth, maxHeight);
        }
    }

    public static void setJavascriptInterfaceScheme(int index, String scheme) {
        Message msg = new Message();
        msg.what = kWebViewTaskSetJsScheme;
        msg.arg1 = index;
        msg.obj = scheme;
        handler.sendMessage(msg);
    }

    private void _setJavascriptInterfaceScheme(int index, String scheme) {
        Log.d(TAG, "_setJavascriptInterfaceScheme");
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.setJavascriptInterfaceScheme(scheme);
        }
    }

    public static void loadUrl(int index, String url) {
        Message msg = new Message();
        msg.what = kWebViewTaskLoadURI;
        msg.arg1 = index;
        msg.obj = url;
        handler.sendMessage(msg);
    }

    private void _loadUrl(int index, String url) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.loadUrl(url);
        }
    }

    public static void loadFile(int index, String filePath) {
        Message msg = new Message();
        msg.what = kWebViewTaskLoadURI;
        msg.arg1 = index;
        msg.obj = filePath;
        handler.sendMessage(msg);
    }

    private void _loadFile(int index, String filePath) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.loadUrl(filePath);
        }
    }

    public static boolean canGoBack(int index) {
        Cocos2dxWebView webView = webViews.get(index);
        return webView != null && webView.canGoBack();
    }

    public static boolean canGoForward(int index) {
        Cocos2dxWebView webView = webViews.get(index);
        return webView != null && webView.canGoForward();
    }

    public static void goBack(int index) {
        Message msg = new Message();
        msg.what = kWebViewTaskGoBack;
        msg.arg1 = index;
        handler.sendMessage(msg);
    }

    private void _goBack(int index) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.goBack();
        }
    }

    public static void goForward(int index) {
        Message msg = new Message();
        msg.what = kWebViewTaskGoForward;
        msg.arg1 = index;
        handler.sendMessage(msg);
    }

    private void _goForward(int index) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.goForward();
        }
    }

    public static void evaluateJS(int index, String js) {
        Message msg = new Message();
        msg.what = kWebViewTaskEvaluateJS;
        msg.arg1 = index;
        msg.obj = js;
        handler.sendMessage(msg);
    }

    private void _evaluateJS(int index, String js) {
        Cocos2dxWebView webView = webViews.get(index);
        if (webView != null) {
            webView.loadUrl("javascript:" + js);
        }
    }
}
