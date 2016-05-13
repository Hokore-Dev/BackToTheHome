/****************************************************************************
* Date : 2016.05.13
* Name : AppActvity
* Email : haminjun0@gmail.com
* GitHub : https://github.com/haminjun
* Cocos2d-x API Main Class
****************************************************************************/
package org.cocos2dx.cpp;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;

import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.ShareApi;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.Sharer;
import com.facebook.share.Sharer.Result;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;
import com.unity3d.ads.android.IUnityAdsListener;
import com.unity3d.ads.android.UnityAds;

import android.content.Intent;
import android.graphics.Region;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * 
 * @author haminjun
 * @since 2016.04.28
 * @version 0.01
 * @see static method JNI CALL Twitter 까지 하고 코드 정리
 */
public class AppActivity extends Cocos2dxActivity implements IUnityAdsListener {

	private final static String NAVERAPPID = "naver.com.cafesample";
	private final static String NAVERCAFE_URL = "sangmowarrior";
	private final static String UNITYADS_ID = "1069367";

	private static CallbackManager callbackManager;
	private static AppActivity _self = null;
	private static ShareDialog shareDialog;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_self = this;
		
		// Facebook Initalize
		FacebookSdk.sdkInitialize(getApplicationContext());
		callbackManager = CallbackManager.Factory.create();

		shareDialog = new ShareDialog(this);
		shareDialog.registerCallback(callbackManager, shareCallBack);
	}

	/**
	 * Cocos2d-x JNI Call
	 * 
	 * 네이버 카페 이동
	 */
	public static void connectNaverCafe() {
		new NaverCafe(_self, NAVERAPPID).cafe(NAVERCAFE_URL);
	}

	/**
	 * Cocos2d-x JNI Call
	 * 
	 * 페이스북 공유
	 */
	public static void facebookShare(int score) {
		if (ShareDialog.canShow(ShareLinkContent.class)) {
			ShareLinkContent linkContent = new ShareLinkContent.Builder().setContentTitle("Back to the Home")
					.setContentDescription("Player Acheieve " + score + " score !!! Just play it fast!").setContentUrl(Uri.parse("https://www.facebook.com/SangmoWarrior/?fref=ts"))
					.setImageUrl(Uri.parse("https://scontent-icn1-1.xx.fbcdn.net/v/t1.0-9/12993399_507392352781589_2310251201340368500_n.jpg?oh=fdf05dd3ce285d84da640c249b6553af&oe=57DEC679")).build();
			shareDialog.show(linkContent);
		}
	}

	/**
	 * Cocos2d-x JNI Call
	 * 
	 * 페이스북 로그인 요청
	 */
	public static void facebookLogin() {
		LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
			@Override
			public void onSuccess(final LoginResult loginResult) {
				Log.d("Facebook API", "Login Success");
				Handler mHandler = new Handler(Looper.getMainLooper());
				mHandler.post(new Runnable() {
					@Override
					public void run() {

						new GraphRequest(loginResult.getAccessToken(), "/me/", null, HttpMethod.GET,
								new GraphRequest.Callback() {
							public void onCompleted(GraphResponse response) {
								try {
									String id = (String) response.getJSONObject().get("id");
									String name = (String) response.getJSONObject().get("name");
									String email = (String) response.getJSONObject().get("email");
									Log.d("Facebook API", id.toString() + name.toString() + email.toString());
								} catch (JSONException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
							}
						}).executeAsync();
					}
				});
			}

			@Override
			public void onCancel() {
				Log.d("Facebook API", "Login Cancel");
			}

			@Override
			public void onError(FacebookException exception) {
				Log.d("Facebook API", "Login Error");
			}
		});

		LoginManager.getInstance().logInWithPublishPermissions(_self,
				Arrays.asList("publish_actions", "publish_pages"));
		LoginManager.getInstance().logInWithReadPermissions(_self,
				Arrays.asList("public_profile", "user_friends", "read_custom_friendlists"));
	}

	/**
	 * Cocos2d-x JNI Call
	 * 
	 * @return 유니티 광고를 시청할 수 있는지 여부를 반환
	 */
	public static boolean unityAdsShowConfirm() {
		if (UnityAds.canShow() && UnityAds.canShowAds()) {
			return true;
		}
		return false;
	}

	/**
	 * Cocos2d-x JNI Call 유니티 광고를 초기화 시킨다
	 */
	public static void unityAdsInit() {
		UnityAds.setDebugMode(false);
		UnityAds.init(_self, UNITYADS_ID, _self);
		UnityAds.setListener(_self);
	}

	/**
	 * Cocos2d-x JNI Call 유니티 광고를 재생한다
	 */
	public static void unityAdsPlay() {
		UnityAds.show();
	}

	// Facebook Callback
	public FacebookCallback<Sharer.Result> shareCallBack = new FacebookCallback<Sharer.Result>() {
		@Override
		public void onCancel() {
		}

		@Override
		public void onError(FacebookException error) {
		}

		@Override
		public void onSuccess(com.facebook.share.Sharer.Result result) {
			// TODO Auto-generated method stub
		}
	};

	@Override
	protected void onActivityResult(final int requestCode, final int resultCode, final Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		callbackManager.onActivityResult(requestCode, resultCode, data);
	}

	// Unity Ads Callback Function
	public void onResume() {
		super.onResume();
		UnityAds.changeActivity(this);
		UnityAds.setListener(this);
	}

	@Override
	public void onHide() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onShow() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onVideoStarted() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onVideoCompleted(String rewardItemKey, boolean skipped) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onFetchCompleted() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onFetchFailed() {
		// TODO Auto-generated method stub

	}
}
