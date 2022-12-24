package org.cocos2dx.cpp;

import android.app.Activity;
import android.graphics.Color;
import android.view.View;
import android.widget.FrameLayout;
import androidx.annotation.NonNull;
import com.boring.slideblockpuzzle.R;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;

public class AndroidAdManager {
    private Activity activity;
    private static AdView top_bannerView;
    private static AdView bottom_bannerView;
    private static boolean top_bannerViewShowing = false;
    private static boolean bottom_bannerViewShowing = false;
    private String top_bannerId;
    private String bottom_bannerId;
    private String interstitialId;

    public AndroidAdManager(Activity activity) {
        this.activity = activity;
        top_bannerId = activity.getBaseContext().getString(R.string.bannerId);
        bottom_bannerId = activity.getBaseContext().getString(R.string.bannerId);
        interstitialId = activity.getBaseContext().getString(R.string.interstitialId);
        setupAdBannerView();
    }

    public void onDestroy() {
        top_bannerView.destroy();
        bottom_bannerView.destroy();
    }

    public void showBannerAd() {
        showTopBannerAd();
        showBottomBannerAd();
    }

    public void hideBanner() {
       hideBanner(top_bannerView);
       hideBanner(bottom_bannerView);
    }

    public void showInterstitial() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                InterstitialAd.load(activity, interstitialId, new AdRequest.Builder().build(), new InterstitialAdLoadCallback() {
                    @Override
                    public void onAdLoaded(@NonNull InterstitialAd interstitialAd) {
                        interstitialAd.show(activity);
                    }
                    @Override
                    public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                    }
                });
            }
        });
    }

    private void setupAdBannerView() {
        setupTopBannerView();
        setupBottomAdView();
        hideBanner();
    }

    private void setupTopBannerView() {
        top_bannerView = new AdView(activity);
        top_bannerView.setAdSize(AdSize.FULL_BANNER);
        top_bannerView.setAdUnitId(top_bannerId);
        FrameLayout.LayoutParams topLayoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, android.view.Gravity.TOP | android.view.Gravity.CENTER_HORIZONTAL);
        top_bannerView.setBackgroundColor(Color.BLACK);
        top_bannerView.setBackgroundColor(0);
        activity.addContentView(top_bannerView, topLayoutParams);
    }

    private void setupBottomAdView() {
        bottom_bannerView = new AdView(activity);
        bottom_bannerView.setAdSize(AdSize.FULL_BANNER);
        bottom_bannerView.setAdUnitId(bottom_bannerId);
        FrameLayout.LayoutParams bottom_adParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, android.view.Gravity.BOTTOM|android.view.Gravity.CENTER_HORIZONTAL);
        bottom_bannerView.setBackgroundColor(Color.BLACK);
        bottom_bannerView.setBackgroundColor(0);
        activity.addContentView(bottom_bannerView, bottom_adParams);
    }

    public void hideBanner(AdView adView) {
        if (adView != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (adView.isEnabled()) {
                        adView.setEnabled(false);
                    }
                    if (adView.getVisibility() != View.INVISIBLE) {
                        adView.setVisibility(View.INVISIBLE);
                    }
                }
            });
        }
    }

    private void showTopBannerAd() {
        if (top_bannerView != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!top_bannerViewShowing) {
                        top_bannerView.loadAd(new AdRequest.Builder().build());
                        top_bannerViewShowing = true;
                    }
                    if (!top_bannerView.isEnabled()) {
                        top_bannerView.setEnabled(true);
                    }
                    if (top_bannerView.getVisibility() == View.INVISIBLE) {
                        top_bannerView.setVisibility(View.VISIBLE);
                    }
                }
            });
        }
    }

    private void showBottomBannerAd() {
        if (bottom_bannerView != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!bottom_bannerViewShowing) {
                        bottom_bannerView.loadAd(new AdRequest.Builder().build());
                        bottom_bannerViewShowing = true;
                    }
                    if (!bottom_bannerView.isEnabled()) {
                        bottom_bannerView.setEnabled(true);
                    }
                    if (bottom_bannerView.getVisibility() == View.INVISIBLE) {
                        bottom_bannerView.setVisibility(View.VISIBLE);
                    }
                }
            });
        }
    }
}