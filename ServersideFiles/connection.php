<?php

$db_host = "51.38.80.233";
$db_name = "aleksdb";
$db_user = "aleks";
$db_pass = "V3e*jZnq8";

$conn = new PDO("mysql:host={$db_host};dbname={$db_name};charset=UTF8", $db_user, $db_pass);
$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

?>
