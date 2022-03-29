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
$hash = $jsonObj->Sig;
$hashedPassword = password_hash($password,PASSWORD_DEFAULT);
//$hashedPassword = hash('sha1',$password);

#print($username);
#print($password);
if($conn)
{
        $sqlCheckUsername = "SELECT uID, fPassword FROM aleksdb.Users WHERE fUsername LIKE :username";
        $stmt = $conn->prepare($sqlCheckUsername);
        $stmt->bindValue(':username',$username,PDO::PARAM_STR);
        $stmt->execute();
        $match = false;
        while($row = $stmt->fetch())
        {
                $match = password_verify($password,$row["fPassword"]);
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
			$insertSigStatement = "INSERT INTO aleksdb.Signatures(Signature) VALUES (:hash)";
			$stmt = $conn->prepare($insertSigStatement);
			$stmt->bindValue(":hash",$hash,PDO::PARAM_STR);
			if($stmt->execute())
			{
				print("1");
			}
			else
			{
				print("2");
			}
                }
                else
                {
                        print('3');
                }
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

?>
