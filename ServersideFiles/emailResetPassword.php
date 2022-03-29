<?php

ini_set("display_errors",1);

require "connection.php";
$rdir = str_replace("\\", "/", __DIR__);                    //Root Dir
require $rdir.'/PHPMailer/src/Exception.php';
require $rdir.'/PHPMailer/src/PHPMailer.php';
require $rdir.'/PHPMailer/src/SMTP.php';

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\SMTP;
use PHPMailer\PHPMailer\Exception;

//Load Composer's autoloader
require 'vendor/autoload.php';

if(isset($_POST))
{
        $email = $_POST;
}
print("$");
$email = array_keys($email)[0];
$expDate = date('Y-m-d', strtotime("+7 day"));
$email = str_replace('_','.',$email);
$token = bin2hex(random_bytes(18));
if($conn)
{
    $sqlCheckEmail = "SELECT * FROM aleksdb.Users WHERE fEmail LIKE :email";
    $stmt = $conn->prepare($sqlCheckEmail);
    $stmt->bindValue(':email',$email,PDO::PARAM_STR);
    $stmt->execute();
    if($stmt->rowCount() == 0)
    {
        //print("No matching emails");
        print('2'); //no emails found
    }
    else if($stmt->rowCount() > 0)
    {
        $SQLInsertDetails = "UPDATE Users SET exp_date = :date, reset_link_token = :token WHERE Users.fEmail = :email";
        $stmt = $conn->prepare($SQLInsertDetails);
        $stmt->bindValue(":date",$expDate,PDO::PARAM_STR);
        $stmt->bindValue(":token",$token,PDO::PARAM_STR);
        $stmt->bindValue(":email",$email,PDO::PARAM_STR);
        if($stmt->execute())
        {
            
            //$passwordResetLink = "http://51.38.80.233/aleks/resetPassword.html?token'{$token}'"; 
            $content = "http://51.38.80.233/aleks/resetPassword.html?token={$token}";
            $mail = new PHPMailer(); // create a new object
            $mail->IsSMTP(); // enable SMTP
	    $mail->SMTPDebug = 0;
	    $mail->do_debug = 0; // debugging: 1 = errors and messages, 2 = messages only
            $mail->SMTPAuth = true; // authentication enabled
            $mail->SMTPSecure = 'ssl'; // secure transfer enabled REQUIRED for Gmail
            $mail->Host = "smtp.gmail.com";
            $mail->Port = 465; // or 587
            $mail->IsHTML(true);
            $mail->Username = "aleksbmacs@gmail.com";
            $mail->Password = "ax#000888";
            $mail->SetFrom("aleksbmacs@gmail.com");
            $mail->Subject = "Password Reset";
            $mail->Body = $content;
            $mail->AddAddress($email);
	    if(!$mail->Send())
	    {
                echo "Mailer Error: " . $mail->ErrorInfo;
	    }
	    else
	    {
                print("1");
            }
            
        }
        else
        {
            //print("Failed to set tokens");
            print("3");
        }
    }
}
print("?");
?>
