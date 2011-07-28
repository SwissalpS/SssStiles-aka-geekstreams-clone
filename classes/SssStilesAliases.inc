<?php
/* * * *
 * * SssStilesAliases.inc
 * *
 * * @version 20110728_045324 (CC) Luke JZ aka SwissalpS
 * * * */

class SssStilesAliases {

	protected $oP; // plist representation

	protected $sPath;

	function __construct($iID = null) {

		$this->sPath = RUNTIME_DIR . '/aliases' . SssStilesPlistExt;

		$this->oP = new SssS_Plist();

		$this->oP->load($this->sPath, true);

	} // __construct


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


	static function sharedAliases() {

		static $o = null;
		if (!$o) $o = new SssStilesAliases();

		return $o;

	} // sharedAliases


	static function getAliasesArray() {

		return self::sharedAliases()->peekArray();

	} // getAliasesArray


	static function setAliasesArray($a) {

		return self::sharedAliases()->pokeArray($a);

	} // setAliasesArray


	static function saveAliases() {

		return self::sharedAliases()->save();

	} // saveAliases

} // SssStilesAliases

/* * * *\ SssStilesAliases.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
