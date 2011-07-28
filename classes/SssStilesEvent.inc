<?php
/* * * *
 * * SssStilesEvent.inc
 * *
 * * @version 20110725_150355 (CC) Luke JZ aka SwissalpS
 * * * */

class SssStilesEvent extends SssStilesBaseObject {

	protected $oP; // plist representation
	protected $sPath;

	function __construct($iID = null) {

		parent::__construct();

		if (null === $iID) $iID = microTime(true) * 10000;

		// attempt to read from db
		$this->sPath = RUNTIME_DIR . '/events/' . floatVal($iID) . SssStilesPlistExt;

		$this->oP = new SssS_Plist();
		$this->oP->load($this->sPath, true);

WFLog::logToFile('SssStilesEvent.log', 'construct ' . $this->sPath . '   ' . print_r($this->oP->__toPlistArray(), true));

		$this->oP->getOrSet('cid', $iID);
		$this->oP->getOrSet('type', SssStilesCmd_NULL);

		if (0 === $iID) {

			$this->oP->getOrSet('mode', SssStilesMode_4);
			$this->oP->setInt('type', SssStilesCmd_MODE);

		} // if special case 0 -> mode

		// TODO: reduce disk activity by checking if anything changed
		$this->save($this->sPath);

	} // __construct


	function delete() {

		return @unlink($this->sPath);

	} // delete


	function getEventArray() {

		return $this->oP->__toPlistArray();

	} // getEventArray


	function getParam($sParamPath) {

		return $this->oP->get($sParamPath);

	} // getParam


	function save() {

		// don't save
		if (SssStilesCmd_NULL == $this->oP->get('type')) return null;

		$sPath = RUNTIME_DIR . '/events/' . $this->oP->get('cid') . SssStilesPlistExt;

		$mRes = $this->oP->saveTo($sPath);

WFLog::logToFile('SssStilesEvent.log', 'save to ' . $sPath . '   ' . print_r($this->oP->__toPlistArray(), true) . '    ' . print_r($mRes, true));

		return $mRes;

	} // save


	function setParam($sParamPath, $mValue = null) {

		return $this->oP->set($sParamPath, $mValue);

	} // getParam


	function xmlBlob() {

		$sOut = '
  <cmd>';
$a = $this->oP->__toPlistArray();
WFLog::logToFile('SssStilesEvent.log', 'xmlBlob ' . print_r($a, true));
		forEach ($a as $sKey => $mVal) {

			$sOut .= '
    <' . $sKey . '>' . $mVal . '</' . $sKey . '>';

		} // loop each param

		$sOut .= '
  </cmd>';

		return $sOut;

	} // xmlBlob

} // SssStilesEvent

/* * * *\ SssStilesEvent.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
