<?php
require "connection.php";

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

if(isset($_POST))
{
	$jsonInput = $_POST;
}
$jsonObj = json_decode(array_key_first($jsonInput));
$adminUsername = $jsonObj->Username;
$adminPassword = $jsonObj->Password;
$userToChange = $jsonObj->UserToChange;
$license = $jsonObj->License;

print("$");
if($conn)
{
	$sqlCheckUsername = "SELECT uID, fPassword FROM aleksdb.Users WHERE fUsername LIKE :username";
	$stmt = $conn->prepare($sqlCheckUsername);
	$stmt->bindValue(':username',$adminUsername,PDO::PARAM_STR);
	$stmt->execute();
	$match = false;
	while($row = $stmt->fetch())
	{
		$match = password_verify($adminPassword,$row["fPassword"]);
		$uID = $row["uID"];
	}
	if($match == true)
	{
		$sqlCheckAdmin = "SELECT adminState FROM aleksdb.Admins WHERE uID like :uid";
		$stmt = $conn->prepare($sqlCheckAdmin);
		$stmt->bindValue(":uid",$uID,PDO::PARAM_INT);
		$stmt->execute();
		if($stmt->rowCount() > 0)
		{
			$sql = "SELECT uID FROM Users WHERE fUsername = :username";
			$stmt = $conn->prepare($sql);
			$stmt->bindValue(':username',$userToChange,PDO::PARAM_STR);
			$stmt->execute();
			$uIDnum = $stmt->fetchColumn();
			if($uIDnum != false)
			{
				$LicenseStateUpdateSQL = "UPDATE UserLicenses SET licenseState = :licenseType WHERE uID = :uIDnum";
				$stmt = $conn->prepare($LicenseStateUpdateSQL);
				$stmt->bindValue(':licenseType',$license,PDO::PARAM_STR);
				$stmt->bindValue(':uIDnum',$uIDnum,PDO::PARAM_INT);
				if($stmt->execute())
				{
					print("1"); //success
				}
				else
				{
					print("2"); //invalid username
				}
			}
			else
			{
				print("2");
			}
		}
		else
		{
			print("3"); //not admin
		}
	}
	else
	{
		print("0");
	}
}
else
{
	print("4");
}

print("?");

?>
