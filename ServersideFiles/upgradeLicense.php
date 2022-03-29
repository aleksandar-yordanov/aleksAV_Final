<?php

require "connection.php";

if(isset($_POST))
{
	$jsonInput = $_POST;
}
print("$");
$jsonObj = json_decode(array_key_first($jsonInput));
$username = $jsonObj->Username;
$licenseType = $jsonObj->LicenseType;
if($conn)
{
	$sql = "SELECT uID FROM Users WHERE fUsername = :username";
	$stmt = $conn->prepare($sql);
	$stmt->bindValue(':username',$username,PDO::PARAM_STR);
	$stmt->execute();
	$uIDnum = $stmt->fetchColumn();
	$LicenseStateUpdateSQL = "UPDATE UserLicenses SET licenseState = :licenseType WHERE uID = :uIDnum";
	$stmt = $conn->prepare($LicenseStateUpdateSQL);
	$stmt->bindValue(':licenseType',$licenseType,PDO::PARAM_STR);
	$stmt->bindValue(':uIDnum',$uIDnum,PDO::PARAM_INT);
	if($stmt->execute())
	{
		print("1");
	}
	else
	{
		print("0");
	}
}

print("?");
?>
