<?php
/* * * *
 * * tilesConstants.inc
 * *
 * * @version 20110725_150744 (CC) Luke JZ aka SwissalpS
 * * * */
define('SssStilesPlistExt', '.plist');

// Mode 4			// Mode 6			// Mode 9
// +-----------+	// +-------+---+	// +---+---+---+
// |           |	// |       | 1 |	// | 0 | 1 | 2 |
// +     0     |	// +   0   +---+	// +---+---+---+
// |           |	// |       | 2 |	// | 3 | 4 | 5 |
// +---+---+---+	// +---+---+---+	// +---+---+---+
// | 1 | 2 | 3 |	// | 3 | 4 | 5 |	// | 6 | 7 | 8 |
// +---+---+---+	// +---+---+---+	// +---+---+---+

define('SssStilesMode_4', 4);
define('SssStilesMode_6', 6);
define('SssStilesMode_9', 9);

define('SssStilesCmd_NULL', 0);
define('SssStilesCmd_CLOSE', 10);
define('SssStilesCmd_CLOSEALL', 20);
define('SssStilesCmd_MAXIMIZE', 30);
define('SssStilesCmd_MINIMIZE', 40);
define('SssStilesCmd_MODE', 45);
define('SssStilesCmd_MOVE', 50);
define('SssStilesCmd_OPEN', 60);
define('SssStilesCmd_REFRESHPAGE', 70);
define('SssStilesCmd_RELOAD', 80);
define('SssStilesCmd_RELOADALL', 90);
define('SssStilesCmd_SBOFF', 92);
define('SssStilesCmd_SBON', 94);
define('SssStilesCmd_SBOPEN', 96);
define('SssStilesCmd_SBRELOAD', 98);
define('SssStilesCmd_SWAP', 100);
define('SssStilesCmd_VOL', 120);		// !! duplicate assignement !!
define('SssStilesCmd_HIDEALL', 110);
define('SssStilesCmd_SHOWALL', 120);	// !! duplicate assignement !!

define('SssStilesCmd_ALIAS_CLEAR', 130);
define('SssStilesCmd_ALIAS_SET', 131);
define('SssStilesCmd_ALIAS_SETMETA', 132);
define('SssStilesCmd_INFO', 140);

define('SssStilesUrlType_ATOM', 5);
define('SssStilesUrlType_BING', 10);
define('SssStilesUrlType_CURRENT', 15);
define('SssStilesUrlType_DAILYMOTION', 20);
define('SssStilesUrlType_FORATV', 25);
define('SssStilesUrlType_FUNNYORDIE', 30);
define('SssStilesUrlType_GOOGLEVIDEO', 40);
define('SssStilesUrlType_JUSTINTVL', 51);
define('SssStilesUrlType_JUSTINTVR', 52);
define('SssStilesUrlType_LIVELEAK', 60);
define('SssStilesUrlType_METACAFE', 65);
define('SssStilesUrlType_MITOCW', 70);
define('SssStilesUrlType_MYSPACE', 80);
define('SssStilesUrlType_QIKL', 91);
define('SssStilesUrlType_QIKR', 92);
define('SssStilesUrlType_REVISION3', 95);
define('SssStilesUrlType_REVVER', 97);
define('SssStilesUrlType_USTREAML', 101);
define('SssStilesUrlType_USTREAMR', 102);
define('SssStilesUrlType_VIDDLER', 110);
define('SssStilesUrlType_VIMEO', 120);
define('SssStilesUrlType_YOUTUBE', 130);

/* * * *\ tilesConstants.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
