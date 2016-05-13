package org.cocos2dx.cpp;

import java.util.ArrayList;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;

/*
 * ?•„?˜ ì½”ë“œ?Š” ì¹´í˜?•± ?—°?™?„ ?œ„?•œ ?˜ˆ? œë¡? ? œ?‹œ?œ ê²ƒì´ë©?, 
 * ?›?•˜?Š” ?‹œ?‚˜ë¦¬ì˜¤?— ë§ì¶”?–´ ? œ?•œ ?—†?´ ë³?ê²½í•˜?—¬ ?‚¬?š© ê°??Š¥ ?•©?‹ˆ?‹¤.
 */

public class NaverCafe {
	public static final String APP_NAVER_APPSTORE = "com.nhn.android.appstore";
	public static final String CAFE_INSTALL_URL = "market://details?id=com.nhn.android.navercafe";
	public static final String CAFE_INSTALL_URL_NAVER = "appstore://store?version=7&action=view&packageName=com.nhn.android.navercafe";

	public static final String SCHEME_NAVERCAFE = "navercafe";
	public static final String AUTHORITY_CAFE = "cafe";
	public static final String AUTHORITY_READ = "read";
	public static final String AUTHORITY_WRITE = "write";
	public static final String QUERY_CAFEURL = "cafeUrl";
	public static final String QUERY_ARTICLEID = "articleId";
	public static final String QUERY_MENUID = "menuId";
	public static final String QUERY_SUBJECT = "subject";
	public static final String QUERY_CONTENTS = "contents";
	public static final String QUERY_ATTACHMENT = "attachment";
	public static final String QUERY_APPID = "appId";
	
	private Context context;
	private String appId;
	
	public NaverCafe(Context context, String appId) {
		this.context = context;
		this.appId = appId;
	}

	/**?Š¹? •ì¹´í˜ ë©”ì¸
	 * @param cafeUrl ì¹´í˜URL
	 */
	public void cafe(String cafeUrl) {
		Uri cafeUri = CafeUriBuilder.cafe(appId, cafeUrl);
		Intent cafeIntent = new Intent();
		cafeIntent.setData(cafeUri);
		try {
			context.startActivity(cafeIntent);
		} catch (ActivityNotFoundException e) {
			
			// ì¹´í˜?•±?´ ?„¤ì¹? ?˜ì§? ?•Š?? ê²½ìš°, ëª¨ë°”?¼ ?›¹?œ¼ë¡? ?´?™?‹œ,
			/*
			if (!isAppInstalled("com.nhn.android.navercafe")) {
				Intent intent = new Intent(Intent.ACTION_VIEW);
				intent.setData(Uri.parse("http://m.cafe.naver.com/" + cafeUrl));
				context.startActivity(intent);
				return;
			}
			*/
			
			gotoMarket();
		}
	}

	/**?Š¹? •ì¹´í˜ ê²Œì‹œê¸? ?½ê¸?
	 * @param cafeUrl ì¹´í˜URL
	 * @param articleId ê²Œì‹œê¸?ID
	 */
	public void read(String cafeUrl, String articleId) {
		Uri readUri = CafeUriBuilder.read(appId, cafeUrl, articleId);
		Intent readIntent = new Intent();
		readIntent.setData(readUri);
		try {
			context.startActivity(readIntent);
		} catch (ActivityNotFoundException e) {
			gotoMarket();
		}
	}

	/**?Š¹? •ì¹´í˜?— ê¸??“°ê¸?
	 * @param cafeUrl ì¹´í˜URL
	 * @param menuId ê²Œì‹œ?ŒID
	 * @param subject ? œëª?
	 * @param content ?‚´?š©
	 */
	public void write(String cafeUrl, String menuId, String subject, String content) {
		Uri writeUri = CafeUriBuilder.write(appId, cafeUrl, menuId, subject, content, null);
		Intent writeIntent = new Intent();
		writeIntent.setData(writeUri);
		try {
			context.startActivity(writeIntent);
		} catch (ActivityNotFoundException e) {
			gotoMarket();
		}
	}

	/**?Š¹? •ì¹´í˜?— ?‚¬ì§?,?™?˜?ƒ,ì²¨ë??ŒŒ?¼ ?“±?„ ì²¨ë??•˜?—¬ ê¸??“°ê¸?
	 * @param cafeUrl ì¹´í˜URL
	 * @param menuId ê²Œì‹œ?ŒID
	 * @param subject ? œëª?
	 * @param content ?‚´?š©
	 * @param attachments ì²¨ë??ŒŒ?¼?˜ SDì¹´ë“œê²½ë¡œ(?‹¤ì¤‘ì²¨ë¶? ê°??Š¥, ';'ë¡? êµ¬ë¶„)
	 */
	public void write(String cafeUrl, String menuId, String subject, String content, ArrayList<String> attachments) {
		Uri writeUri = CafeUriBuilder.write(appId, cafeUrl, menuId, subject, content, attachments);
		Intent writeIntent = new Intent();
		writeIntent.setData(writeUri);
		try {
			context.startActivity(writeIntent);
		} catch (ActivityNotFoundException e) {
			gotoMarket();
		}
	}

	public static class CafeUriBuilder {

		/**ì¹´í˜ë©”ì¸?œ¼ë¡? ?´?™
		 * @param cafeUrl ì¹´í˜URL
		 * @return uri
		 */
		public static Uri cafe(String appId, String cafeUrl) {
			Uri.Builder uriBuilder = new Uri.Builder();
			uriBuilder.scheme(SCHEME_NAVERCAFE);
			uriBuilder.authority(AUTHORITY_CAFE);
			uriBuilder.appendQueryParameter(QUERY_CAFEURL, cafeUrl);
			uriBuilder.appendQueryParameter(QUERY_APPID, appId);
				
			return uriBuilder.build();
		}

		/**ê¸??½ê¸?
		 * @param cafeUrl ì¹´í˜URL
		 * @param articleId ê²Œì‹œë¬¼ID
		 * @return uri
		 */
		public static Uri read(String appId, String cafeUrl, String articleId) {
			Uri.Builder uriBuilder = new Uri.Builder();
			uriBuilder.scheme(SCHEME_NAVERCAFE);
			uriBuilder.authority(AUTHORITY_READ);
			uriBuilder.appendQueryParameter(QUERY_CAFEURL, cafeUrl);
			uriBuilder.appendQueryParameter(QUERY_ARTICLEID, articleId);
			uriBuilder.appendQueryParameter(QUERY_APPID, appId);

			return uriBuilder.build();
		}

		/**ê¸??“°ê¸?
		 * @param cafeUrl ì¹´í˜URL
		 * @param menuId ê²Œì‹œ?ŒID
		 * @param subject ? œëª?
		 * @param content ?‚´?š©
		 * @param attachments ì²¨ë??ŒŒ?¼?˜ SDì¹´ë“œê²½ë¡œ
		 * @return uri
		 */
		public static Uri write(String appId, String cafeUrl, String menuId, String subject, String content, ArrayList<String> attachments) {

			Uri.Builder uriBuilder = new Uri.Builder();
			uriBuilder.scheme(SCHEME_NAVERCAFE);
			uriBuilder.authority(AUTHORITY_WRITE);
			uriBuilder.appendQueryParameter(QUERY_CAFEURL, cafeUrl);
			uriBuilder.appendQueryParameter(QUERY_MENUID, menuId);
			uriBuilder.appendQueryParameter(QUERY_SUBJECT, subject);
			uriBuilder.appendQueryParameter(QUERY_CONTENTS, content);

			if (attachments != null) {
				StringBuilder stringBuilder = new StringBuilder();
				for (String aAttachment : attachments) {
					stringBuilder.append(aAttachment);
					stringBuilder.append(';');
				}
				String attachment = stringBuilder.length() > 0 ? stringBuilder.substring(0, stringBuilder.length() - 1) : "";
				uriBuilder.appendQueryParameter(QUERY_ATTACHMENT, attachment);
			}

			uriBuilder.appendQueryParameter(QUERY_APPID, appId);
			return uriBuilder.build();
		}
	}

	/**?•± ?„¤ì¹? ?™•?¸
	 */
	public boolean isAppInstalled(String packname) {
		try {
			PackageInfo info = context.getPackageManager().getPackageInfo(
					packname, 0);
			if (info != null) {
				return true;
			}
		} catch (NameNotFoundException e) {
		}
		return false;
	}
	
	/**ë§ˆì¼“?œ¼ë¡? ?´?™
	 */
	public void gotoMarket() {
		// ?”Œ? ˆ?´ ?Š¤?† ?–´ë¡? ë°”ë¡œ ?´?™?‹œ,
		// gotoPlayStore();
		// return;
		
		// ?„¤?´ë²? ?•±?Š¤?† ?–´ë¡? ê°? ?ˆ˜ ?—†?œ¼ë©?, ?”Œ? ˆ?´ ?Š¤?† ?–´ë¡? ?´?™?‹œ,
		if (!gotoNaverAppStore()) {
			gotoPlayStore();
		}
	}
	
	public boolean gotoNaverAppStore() {
		if (isAppInstalled(APP_NAVER_APPSTORE)) {
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setData(Uri.parse(CAFE_INSTALL_URL_NAVER));
			context.startActivity(intent);
			return true;
		}
		
		return false;
	}
	
	public void gotoPlayStore() {
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(Uri.parse(CAFE_INSTALL_URL));
		context.startActivity(intent);
	}
}
