<?php
/* * * *
 * * SssStilesResponder.inc
 * *
 * * @version 20110725_145628 (CC) Luke JZ aka SwissalpS
 * * * */
class SssStilesResponder extends SssStilesBaseObject {

	const SoPEN = '<?xml version="1.0" encoding="utf-8" ?>
<cmdlist>';

	const SsCIPTbLOB = '
  <script/>';

	const ScLOSE = '</cmdlist>';


	protected $aOutgoingEvents;


	function __construct() {

		parent::__construct();

		$this->aOutgoingEvents = array();

		$this->readRequest();

		$this->parseRequest();

	} // __construct


	function outgoingEvents() {

		return $this->aOutgoingEvents;

	} // outgoingEvents


	function parseRequest() {

		return $this;

	} // parseRequest


	function readRequest() {

		return $this;

	} // readRequest


	protected function respond404($sMessage = null) {

		header('HTTP/1.0 404 Not Found');

		echo $sMessage;

		exit(1);

	} // respond404


	protected function verifyToken() {

		$sToken = isset($_REQUEST['tkn']) ? $_REQUEST['tkn'] : 0;

		// if invalid token
		if ($sToken != SssStilesServer::currentToken())
				$this->respond404('Authentication Error');

	} // verifyToken


	function xmlBlob() {

		$oNullEvent = new SssStilesEvent();

		$sOut = self::SoPEN;

		if (!empty($this->aOutgoingEvents)) $sOut .= self::SsCIPTbLOB . $oNullEvent->xmlBlob();

		forEach ($this->aOutgoingEvents as $oEvent) {

			$sOut .= $oEvent->xmlBlob();

		} // loop each event since last CommandID

		$oNullEvent = new SssStilesEvent();

		if (!empty($this->aOutgoingEvents)) $sOut .= $oNullEvent->xmlBlob() . chr(10);

		return $sOut . self::ScLOSE;

	} // xmlBlob

} // SssStilesResponder

/* * * *\ SssStilesResponder.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
