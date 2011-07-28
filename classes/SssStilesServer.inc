<?php
/* * * *
 * * SssStilesServer.inc
 * *
 * * @version 20110725_151521 (CC) Luke JZ aka SwissalpS
 * * * */

class SssStilesServer extends WFObject {

	function __construct() {

		parent::__construct();

	} // __construct


	static function aliasHash() {

		static $aAliasHash = null;
		if (!$aAliasHash) {

			$aAliasHash = SssStilesAliases::getAliasesArray();

		} // if first call

		return $aAliasHash;

	} // aAliasHash


	// return the cleaned alias if valid or null
	static function aliasIsValid($sAlias) {

		$sAliasClean = self::cleanAlias($sAlias);

		$aAliasHash = self::aliasHash();

		if (isset($aAliasHash[$sAliasClean])) return $sAliasClean;

		// keep track of aliases
		WFLog::logToFile('SssStilesServerAlias.log', 'unknown alias: ' . $sAliasClean);

		return null;

	} // aliasIsValid


	static function cleanAlias($sAlias) {

		if (empty($sAlias)) return null;

		static $sPermittedPattern = null;
		if (!$sPermittedPattern) {

			$sPermittedPattern = '|[^a-z_0-9]|i';

		} // if first call

		$sAliasClean = preg_replace($sPermittedPattern, '_', $sAlias);

		return $sAliasClean;

	} // cleanAlias


	static function currentToken($iTS = null) {

		if (!$iTS) $iTS = time();

		// date rounded to 10 mins
		$sDate = round($iTS / 60, -1);

		$sToken = sha1('salt1./' . $sDate . '2salt2.9*,rkv094et-4!');

		return $sToken;

	} // currentToken


	static function eventForAlias($sAlias) {

		$sAliasClean = self::aliasIsValid($sAlias);

		if (empty($sAliasClean)) return null;

		$aAliasHash = self::aliasHash();

		// basically double checking ... an effect of refactoring
		if (isset($aAliasHash[$sAliasClean]))
				return new SssStilesEvent($aAliasHash[$sAliasClean]);

		return false;

	} // eventForAlias


	static function eventForUrl($sUrl = '') {

		$aUrl = parse_url($sUrl);

		if (!isset($aUrl['scheme'], $aUrl['host'], $aUrl['path'])) return null;

		$aWL = self::whiteListArray();

		if (!isset($aWL[$aUrl['host']])) return null;

		switch($aWL[$aUrl['host']]) {

			case SssStilesUrlType_ATOM : return self::newEvent_ATOM($aUrl, $sUrl); break;

			case SssStilesUrlType_BING : return self::newEvent_BING($aUrl, $sUrl); break;

			case SssStilesUrlType_CURRENT : return self::newEvent_CURRENT($aUrl, $sUrl); break;

			case SssStilesUrlType_DAILYMOTION : return self::newEvent_DAILYMOTION($aUrl, $sUrl); break;

			case SssStilesUrlType_FORATV : return self::newEvent_FORATV($aUrl, $sUrl); break;

			case SssStilesUrlType_FUNNYORDIE : return self::newEvent_FUNNYORDIE($aUrl, $sUrl); break;

			case SssStilesUrlType_GOOGLEVIDEO : return self::newEvent_GOOGLEVIDEO($aUrl, $sUrl); break;

			case SssStilesUrlType_JUSTINTVL :
			case SssStilesUrlType_JUSTINTVR : return self::newEvent_JUSTINTV($aUrl, $sUrl); break;

			case SssStilesUrlType_LIVELEAK : return self::newEvent_LIVELEAK($aUrl, $sUrl); break;

			case SssStilesUrlType_METACAFE : return self::newEvent_METACAFE($aUrl, $sUrl); break;

			case SssStilesUrlType_MITOCW : return self::newEvent_MITOCW($aUrl, $sUrl); break;

			case SssStilesUrlType_MYSPACE : return self::newEvent_MYSPACE($aUrl, $sUrl); break;

			case SssStilesUrlType_QIKL :
			case SssStilesUrlType_QIKR : return self::newEvent_QIK($aUrl, $sUrl); break;

			case SssStilesUrlType_REVISION3 : return self::newEvent_REVISION3($aUrl, $sUrl); break;

			case SssStilesUrlType_REVVER : return self::newEvent_REVVER($aUrl, $sUrl); break;

			case SssStilesUrlType_USTREAML :
			case SssStilesUrlType_USTREAMR : return self::newEvent_USTREAM($aUrl, $sUrl); break;

			case SssStilesUrlType_VIDDLER : return self::newEvent_VIDDLER($aUrl, $sUrl); break;

			case SssStilesUrlType_VIMEO : return self::newEvent_VIMEO($aUrl, $sUrl); break;

			case SssStilesUrlType_YOUTUBE : return self::newEvent_YOUTUBE($aUrl, $sUrl); break;

			default : return null;	break;

		} // switch urltype

	} // allowHost


	static function newEvent_ATOM(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_ATOM


	static function newEvent_BING(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_BING


	static function newEvent_CURRENT(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_CURRENT


	static function newEvent_DAILYMOTION(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_DAILYMOTION


	static function newEvent_FORATV(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_FORATV


	static function newEvent_FUNNYORDIE(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_FUNNYORDIE


	static function newEvent_GOOGLEVIDEO(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_GOOGLEVIDEO

	// (y) http://www.justin.tv/twit
	// (y) http://www.justin.tv/twit/b/290944016
	static function newEvent_JUSTINTV(&$aUrl, &$sUrl) {

		// fetch the whole page
		$sHTML = @file_get_contents($sUrl);
		if (empty($sHTML)) return null;

		// parse title from title tag
		$sTitle = '';
		$mPos1 = strPos($sHTML, '<title>');
		$mPos2 = strPos($sHTML, '</title>');

		if (false !== $mPos1 && false !== $mPos2) {

			$sTitle = trim(subStr($sHTML, $mPos1 + 7, $mPos2 - $mPos1 -7));

		} // if got title tag

		// parse img-url from meta tag
		$sImageUrl = '';
		$mPos = strPos($sHTML, '<meta property="og:image" content="');

		if (false !== $mPos) {

			$s = subStr($sHTML, $mPos + 35);
			$mPos = strPos($s, '"');

			if (false !== $mPos) {

				$sImageUrl = trim(subStr($s, 0, $mPos));

			} // if got ending "

		} // if got image-meta tag

		// parse for vid whitch should be 'channel(vid)<tab>consumer_key' for channels
		// 'channel(vid)<tab>archive_id<tab>consumer_key' for recorded
		$mPos = strPos($sHTML, '<meta property="og:video" content="');

		if (false !== $mPos) {

			$s = subStr($sHTML, $mPos + 35);
			$mPos = strPos($s, '"');

			if (false !== $mPos) {

				$s = trim(subStr($s, 0, $mPos));

				$sPattern = '|channel=([^&]+)|';
				$aMatches = array();
				if (!preg_match($sPattern, $s, $aMatches)) return null;

				$sID = $aMatches[1] . chr(9);

				$sPattern = '|archive_id=([0-9]+)|';
				$aMatches = array();
				if (!preg_match($sPattern, $s, $aMatches)) {

					$iUrlType = SssStilesUrlType_JUSTINTVL;

				} else {

					$iUrlType = SssStilesUrlType_JUSTINTVR;

					$sID .= $aMatches[1] . chr(9);

				} // if live or recorded

			} // if got cid

		} // if got video-meta tag

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('alias', ''); // none here
		$oEvent->setParam('ccode', ''); // country code
		$oEvent->setParam('imgurl', $sImageUrl); // possibly shows when audio only or offline?
		$oEvent->setParam('param', ''); // embed params
		$oEvent->setParam('stime', 0);
		$oEvent->setParam('timeoffset', -1440);
		$oEvent->setParam('title', (empty($sTitle) ? $aUrl['host'] : $sTitle));
		$oEvent->setParam('type', SssStilesCmd_OPEN);
		$oEvent->setParam('url', $sUrl);
		$oEvent->setParam('urltype', $iUrlType);
		$oEvent->setParam('vid', $sID);

		return $oEvent;

		return null;

	} // newEvent_JUSTINTV


	static function newEvent_LIVELEAK(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_LIVELEAK


	static function newEvent_METACAFE(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_METACAFE


	static function newEvent_MITOCW(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_MITOCW


	static function newEvent_MYSPACE(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_MYSPACE


	static function newEvent_QIK(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_QIK


	// (n) http://revision3.com/tekzilla/toshibathrive
	static function newEvent_REVISION3(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_REVISION3


	static function newEvent_REVVER(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_REVVER

	// (y) http://www.ustream.tv/recorded/15722015
	// (y) http://www.ustream.tv/channel/robot-party
	static function newEvent_USTREAM(&$aUrl, &$sUrl) {

		$aMatches = array();
		if (preg_match('|/recorded/([0-9]+)|', $aUrl['path'], $aMatches)) {

			$sID = $aMatches[1];

			$oEvent = new SssStilesEvent();

			$oEvent->setParam('alias', ''); // none here
			$oEvent->setParam('ccode', ''); // country code
			$oEvent->setParam('imgurl', ''); // possibly shows when audio only or offline?
			$oEvent->setParam('param', ''); // embed params
			$oEvent->setParam('stime', 0);
			$oEvent->setParam('timeoffset', -1440);
			$oEvent->setParam('title', $aUrl['host']);
			$oEvent->setParam('type', SssStilesCmd_OPEN);
			$oEvent->setParam('url', $sUrl);
			$oEvent->setParam('urltype', SssStilesUrlType_USTREAMR);
			$oEvent->setParam('vid', $sID);

			return $oEvent;

		} else if (0 === strPos($aUrl['path'], '/channel/')) {

			// fetch the whole page
			$sHTML = @file_get_contents($sUrl);
			if (empty($sHTML)) return null;

			// parse vid from meta tag
			$mPos = strPos($sHTML, '<meta name="og:video" content="');

			if (false === $mPos) return null;

			$s = subStr($sHTML, $mPos + 31);
			$mPos = strPos($s, '"');

			if (false === $mPos) return null;

			$s = trim(subStr($s, 0, $mPos));

			$sPattern = '|cid=([^&]+)|';
			$aMatches = array();
			if (!preg_match($sPattern, $s, $aMatches)) return null;

			$sID = $aMatches[1];

			// parse title from meta tag
			$sTitle = '';
			$mPos = strPos($sHTML, '<meta name="title" content="');

			if (false !== $mPos) {

				$s = subStr($sHTML, $mPos + 28);
				$mPos = strPos($s, '"');

				if (false !== $mPos) {

					$sTitle = trim(subStr($s, 0, $mPos));

				} // if got ending "

			} // if got title-meta tag

			// parse img-url from meta tag
			$sImageUrl = '';
			$mPos = strPos($sHTML, '<meta property="og:image" content="');

			if (false !== $mPos) {

				$s = subStr($sHTML, $mPos + 35);
				$mPos = strPos($s, '"');

				if (false !== $mPos) {

					$sImageUrl = trim(subStr($s, 0, $mPos));

				} // if got ending "

			} // if got image-meta tag

			$oEvent = new SssStilesEvent();

			$oEvent->setParam('alias', ''); // none here
			$oEvent->setParam('ccode', ''); // country code
			$oEvent->setParam('imgurl', $sImageUrl); // possibly shows when audio only or offline?
			$oEvent->setParam('param', ''); // embed params
			$oEvent->setParam('stime', 0);
			$oEvent->setParam('timeoffset', -1440);
			$oEvent->setParam('title', (empty($sTitle) ? $aUrl['host'] : $sTitle));
			$oEvent->setParam('type', SssStilesCmd_OPEN);
			$oEvent->setParam('url', $sUrl);
			$oEvent->setParam('urltype', SssStilesUrlType_USTREAML);
			$oEvent->setParam('vid', $sID);

			return $oEvent;

		} // if recorded or live channel

		return null;

	} // newEvent_USTREAM


	// (n) http://www.viddler.com/player/vid/0
	// (n) http://www.viddler.com/file/vid/html5mobile/
	// (n) http://www.viddler.com/thumbnail/d6c37b62/
	// (n) http://www.viddler.com/embed/vid/?f=1&offset=0&autoplay=0&disablebranding=0
	static function newEvent_VIDDLER(&$aUrl, &$sUrl) {

		return null;

	} // newEvent_VIDDLER


	// (y) http://www.vimeo.com/vid
	static function newEvent_VIMEO(&$aUrl, &$sUrl) {

		$sID = trim($aUrl['path'], '/');

		$mPos = strPos($sID, '/');
		if (false !== $mPos) {

			$sID = subStr($sID, 0, $mPos);

		} // if got /

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('alias', ''); // none here
		$oEvent->setParam('ccode', ''); // country code
		$oEvent->setParam('imgurl', ''); // possibly shows when audio only or offline?
		$oEvent->setParam('param', ''); // embed params
		$oEvent->setParam('stime', 0);
		$oEvent->setParam('timeoffset', -1440);
		$oEvent->setParam('title', $aUrl['host']);
		$oEvent->setParam('type', SssStilesCmd_OPEN);
		$oEvent->setParam('url', $sUrl);
		$oEvent->setParam('urltype', SssStilesUrlType_VIMEO);
		$oEvent->setParam('vid', $sID);

		return $oEvent;

	} // newEvent_VIMEO


	// (y) http://www.youtu.be/vid
	// (y) http://www.youtube.com/v/vid
	// (y) http://www.youtube.com/watch/v=vid
	static function newEvent_YOUTUBE(&$aUrl, &$sUrl) {

		$sID = null;

		$sPathStart = subStr($aUrl['path'], 0, 3);

		switch($sPathStart) {

			case '/wa' :

				$mPos = striPos($aUrl['query'], 'v=');
				if (false === $mPos) return null;

				$sID = subStr($aUrl['query'], $mPos + 2);

				$mPos = strPos($sID, '&');
				if (false !== $mPos) {

					$sID = subStr($sID, 0, $mPos);

				} // if got other arguments in query

			break;

			case '/v/' :

				$sID = subStr($aUrl['path'], 3);

			break;

			default :

				if ('www.youtu.be' == $aUrl['host']) {

					$sID = trim($aUrl['path'], '/');

				} // if youtu.be host

				break;

		} // switch first 3 chars of path

		if (!$sID) return null;

		$mPos = strPos($sID, '/');
		if (false !== $mPos) {

			$sID = subStr($sID, 0, $mPos);

		} // if got /

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('alias', ''); // none here
		$oEvent->setParam('ccode', ''); // country code
		$oEvent->setParam('imgurl', ''); // possibly shows when audio only or offline?
		$oEvent->setParam('param', ''); // embed params
		$oEvent->setParam('stime', 0);
		$oEvent->setParam('timeoffset', -1440);
		$oEvent->setParam('title', $aUrl['host']);
		$oEvent->setParam('type', SssStilesCmd_OPEN);
		$oEvent->setParam('url', $sUrl);
		$oEvent->setParam('urltype', SssStilesUrlType_YOUTUBE);
		$oEvent->setParam('vid', $sID);

		return $oEvent;

	} // newEvent_YOUTUBE


	static function pull() {

		return self::sharedServer()->runPull();

	} // pull


	static function push() {

		return self::sharedServer()->runPush();

	} // push


	function runPull() {

		$o = new SssStilesPull();

		echo $o->xmlBlob();

	} // runPull


	function runPush() {

		$o = new SssStilesPush();

		echo $o->xmlBlob();

	} // runPush


	static function sharedServer() {

		static $o = null;
		if (!$o) $o = new SssStilesServer();

		return $o;

	} // sharedServer


	static function whiteListArray() {

		static $aWL = null;
		if (!$aWL) $aWL = array(
			'www.atom.com'			=> SssStilesUrlType_ATOM,
			'bing.com'				=> SssStilesUrlType_BING,
			'current.com'			=> SssStilesUrlType_CURRENT,
			'www.dailymotion.com'	=> SssStilesUrlType_DAILYMOTION,
			'fora.tv'				=> SssStilesUrlType_FORATV,
//			''		=> SssStilesUrlType_FUNNYORDIE,
			'video.google.com'		=> SssStilesUrlType_GOOGLEVIDEO,
			'www.justin.tv'			=> SssStilesUrlType_JUSTINTVL,
			'justin.tv'				=> SssStilesUrlType_JUSTINTVR, // checking live or recorded later
			'www.liveleak.com'		=> SssStilesUrlType_LIVELEAK,
			'www.metacafe.com'		=> SssStilesUrlType_METACAFE,
			'ocw.mit.edu'			=> SssStilesUrlType_MITOCW,
			'www.myspace.com'		=> SssStilesUrlType_MYSPACE,
			'qik.com'				=> SssStilesUrlType_QIKL,
			'qik.com'				=> SssStilesUrlType_QIKR,
			'revision3.com'			=> SssStilesUrlType_REVISION3,
			'www.revver.com'		=> SssStilesUrlType_REVVER,
			'www.ustream.tv'		=> SssStilesUrlType_USTREAML,
			'ustream.tv'			=> SssStilesUrlType_USTREAMR, // checking live or recorded later
			'www.viddler.com'		=> SssStilesUrlType_VIDDLER,
			'vimeo.com'				=> SssStilesUrlType_VIMEO,
			'www.vimeo.com'			=> SssStilesUrlType_VIMEO,
			'www.youtube.com'		=> SssStilesUrlType_YOUTUBE,
			'www.youtu.be'			=> SssStilesUrlType_YOUTUBE,
			'live.twit.tv'			=> null); // TODO: load from file

		return $aWL;

	} // whiteListArray

} // SssStilesServer

/* * * *\ SssStilesServer.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
