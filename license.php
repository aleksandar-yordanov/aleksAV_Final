<?php
require "connection.php";
if(isset($_POST)) {
	$jsonInput = $_POST;
	print('$');
	$jsonObj = json_decode(array_key_first($jsonInput));
	$username = $jsonObj->Username;
	if($conn) {
		$sql = "SELECT licenseState FROM Users, UserLicenses WHERE Users.uID = UserLicenses.uID AND Users.fUsername = :username LIMIT 0,1";
		$stmt = $conn->prepare($sql);
		$stmt->bindValue(':username', $username, PDO::PARAM_STR);
		$stmt->execute();
		if ($stmt->rowCount() > 0){
			$licenseState = $stmt->fetchColumn();
			//print($uIDnum);
		
			if($licenseState == "license_none"){
				print("1");
			}elseif($licenseState == "license_standard"){
				print("2");
			}elseif($licenseState == "license_premium"){
				print("3");
			}else{
				print($licenseState);
			}
		}else{
			print('0');
		}
		print('?');
	}
}
?>
