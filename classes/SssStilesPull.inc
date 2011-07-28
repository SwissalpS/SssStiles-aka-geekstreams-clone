<?php
/* * * *
 * * SssStilesPull.inc
 * *
 * * @version 20110725_145944 (CC) Luke JZ aka SwissalpS
 * * * */

/*

3 modes with 4, 6 or 9 tiles
Needs a db with table for tiles parameters
Needs a db with table for events parameters
Needs a db with table for preset tile parameters (cluster of events think .bat)

*/

class SssStilesPull extends SssStilesResponder {

	var $sClientID;		// made by client on first load
	var $iLastCommandID;// primary value of interest
	var $iPollCount;	// browsers count of polls
	var $iWindowHeight;	// browser window height
	var $iWindowWidth;	// browser window width
	var $iSomething;

	function __construct() {

		parent::__construct();

	} // __construct


	function parseRequest() {

WFLog::logToFile('SssStilesPullParse.log', 'starting to parse. lastCID: ' . $this->iLastCommandID);
		// first call?
		if (!$this->iLastCommandID) {

			// add mode info
			$oModeEvent = new SssStilesEvent(0);
			$this->aOutgoingEvents[] = $oModeEvent;
			$iMax = $oModeEvent->getParam('mode');

			// add the currently active tiles
			$aCurrent = SssStilesCurrent::getCurrentArray();

WFLog::logToFile('SssStilesPullParse.log', 'current: ' . print_r($aCurrent, true));

			forEach ($aCurrent as $sIndex => $iCID) {

				$oEvent = new SssStilesEvent($iCID);
				//$oEvent->setParam('ti1', substr($sIndex, 1));
WFLog::logToFile('SssStilesPullParse.log', 'adding current: ' . $iCID);
				$this->aOutgoingEvents[] = $oEvent;

				if (0 == --$iMax) break;

			} // loop each current event

			$iLastID = microTime(true) * 10000;

		} else $iLastID = $this->iLastCommandID; // if first call

		// figure out which events need to be sent
		$sPath = RUNTIME_DIR . '/events';
		if ($rD = openDir($sPath)) {

			while (false !== ($sFile = readDir($rD))) {

				// skip hidden files
				if ('.' == $sFile{0}) continue;

				$sPathFile = $sPath . '/' . $sFile;

				// skip dirs
				if (is_dir($sPathFile)) continue;

				// only look at .plist files
				if (SssStilesPlistExt != strRchr($sPathFile, SssStilesPlistExt)) continue;

				// skip older events
				$iCID = floatVal(basename($sFile, SssStilesPlistExt));
				if ($iLastID >= $iCID) continue;

				$oEvent = new SssStilesEvent($iCID);

WFLog::logToFile('SssStilesPullParse.log', 'checking type of newer: ' . $oEvent->getParam('type', null) . '   ' . $iCID);
				if ($oEvent->getParam('type', null)) {
WFLog::logToFile('SssStilesPullParse.log', 'adding newer: ' . $iCID);
					$this->aOutgoingEvents[] = $oEvent;
				} // if got a recent event

			} // loop all files

		} else {

			// output some error
			WFLog::logToFile('SssStilesPull.log', 'could not open dir: ' . $sPath);

		} // if able to open events dir

		return $this;

	} // parseRequest


	function readRequest() {
WFLog::logToFile('SssStilesPullRequest.log', print_r($_REQUEST, true));

		$this->sClientID		= isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;

		$this->iLastCommandID	= isset($_REQUEST['lastcid']) ? abs(floatVal($_REQUEST['lastcid'])) : 0;

		$this->iPollCount		= isset($_REQUEST['p']) ? abs(intVal($_REQUEST['p'])) : 0;

		$this->iWindowHeight	= isset($_REQUEST['h']) ? abs(intVal($_REQUEST['h'])) : 0;

		$this->iWindowWidth		= isset($_REQUEST['w']) ? abs(intVal($_REQUEST['w'])) : 0;

		$this->iSomething		= isset($_REQUEST['_']) ? abs(intVal($_REQUEST['_'])) : 0;

		return $this;

	} // readRequest

} // SssStilesPull

/* * * *\ SssStilesPull.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
