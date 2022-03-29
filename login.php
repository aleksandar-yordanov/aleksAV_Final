<?php

require "connection.php";

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

#print($connection);

if(isset($_POST))
{
	$jsonInput = $_POST;
}

print("$"); #indicates the start of response body

$jsonObj = json_decode(array_key_first($jsonInput));
$username = $jsonObj->Username;
$password = $jsonObj->Password;
$hashedPassword = password_hash($password,PASSWORD_DEFAULT);
//$hashedPassword = hash('sha1',$password);

#print($username);
#print($password);
if($conn)
{
	$sqlCheckUsername = "SELECT fPassword FROM aleksdb.Users WHERE fUsername LIKE :username";
	$stmt = $conn->prepare($sqlCheckUsername);
	$stmt->bindValue(':username',$username,PDO::PARAM_STR);
	$stmt->execute();
	$match = false;
	while($row = $stmt->fetch())
	{
		$match = password_verify($password,$row["fPassword"]);
	}
	if($match == true)
	{
		print('1');
	}
	else
	{
		print('0');
	}
	
}
else
{
	print('2'); #code for connection error
}



print("?");

/*
if($jsonObj->Username == "user123" and $jsonObj->Password == "pass123")
{
	print("true");
}
else{
	print("false");
}
print('?');

i#print($jsonObj->Username);
#print("?");#indicates start of next varible position
#
#print($jsonObj->Password);
 */
?>
