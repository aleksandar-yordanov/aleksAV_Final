<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require "connection.php";

if(isset($_POST))
{
        $jsonInput = $_POST;
}
print('$');
$jsonObj = json_decode(array_key_first($jsonInput));
$username = $jsonObj->Username;
$password = $jsonObj->Password;
$email = $jsonObj->Email;
$email = str_replace('_','.',$email);
$hashedPassword = password_hash($password,PASSWORD_DEFAULT);
//print($hashedPassword);
//$hashedPassword = hash('sha1',$password);
$isValidEmail = filter_var($email, FILTER_VALIDATE_EMAIL);

if($conn)
{
	if(strlen($password) < 6){
		print('2');//invalid pw len
	}
	else if($isValidEmail == false)
	{
		print('4');//invalid email
	}
    	else{
		$sqlCheckEmail = "SELECT * FROM aleksdb.Users WHERE fEmail LIKE :email";
		$stmt = $conn->prepare($sqlCheckEmail);
		$stmt->bindValue(':email',$email,PDO::PARAM_STR);
        	$stmt->execute();
		if($stmt->rowCount() > 0){
			print('3'); //email is registered
		}
		else
		{
			$sqlQuery = "INSERT INTO aleksdb.Users(`fUsername`, `fPassword`, `fEmail`) VALUES (:username, :hashedPassword , :email)";
			$stmt = $conn->prepare($sqlQuery);
			$stmt->bindValue(':username',$username,PDO::PARAM_STR);
			$stmt->bindValue(':hashedPassword',$hashedPassword,PDO::PARAM_STR);
			$stmt->bindValue(':email',$email,PDO::PARAM_STR);
			if($stmt->execute()){
				print('1');
			}
			else
			{
				print('0');
			}
			$lQueryResult = $conn->lastInsertId();
			$newQuery = "INSERT INTO `UserLicenses`(`uID`, `licenseState`) VALUES (:lQueryResult,'license_none')";
			$stmt = $conn->prepare($newQuery);
			$stmt->bindValue(':lQueryResult',$lQueryResult,PDO::PARAM_INT);
			$stmt->execute();
		}
	}
}
else
{
	print("0");
}
print('?');
?>
