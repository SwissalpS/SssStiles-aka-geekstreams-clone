<?php
/* * * *
 * * SssStilesPush.inc
 * *
 * * @version 20110725_145944 (CC) Luke JZ aka SwissalpS
 * * * */

class SssStilesPush extends SssStilesResponder {

	var $aRequest;

	function __construct() {

		parent::__construct();

		$this->aRequest = array();

	} // __construct


	function parseRequest() {

		switch($this->aRequest['c']) {

			case SssStilesCmd_ALIAS_SET :
			case SssStilesCmd_ALIAS_CLEAR :
			case SssStilesCmd_ALIAS_SETMETA : $this->respondTo_ALIAS(); break;

			case SssStilesCmd_CLOSE : $this->respondTo_CLOSE(); break;

			case SssStilesCmd_CLOSEALL : $this->respondTo_CLOSEALL(); break;

			case SssStilesCmd_HIDEALL : $this->respondTo_HIDEALL(); break;

			case SssStilesCmd_INFO : $this->respondTo_INFO(); break;

			case SssStilesCmd_MAXIMIZE : $this->respondTo_MAXIMIZE(); break;

			case SssStilesCmd_MINIMIZE : $this->respondTo_MINIMIZE(); break;

			case SssStilesCmd_MODE : $this->respondTo_MODE(); break;

			case SssStilesCmd_MOVE : $this->respondTo_MOVE(); break;

			case SssStilesCmd_OPEN : $this->respondTo_OPEN(); break;

			case SssStilesCmd_REFRESHPAGE : $this->respondTo_REFRESHPAGE(); break;

			case SssStilesCmd_RELOAD : $this->respondTo_RELOAD(); break;

			case SssStilesCmd_RELOADALL : $this->respondTo_RELOADALL(); break;

			case SssStilesCmd_SBOFF : $this->respondTo_SBOFF(); break;

			case SssStilesCmd_SBON : $this->respondTo_SBON(); break;

			case SssStilesCmd_SBOPEN : $this->respondTo_SBOPEN(); break;

			case SssStilesCmd_SBRELOAD : $this->respondTo_SBRELOAD(); break;

			case SssStilesCmd_SHOWALL : $this->respondTo_SHOWALL(); break;

			case SssStilesCmd_SWAP : $this->respondTo_SWAP(); break;

			case SssStilesCmd_VOL : $this->respondTo_(); break;

			case null :
			case SssStilesCmd_NULL :
			default :  $this->respondTo_(); break;

		} // switch command

		return $this;

	} // parseRequest


	function readRequest() {

WFLog::logToFile('SssStilesPush.log', print_r($_REQUEST, true));

		// first check token and quit if invalid
		$this->verifyToken();

		// if list requested, send it and quit
		if (isset($_REQUEST['l']) && 'l' == $_REQUEST['l']) $this->sendList();

		// alias or url
		$this->aRequest['a'] = isset($_REQUEST['a']) ? $_REQUEST['a'] : null;

		// alias command arguments
		$this->aRequest['a2'] = isset($_REQUEST['a2']) ? $_REQUEST['a2'] : null;
		$this->aRequest['a3'] = isset($_REQUEST['a3']) ? $_REQUEST['a3'] : null;

		// command type
		$this->aRequest['c'] = isset($_REQUEST['c'])
				? abs(intVal($_REQUEST['c'])) : SssStilesCmd_NULL;

		// mode change requested
		$this->aRequest['m'] = isset($_REQUEST['m']) ? abs(intVal($_REQUEST['m'])) : null;

		// numerical value 1
		$this->aRequest['ti1'] = isset($_REQUEST['ti1']) ? intVal($_REQUEST['ti1']) : -1;

		// numerical value 2
		$this->aRequest['ti2'] = isset($_REQUEST['ti2']) ? abs(intVal($_REQUEST['ti2'])) : 0;

		return $this;

	} // readRequest


	function respondTo_() {

		// dummy method for unsuported commands

	} // respondTo_


	function respondTo_ALIAS() {

		$oAlias = SssStilesServer::eventForAlias($this->aRequest['a']);

		switch($this->aRequest['c']) {

			case SssStilesCmd_ALIAS_SET :

				if ($oAlias) $this->respond404('alias already exists');

				$oAlias = SssStilesServer::eventForUrl($this->aRequest['a2']);

				if (!$oAlias) $this->respond404('invalid url');

				$sAlias = SssStilesServer::cleanAlias($this->aRequest['a']);

				if (empty($sAlias)) $this->respond404('invalid parameter alias');

				$oAlias->setParam('alias', $sAlias);

				$oAlias->save();

				// add to alias hash
				$aAliasHash = SssStilesServer::aliasHash();
				$aAliasHash[$sAlias] = $oAlias->getParam('cid');
				SssStilesAliases::setAliasesArray($aAliasHash);
				SssStilesAliases::saveAliases();

				break;

			case SssStilesCmd_ALIAS_CLEAR :

				if (!$oAlias) $this->respond404('unknown alias');

				// remove from alias hash
				$aAliasHash = SssStilesServer::aliasHash();
				unset($aAliasHash[$oAlias->getParam('alias')]);
				SssStilesAliases::setAliasesArray($aAliasHash);
				SssStilesAliases::saveAliases();

				// remove from db
				$oAlias->delete();

				break;

			case SssStilesCmd_ALIAS_SETMETA :

				if (!$oAlias) $this->respond404('unknown alias');

				switch(strToLower($this->aRequest['a2'])) {

					case 'ccode' :

						// lower case a-z 2 chars
						$sCC = strToLower($this->aRequest['a3']);

						$aMatches = array();
						$sPattern = '|^[a-z]{2}$|';

						if (!preg_match($sPattern, $sCC, $aMatches))
								$this->respond404('invalid argument');

						$oAlias->setParam('ccode', $aMatches[0]);
						$oAlias->save();

						break;

					case 'imgurl' :

						// any syntacticly valid url
						$sUrl = $this->aRequest['a3'];
						$aUrl = parse_url($sUrl);

						if (!isset($aUrl['scheme'], $aUrl['host'], $aUrl['path']))
								$this->respond404('invalid argument');

						$oAlias->setParam('imgurl', $sUrl);
						$oAlias->save();

						break;

					case 'param' :

						//
						$sCC = strToLower($this->aRequest['a3']);

						$aMatches = array();
						$sPattern = '|^[a-z0-9;:\-\+=_\.,]+$|i';

						if (!preg_match($sPattern, $sCC, $aMatches))
								$this->respond404('invalid argument');

						$oAlias->setParam('param', $aMatches[0]);
						$oAlias->save();

						break;

					case 'stime' :

						// signed int
						$iTime = intVal($this->aRequest['a3']);

						$oAlias->setParam('stime', $iTime);
						$oAlias->save();

						break;

					case 'timeoffset' :

						// signed int
						$iTime = intVal($this->aRequest['a3']);

						$oAlias->setParam('timeoffset', $iTime);
						$oAlias->save();

						break;

					case 'title' :

						// same restrictions as alias
						$sTitle = SssStilesServer::cleanAlias($this->aRequest['a3']);

						$oAlias->setParam('title', $sTitle);
						$oAlias->save();

						break;

					case 'vid' :

						//
						$sCC = strToLower($this->aRequest['a3']);

						$aMatches = array();
						$sPattern = '|^[a-z0-9;:\-\_\.]+$|i';

						if (!preg_match($sPattern, $sCC, $aMatches))
								$this->respond404('invalid argument');

						$oAlias->setParam('vid', $aMatches[0]);
						$oAlias->save();

						break;

					case 'alias' :
					case 'type' :
					case 'url' :
					case 'urltype' :
					default :  $this->respond404('invalid argument'); break;

				} // switch param to set

				break;

		} // switch sub command

		return $this;

	} // respondTo_ALIAS


	function respondTo_CLOSE() {

		$iArg1 = $this->aRequest['ti1'];

		$sAlias = SssStilesServer::cleanAlias($this->aRequest['a']);

		// fetch the current list
		$aCurrent = SssStilesCurrent::getCurrentArray();

		if (!empty($sAlias)) {

			// check if it is open at all, if so set $iAr1 to it's tile id
			forEach ($aCurrent as $sIndex => $iCid) {

				$iTile = subStr($sIndex, 1);

				$oEvent = new SssStilesEvent($iCid);

				if ($sAlias == $oEvent->getParam('alias', '')) {

					$iArg1 = $iTile;

					break;

				} // if found match

			} // loop each current event

		} // if got alias

		// find first open tile
		if (0 > $iArg1) {

			$iArg1 = SssStilesCurrent::firstOpenTile();

			if (null === $iArg1) return $this;

		} // if negative find first available tile to close

		$oClose = new SssStilesEvent();

		$oClose->setParam('type', SssStilesCmd_CLOSE);
		$oClose->setParam('ti1', $iArg1);

		$oClose->save();

		// update current.plist
		$aCurrent['t' . $iArg1] = null;

		// save/apply to db
		SssStilesCurrent::setCurrentArray($aCurrent);
		SssStilesCurrent::saveCurrent();

		return $this;

	} // respondTo_CLOSE


	function respondTo_CLOSEALL() {

		// fetch the current list
		$aCurrent = SssStilesCurrent::getCurrentArray();

		for ($i = 0; $i < 9; $i++) {

			$aCurrent['t' . $i] = null;

		} // loop

		// update current.plist
		SssStilesCurrent::setCurrentArray($aCurrent);
		SssStilesCurrent::saveCurrent();

		$oClose = new SssStilesEvent();

		$oClose->setParam('type', SssStilesCmd_CLOSEALL);

		$oClose->save();

		return $this;

	} // respondTo_CLOSEALL


	function respondTo_HIDEALL() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_HIDEALL);

		$oEvent->save();

		return $this;

	} // respondTo_HIDEALL


	function respondTo_INFO() {

		$sOut = '';

		$iArg1 = $this->aRequest['ti1'];

		$oAlias = SssStilesServer::eventForAlias($this->aRequest['a']);

		if ($oAlias) {

			$oEvent = $oAlias;

		} else {

			$oEvent = null;

			if (0 > $iArg1) {

				$iArg1 = SssStilesCurrent::firstOpenTile();

				if (null === $iArg1) return $this;

			} // if negative arg1 -> find first Open tile

			$aCurrent = SssStilesCurrent::getCurrentArray();

			$sKey = 't' . $iArg1;

			if (isset($aCurrent[$sKey])) {

				$oEvent = new SssStilesEvent($aCurrent[$sKey]);

				$sOut .= $iArg1 . ' ';

			} // if have something in tile

		} // if valid alias given or not

		if (!$oEvent) $this->respond404('invalid argument');

		$sOut .= print_r($oEvent->getEventArray(), true);
			//' ' . $oAlias->getParam('alias') . chr(9)	. $oAlias->getParam('url') . chr(10);

		echo $sOut;

		exit(0);

	} // respondTo_INFO


	function respondTo_MAXIMIZE() {

		$iArg1 = $this->aRequest['ti1'];

		$sAlias = SssStilesServer::cleanAlias($this->aRequest['a']);

		if (!empty($sAlias)) {

			// fetch the current list
			$aCurrent = SssStilesCurrent::getCurrentArray();

			// check if it is open at all, if so set $iAr1 to it's tile id
			forEach ($aCurrent as $sIndex => $iCid) {

				$iTile = subStr($sInex, 1);

				$oEvent = new SssStilesEvent($iCid);

				if ($sAlias == $oEvent->getParam('alias', '')) {

					$iArg1 = $iTile;

					break;

				} // if found match

			} // loop each current event

		} elseif (0 > $iArg1) {

			$iArg1 = SssStilesCurrent::firstOpenTile();

			if (null === $iArg1) return $this;

		} // if got alias or iArg1 is negative

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_MAXIMIZE);
		$oEvent->setParam('ti1', $iArg1);

		$oEvent->save();

		return $this;

	} // respondTo_MAXIMIZE


	function respondTo_MINIMIZE() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_MINIMIZE);

		$oEvent->save();

		return $this;

	} // respondTo_MINIMIZE


	function respondTo_MODE() {

		$m = $this->aRequest['m'];

		// get current mode
		$oModeEvent = new SssStilesEvent(0);

		if (in_array($m, array(SssStilesMode_4, SssStilesMode_6, SssStilesMode_9))) {

			// change mode to current
			$oModeEvent->setParam('mode', $m);

			$oModeEvent->save();

			// make mode change event
			$oModeEvent = new SssStilesEvent();

			$oModeEvent->setParam('mode', $m);
			$oModeEvent->setParam('type', SssStilesCmd_MODE);

			$oModeEvent->save();

		} else {

			echo 'current mode is ' . $oModeEvent->getParam('mode');

			exit(0);

		} // if valid mode

		return $this;

	} // respondTo_MODE


	function respondTo_MOVE() {

		$iArg1 = $this->aRequest['ti1'];
		$iArg2 = $this->aRequest['ti2'];

		if ($iArg1 === $iArg2) {

			$this->respond404('syntax error');

		} else {

			$oMove = new SssStilesEvent();

			$oMove->setParam('type', SssStilesCmd_MOVE);
			$oMove->setParam('ti1', $iArg1);
			$oMove->setParam('ti2', $iArg2);

			$oMove->save();

		} // check arguments

		return $this;

	} // respondTo_MOVE


	function respondTo_OPEN() {

		$iArg1 = $this->aRequest['ti1'];
		$sArg = $this->aRequest['a']; // either alias or url

		if (0 > $iArg1) {

			$iArg1 = SssStilesCurrent::firstFreeTile();

			if (null === $iArg1) $this->respond404('no free tiles');

		} // if tile not given

		$oAlias = SssStilesServer::eventForAlias($sArg);

		$oOpen = ($oAlias) ? null : SssStilesServer::eventForUrl($sArg);

		if (empty($oOpen) && empty($oAlias)) {

			$this->respond404('syntax error or failed to pass filters or not yet supported');

		} elseif ($oAlias) {

			// retrieve aliased OPEN (60) event type

			// duplicate params to new event
			$oOpen = new SssStilesEvent();

			$oOpen->setParam('alias', $oAlias->getParam('alias'));
			$oOpen->setParam('ccode', $oAlias->getParam('ccode'));
			$oOpen->setParam('imgurl', $oAlias->getParam('imgurl'));
			$oOpen->setParam('param', $oAlias->getParam('param'));
			$oOpen->setParam('stime', $oAlias->getParam('stime'));
			$oOpen->setParam('ti1', $iArg1);
			$oOpen->setParam('timeoffset', $oAlias->getParam('timeoffset'));
			$oOpen->setParam('title', $oAlias->getParam('title'));
			$oOpen->setParam('type', SssStilesCmd_OPEN);
			$oOpen->setParam('url', $oAlias->getParam('url'));
			$oOpen->setParam('urltype', $oAlias->getParam('urltype'));
			$oOpen->setParam('vid', $oAlias->getParam('vid'));

		} elseif ($oOpen) {

			// got a valid url -> prepared event object

			$oOpen->setParam('ti1', $iArg1);

		} // check arguments

		// save
		$oOpen->save();

		// update current.plist
		$aCurrent = SssStilesCurrent::getCurrentArray();

		$aCurrent['t' . $iArg1] = $oOpen->getParam('cid');

		SssStilesCurrent::setCurrentArray($aCurrent);
		SssStilesCurrent::saveCurrent();

		return $this;

	} // respondTo_OPEN


	function respondTo_REFRESHPAGE() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_REFRESHPAGE);

		$oEvent->save();

		return $this;

	} // respondTo_REFRESHPAGE


	function respondTo_RELOAD() {

		$iArg1 = $this->aRequest['ti1'];

		$sAlias = SssStilesServer::cleanAlias($this->aRequest['a']);

		if (!empty($sAlias)) {

			// fetch the current list
			$aCurrent = SssStilesCurrent::getCurrentArray();

			// check if it is open at all, if so set $iAr1 to it's tile id
			forEach ($aCurrent as $sIndex => $iCid) {

				$iTile = subStr($sInex, 1);

				$oEvent = new SssStilesEvent($iCid);

				if ($sAlias == $oEvent->getParam('alias', '')) {

					$iArg1 = $iTile;

					break;

				} // if found match

			} // loop each current event

		} // if got alias

		if (0 > $iArg1) {

			$iArg1 = SssStilesCurrent::firstOpenTile();

			if (null === $iArg1) return $this;

		} // if tile not given

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_RELOAD);
		$oEvent->setParam('ti1', $iArg1);

		$oEvent->save();

		return $this;

	} // respondTo_RELOAD


	function respondTo_RELOADALL() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_RELOADALL);

		$oEvent->save();

		return $this;

	} // respondTo_RELOADALL


	function respondTo_SBOFF() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_SBOFF);

		$oEvent->save();

		return $this;

	} // respondTo_SBOFF


	function respondTo_SBON() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_SBON);

		$oEvent->save();

		return $this;

	} // respondTo_SBON


	function respondTo_SBOPEN() {

		$sArg = $this->aRequest['a']; // either alias or url

		$oAlias = SssStilesServer::eventForAlias($sArg);

		$oOpen = ($oAlias) ? null : SssStilesServer::eventForUrl($sArg);

		if (empty($oOpen) && empty($oAlias)) {

			$this->respond404('syntax error or failed to pass filters or not yet supported');

		} elseif ($oAlias) {

			// retrieve aliased OPEN (60) event type

			// duplicate params to new event
			$oOpen = new SssStilesEvent();

			$oOpen->setParam('alias', $oAlias->getParam('alias'));
			$oOpen->setParam('ccode', $oAlias->getParam('ccode'));
			$oOpen->setParam('imgurl', $oAlias->getParam('imgurl'));
			$oOpen->setParam('param', $oAlias->getParam('param'));
			$oOpen->setParam('stime', $oAlias->getParam('stime'));
			$oOpen->setParam('ti1', null);
			$oOpen->setParam('timeoffset', $oAlias->getParam('timeoffset'));
			$oOpen->setParam('title', $oAlias->getParam('title'));
			$oOpen->setParam('type', SssStilesCmd_SBOPEN);
			$oOpen->setParam('url', $oAlias->getParam('url'));
			$oOpen->setParam('urltype', $oAlias->getParam('urltype'));
			$oOpen->setParam('vid', $oAlias->getParam('vid'));

		} elseif ($oOpen) {

			// got a valid url -> prepared event object

			$oOpen->setParam('ti1', null);

		} // check arguments

		// save
		$oOpen->save();

		return $this;

	} // respondTo_SBOPEN


	function respondTo_SBRELOAD() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_SBRELOAD);

		$oEvent->save();

		return $this;

	} // respondTo_SBRELOAD


	function respondTo_SHOWALL() {

		$oEvent = new SssStilesEvent();

		$oEvent->setParam('type', SssStilesCmd_SHOWALL);

		$oEvent->save();

		return $this;

	} // respondTo_SHOWALL


	function respondTo_SWAP() {

		$iArg1 = $this->aRequest['ti1'];
		$iArg2 = $this->aRequest['ti2'];

		if ($iArg1 === $iArg2 || 0 > $iArg1) {

			$this->respond404('syntax error');

		} else {

			$oMove = new SssStilesEvent();

			$oMove->setParam('type', SssStilesCmd_SWAP);
			$oMove->setParam('ti1', $iArg1);
			$oMove->setParam('ti2', $iArg2);

			$oMove->save();

		} // check arguments

		return $this;

	} // respondTo_SWAP


	function sendList() {

		$sOut = '';

		// add mode info
		$oModeEvent = new SssStilesEvent(0);
		$iMax = $oModeEvent->getParam('mode');

		// add the currently active tiles
		$aCurrent = SssStilesCurrent::getCurrentArray();

		for ($i = 0; $i < $iMax; $i++) {

			if (!isset($aCurrent['t' . $i])) {

				$sOut .= $i . ' ' . chr(10);

			} else {

				$oEvent = new SssStilesEvent($aCurrent['t' . $i]);
				$sOut .= $i . ' ' . $oEvent->getParam('alias') . chr(9)
					. $oEvent->getParam('url') . chr(10);

			} // if tile has smth

		} // loop currently visible tiles

		echo $sOut;

		exit(0);

	} // sendList

} // SssStilesPush

/* * * *\ SssStilesPush.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
