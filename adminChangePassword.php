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
$passwordToChange = $jsonObj->PasswordToChange;
print("$");

$hashedNewPassword = password_hash($passwordToChange,PASSWORD_DEFAULT);
if($conn)
{
	if(strlen($passwordToChange) > 64 || strlen($passwordToChange) < 6)
	{
		print("2"); //pw len invalid
	}
	else
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
				 $updatePwSQL = "UPDATE aleksdb.Users SET fPassword = :newpass WHERE fUsername = :username";
				 $stmt = $conn->prepare($updatePwSQL);
				 $stmt->bindValue(":newpass",$hashedNewPassword,PDO::PARAM_STR);
				 $stmt->bindValue(":username",$userToChange,PDO::PARAM_STR);
				 if($stmt->execute())
				 {
					 print("1"); //change success
				 }
				 else
				 {
					print("5"); //invalid user
				 }
			}
			else
			{
				print("3"); //not admin
			}
		}
		else
		{
			print("0"); //admin login failed
		}
	}
}
else
{
	print("4"); //database down
}
print("?");

?>
