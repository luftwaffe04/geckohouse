<?php
	$conn = mysqli_connect("localhost", "root", "1234", "geckohouse");
	$sql = "UPDATE sensordata SET feed=1;";
	$result = mysqli_query($conn, $sql);
	$row = mysqli_fetch_array($result);
?>
