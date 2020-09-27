<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Gecko`s House</title>
	<meta name="viewport" content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no">

	<link rel="stylesheet" type="text/css" href="./css/index.css">
	<link rel="stylesheet" type="text/css" href="./css/common.css">
	<link rel="stylesheet" type="text/css" href="./css/media.css">

	<script src="./js/jquery-1.7.2.min.js"></script>
	<script src="./js/jquery-ui.min.js"></script>
	<script src="./js/jquery.ui.touch.js"></script>
	<script src="./js/index.js"></script>

</head>


<body>
	<header id="header">
		<div class="wrap">
			<div>
				<div class="cover">
					<h1>Gecko`s House</h1>
					<img src="./img/lizard.png"/>
				</div>
			</div>
		</div>
	</header>



	<section id="main">
		<div class="wrap">
			<ul>
				<li>
					<article class="content temperature">
						<img src="./img/temperature.png">
						<section>
							<h2>Temperature</h2>
							<section class="cover">
								<div class="progressbar">
									<div id="tempbar" class="bar"></div>
									<div class="bar_back"></div>
								</div>
								<p class="percent">숫자</p>
							</section>
						</section>
					</article>
				</li>

				<li>
					<article class="content humidity">
						<img src="./img/humidity.png">
						<section>
							<h2>Humidity</h2>
							<section class="cover">
								<div class="progressbar">
									<div id="humibar" class="bar"></div>
									<div class="bar_back"></div>
								</div>
								<p class="percent">숫자</p>
							</section>
						</section>
					</article>
				</li>

				<li>
					<article class="content water">
						<img src="./img/water.png">
						<section>
							<h2>Water</h2>
							<section class="cover">
								<div class="progressbar">
									<div id="waterbar" class="bar"></div>
									<div class="bar_back"></div>
								</div>
								<p class="percent">숫자</p>
							</section>
						</section>
					</article>
				</li>
			</ul>
			<article class="content food">
				<img src="./img/food.png">
				<section class="food">
					<h2>Food</h2>
					<div class="food">
						<p id="full">FULL</p>
						<p id="feed">FEED</p>
					</div>
				</section>
			</article>
		</div>
	</section>



	<footer id="footer">
		<div class="wrap">
			<p>Copyright © 2020 Gecko`s House.co.,Ltd. All rights reserved.</p>
				<address>Contact webmaster for more information. 010-1234-5678</address>
		</div>
	</footer>
</body>
</html>
