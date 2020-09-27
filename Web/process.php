<?php
	$conn = mysqli_connect("localhost", "root", "1234", "geckohouse");
	$sql = "SELECT * FROM sensordata;";
	$result = mysqli_query($conn, $sql);
	$row = mysqli_fetch_array($result);

	echo $row[0].",";
	echo $row[1].",";
	echo $row[2].",";
	echo $row[3].",";
	echo $row[4];
?>
