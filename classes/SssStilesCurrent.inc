<?php
/* * * *
 * * SssStilesCurrent.inc
 * *
 * * @version 20110728_002716 (CC) Luke JZ aka SwissalpS
 * * * */

class SssStilesCurrent {

	protected $oP; // plist representation

	protected $sPath;

	function __construct($iID = null) {

		$this->sPath = RUNTIME_DIR . '/current' . SssStilesPlistExt;

		$this->oP = new SssS_Plist();

		$this->oP->load($this->sPath, true);

	} // __construct


	static function firstFreeTile() {

		$oModeEvent = new SssStilesEvent(0);
		$iMax = $oModeEvent->getParam('mode');

		$aCurrent = self::sharedCurrent()->peekArray();

		for ($i = 0; $i < $iMax; $i++) {

			if (!isset($aCurrent['t' . $i])) {

				return $i;

			} // if found next open tile (mybe not from this mode..who cares)

		} // loop each possible tile

		return null;

	} // firstFreeTile


	static function firstOpenTile() {

		$oModeEvent = new SssStilesEvent(0);
		$iMax = $oModeEvent->getParam('mode');

		$aCurrent = self::sharedCurrent()->peekArray();

		for ($i = 0; $i < $iMax; $i++) {

			if (isset($aCurrent['t' . $i])) {

				return $i;

			} // if found next open tile (mybe not from this mode..who cares)

		} // loop each possible tile

		return null;

	} // firstFreeTile


	function peekArray() {

		return $this->oP->__toPlistArray();

	} // peekArray


	function pokeArray($a) {

		return $this->oP->initFromArray($a);

	} // pokeArray


	function save() {

		$mRes = $this->oP->saveTo($this->sPath);

		return $mRes;

	} // save


	static function sharedCurrent() {

		static $o = null;
		if (!$o) $o = new SssStilesCurrent();

		return $o;

	} // sharedCurrent


	static function getCurrentArray() {

		return self::sharedCurrent()->peekArray();

	} // getCurrentArray


	static function setCurrentArray($a) {

		return self::sharedCurrent()->pokeArray($a);

	} // setCurrentArray


	static function saveCurrent() {

		return self::sharedCurrent()->save();

	} // saveCurrent

} // SssStilesCurrent

/* * * *\ SssStilesCurrent.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
