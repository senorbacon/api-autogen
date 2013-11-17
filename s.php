<?php

/************************************************************
*                                                           *
*  Copyright (c) SingleClick Systems. All rights reserved.  *
*                                                           *
*************************************************************/

// a defensive programming measure that increases odds that use of the
// header() function works properly.  see ob_end_clean() below.

define('SPEEDTEST_PAYLOAD_SIZE', 2048000);
define('SPEEDTEST_MOBILE_PAYLOAD_SIZE', 102400);
define('SPEEDTEST_MAX_ITERATIONS', 1);

// setup the headers, don't cache
header("Cache-Control: no-store, no-cache, must-revalidate, proxy-revalidate, post-check=0, pre-check=0, no-transform"); // HTTP/1.1
header("Pragma: no-cache");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header("Content-Type: text/html");

// get current time
$timestamp = sprintf("%.3f", microtime(true));

// output bytes in chunks until we reach the payload size
$mobile = false;
$ajax_enabled = false;
$size = ($mobile ? SPEEDTEST_MOBILE_PAYLOAD_SIZE : SPEEDTEST_PAYLOAD_SIZE) / 1024;

for ($i = 0; $i < $size; $i++)
{
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                                                                                                    ';
	echo '                        ';
}

flush();

// get current time
$end_timestamp = sprintf("%.3f", microtime(true));
$total_time = $end_timestamp - $timestamp;

$rate_kbs = sprintf("%.1f", ((($mobile ? SPEEDTEST_MOBILE_PAYLOAD_SIZE : SPEEDTEST_PAYLOAD_SIZE) * 8) / $total_time) / 1024);

?>

<script type='text/javascript'>
alert('rate: ' + <?= $rate_kbs ?>);
</script>
