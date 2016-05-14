/****************************************************************************
* Date : 2016.05.13
* Name : AppActvity
* Email : haminjun0@gmail.com
* GitHub : https://github.com/haminjun
* Cocos2d-x API Main Class
****************************************************************************/
package org.cocos2dx.cpp;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
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
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.leaderboard.LeaderboardVariant;
import com.google.android.gms.games.leaderboard.Leaderboards;
import com.unity3d.ads.android.IUnityAdsListener;
import com.unity3d.ads.android.UnityAds;

import android.R;
import android.annotation.TargetApi;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.Region;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

/**
 * 
 * @author haminjun
 * @since 2016.04.28
 * @version 0.01
 * @see static method JNI CALL Twitter 까지 하고 코드 정리
 */
public class AppActivity extends BaseGameActivity implements IUnityAdsListener {

	// -----------------------------------------------------
	private final static String NAVERAPPID = "naver.com.cafesample";
	private final static String NAVERCAFE_URL = "sangmowarrior";
	private final static String UNITYADS_ID = "1069367";
	private static CallbackManager callbackManager;
	private static AppActivity _self = null;
	private static ShareDialog shareDialog;
	static int currentID;
	static int currentAchievementID;
	static boolean gpgAvailable;
	static String[] leaderboardIDs;
	protected static int currentScore;

	public static native void callCppCallback();

	// -----------------------------------------------------
	private static AppActivity _appActiviy;
	private AdView adView;
	private static final String AD_UNIT_ID = "ca-app-pub-3798516121036315/7737995786";

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_self = this;

		String leaderboardIdsRaw = "CgkI6cvA3ZgPEAIQAA";

		leaderboardIDs = leaderboardIdsRaw.split(";");

		// Facebook Initalize
		FacebookSdk.sdkInitialize(getApplicationContext());
		callbackManager = CallbackManager.Factory.create();

		shareDialog = new ShareDialog(this);
		shareDialog.registerCallback(callbackManager, shareCallBack);

		// Admob
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(AdView.LayoutParams.WRAP_CONTENT,
				AdView.LayoutParams.WRAP_CONTENT);
		adParams.gravity = (Gravity.BOTTOM | Gravity.CENTER);

		adView = new AdView(this);
		adView.setAdSize(AdSize.SMART_BANNER);
		adView.setAdUnitId(AD_UNIT_ID);

		AdRequest adRequest = new AdRequest.Builder().addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
				.addTestDevice("D123746919D9CCD280CFF5366C919212").build();

		adView.loadAd(adRequest);
		adView.setBackgroundColor(Color.BLACK);
		adView.setBackgroundColor(0);

		addContentView(adView, adParams);

		_appActiviy = this;

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
					.setContentDescription("Player Acheieve " + score + " score !!! Just play it fast!")
					.setContentUrl(Uri.parse("https://www.facebook.com/SangmoWarrior/?fref=ts"))
					.setImageUrl(Uri
							.parse("https://scontent-icn1-1.xx.fbcdn.net/v/t1.0-9/12993399_507392352781589_2310251201340368500_n.jpg?oh=fdf05dd3ce285d84da640c249b6553af&oe=57DEC679"))
					.build();
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

	// Admob
	// Helper get display screen to avoid deprecated function use
	private Point getDisplaySize(Display d) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			return getDisplaySizeGE11(d);
		}
		return getDisplaySizeLT11(d);
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
	private Point getDisplaySizeGE11(Display d) {
		Point p = new Point(0, 0);
		d.getSize(p);
		return p;
	}

	private Point getDisplaySizeLT11(Display d) {
		try {
			Method getWidth = Display.class.getMethod("getWidth", new Class[] {});
			Method getHeight = Display.class.getMethod("getHeight", new Class[] {});
			return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(),
					((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
		} catch (NoSuchMethodException e2) // None of these exceptions should
		// ever occur.
		{
			return new Point(-1, -1);
		} catch (IllegalArgumentException e2) {
			return new Point(-2, -2);
		} catch (IllegalAccessException e2) {
			return new Point(-3, -3);
		} catch (InvocationTargetException e2) {
			return new Point(-4, -4);
		}
	}

	public static void hideAd() {
		_appActiviy.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (_appActiviy.adView.isEnabled())
					_appActiviy.adView.setEnabled(false);
				if (_appActiviy.adView.getVisibility() != 4)
					_appActiviy.adView.setVisibility(View.INVISIBLE);
			}
		});

	}

	public static void showAd() {
		_appActiviy.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (!_appActiviy.adView.isEnabled())
					_appActiviy.adView.setEnabled(true);
				if (_appActiviy.adView.getVisibility() == 4)
					_appActiviy.adView.setVisibility(View.VISIBLE);
			}
		});

	}

	@Override
	protected void onPause() {
		if (adView != null) {
			adView.pause();
		}
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		adView.destroy();
		super.onDestroy();
	}

	// 구글 플레이
	@Override
	public void onSignInSucceeded() {
		Log.d("GPGS", "로그인완료 ");
		gpgAvailable = true;
	}

	@Override
	public void onSignInFailed() {
		Log.d("GPGS", "로그인실패 ");
		gpgAvailable = false;
	}

	/*
	 * @brief Changes the actvie leaderboard
	 * 
	 * @param The index of the leaderboard
	 */
	static public void openLeaderboard(int leaderboardID) {
		Log.d("GPGS", "들오냐 ");
		currentID = leaderboardID;
		Log.d("GPGS", "들온다 ");
	}

	/* @brief This function opens the leaderboards ui for an leaderboard id */
	static public void openLeaderboardUI() {
		if (gpgAvailable) {
			((AppActivity) _self).runOnUiThread(new Runnable() {
				public void run() {
					((AppActivity) _self).startActivityForResult(
							Games.Leaderboards.getLeaderboardIntent(
									((AppActivity) _self).getGameHelper().getApiClient(), leaderboardIDs[currentID]),
							2);
				}
			});
		}
	}

	static public boolean isGPGSupported() {
		return gpgAvailable;
	}

	/* @brief Submits a score to the leaderboard that is currently actvie */
	static public void submitScoreToLeaderboard(int score) {
		if (gpgAvailable) {
			Games.Leaderboards.submitScore(((AppActivity) _self).getGameHelper().getApiClient(),
					leaderboardIDs[currentID], score);
		}
	}

	static public void requestScoreFromLeaderboard() {
		if (gpgAvailable) {
			Games.Leaderboards
					.loadCurrentPlayerLeaderboardScore(((AppActivity) _self).getGameHelper().getApiClient(),
							leaderboardIDs[currentID], LeaderboardVariant.TIME_SPAN_ALL_TIME,
							LeaderboardVariant.COLLECTION_PUBLIC)
					.setResultCallback(new ResultCallback<Leaderboards.LoadPlayerScoreResult>() {
						@Override
						public void onResult(final Leaderboards.LoadPlayerScoreResult scoreResult) {
							if (scoreResult.getStatus().getStatusCode() == GamesStatusCodes.STATUS_OK) {
								AppActivity.currentScore = (int) scoreResult.getScore().getRawScore();
								AppActivity.callCppCallback();
							}
						}
					});
		}
	}

	static public int collectScore() {
		return AppActivity.currentScore;
	}

	/* @brief Shows the achievements ui */
	static public void showAchievements() {
		if (gpgAvailable) {
			((AppActivity) _self).runOnUiThread(new Runnable() {
				public void run() {
					((AppActivity) _self).startActivityForResult(Games.Achievements
							.getAchievementsIntent(((AppActivity) _self).getGameHelper().getApiClient()), 5);
				}
			});
		}
	}

	/*
	 * @brief Changes the actvie Achievement
	 * 
	 * @param The index of the achievement in the list
	 */
	static public void openAchievement(int achievementID) {
		currentAchievementID = achievementID;
	}

	static public void updateAchievement(int percentage) {
		if (gpgAvailable) {
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// inflater.inflate(R.layout.menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		return true;
	}

	static public void exitGame() {
		Intent intent = new Intent(_self, MainActivity.class);
		MainActivity.exiting = true;
		_self.startActivity(intent);
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
		if (adView != null) {
			adView.resume();
		}
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
