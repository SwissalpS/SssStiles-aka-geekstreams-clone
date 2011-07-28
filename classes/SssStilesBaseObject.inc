<?php
/* * * *
 * * SssStilesBaseObject.inc
 * *
 * * @version 20110725_145628 (CC) Luke JZ aka SwissalpS
 * * * */
class SssStilesBaseObject extends WFObject {

	function __construct() {

		parent::__construct();

	} // __construct


	function __toString() {

		return $this->xmlBlob();

	} // __toString


	function xmlBlob() {

		$sOut = '';

		return $sOut;

	} // nullBlob

} // SssStilesBaseObject

/* * * *\ SssStilesBaseObject.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
