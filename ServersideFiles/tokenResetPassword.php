<?php

ini_set('display_errors', 1);

require "connection.php";

print("$");
if(isset($_POST))
{
    $jsonInput = $_POST;
}
$jsonObj = json_decode(array_key_first($jsonInput));
$nPassword = $jsonObj->nPassword;
$cPassword = $jsonObj->cPassword;
$token = $jsonObj->token;
//var_dump($nPassword);
//var_dump($cPassword);
//var_dump($token);
$curDate = date("Y-m-d");
if($conn)
{
    if($nPassword != $cPassword)
    {
        print("0");//Passwords dont match
    }
    else if(strlen($nPassword) > 64 || strlen($nPassword) < 6)
    {
        print('2');//invalid pw len
    }
    else
    {
        $tokenCheckSQL = "SELECT exp_date FROM Users WHERE reset_link_token = :token";
        $stmt = $conn->prepare($tokenCheckSQL);
	$stmt->bindValue(':token',$token,PDO::PARAM_STR);
	$stmt->execute();
        if($stmt->rowCount() > 0)
        {
            $expDate = $stmt->fetchColumn();
            //var_dump($curDate);
            //var_dump($expDate);
            if($curDate < $expDate)
            {
                $sqlUpdate = "UPDATE aleksdb.Users SET fPassword = :newpass WHERE reset_link_token = :token";
                $stmt = $conn->prepare($sqlUpdate);
                $stmt->bindValue(':newpass',password_hash($nPassword,PASSWORD_DEFAULT),PDO::PARAM_STR);
                $stmt->bindValue(':token',$token,PDO::PARAM_STR);
                if($stmt->execute())
                {
                    $nullifyToken = "UPDATE aleksdb.Users SET reset_link_token = NULL, exp_date = NULL WHERE reset_link_token = :token";
                    $stmt = $conn->prepare($nullifyToken);
                    $stmt->bindValue(':token',$token,PDO::PARAM_STR);
                    $stmt->execute();
                    print('1');
                }
                else
                {
                    print("5");
                }
                
            }
            else
            {
                print("4");//expired Token
            }
        }
        else
        {
            print("3"); //Invalid Token
        }
    }
}
print("?");


?>
