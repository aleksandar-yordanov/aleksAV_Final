<?php

require "connection.php";

if(isset($_POST))
{
	$jsonInput = $_POST;
}
print('$');
$jsonObj = json_decode(array_key_first($jsonInput));
$username = $jsonObj->Username;
//$hashedPassword = $hash('sha1',$jsonObj->Password);
//$hashedNewPassword = $hash('sha1',$jsonObj->Updated);
$hashedPassword = password_hash($jsonObj->Password,PASSWORD_DEFAULT);
$hashedNewPassword = password_hash($jsonObj->Updated,PASSWORD_DEFAULT);
if($conn)
{
	if(strlen($jsonObj->Password) > 64 || strlen($jsonObj->Password) < 6)
	{
		print('2');
	}
	else
	{
	$sqlUserCheck = "SELECT * FROM aleksdb.Users WHERE fUsername LIKE :username";
	$stmt = $conn->prepare($sqlUserCheck);
	$stmt->bindValue(':username',$username,PDO::PARAM_STR);
	$stmt->execute();
	if($stmt->rowCount() > 0)
	{
		$sqlPasswordCheck = "SELECT fPassword FROM aleksdb.Users WHERE fUsername LIKE :username";
		$stmt = $conn->prepare($sqlPasswordCheck);
		$stmt->bindValue(':username',$username,PDO:PARAM_STR);
		$stmt->execute();
		$result = $stmt->fetchColumn();
		if(password_verify($password,$result))
		{
			$sqlUpdate = "UPDATE aleksdb.Users SET fPassword = :newpass WHERE fUsername = :username";
			$stmt = $conn->prepare($sqlUpdate);
			$stmt->bindValue(':newpass',$hashedNewPassword,PDO::PARAM_STR);
			$stmt->bindValue(':username',$username,PDO::PARAM_STR);
			$stmt->execute();
			print('1');
			
		}
		else
		{
			print('4');
		}
	}
	else
	{
		print('3')
	}
	}


}
print('?');


?>
