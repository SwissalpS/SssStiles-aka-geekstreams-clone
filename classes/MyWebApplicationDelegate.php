<?php
/**
  * This file contains delegate implementations for the basic parts of this cli Application.
  */


// custom WFWebApplication delegate
class MyWebApplicationDelegate
{

    /**
     *  Hook to call the initialize method fo the web application.
     *  Applications will typically initialize DB stuff here.
     */
    function initialize() {
        // manifest core modules that we want to use -- if you don't want people to access a module, remove it from this list!
        $webapp = WFWebApplication::sharedApplication();
        $webapp->addModulePath('login', FRAMEWORK_DIR . '/modules/login');
        $webapp->addModulePath('menu', FRAMEWORK_DIR . '/modules/menu');
        $webapp->addModulePath('css', FRAMEWORK_DIR . '/modules/css');
        $webapp->addModulePath('examples', FRAMEWORK_DIR . '/modules/examples');

        // load propel
        //Propel::init(PROPEL_CONF);

        $this->bRunning = false;

    } // initialize

    function defaultInvocationPath() {
        return 'examples/widgets/toc';
    } // defaultInvocationPath

    // switch between different skin catalogs; admin, public, partner reporting, etc
    function defaultSkinDelegate() {
        return 'simple';
    } // defaultSkinDelegate

	function handleUncaughtException($e) {
		if (!$this->bRunning) return false;

		echo chr(10) . '!!' . chr(10) . $e . chr(10) . '!!' . chr(10);
		return true;
	} // handleUncaughtException

	function handleError($errNum, $errString, $file, $line, $contextArray) {
		echo '!#!' . chr(10) . $errNum . ' ' . $errString . chr(10) . '!#!';
	} // handleError

	function handleCLIRequest() {

		$this->bRunning = true;
        static $errset = null;
		static $sWelcome = null;
		static $sPrompt = null;
		static $oPiSR = null;
		static $sAppName = null;
		static $nl = null;
		static $aChoices = null;

		if (!$oPiSR) {
			$nl = chr(10);
			$sAppName = basename(APP_ROOT);
			$oPiSR = new SssS_PHPinlineScriptRunner($this);
			$sPrompt = 'PiSR> ';
			$sWelcome = $nl . 'You are running "' . $sAppName
					. '" on PHOCOA_VERSION: ' . PHOCOA_VERSION . $nl . $nl
					. 'SssS_PHPinlineScriptRunner: type "help" for some info'
					. $nl . 'iphp or stop to see the html-dump and then move'
					. ' on to iphp console.' . $nl . ' there it takes a \stop'
					. ' command to get you back here.' . $nl . 'exit, end or'
					. ' quit will terminate this instance of ' . $sAppName . $nl
					. ' so will any fatal errors in the consoles (not so much'
					. ' in iphp)' . $nl . $nl . 'Actually the exact procedure'
					. ' may vary as this is all rather experimental yet' . $nl
					. ' here are the arguments I got:' . chr(0x0a);
			$aChoices = array('brainX' => 'start chatserver',
					'PiSR' => 'use SssS_PHPinlineScriptRunner console',
					'WFRequestController' => 'back to WFRequestController (menu up)',
					'quit' => 'quit ' . $sAppName);

			$errset = E_ERROR | E_CORE_ERROR | E_COMPILE_ERROR;
       		if (defined("E_RECOVERABLE_ERROR")) $errset |= E_RECOVERABLE_ERROR;
		} // first call

		$lastHandler = set_error_handler(array($this, 'handleError'), $errset);

        $bReturn = false; $bRunPiSR = true;

		echo $sWelcome;

		$a = $_SERVER['argv'];
		//array_shift($a); // get (rid of) invocation path

		foreach ($a as $s) { echo $s . $nl; }

		echo $nl;

		$sChoice = SssS_CLI_Tools::getChoiceCLI(
										'what do we do?', $aChoices, 'PiSR');
		switch ($sChoice) {
			case 'quit' : return true;
			case 'WFRequestController' : return false;

			case 'brainX' :
				$bRunPiSR = false;
				$sPathPlist = APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'brainXconfig.plist';

				$sHow = SssS_CLI_Tools::getChoiceCLI('how do we run it?', array(
						'a' => 'foreground, let us watch',
						'b' => 'background, and return to WFRequestController',
						'c' => 'background and quit this instance',
						'd' => 'back to WFRequestController'), 'a');

				if ('a' == $sHow) {
					$oServer = new SssS_SocketController($sPathPlist);

				} else if ('b' == $sHow || 'c' == $sHow) {
					if ('c' == $sHow) $bReturn = true;
					$sPath = RUNTIME_DIR . DIR_SEP . 'runBrainX';
					$oRunner = new SssS_ShellScriptRunner();
					$sScript = sprintf('<?php%1$srequire(%2$s);%1$s$sPathPlist'
						. ' = "%3$s";%1$s$oServer = new SssS_SocketController('
						. '$sPathPlist);%1$s?>', $nl, SwissalpS_FRAMEWORK_DIR
						. DIR_SEP . 'SssS_SocketController.inc', $sPathPlist);

					$oRunner->doScriptAsPHPFile($sScript, $sPath, 700, false, true);

				} // choices

				break;

			default :
			case 'PiSR' :
				break;

		} // switch choice

		while ($bRunPiSR) {
			echo $sPrompt;
			$sInput =  fgets(STDIN);

			// catch ctl-d or other errors
			if (false === $sInput) { echo '--got false input--' . chr(10); $bReturn = true; break; }

        	$sInput = trim($sInput);

        	if ('help' == $sInput) { echo $this->helpCLI(); continue; }

        	if (in_array($sInput, array('iphp', 'stop'))) { echo '--handing over to iphp--' . chr(10); break; }

        	if (in_array($sInput, array('exit', 'end', 'quit'))) { echo '--quitting--' . chr(10); $bReturn = true; break; }

			$aRes = $oPiSR->doScript($sInput, false); // true); // @-muting eval

			if (isset($aRes['scriptOutput'])) echo '=>' . $aRes['scriptOutput'] . '<=' . chr(10);

			if (isset($aRes['scriptReturn'])) var_dump($aRes['scriptReturn']) . chr(10);

		} // loop

        set_error_handler($lastHandler, $errset);

		return $bReturn; // false; // true; // return false to have WFRequestController deal with the cli call (if using from _synergize/PHOCOA then it starts iphp session after running 'webapp') ~due to change, might stay
	} // handleCLIRequest

	function helpCLI() {
		return ' * * SssS_PHPinlineScriptRunner.inc
 * * was worth a try. Works for correctly when given correct code syntax.
 * * Can be used for interaktive shell. Also take a look at iphp which is
 * * different mainly that it doesn\'t crash the mothership quit as much :-)
 * * The crucial difference is that this class uses eval() -> inline
 * * while iphp uses exec() -> runs on seperate thread from file rebuilding
 * * per successfull call. This is a little disk intensive but has a clear
 * * stableizing advantage.
 * * I have tested the $_-feature and did not like it, that\'s why this class
 * * does not have a "last result" variable. It works more like a bash would
 * * in that you must echo or return a value to be printed (less screen filling
 * * when working with objects). With "return" the value is printed using
 * * var_dump().
 * *
 * * The advantage of eval() is that you can define new functions on the fly:
 * * enter "function help() { echo "hi there :-)";}" to see what I mean type the
 * * equivelant in iphp prepending some value to satisfy $_:
 * *  "3; function help() { echo "hi there :-)";}" so far so good. Now comes the
 * * difference, type "help()" in both script runners. iphp will roll up some
 * * error while SssS_PHPinlineScriptRunner will print "hi there :-)".
 * *
 * * to have a value in aResults[\'scriptReturn\'] your snippet must actually
 * * return something e.g. \'$a = 45*123; return $a;
 * *
 * * + you can instantiate new objects modify existing.
 * * + declare functions and classes but be aware not to redefine as that
 * *   crashes uncatchably.
 * *
 * * $this is SssS_PHPinlineScriptRunner object if you omit $this-> and just
 * * type $a, it will be converted to $this->oDelegate->a (or if no delegate
 * * just $this->a). $$a becomes $$this->oDelegate->a resp $$this->a
 * *
 * * $_a stays $_a which unless it\'s a global (e.g. $_SERVER) it will not be
 * * accessable outside the snippet scope. To keep this speedy do not declare
 * * functions and classes or other loops (unless you prepend function vars with _)
 * * wherever the snippet contains $this, it is not touched same goes for $this->oDelegate
 * *
 * * @version 20100502_203256 + treatScript() $a -> $this->a conversions
 * * @version 20091104_143909 (CC) Luke JZ aka SwissalpS
 ';
	} // helpCLI

    function autoload($className) {

        $requirePath = NULL;
    	switch ($className) {
            // Custom Classes - add in handlers for any custom classes used here.
            case 'Propel':
                $requirePath = 'propel/Propel.php';
                break;

           case 'PropelConfiguration' :
            	$requirePath = 'propel/util/PropelConfiguration.php';
            	break;

           case 'Criteria' :
            	$requirePath = 'propel/util/Criteria.php';
            	break;

           case 'DatabaseMap' :
            	$requirePath = 'propel/map/DatabaseMap.php';
            	break;

           case 'sidenav_SkinManifestDelegate' :
            	$requirePath = 'skins/simple/sidenav/sidenav_SkinManifestDelegate.php';
            	break;

           case 'Blog' :
            	$requirePath = 'lukecom_phocoa/Blog.php';
            	break;

           case 'BlogPeer' :
            	$requirePath = 'lukecom_phocoa/BlogPeer.php';
            	break;/**/
        }

        if ($requirePath) {
            require($requirePath);
            return true;
        }

WFLog::log('   autoload in appDelegate not found:  ' . $className);
        return false;
    } // autoload


    /**
     *  Hook to provide opportunity for the web application to munge the session config before php's session_start() is called.
     */
    function sessionWillStart() {

    }

    /**
     *  Hook to provide opportunity for the web application to munge the session data after php's session_start() is called.
     */
    function sessionDidStart() {

    } // sessionDidStart

    /**
     *  Hook to call the authorizationInfoClass method fo the web application.
     *  @see WFWebApplicationDelegate::authorizationInfoClass()
     */
    function authorizationInfoClass() {

        return 'WFAuthorizationInfo';

    } // authorizationInfoClass

} // MyWebApplicationDelegate
?>
