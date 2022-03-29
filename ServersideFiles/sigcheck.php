<?php

//ini_set('display_errors', 1);
//ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);

require "connection.php";

if(isset($_POST))
{
	$jsonInput = $_POST;
}

print("$"); #indicates the start of response body

$jsonObj = json_decode(array_key_first($jsonInput));
$sigs = array_values((array)$jsonObj);
if($conn)
{
	$sqlCheckSig = "SELECT * FROM aleksdb.Signatures WHERE Signature in (";
	$count = 1;
	foreach($sigs as $sig)
	{
		$sqlCheckSig.=":sig".$count.",";
		$count += 1;
	}
	$sqlCheckSig = substr($sqlCheckSig,0,strlen($sqlCheckSig)-1);
	$sqlCheckSig.=")";
	//var_dump($sqlCheckSig);
	$stmt = $conn->prepare($sqlCheckSig);
	for($i = 1; $i <= count($sigs); $i++)
	{	
		//print(":sig".$i);
		$stmt->bindValue(":sig".$i,$sigs[$i-1],PDO::PARAM_STR);
	}	
	$stmt->execute();
	//$stmt->debugDumpParams();
	$count = 0;
	$sigsMatched = [];
	while($row = $stmt->fetch(PDO::FETCH_ASSOC))
	{
		//var_dump($row);
		array_push($sigsMatched,$row["Signature"]);
	}
	foreach($sigs as $sig)
	{
		$matched = false;
		foreach($sigsMatched as $match)
		{
			if($sig == $match)
			{
				$matched = true;
				break;
			}
		}
		if($matched == true)
		{
			print("1");
		}
		else
		{
			print("0");
		
		}
	}
}
else
{
	print("2");
}

print("?");


?>
